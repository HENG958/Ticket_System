#include "command.hpp"
CommandParser::CommandParser(UserManager &user_manager_, TrainManager &train_manager_, OrderManager &order_manager_)
    : user_manager(user_manager_), train_manager(train_manager_), order_manager(order_manager_) {
  mapFunction.insert(std::make_pair("add_user", &CommandParser::ParseAddUser));
  mapFunction.insert(std::make_pair("login", &CommandParser::ParseLogin));
  mapFunction.insert(std::make_pair("logout", &CommandParser::ParseLogout));
  mapFunction.insert(std::make_pair("query_profile", &CommandParser::ParseQueryProfile));
  mapFunction.insert(std::make_pair("modify_profile", &CommandParser::ParseModifyProfile));
  mapFunction.insert(std::make_pair("add_train", &CommandParser::ParseAddTrain));
  mapFunction.insert(std::make_pair("delete_train", &CommandParser::ParseDeleteTrain));
  mapFunction.insert(std::make_pair("release_train", &CommandParser::ParseReleaseTrain));
  mapFunction.insert(std::make_pair("query_train", &CommandParser::ParseQueryTrain));
  mapFunction.insert(std::make_pair("query_ticket", &CommandParser::ParseQueryTicket));
  mapFunction.insert(std::make_pair("query_transfer", &CommandParser::ParseQueryTransfer));
  mapFunction.insert(std::make_pair("buy_ticket", &CommandParser::ParseBuyTicket));
  mapFunction.insert(std::make_pair("query_order", &CommandParser::ParseQueryOrder));
  mapFunction.insert(std::make_pair("refund_ticket", &CommandParser::ParseRefundTicket));
  mapFunction.insert(std::make_pair("clean", &CommandParser::ParseClean));
  mapFunction.insert(std::make_pair("exit", &CommandParser::ParseExit));
}

void CommandParser::Run() {
  std::string parser_carrier;
  if (std::getline(std::cin, parser_carrier)) {
    bool all_blank_flag = true;
    for (char i: parser_carrier) {
      if (i != ' ') {
        all_blank_flag = false;
        break;
      }
    }
    if (!all_blank_flag) {
      std::vector<std::string> parser_list;
      std::string parser_list_to_use[15];

      SplitString(parser_carrier, parser_list, ' ');
      std::string timestamp = parser_list[0].substr(1, parser_list[0].length() - 2);
      std::cout << "[" << timestamp << "] ";
      int time_tag = std::stoi(timestamp);
      std::string cmd_type = parser_list[1];
      for (int i = 2; i < parser_list.size(); i += 2) {
        parser_list_to_use[GetInfoRank(parser_list[i][1])] = parser_list[i + 1];
      }
      try {
        if (mapFunction.find(cmd_type) != mapFunction.end()) (this->*mapFunction[cmd_type])(parser_list_to_use);
      } catch (Error &get_error) {
        Failure();
        std::cerr << get_error << std::endl;
      }

    }
  }
}

void CommandParser::ParseAddUser(std::string *cmd) {
  if (user_manager.AddUser(cmd))Success();
  else Failure();
}
void CommandParser::ParseLogin(std::string *cmd) {
  if (user_manager.Login(cmd))Success();
  else Failure();
}
void CommandParser::ParseLogout(std::string *cmd) {
  if (user_manager.Logout(cmd))Success();
  else Failure();
}
void CommandParser::ParseQueryProfile(std::string *cmd) {
  std::string result;
  if (!user_manager.queryProfile(cmd, result))Failure();
  else std::cout << result << "\n";

}
void CommandParser::ParseModifyProfile(std::string *cmd) {
  std::string result;
  if (!user_manager.modifyProfile(cmd, result))Failure();
  else std::cout << result << "\n";
}
void CommandParser::ParseAddTrain(std::string *cmd) {
  if (ifTAdd(cmd[ticketsys::_i])) {
    throw Error("Train Is Added");
    Failure();
    return;
  }
  std::vector<std::string> stations, _prices, _travelTimes, _stopoverTimes, saleDate;
  std::vector<int> prices, travelTimes, stopoverTimes;
  SplitString(cmd[ticketsys::_s], stations, '|');
  SplitString(cmd[ticketsys::_p], _prices, '|');
  SplitString(cmd[ticketsys::_t], _travelTimes, '|');
  SplitString(cmd[ticketsys::_o], _stopoverTimes, '|');
  SplitString(cmd[ticketsys::_d], saleDate, '|');
  for (auto &_price: _prices)prices.push_back(std::stoi(_price));
  for (auto &_travelTime: _travelTimes)travelTimes.push_back(std::stoi(_travelTime));
  if (std::stoi(cmd[ticketsys::_n]) > 2)
    for (auto &_stopoverTime: _stopoverTimes)
      stopoverTimes.push_back(std::stoi(_stopoverTime));
  char type = cmd[ticketsys::_y][0];
  train_manager.addTrain(cmd[ticketsys::_i],
                         std::stoi(cmd[ticketsys::_n]),
                         std::stoi(cmd[ticketsys::_m]),
                         stations,
                         prices,
                         cmd[ticketsys::_x],
                         travelTimes,
                         stopoverTimes,
                         saleDate, type);
  Success();
}
void CommandParser::ParseDeleteTrain(std::string *cmd) {
  if (!ifTAdd(cmd[ticketsys::_i]))
    throw Error("Train Isn't Added");
  if (ifTRel(cmd[ticketsys::_i]))
    throw Error("Train Is Released");
  train_manager.deleteTrain(cmd[ticketsys::_i]);
  Success();
}
void CommandParser::ParseReleaseTrain(std::string *cmd) {
  if (!ifTAdd(cmd[ticketsys::_i]))
    throw Error("Train Isn't Added");
  if (ifTRel(cmd[ticketsys::_i]))
    throw Error("Train Is Released");
  train_manager.releaseTrain(cmd[ticketsys::_i]);
  Success();
}
void CommandParser::ParseQueryTrain(std::string *cmd) {
  if (!ifTAdd(cmd[ticketsys::_i]))
    throw Error("Train Isn't Added");
  std::vector<std::string> result;
  train_manager.queryTrain(cmd[ticketsys::_i], cmd[ticketsys::_d], result);
  std::cout << result[0] << " ";
  for (int i = 1; i < result.size(); ++i)std::cout << result[i] << "\n";
}

