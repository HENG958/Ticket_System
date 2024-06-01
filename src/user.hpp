#ifndef USER_HPP
#define USER_HPP

#include "char.hpp"
#include "MySTL/bpTree.hpp"
#include "define.hpp"
#include "error.hpp"
#include <unordered_map>
#include <vector>

using ticketsys::ull;

class UserManager;
class User
{
  friend class Usermanager;

public:
  ticketsys::passwordType password;
  ticketsys::usernameType username;
  ticketsys::nameType name;
  ticketsys::mailAddrType mailAddr;
  int privilege{};
  User() = default;
  User(const std::string &username_, const std::string &password_, const std::string &name_, const std::string &mailAddr_, int privilege_);
  User(const User &lhs);
  bool operator<(const User &rhs) const;
  bool operator>(const User &rhs) const;
  bool operator<=(const User &rhs) const;
  bool operator>=(const User &rhs) const;
  std::string to_string()
  {
    std::string ans;
    ans += username.str() + " " + name.str() + " ";
    ans += mailAddr.str() + " " + std::to_string(privilege);
    return ans;
  }
};

class UserManager
{
private:
  Bptree<unsigned long long, User, 339, 67> userDatabase; 
  std::unordered_map<unsigned long long, int> onlineUser;
  std::hash<std::string> hash_str;
public:
  explicit UserManager(const std::string &filenameUD);
  unsigned long long CalHash(const std::string &username_);
  bool AddUser(std::string *info);
  bool isReg(const std::string &username_);
  bool isLogin(const std::string &username_);
  bool checkPassword(const std::string &username_, const std::string &password_);
  bool Login(std::string *info);
  bool Logout(std::string *info);
  bool queryProfile(std::string *info, std::string &result);
  bool modifyProfile(std::string *info, std::string &result);
  void Clean();
  void Exit();
};

#endif 