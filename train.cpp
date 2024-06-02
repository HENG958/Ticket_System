#include "train.hpp"
#include "time.hpp"
#include "algorithm.hpp"
#include <vector>
#include <unordered_map>
Train::Train(const std::string &trainID_, std::vector<std::string> &stations_, std::vector<int> &sumPrice_, std::vector<int> &arrivaltimes_, std::vector<int> &leavingtimes_)
{
  trainID = trainID_;
  for (int i = 0; i < stations_.size(); ++i)
    stations[i] = stations_[i];
  for (int i = 0; i < sumPrice_.size(); ++i)
    sumPrice[i] = sumPrice_[i];
  for (int i = 0; i < arrivaltimes_.size(); ++i)
    arrivaltime[i] = arrivaltimes_[i];
  for (int i = 0; i < leavingtimes_.size(); ++i)
    leavingtime[i] = leavingtimes_[i];
}

Train::Train(const Train &rhs)
{
  trainID = rhs.trainID;
  for (int i = 0; i < ticketsys::max_stationNum; ++i)
    stations[i] = rhs.stations[i];
  for (int i = 0; i < ticketsys::max_stationNum; ++i)
    sumPrice[i] = rhs.sumPrice[i];
  for (int i = 0; i < ticketsys::max_stationNum; ++i)
    arrivaltime[i] = rhs.arrivaltime[i];
  for (int i = 0; i < ticketsys::max_stationNum; ++i)
    leavingtime[i] = rhs.leavingtime[i];
}

TrainManager::DayTrain::DayTrain(const TrainManager::DayTrain &rhs)
{
  if (this == &rhs)
    return;
  for (int i = 0; i < ticketsys::max_stationNum; ++i)
    seatRemain[i] = rhs.seatRemain[i];
}
int TrainManager::DayTrain::findMin(int lp, int rp)
{
  int ans = INT32_MAX;
  for (int i = lp; i <= rp; ++i)
    ans = std::min(seatRemain[i], ans);
  return ans;
}
void TrainManager::DayTrain::rangeAdd(int lp, int rp, int d)
{
  for (int i = lp; i <= rp; ++i)
    seatRemain[i] += d;
}
TrainManager::TrainStation::TrainStation(const std::string &trainID_, int startSellDate_, int endSellDate_)
{
  trainID = trainID_;
  startSaleDate = startSellDate_;
  endSaleDate = endSellDate_;
}
TrainManager::TrainManager(const std::string &filename,
                           const std::string &filename_dtts,
                           const std::string &filename_sdb, const std::string &filename_btb)
    : trainDataBase(filename),
      DayTrainToSeat(filename_dtts),
      stationDataBase(filename_sdb),
      basicTrainBackUp(filename_btb)
{
  if (basicTrainBackUp.size())
  {
    std::vector<std::pair<ull, BasicTrain>> ca;
    basicTrainBackUp.range_search(0, UINT64_MAX, ca);
    for (auto &T : ca)
      basicTrainDatabase.insert(T);
  }
}

ull TrainManager::CalHash(const std::string &str_)
{
  return hash_str(str_);
}

