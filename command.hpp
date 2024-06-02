#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <iostream>
#include <cstring>
#include "order.hpp"
#include "train.hpp"
#include "user.hpp"
#include "error.hpp"
#include <vector>
#include <unordered_map>
class CommandParser {
 private:
  UserManager &user_manager;
  TrainManager &train_manager;
  OrderManager &order_manager;
  std::unordered_map<std::string, void (CommandParser::*)(std::string *)> mapFunction;
  ticketsys::infoType GetInfoRank(char tag);

 public:
  CommandParser(UserManager &user_manager_, TrainManager &train_manager_, OrderManager &order_manager_);
  void Run();
  void ParseAddUser(std::string *info);
  void ParseLogin(std::string *info);
  void ParseLogout(std::string *info);
  void ParseQueryProfile(std::string *info);
  void ParseModifyProfile(std::string *info);
  void ParseAddTrain(std::string *info);
  void ParseDeleteTrain(std::string *info);
  void ParseReleaseTrain(std::string *info);
  void ParseQueryTrain(std::string *info);
  void ParseQueryTicket(std::string *info);
  void ParseQueryTransfer(std::string *info);
  void ParseBuyTicket(std::string *info);
  void ParseQueryOrder(std::string *info);
  void ParseRefundTicket(std::string *info);
  void ParseClean(std::string *info);
  void ParseExit(std::string *info);
  void SplitString(const std::string &cmd, std::vector<std::string> &x, const char &flag);
  static void Success();
  static void Failure();
  static void Failure(const std::string &);
  bool ifUReg(const std::string &username_);
  int ifULog(const std::string &username_);
  bool ifTAdd(const std::string &trainID_);
  bool ifTRel(const std::string &trainID_);

};

#endif 