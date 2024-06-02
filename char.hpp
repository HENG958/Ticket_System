#ifndef CHAR_HPP
#define CHAR_HPP
#include <string>
#include <cstring>
#include <iostream>
template <int maxLength = 65>
struct Char
{
  char content[maxLength + 1]{};
  Char()
  {
    memset(this->content, '\0', sizeof(content));
  };
  Char(const std::string &s)
  {
    memset(this->content, '\0', sizeof(content));
    strcpy(content, s.c_str());
  };
  Char(const char *cstr)
  {
    memset(this->content, '\0', strlen(content) + 1);
    strcpy(content, cstr);
  };
  operator std::string() const { return str(); };
  std::string str() const { return std::string(content); };
  Char &operator=(const Char<maxLength> &that)
  {
    if (this == &that)
      return *this;
    strcpy(content, that.content);
    return *this;
  };
  bool operator<(const Char<maxLength> &that) const 
  { 
    return strcmp(content, that.content) < 0; 
  };
  bool operator>(const Char<maxLength> &that) const 
  { 
    return strcmp(content, that.content) > 0; 
  };
  bool operator<=(const Char<maxLength> &that) const 
  { 
    return strcmp(content, that.content) <= 0; 
  };
  bool operator>=(const Char<maxLength> &that) const 
  { 
    return strcmp(content, that.content) >= 0; 
  };
  bool operator==(const Char<maxLength> &that) const 
  { 
    return strcmp(content, that.content) == 0; 
  };
  bool operator!=(const Char<maxLength> &that) const 
  { 
    return strcmp(content, that.content) != 0; 
  };
  friend std::ostream &operator<<(std::ostream &os, const Char<maxLength> &obj)
  {
    std::string carrier(obj.content);
    os << carrier;
    return os;
  }
};

#endif 