void TrainManager::addTrain(const std::string &trainID_,
                            int stationNum_,
                            int totalSeatNum_,
                            std::vector<std::string> &stations_,
                            std::vector<int> &Price_,
                            const std::string &startTime_,
                            std::vector<int> &travelTimes_,
                            std::vector<int> &stopoverTimes_,
                            std::vector<std::string> &saleDate_,
                            char type_)
{
  int startTime = ticketsys::Clock(startTime_).GetMinute();
  std::vector<int> arrivaltimes;
  std::vector<int> leavingtimes;
  int tmp = 0;
  arrivaltimes.push_back(tmp);
  tmp = startTime;
  leavingtimes.push_back(tmp);
  for (int i = 0; i < stationNum_ - 2; ++i)
  {
    tmp += travelTimes_[i];
    arrivaltimes.push_back(tmp);
    tmp += stopoverTimes_[i];
    leavingtimes.push_back(tmp);
  }
  tmp += travelTimes_[stationNum_ - 2];
  arrivaltimes.push_back(tmp);
  leavingtimes.push_back(INT32_MAX);
  std::vector<std::string> stations;
  for (int i = 0; i < stationNum_; ++i)
    stations.push_back(stations_[i]);
  DayTrain dt_ca;
  std::vector<int> sumPrice;
  sumPrice.push_back(0);

  dt_ca.rangeAdd(0, stationNum_ - 1, totalSeatNum_);

  for (int i = 0; i < stationNum_ - 1; ++i)
  {
    sumPrice.push_back(sumPrice[i] + Price_[i]);
  }
  ticketsys::Calendar startSellDate(saleDate_[0]);
  ticketsys::Calendar endSellDate(saleDate_[1]);
  Train tr_ca(trainID_, stations, sumPrice, arrivaltimes, leavingtimes);
  ull tidHash = CalHash(trainID_);
  trainDataBase.insert(std::make_pair(tidHash, tr_ca));

  int s_date_rank = startSellDate.GetDay(), e_date_rank = endSellDate.GetDay();

  BasicTrain b_tr_ca(trainID_, stationNum_, totalSeatNum_, false, startTime, s_date_rank, e_date_rank, type_);
  basicTrainDatabase.insert(std::make_pair(tidHash, b_tr_ca));
  basicTrainBackUp.insert(std::make_pair(tidHash, b_tr_ca));
}

bool TrainManager::isAdded(const std::string &trainID_)
{
  return basicTrainDatabase.find(CalHash(trainID_)) != basicTrainDatabase.end();
}

bool TrainManager::isReleased(const std::string &trainID_)
{
  if (!isAdded(trainID_))
    return false;
  return basicTrainDatabase.find(CalHash(trainID_))->second.isReleased;
}

void TrainManager::deleteTrain(const std::string &trainID_)
{
  ull tr_hash = CalHash(trainID_);
  Train tr_ca;
  trainDataBase.find(tr_hash, tr_ca);
  BasicTrain b_tr_ca = basicTrainDatabase[tr_hash];
  int s_date_rank = b_tr_ca.startSellDate, e_date_rank = b_tr_ca.endSellDate;
  for (int i = s_date_rank; i <= e_date_rank; ++i)
  {
    DayTrainToSeat.erase(std::make_pair(i, tr_hash));
  }
  basicTrainDatabase.erase(basicTrainDatabase.find(tr_hash));
  basicTrainBackUp.erase(tr_hash);
  trainDataBase.erase(tr_hash);
  for (int i = 0; i < b_tr_ca.stationNum; ++i)
  {
    stationDataBase.erase(std::make_pair(CalHash(tr_ca.stations[i]), std::make_pair(b_tr_ca.startSellDate, tr_hash)));
  }
}

void TrainManager::releaseTrain(const std::string &trainID_)
{
  ull tr_hash = CalHash(trainID_);
  basicTrainDatabase[tr_hash].isReleased = true;
  Train tr_ca;
  BasicTrain b_tr_ca = basicTrainDatabase[tr_hash];
  trainDataBase.find(tr_hash, tr_ca);

  TrainStation train_station(trainID_, b_tr_ca.startSellDate, b_tr_ca.endSellDate);

  for (int i = 0; i < b_tr_ca.stationNum; ++i)
  {
    train_station.rank = i;
    train_station.priceSum = tr_ca.sumPrice[i];
    train_station.startTime = b_tr_ca.startTime;
    train_station.arrivaltime = tr_ca.arrivaltime[i];
    train_station.leavingtime = tr_ca.leavingtime[i];
    stationDataBase.insert(std::make_pair(std::make_pair(CalHash(tr_ca.stations[i]),
                                                         std::make_pair(b_tr_ca.startSellDate, tr_hash)),
                                          train_station));
  }
}