void CommandParser::ParseQueryTicket(std::string *cmd) {
  std::vector<std::string> result;
  train_manager.QueryTicket(cmd, result);
  for (auto &i: result)std::cout << i << "\n";
}

void CommandParser::ParseQueryTransfer(std::string *cmd) {
  std::vector<std::string> result;
  train_manager.QueryTransfer(cmd, result);
  for (auto &i: result)std::cout << i << "\n";
}

void CommandParser::ParseBuyTicket(std::string *cmd) {
  if (!ifUReg(cmd[ticketsys::_u]))
    throw Error("Target User Isn't Added");
  if (!ifULog(cmd[ticketsys::_u]))
    throw Error("Target User Isn't Online");
  if (!ifTAdd(cmd[ticketsys::_i]))
    throw Error("Train Isn't Added");
  if (!ifTRel(cmd[ticketsys::_i]))
    throw Error("Train Isn't Released");
  std::string ans = train_manager.BuyTicket(cmd, order_manager);
  std::cout << ans << "\n";
}

void CommandParser::ParseRefundTicket(std::string *cmd) {
  if (!ifULog(cmd[ticketsys::_u]))
    throw Error("Target User Isn't Online");
  int rank = 1;
  if (!cmd[ticketsys::_n].empty())rank = std::stoi(cmd[ticketsys::_n]);
  if (train_manager.RefundTicket(cmd[ticketsys::_u], rank, order_manager))Success();
}
void CommandParser::ParseQueryOrder(std::string *cmd) {
  if (!ifUReg(cmd[ticketsys::_u]))
    throw Error("Target User Isn't Added");
  if (!ifULog(cmd[ticketsys::_u]))
    throw Error("Target User Isn't Online");
  std::vector<std::string> ans;
  order_manager.QueryOrder(cmd[ticketsys::_u], ans);
  std::cout << ans[0] << "\n";
  for (int i = ans.size() - 1; i >= 1; --i)std::cout << ans[i] << "\n";
}

void CommandParser::ParseClean(std::string *cmd) {
  user_manager.Clean();
  train_manager.Clean();
  order_manager.Clean();
  Success();
}
void CommandParser::ParseExit(std::string *cmd) {
  user_manager.Exit();
  train_manager.Exit();
  order_manager.Exit();
  std::cout << "bye\n";
  exit(0);
}
void CommandParser::SplitString(const std::string &cmd, std::vector<std::string> &x, const char &flag) {
  int l = cmd.find_first_not_of(flag), r = cmd.find_last_not_of(flag);
  int l_pointer = l, r_pointer = l;
  std::string carrier;
  for (int i = l; i <= r; ++i) {
    if (cmd[i] == flag) {
      r_pointer = i;
      carrier = cmd.substr(l_pointer, r_pointer - l_pointer);
      x.push_back(carrier);
      while (cmd[i] == flag && (i + 1) <= r)++i;
      l_pointer = i;
      i--;
    } else if (i == r) {
      r_pointer = i;
      carrier = cmd.substr(l_pointer, r_pointer - l_pointer + 1);
      x.push_back(carrier);
    }
  }
}

void CommandParser::Success() {
  std::cout << "0\n";
}
void CommandParser::Failure() {
  std::cout << "-1\n";
}
void CommandParser::Failure(const string &description) {
  std::cout << "-1\n";
  throw Error(description);
}
int CommandParser::ifULog(const std::string &username_) {
  return user_manager.isLogin(username_);
}
bool CommandParser::ifUReg(const string &username_) {
  return user_manager.isReg(username_);
}
bool CommandParser::ifTAdd(const std::string &trainID_) {
  return train_manager.isAdded(trainID_);
}
bool CommandParser::ifTRel(const std::string &trainID_) {
  return train_manager.isReleased(trainID_);
}
ticketsys::infoType CommandParser::GetInfoRank(char tag) {
  switch (tag) {
    case 'c':return ticketsys::_c;
    case 'd':return ticketsys::_d;
    case 'f':return ticketsys::_f;
    case 'g':return ticketsys::_g;
    case 'i':return ticketsys::_i;
    case 'm':return ticketsys::_m;
    case 'n':return ticketsys::_n;
    case 'o':return ticketsys::_o;
    case 'p':return ticketsys::_p;
    case 'q':return ticketsys::_q;
    case 's':return ticketsys::_s;
    case 't':return ticketsys::_t;
    case 'u':return ticketsys::_u;
    case 'x':return ticketsys::_x;
    default:return ticketsys::_y;
  }
}