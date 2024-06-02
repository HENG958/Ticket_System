#include "user.hpp"
User::User(const std::string &username_, const std::string &password_, const std::string &name_, const std::string &mailAddr_, int privilege_)
{
  username = username_;
  password = password_;
  name = name_;
  mailAddr = mailAddr_;
  privilege = privilege_;
}
User::User(const User &lhs)
{
  username = lhs.username;
  password = lhs.password;
  name = lhs.name;
  mailAddr = lhs.mailAddr;
  privilege = lhs.privilege;
}
bool User::operator<(const User &rhs) const
{
  return username < rhs.username;
}
bool User::operator>(const User &rhs) const
{
  return rhs < *this;
}
bool User::operator<=(const User &rhs) const
{
  return !(rhs < *this);
}
bool User::operator>=(const User &rhs) const
{
  return !(*this < rhs);
}

UserManager::UserManager(const std::string &filenameUD) : userDatabase(filenameUD) {}
ull UserManager::CalHash(const std::string &username_)
{
  return hash_str(username_);
}

bool UserManager::isReg(const std::string &username_)
{
  User user_cache;
  return userDatabase.find(CalHash(username_), user_cache);
}

bool UserManager::isLogin(const string &username_)
{
  return onlineUser.find(CalHash(username_)) != onlineUser.end();
}

bool UserManager::AddUser(std::string *info)
{
  if (userDatabase.size())
  {
    User user_cache;
    if (userDatabase.find(CalHash(info[ticketsys::_u]), user_cache))
      throw Error("Target User Is Added");

    if (!isLogin(info[ticketsys::_c]))
      throw Error("Current User Isn't Online");

    if (onlineUser[CalHash(info[ticketsys::_c])] <= std::stoi(info[ticketsys::_g]))
      throw Error("Current Privilege Is Too Low");
  }
  else
    info[ticketsys::_g] = std::to_string(10);

  User freshman(info[ticketsys::_u], info[ticketsys::_p], info[ticketsys::_n], info[ticketsys::_m], std::stoi(info[ticketsys::_g]));
  userDatabase.insert(std::make_pair(CalHash(info[ticketsys::_u]), freshman));
  return true;
}

bool UserManager::checkPassword(const std::string &username_, const std::string &password_)
{
  ticketsys::passwordType rhs = password_;
  User ca;
  userDatabase.find(CalHash(username_), ca);
  ticketsys::passwordType lhs = ca.password;
  return rhs == lhs;
}

bool UserManager::Login(std::string *info)
{
  User user_cache;
  if (!userDatabase.find(CalHash(info[ticketsys::_u]), user_cache))
    throw Error("Target User Isn't Added");
  if (isLogin(info[ticketsys::_u]))
    throw Error("Target User Already Online");
  if (!checkPassword(info[ticketsys::_u], info[ticketsys::_p]))
    throw Error("Wrong Password");
  onlineUser.insert(std::make_pair(CalHash(info[ticketsys::_u]), user_cache.privilege));
  return true;
}
bool UserManager::Logout(std::string *info)
{
  if (!isLogin(info[ticketsys::_u]))
    throw Error("Target User Isn't Online");
  onlineUser.erase(onlineUser.find(CalHash(info[ticketsys::_u])));
  return true;
}

bool UserManager::queryProfile(std::string *info,
                               std::string &result)
{
  User ta_ca;
  if (!isLogin(info[ticketsys::_c]))
    throw Error("Current User Isn't Online");
  if (!userDatabase.find(CalHash(info[ticketsys::_u]), ta_ca))
    throw Error("Target User Isn't Added");
  if (onlineUser[CalHash(info[ticketsys::_c])] <= ta_ca.privilege && info[ticketsys::_c] != ta_ca.username.str())
    throw Error("Current Privilege Is Too Low");
  result = ta_ca.to_string();
  return true;
}
bool UserManager::modifyProfile(std::string *info,
                                std::string &result)
{
  User target_cache;
  if (!userDatabase.find(CalHash(info[ticketsys::_u]), target_cache))
    throw Error("Target User Isn't Added");
  if (!isLogin(info[ticketsys::_c]))
    throw Error("Current User isn't online");
  if (onlineUser[CalHash(info[ticketsys::_c])] <= target_cache.privilege && info[ticketsys::_c] != target_cache.username.str())
    throw Error("Current Privilege Is Too Low");
  if (!info[ticketsys::_p].empty())
    target_cache.password = info[ticketsys::_p];
  if (!info[ticketsys::_n].empty())
    target_cache.name = info[ticketsys::_n];
  if (!info[ticketsys::_m].empty())
    target_cache.mailAddr = info[ticketsys::_m];
  if (!info[ticketsys::_g].empty())
  {
    target_cache.privilege = std::stoi(info[ticketsys::_g]);
    if (target_cache.privilege >= onlineUser[CalHash(info[ticketsys::_c])])
      throw Error("Current Privilege Is Too Low");
    if (isLogin(info[ticketsys::_u]))
      onlineUser[CalHash(info[ticketsys::_u])] = target_cache.privilege;
  }
  userDatabase.erase(CalHash(info[ticketsys::_u]));
  userDatabase.insert(std::make_pair(CalHash(info[ticketsys::_u]), target_cache));
  result = target_cache.to_string();
  return true;
}

void UserManager::Clean()
{
  userDatabase.clear();
  onlineUser.clear();
}

void UserManager::Exit()
{
}