bool TrainManager::queryTrain(const std::string &trainID_,
                              const std::string &date_,
                              std::vector<std::string> &result)
{
  ticketsys::Day wanted_date = ticketsys::Calendar(date_).GetDay();
  if (!isAdded(trainID_))
    throw Error("Train Isn't Added");
  ull tr_hash = CalHash(trainID_);
  BasicTrain b_tr_ca = basicTrainDatabase[tr_hash];
  if (wanted_date < b_tr_ca.startSellDate || wanted_date > b_tr_ca.endSellDate)
    throw Error("Isn't Sell Date");
  Train tr_ca;
  trainDataBase.find(tr_hash, tr_ca);

  bool if_added = true;
  DayTrain dt_ca;
  if (!DayTrainToSeat.find(std::make_pair(wanted_date, tr_hash), dt_ca))
    if_added = false;

  result.push_back(trainID_);
  std::string st_ca;
  result.push_back(st_ca + b_tr_ca.type);
  st_ca.clear();
  int add_min = 24 * 60 * wanted_date;
  ticketsys::Time transfer;
  st_ca += std::string(tr_ca.stations[0]) + " xx-xx xx:xx -> " +
           (transfer + tr_ca.leavingtime[0] + add_min).ToStr() + " " + std::to_string(tr_ca.sumPrice[0]) + " ";
  st_ca += if_added ? std::to_string(dt_ca.seatRemain[0]) : std::to_string(b_tr_ca.totalSeatNum);
  result.push_back(st_ca);

  for (int i = 1; i < b_tr_ca.stationNum - 1; ++i)
  {
    st_ca.clear();
    st_ca += std::string(tr_ca.stations[i]) + " " +
             (transfer + tr_ca.arrivaltime[i] + add_min).ToStr() + " -> " +
             (transfer + tr_ca.leavingtime[i] + add_min).ToStr() + " " + std::to_string(tr_ca.sumPrice[i]) + " ";
    st_ca += if_added ? std::to_string(dt_ca.seatRemain[i]) : std::to_string(b_tr_ca.totalSeatNum);
    result.push_back(st_ca);
  }
  st_ca.clear();
  st_ca += std::string(tr_ca.stations[b_tr_ca.stationNum - 1]) + " " +
           (transfer + tr_ca.arrivaltime[b_tr_ca.stationNum - 1] + add_min).ToStr() +
           " -> " + "xx-xx xx:xx" + " ";
  st_ca += std::to_string(tr_ca.sumPrice[b_tr_ca.stationNum - 1]) + " " + "x";
  result.push_back(st_ca);

  return true;
}

