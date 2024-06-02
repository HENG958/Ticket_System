#include "order.hpp"
Order::Order(ticketsys::orderStatusType order_status,
             const ticketsys::trainIDType &train_id,
             const ticketsys::usernameType &username,
             int start_rank,
             int end_rank,
             const ticketsys::stationType &start_station,
             const ticketsys::stationType &end_station,
             int start_day,
             int start_time,
             int leaving_time,
             int arriving_time,
             int order_id,
             int price,
             int num)
    : orderStatus(order_status),
      trainID(train_id),
      username(username),
      startRank(start_rank),
      endRank(end_rank),
      startStation(start_station),
      endStation(end_station),
      startTime(start_time),
      leavingTime(leaving_time),
      arrivingTime(arriving_time),
      orderID(order_id),
      price(price),
      num(num)
{
  startDay += start_day;
}
OrderManager::OrderManager(const std::string &filenameO, const std::string &filenameP) : orderDataBase(filenameO),
                                                                                         pendingQueue(filenameP)
{}

int OrderManager::QueryOid()
{
  return (int)orderDataBase.size();
}

ull OrderManager::CalHash(const std::string &str_)
{
  return hash_str(str_);
}

std::string OrderManager::OrderStr(Order &order_)
{
  std::string ans;
  switch (order_.orderStatus)
  {
  case ticketsys::SUCCESS:
    ans += "[success] ";
    break;
  case ticketsys::PENDING:
    ans += "[pending] ";
    break;
  case ticketsys::REFUNDED:
    ans += "[refunded] ";
  }
  ticketsys::Time transfer;
  transfer += order_.startDay.GetHour() * 60;
  ans +=
      order_.trainID.str() + " " + order_.startStation.str() + " " + (transfer + order_.leavingTime).ToStr() + " -> ";

  ans += order_.endStation.str() + " " + (transfer + order_.arrivingTime).ToStr() + " " + std::to_string(order_.price) + " " + std::to_string(order_.num);
  return ans;
}

void OrderManager::QueryOrderPrivate(const std::string &username_, std::vector<Order> &result)
{
  ull Hash = CalHash(username_);
  orderDataBase.range_search(std::make_pair(Hash, 0), std::make_pair(Hash + 1, 0), result);
}

void OrderManager::QueryOrder(const std::string &username_, std::vector<std::string> &result)
{
  std::vector<Order> orderList;
  QueryOrderPrivate(username_, orderList);
  result.push_back(std::to_string(orderList.size()));
  for (auto &i : orderList)
    result.push_back(OrderStr(i));
}

void OrderManager::QueryPendingOrderPrivate(int date_, ull tidHash_, std::vector<PendingOrder> &result)
{
  pendingQueue.range_search(std::make_pair(std::make_pair(date_, tidHash_), 0),
                            std::make_pair(std::make_pair(date_, tidHash_ + 1), 0), result);
}

void OrderManager::AddOrder(const std::string &username_, Order &order_)
{
  orderDataBase.insert(std::make_pair(std::make_pair(CalHash(username_), order_.orderID), order_));
}

void OrderManager::RemoveOrder(ull uidHash_, int Oid_)
{
  orderDataBase.erase(std::make_pair(uidHash_, Oid_));
}

void OrderManager::AddPendingOrder(int date_, ull tidHash_, int Oid_, PendingOrder &pending_order_)
{
  pendingQueue.insert(std::make_pair(std::make_pair(std::make_pair(date_, tidHash_), Oid_), pending_order_));
}
void OrderManager::RemovePendingOrder(int date_, ull tidHash_, int Oid_)
{
  pendingQueue.erase(std::make_pair(std::make_pair(date_, tidHash_), Oid_));
}
void OrderManager::PendingToSuccess(ull uidHash_, int orderID_)
{
  Order or_ca;
  orderDataBase.find(std::make_pair(uidHash_, orderID_), or_ca);
  orderDataBase.erase(std::make_pair(uidHash_, orderID_));
  or_ca.orderStatus = ticketsys::SUCCESS;
  orderDataBase.insert(std::make_pair(std::make_pair(uidHash_, orderID_), or_ca));
}
void OrderManager::Clean()
{
  orderDataBase.clear();
  pendingQueue.clear();
}
void OrderManager::Exit() {}