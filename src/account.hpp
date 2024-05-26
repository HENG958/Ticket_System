#include <cstring>
#include <string>
#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP
using std::string;
class AccountIndex {
private:
  int index = 0;
  int privilege = 0;

public:
  AccountIndex() = default;
  ~AccountIndex() = default;
  
  friend void AddAccount(string &);

  friend void AddFirstAccount(string &);

  friend void Login(string &);

  friend void Buy(std::string &, const int);

  friend void Refund(std::string &);

  friend string QueryAccount(string &);

  friend string ModifyAccount(string &);

  bool operator<(const AccountIndex &other) const;

  bool operator>(const AccountIndex &other) const;

  bool operator==(const AccountIndex &other) const;
};
class Account 
{
private:
  char username[21] = "";
  char password[31] = "";
  char name[21] = "";
  char mail[31] = "";
  int privilege = 0;

  void ModifyPassword(const char *);

  void ModifyName(const char *);

  void ModifyMail(const char *);

  void ModifyPrivilege(const int);


public:
  Account() = default;

  Account(const char *_u, const char *_p, const char *_n, const char *_m, const int pr);

  ~Account() = default;

  friend void AddAccount(string &);

  friend void Login(string &);

  friend string QueryAccount(string &);

  friend string ModifyAccount(string &);

};
void AddAccount(string &);

void Login(string &);

void Logout(string &);

std::string QueryAccount(string &);

std::string ModifyAccount(string &);

void AddFirstAccount(string &);

#endif