void TrainManager::QueryTicket(std::string *info,
                               std::vector<std::string> &result)
{
  ticketsys::Day wanted_date = ticketsys::Calendar(info[ticketsys::_d]).GetDay();

  std::string startStation = info[ticketsys::_s], endStation = info[ticketsys::_t];

  ull start_hash = CalHash(startStation), terminal_hash = CalHash(endStation);
  std::vector<std::pair<std::pair<ull, std::pair<int, ull>>, TrainStation>> result_start, result_terminal;
  stationDataBase.range_search(std::make_pair(start_hash, std::make_pair(0, 0)),
                               std::make_pair(start_hash, std::make_pair(wanted_date + 1, 0)),
                               result_start);
  stationDataBase.range_search(std::make_pair(terminal_hash, std::make_pair(0, 0)),
                               std::make_pair(terminal_hash, std::make_pair(wanted_date + 1, 0)),
                               result_terminal);
  bool if_time = true;
  if (!info[ticketsys::_p].empty() && info[ticketsys::_p] == "cost")
    if_time = false;

  std::vector<std::pair<std::pair<int, ticketsys::trainIDType>, std::pair<int, int>>> possible_ans;

  for (int i = 0, j = 0; i < result_start.size() && j < result_terminal.size(); ++i)
  {
    while (j < result_terminal.size() && result_start[i].first.second > result_terminal[j].first.second)
      j++;
    if (j >= result_terminal.size())
      break;
    if (result_start[i].second.trainID == result_terminal[j].second.trainID)
    {
      int add_time = result_start[i].second.leavingtime / (60 * 24);
      if (result_start[i].second.endSaleDate + add_time >= wanted_date && result_start[i].second.startSaleDate + add_time <= wanted_date)
      {
        if (result_start[i].second.rank < result_terminal[j].second.rank)
        {
          int ranker;
          if (if_time)
            ranker = result_terminal[j].second.arrivaltime - result_start[i].second.leavingtime;
          else
            ranker = result_terminal[j].second.priceSum - result_start[i].second.priceSum;
          possible_ans.push_back(std::make_pair(std::make_pair(ranker, result_start[i].second.trainID),
                                                std::make_pair(i, j)));
        }
      }
    }
  }
  ticketsys::Sort(possible_ans, 0, int(possible_ans.size() - 1));
  result.push_back(std::to_string(possible_ans.size()));
  for (int i = 0; i < possible_ans.size(); i++)
  {

    int levT_f = result_start[possible_ans[i].second.first].second.leavingtime,
        arvT_t = result_terminal[possible_ans[i].second.second].second.arrivaltime,
        f_rank = result_start[possible_ans[i].second.first].second.rank,
        t_rank = result_terminal[possible_ans[i].second.second].second.rank;

    std::string _trainID = result_start[possible_ans[i].second.first].second.trainID;

    ull tr_hash = CalHash(_trainID);
    int start_date = wanted_date - levT_f / (24 * 60);
    bool if_added = true;
    DayTrain dt_ca;
    if (!DayTrainToSeat.find(std::make_pair(start_date, tr_hash), dt_ca))
      if_added = false;
    int _seat = if_added ? dt_ca.findMin(f_rank, t_rank - 1) : basicTrainDatabase[tr_hash].totalSeatNum;

    ticketsys::Minute start_time = 24 * 60 * start_date + levT_f;
    ticketsys::Minute end_time = 24 * 60 * start_date + arvT_t;
    int _price = result_terminal[possible_ans[i].second.second].second.priceSum - result_start[possible_ans[i].second.first].second.priceSum;

    Ticket tk_ca(_trainID, startStation, endStation, start_time, end_time, _price, _seat);

    result.push_back(tk_ca.Print());
  }
}

