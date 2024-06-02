#ifndef ORDER_HPP
#define ORDER_HPP
#include "define.hpp"
#include "time.hpp"
#include "MySTL/bpTree.hpp"
//#include "ACMstl/Vector.hpp"
#include "algorithm.hpp"
#include "error.hpp"
#include <iostream>
//#include <map>
#include <vector>
using ticketsys::ull;
struct Order
{
  ticketsys::orderStatusType orderStatus{};
  ticketsys::trainIDType trainID{};
  ticketsys::usernameType username{};
  int startRank{}, endRank{};
  ticketsys::stationType startStation{}, endStation{};
  ticketsys::Calendar startDay{};
  int startTime{}, leavingTime{}, arrivingTime{};
  int orderID{}, price{}, num{};
  Order() = default;
  Order(ticketsys::orderStatusType order_status,
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
        int num);
};

struct PendingOrder
{
  ull tidHash{}, uidHash{};
  int startRank{}, endRank{};
  int orderID{}, num{};
  ticketsys::Calendar startDay{};

  PendingOrder() = default;
  PendingOrder(ull tid_hash,
               ull uid_hash,
               int start_rank,
               int end_rank,
               int order_id,
               int num,
               int start_day)
      : tidHash(tid_hash),
        uidHash(uid_hash),
        startRank(start_rank),
        endRank(end_rank),
        orderID(order_id),
        num(num) { startDay += start_day; }
};

class OrderManager
{
  template <class T1, class T2>
  class PairHash
  {
  public:
    ull operator()(const std::pair<T1, T2> &ValueType) const
    {
      return ValueType.first + ValueType.second;
    }
  };

  Bptree<std::pair<ull, int>, Order, 339, 38, ticketsys::pair_hash> orderDataBase;       
  Bptree<std::pair<std::pair<int, ull>, int>, PendingOrder, 254, 144, ticketsys::pair_pair_hash_nd> pendingQueue; 
  std::hash<std::string> hash_str;

public:
  OrderManager(const std::string &filenameO, const std::string &filenameP);
  int QueryOid();
  ull CalHash(const std::string &str_);
  std::string OrderStr(Order &order_);
  void QueryOrderPrivate(const std::string &username_, std::vector<Order> &result);
  void QueryOrder(const std::string &username_, std::vector<std::string> &result);
  void QueryPendingOrderPrivate(int date_, ull tidHash_, std::vector<PendingOrder> &result);
  void AddOrder(const std::string &username_, Order &order_);
  void RemoveOrder(ull uidHash_, int Oid_);
  void AddPendingOrder(int date_, ull tidHash_, int Oid_, PendingOrder &pending_order_);
  void RemovePendingOrder(int date_, ull tidHash_, int Oid_);
  void PendingToSuccess(ull uidHash_, int orderID_); 
  void Clean();
  void Exit();
};

#endif 