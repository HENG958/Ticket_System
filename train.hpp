#ifndef TRAIN_HPP
#define TRAIN_HPP

#include "char.hpp"
#include "MySTL/bpTree.hpp"
//#include "ACMstl/UnorderedMap.hpp"
//#include "ACMstl/Map.hpp"
//#include "ACMstl/Vector.hpp"
#include "define.hpp"
#include "algorithm.hpp"
#include "time.hpp"
#include "order.hpp"
#include "error.hpp"
//#include <utility>
#include <vector>
#include <map>
#include <unordered_map>

using ticketsys::ull;

struct Train
{
  ticketsys::trainIDType trainID;
  ticketsys::stationType stations[ticketsys::max_stationNum];
  int sumPrice[ticketsys::max_stationNum]{};  
  int arrivaltime[ticketsys::max_stationNum]{}; 
  int leavingtime[ticketsys::max_stationNum]{}; 
  Train() = default;
  Train(const std::string &TrainID_, std::vector<std::string> &stations_, std::vector<int> &sumPrice_,
        std::vector<int> &arrivingTimes_, std::vector<int> &leavingTimes_);
  Train(const Train &rhs);
};

struct BasicTrain
{
  int stationNum{};
  int totalSeatNum{};
  bool isReleased = false;
  ticketsys::Minute startTime{};
  ticketsys::Day startSellDate = 0;
  ticketsys::Day endSellDate = 0;
  char type{};
  BasicTrain() = default;
  BasicTrain(const std::string &train_id, int station_num, int total_seat_num, bool is_released, int start_time, int start_sell_date, int end_sell_date, char type)
      :stationNum(station_num), totalSeatNum(total_seat_num), isReleased(is_released), startTime(start_time), startSellDate(start_sell_date), endSellDate(end_sell_date), type(type)
  {
  }

  BasicTrain(const BasicTrain &rhs)
  {
    stationNum = rhs.stationNum;
    totalSeatNum = rhs.totalSeatNum;
    isReleased = rhs.isReleased;
    startTime = rhs.startTime;
    startSellDate = rhs.startSellDate;
    endSellDate = rhs.endSellDate;
    type = rhs.type;
  }
};

class TrainManager
{
private:
  struct DayTrain
  {
    int seatRemain[ticketsys::max_stationNum]{};
    DayTrain() = default;
    DayTrain(const DayTrain &rhs);
    int findMin(int lp, int rp);
    void rangeAdd(int lp, int rp, int d);
  };

  struct TrainStation
  {
    ticketsys::trainIDType trainID{};
    int rank = 0, priceSum = 0;
    ticketsys::Day startSaleDate{}, endSaleDate{};
    int startTime{};
    int arrivaltime{};
    int leavingtime{};
    TrainStation() = default;
    TrainStation(const std::string &trainID_, int startSellDate_, int endSellDate_);
  };

  struct Ticket
  {
    std::string trainID{};
    std::string startStation{}, endStation{};
    int start_time{}, end_time{};
    int cost{}, seat{};

    Ticket() = default;
    Ticket(std::string train_id, std::string start_station, std::string end_station, int start_time, int end_time, int cost, int seat)
        : trainID(std::move(train_id)), startStation(std::move(start_station)), endStation(std::move(end_station)), start_time(start_time), end_time(end_time), cost(cost), seat(seat){}
    Ticket(Ticket &rhs)
    {
      trainID = rhs.trainID;
      startStation = rhs.startStation;
      endStation = rhs.endStation;
      start_time = rhs.start_time;
      end_time = rhs.end_time;
      cost = rhs.cost;
      seat = rhs.seat;
    }

    [[nodiscard]] int lastTime() const
    {
      return end_time - start_time;
    }

    std::string Print();
  };

  struct Transfer
  {
    Ticket ticket1{}, ticket2{};

    Transfer() = default;
    Transfer(Ticket &tk_1_, Ticket &tk_2_)
    {
      ticket1 = tk_1_, ticket2 = tk_2_;
    };

    [[nodiscard]] int totTime() const
    {
      return ticket2.end_time - ticket1.start_time;
    }
    [[nodiscard]] int totCost() const
    {
      return ticket1.cost + ticket2.cost;
    }
  };

  Bptree<ull, Train, 338, 3> trainDataBase;
  unordered_map<ull, BasicTrain> basicTrainDatabase;
  Bptree<ull, BasicTrain, 338, 65> basicTrainBackUp; 
  Bptree<std::pair<ticketsys::Day, ull>, DayTrain, 339, 18, ticketsys::pair_hash>
      DayTrainToSeat; 
  Bptree<std::pair<ull, std::pair<int, ull>>, TrainStation, 339, 101, ticketsys::pair_pair_hash>
      stationDataBase;
  std::hash<std::string> hash_str;

public:
  TrainManager(const std::string &filename_tdb,
               const std::string &filename_dtts,
               const std::string &filename_sdb,
               const std::string &filename_btb);

  void addTrain(const std::string &trainID_,
                int stationNum_,
                int totalSeatNum_,
                std::vector<std::string> &stations,
                std::vector<int> &Price_,
                const std::string &startTime_,
                std::vector<int> &travelTimes_,
                std::vector<int> &stopoverTimes_,
                std::vector<std::string> &saleDate_,
                char type_);

  bool isAdded(const std::string &trainID_);

  void deleteTrain(const std::string &trainID_);

  void releaseTrain(const std::string &trainID_);
  bool queryTrain(const std::string &trainID_, const std::string &date_, std::vector<std::string> &result);
  bool isReleased(const std::string &trainID_);
  ull CalHash(const std::string &str_);
  void QueryTicket(std::string *info, std::vector<std::string> &result);
  void QueryTransfer(std::string *info, std::vector<std::string> &result);
  std::string BuyTicket(std::string *info, OrderManager &order_manager_);
  bool RefundTicket(const std::string &username_, int rank_, OrderManager &order_manager_); //从新到旧第rank_个(1-base)
  void Clean();
  void Exit();
};

#endif 