void TrainManager::QueryTransfer(std::string *info,
                                 std::vector<std::string> &result)
{
  ticketsys::Day wanted_date = ticketsys::Calendar(info[ticketsys::_d]).GetDay();
  std::string start_station = info[ticketsys::_s], terminal_station = info[ticketsys::_t];
  ull start_hash = CalHash(start_station), terminal_hash = CalHash(terminal_station);
  std::vector<TrainStation> result_start, result_terminal;
  stationDataBase.range_search(std::make_pair(start_hash, std::make_pair(0, 0)),
                               std::make_pair(start_hash, std::make_pair(wanted_date + 1, 0)),
                               result_start);
  stationDataBase.range_search(std::make_pair(terminal_hash, std::make_pair(0, 0)),
                               std::make_pair(terminal_hash, std::make_pair(wanted_date + 90, 0)),
                               result_terminal);

  std::unordered_map<ull, std::pair<int, int>> startTime_permit; // trainIDHash,pair(start_date,rank in result_start)
  for (int i = 0; i < result_start.size(); ++i)
  {
    int add_time = result_start[i].leavingtime / (60 * 24);
    if (result_start[i].endSaleDate + add_time >= wanted_date && result_start[i].startSaleDate + add_time <= wanted_date)
    {
      int levT_f = result_start[i].leavingtime;
      int start_time = wanted_date - levT_f / (24 * 60);
      startTime_permit.insert(std::make_pair(CalHash(result_start[i].trainID), std::make_pair(start_time, i)));
    }
  }

  Transfer best_choice;
  bool has_choice = false;
  bool if_time = false;
  if (!info[ticketsys::_p].empty() && info[ticketsys::_p] == "time")
    if_time = true;

  for (auto it_s = startTime_permit.begin(); it_s != startTime_permit.end(); it_s++)
  {
    int arr_rank = it_s->second.second; 
    int f_rank = result_start[arr_rank].rank;
    ull f_tr_hash = it_s->first;
    int start_date_1 = it_s->second.first;
    Train f_tr;
    trainDataBase.find(f_tr_hash, f_tr);
    BasicTrain b_f_tr_ca = basicTrainDatabase[f_tr_hash];
    bool if_added_tr1 = true;
    DayTrain tr_1;
    if (!DayTrainToSeat.find(std::make_pair(start_date_1, f_tr_hash), tr_1))
      if_added_tr1 = false;

    std::unordered_map<ull, std::pair<int, int>>
        StaAndArvMin;
    for (int i = f_rank + 1; i < b_f_tr_ca.stationNum; ++i)
    {
      int which_min = 24 * 60 * start_date_1 + f_tr.arrivaltime[i];
      StaAndArvMin.insert(std::make_pair(CalHash(f_tr.stations[i].str()), std::make_pair(which_min, i)));
    }

    for (auto &T : result_terminal)
    {
      ull t_tr_hash = CalHash(T.trainID.str());
      if (t_tr_hash == f_tr_hash)
        continue;
      Train t_tr;
      trainDataBase.find(t_tr_hash, t_tr);
      BasicTrain b_t_tr_ca = basicTrainDatabase[t_tr_hash];

      if (!isReleased(t_tr.trainID.str()))
        continue;
      for (int i = 0; i < T.rank; ++i)
      {
        std::string trans_station = t_tr.stations[i].str();
        auto iter = StaAndArvMin.find(CalHash(trans_station));
        if (iter == StaAndArvMin.end())
          continue;
        int last_leaving_minute = 24 * 60 * b_t_tr_ca.endSellDate + t_tr.leavingtime[i];
        int minute_diff = last_leaving_minute - iter->second.first;
        if (minute_diff < 0)
          continue;
        int best_trans_day = std::max(b_t_tr_ca.startSellDate, b_t_tr_ca.endSellDate - (minute_diff) / (24 * 60));
        int start_time_1 = 24 * 60 * start_date_1 + f_tr.leavingtime[f_rank];
        int end_time_1 = 24 * 60 * start_date_1 + f_tr.arrivaltime[iter->second.second];
        int start_time_2 = 24 * 60 * best_trans_day + t_tr.leavingtime[i];
        int end_time_2 = 24 * 60 * best_trans_day + t_tr.arrivaltime[T.rank];
        int cost_1 = f_tr.sumPrice[iter->second.second] - f_tr.sumPrice[f_rank];
        int cost_2 = t_tr.sumPrice[T.rank] - t_tr.sumPrice[i];
        int trans_time = start_time_2 - end_time_1;

        bool if_added_tr2 = true;
        DayTrain tr_2;
        if (!DayTrainToSeat.find(std::make_pair(best_trans_day, t_tr_hash), tr_2))
          if_added_tr2 = false;

        int seat_1 = if_added_tr1 ? tr_1.findMin(f_rank, iter->second.second - 1) : b_f_tr_ca.totalSeatNum;
        int seat_2 = if_added_tr2 ? tr_2.findMin(i, T.rank - 1) : b_t_tr_ca.totalSeatNum;
        Ticket tk_1(f_tr.trainID.str(), start_station, trans_station, start_time_1, end_time_1, cost_1, seat_1);
        Ticket tk_2(t_tr.trainID.str(), trans_station, terminal_station, start_time_2, end_time_2, cost_2, seat_2);

        if (!has_choice)
        {
          best_choice = {tk_1, tk_2};
          has_choice = true;
        }
        else
        {
          int tot_time_now = tk_1.lastTime() + tk_2.lastTime() + trans_time;
          int tot_cost_now = cost_1 + cost_2;
          if (if_time)
          {
            if (tot_time_now > best_choice.totTime())
              continue;
            else if (tot_time_now < best_choice.totTime())
              best_choice = {tk_1, tk_2};
            else
            {
              if (tot_cost_now > best_choice.totCost())
                continue;
              else if (tot_cost_now < best_choice.totCost())
                best_choice = {tk_1, tk_2};
              else
              {
                if (tk_1.trainID < best_choice.ticket1.trainID ||
                    tk_1.trainID == best_choice.ticket1.trainID && tk_2.trainID < best_choice.ticket2.trainID)
                  best_choice = {tk_1, tk_2};
              }
            }
          }
          else
          {
            if (tot_cost_now > best_choice.totCost())
              continue;
            else if (tot_cost_now < best_choice.totCost())
              best_choice = {tk_1, tk_2};
            else
            {
              if (tot_time_now > best_choice.totTime())
                continue;
              else if (tot_time_now < best_choice.totTime())
                best_choice = {tk_1, tk_2};
              else
              {
                if (tk_1.trainID < best_choice.ticket1.trainID ||
                    tk_1.trainID == best_choice.ticket1.trainID && tk_2.trainID < best_choice.ticket2.trainID)
                  best_choice = {tk_1, tk_2};
              }
            }
          }
        }
      }
    }
  }

  if (!has_choice)
    result.push_back("0");
  else
  {
    result.push_back(best_choice.ticket1.Print());
    result.push_back(best_choice.ticket2.Print());
  }
}

