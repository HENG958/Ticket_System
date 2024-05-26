#include "account.hpp"
#include <iostream>
using std::string;

Account::Account(const char *_user, const char *user_password, const char *user_name, const char *user_mail, const int user_privilege) 
{
  strcpy(username, _user);
  strcpy(password, user_password);
  strcpy(name, user_name);
  strcpy(mail, user_mail);
  privilege = user_privilege;
}
Account GetAccount(int pos) {
  
}
void AddAccount(string &command) {
  
}
void AddFirstAccount(string &command) {
  
}
void Logout(string &command) {
  
}
void Login(string &command) {
  
}
string QueryAccount(string &command) {
  
}
string ModifyAccount(string &command) {
  
}

void Account::ModifyPassword(const char *user_password) 
{
  strcpy(password, user_password);
  return;
}
void Account::ModifyName(const char *user_name) 
{
  strcpy(name, user_name);
  return;
}
void Account::ModifyMail(const char *user_mail) 
{
  strcpy(mail, user_mail);
  return;
}
void Account::ModifyPrivilege(const int user_privilege) 
{
  privilege = user_privilege;
  return;
}


bool AccountIndex::operator<(const AccountIndex &other) const {
  return index < other.index;
}
bool AccountIndex::operator>(const AccountIndex &other) const {
  return index > other.index;
}
bool AccountIndex::operator==(const AccountIndex &other) const {
  return index == other.index;
}