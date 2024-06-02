#include <iostream>
#include "char.hpp"
#include "command.hpp"
#include "order.hpp"
#include "train.hpp"
#include "user.hpp"
UserManager user_manager("userData");
TrainManager train_manager("trainData", "dtts", "sdb", "btb");
OrderManager order_manager("orderData", "pendingData");
CommandParser command_parser(user_manager, train_manager, order_manager);
int main()
{
  std::ios::sync_with_stdio(false);
  while (true)
  {
    command_parser.Run();
    //std::cout << "__end_of_output__" << std::endl;
  }
}