std::string TrainManager::BuyTicket(std::string *info,
                                    OrderManager &order_manager_)
{

  int wanted_date = ticketsys::Calendar(info[ticketsys::_d]).GetDay();
  std::string trainID = info[ticketsys::_i];
  ull tr_hash = CalHash(trainID);
  std::string start_station = info[ticketsys::_f], terminal_station = info[ticketsys::_t];
  ull start_hash = CalHash(start_station), terminal_hash = CalHash(terminal_station);
  if (basicTrainDatabase.find(tr_hash) == basicTrainDatabase.end())
    return "-1";
  BasicTrain b_tr_ca = basicTrainDatabase[tr_hash];
  TrainStation f_ts_ca, t_ts_ca;
  if (!stationDataBase.find(std::make_pair(start_hash, std::make_pair(b_tr_ca.startSellDate, tr_hash)), f_ts_ca) ||
      !stationDataBase.find(std::make_pair(terminal_hash, std::make_pair(b_tr_ca.startSellDate, tr_hash)), t_ts_ca))
    return "-1";

  int levT_f = f_ts_ca.leavingtime, arvT_t = t_ts_ca.arrivaltime, f_rank = f_ts_ca.rank, t_rank = t_ts_ca.rank;
  if (f_rank > t_rank)
    return "-1";

  int start_date = wanted_date - (levT_f / (24 * 60));
  int wanted_seat = std::stoi(info[ticketsys::_n]);
  if (start_date < b_tr_ca.startSellDate || start_date > b_tr_ca.endSellDate)
    return "-1";
  if (b_tr_ca.totalSeatNum < wanted_seat)
    return "-1";
  DayTrain dt_ca;
  if (!DayTrainToSeat.find(std::make_pair(start_date, tr_hash), dt_ca))
    dt_ca.rangeAdd(0,
                   b_tr_ca.stationNum - 1,
                   b_tr_ca.totalSeatNum);

  int seat = dt_ca.findMin(f_rank, t_rank - 1);

  bool ifQueue = false;
  if (!info[ticketsys::_q].empty() && info[ticketsys::_q] == "true")
    ifQueue = true;

  if (seat < wanted_seat && !ifQueue)
    return "-1";

  int price = t_ts_ca.priceSum - f_ts_ca.priceSum;

  int Oid = order_manager_.QueryOid() + 1;

  Order or_ca(ticketsys::SUCCESS, info[ticketsys::_i], info[ticketsys::_u], f_rank, t_rank,
              info[ticketsys::_f], info[ticketsys::_t], start_date, b_tr_ca.startTime, levT_f,
              arvT_t, Oid, price, wanted_seat);

  if (seat >= wanted_seat)
  {
    dt_ca.rangeAdd(f_rank, t_rank - 1, -wanted_seat);
    DayTrainToSeat.erase(std::make_pair(start_date, tr_hash));
    DayTrainToSeat.insert(std::make_pair(std::make_pair(start_date, tr_hash), dt_ca));
    order_manager_.AddOrder(info[ticketsys::_u], or_ca);
    return std::to_string(price * wanted_seat);
  }
  else
  {
    if (ifQueue)
    {
      or_ca.orderStatus = ticketsys::PENDING;
      order_manager_.AddOrder(info[ticketsys::_u], or_ca);
      PendingOrder por_ca(CalHash(info[ticketsys::_i]),
                          CalHash(info[ticketsys::_u]),
                          f_rank,
                          t_rank,
                          Oid,
                          wanted_seat,
                          start_date);
      order_manager_.AddPendingOrder(start_date, por_ca.tidHash, Oid, por_ca);
      return "queue";
    }
    else
      return "-1";
  }
}

bool TrainManager::RefundTicket(const std::string &username_, int rank_, OrderManager &order_manager_)
{
  std::vector<Order> order_list;
  order_manager_.QueryOrderPrivate(username_, order_list);
  int ra_ca = (int)order_list.size() - rank_;
  if (ra_ca < 0)
    throw Error("Order Number Isn't Enough");
  Order target_order = order_list[ra_ca];
  if (target_order.orderStatus == ticketsys::REFUNDED)
    throw Error("Order Is Refunded");
  else if (target_order.orderStatus == ticketsys::PENDING)
  {
    order_manager_.RemoveOrder(CalHash(target_order.username.str()), target_order.orderID);
    target_order.orderStatus = ticketsys::REFUNDED;
    order_manager_.AddOrder(target_order.username.str(), target_order);
    order_manager_.RemovePendingOrder(target_order.startDay.GetDay(),
                                      CalHash(target_order.trainID.str()),
                                      target_order.orderID);
  }
  else
  {
    int start_date = target_order.startDay.GetDay();
    ull trHash = CalHash(target_order.trainID.str());
    order_manager_.RemoveOrder(CalHash(target_order.username.str()), target_order.orderID);
    target_order.orderStatus = ticketsys::REFUNDED;
    order_manager_.AddOrder(target_order.username.str(), target_order);
    std::vector<PendingOrder> pending_order_list;
    order_manager_.QueryPendingOrderPrivate(start_date, trHash, pending_order_list);

    int f_rank = target_order.startRank, t_rank = target_order.endRank;
    DayTrain dt_ca;
    DayTrainToSeat.find(std::make_pair(start_date, trHash), dt_ca);
    dt_ca.rangeAdd(f_rank, t_rank - 1, target_order.num);

    for (int i = 0; i < pending_order_list.size(); ++i)
    {
      PendingOrder &pod_ca = pending_order_list[i];
      if (pod_ca.startRank >= t_rank || pod_ca.endRank <= f_rank || dt_ca.findMin(pod_ca.startRank, pod_ca.endRank - 1) < pod_ca.num)
        continue;
      dt_ca.rangeAdd(pod_ca.startRank, pod_ca.endRank - 1, -pod_ca.num);
      order_manager_.PendingToSuccess(pod_ca.uidHash, pod_ca.orderID);
      order_manager_.RemovePendingOrder(start_date, trHash, pod_ca.orderID);
    }
    DayTrainToSeat.erase(std::make_pair(start_date, trHash));
    DayTrainToSeat.insert(std::make_pair(std::make_pair(start_date, trHash), dt_ca));
  }
  return true;
}

void TrainManager::Clean()
{
  trainDataBase.clear();
  basicTrainDatabase.clear();
  DayTrainToSeat.clear();
  stationDataBase.clear();
  basicTrainBackUp.clear();
}

void TrainManager::Exit()
{
  for (auto &T : basicTrainDatabase)
  {
    basicTrainBackUp.modify(T.first, T.second);
  }
}
std::string TrainManager::Ticket::Print()
{
  ticketsys::Time ca;
  std::string start_t = (ca + start_time).ToStr();
  std::string end_t = (ca + end_time).ToStr();
  std::string ans;
  ans.reserve(120);
  ans += trainID;
  ans += " ";
  ans += startStation;
  ans += " ";
  ans += start_t;
  ans += " -> ";
  ans += endStation;
  ans += " ";
  ans += end_t;
  ans += " ";
  ans += std::to_string(cost);
  ans += " ";
  ans += std::to_string(seat);
  return ans;
}