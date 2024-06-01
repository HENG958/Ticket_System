#ifndef TIME_HPP
#define TIME_HPP
#include <iostream>
namespace ticketsys
{
  constexpr int mon_daynum[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  constexpr int mon_daysum[13] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
  struct Calendar
  {
    int month, day;
    explicit Calendar(int mm_ = 0, int dd_ = 0) : month(mm_), day(dd_) {}
    Calendar(const std::string &str)
    {
      int len_month = 0;
      for (int i = 0; i < str.length() && str[len_month] != '-'; ++i, ++len_month);

      month = std::stoi(str.substr(0, len_month));
      day = std::stoi(str.substr(len_month + 1, str.length()));
    }
    Calendar(const Calendar &rhs) : month(rhs.month), day(rhs.day) {}
    Calendar &operator=(const Calendar &rhs)
    {
      if (&rhs == this) return *this;
      month = rhs.month;
      day = rhs.day;
      return *this;
    }
    bool operator<(const Calendar &rhs) const
    {
      if (month < rhs.month)
        return true;
      if (rhs.month < month)
        return false;
      return day < rhs.day;
    }
    bool operator>(const Calendar &rhs) const
    {
      return rhs < *this;
    }
    bool operator<=(const Calendar &rhs) const
    {
      return !(rhs < *this);
    }
    bool operator>=(const Calendar &rhs) const
    {
      return !(*this < rhs);
    }
    Calendar operator+(int x)
    {
      Calendar lhs = *this;
      int sum = mon_daysum[month - (month != 0)] + day + x;
      lhs.month = int(std::lower_bound(mon_daysum, mon_daysum + 12, sum) - mon_daysum);
      lhs.day = sum - mon_daysum[lhs.month - (lhs.month != 0)];
      return lhs;
    }
    Calendar operator-(int x)
    {
      return (*this + (-x));
    }
    Calendar &operator+=(int x)
    {
      *this = *this + x;
      return *this;
    }
    bool operator==(const Calendar &rhs) const
    {
      return month == rhs.month && day == rhs.day;
    }
    bool operator!=(const Calendar &rhs) const
    {
      return !(rhs == *this);
    }
    std::string ToStr() const
    {
      std::string ans;
      ans += (month < 10 ? '0' + std::to_string(month) : std::to_string(month)) + '-';
      ans += (day < 10 ? '0' + std::to_string(day) : std::to_string(day));
      return ans;
    }

    int GetDay() const
    {
      return mon_daysum[month - (month != 0)] + day;
    }

    int GetHour() const
    {
      return (mon_daysum[month - (month != 0)] + day) * 24;
    }
  };

  struct Clock
  {
    int hour, minute, day;
    explicit Clock(int hour_ = 0, int minute_ = 0) : hour(hour_), minute(minute_), day(0) {}
    Clock(const std::string &str)
    {
      int len_hour = 0;
      for (int i = 0; i < str.length() && str[len_hour] != ':'; ++i, ++len_hour)
        ;
      hour = std::stoi(str.substr(0, len_hour));
      minute = std::stoi(str.substr(len_hour + 1, str.length()));
      day = 0;
    }
    Clock(const Clock &rhs) : hour(rhs.hour), minute(rhs.minute), day(rhs.day) {}
    Clock &operator=(const Clock &rhs)
    {
      day = rhs.day, hour = rhs.hour, minute = rhs.minute;
      return *this;
    }
    bool operator==(const Clock &rhs) const
    {
      return hour == rhs.hour && minute == rhs.minute;
    }
    bool operator!=(const Clock &rhs) const
    {
      return !(rhs == *this);
    }
    bool operator<(const Clock &rhs) const
    {
      if (hour < rhs.hour)
        return true;
      if (rhs.hour < hour)
        return false;
      return minute < rhs.minute;
    }
    bool operator>(const Clock &rhs) const
    {
      return rhs < *this;
    }
    bool operator<=(const Clock &rhs) const
    {
      return !(rhs < *this);
    }
    bool operator>=(const Clock &rhs) const
    {
      return !(*this < rhs);
    }
    Clock operator+(int x)
    {
      Clock lhs = *this;
      int sum = lhs.day * 60 * 24 + lhs.hour * 60 + lhs.minute + x;
      lhs.day = sum / (24 * 60);
      sum %= (24 * 60);
      lhs.hour = sum / 60;
      sum %= 60;
      lhs.minute = sum;
      return lhs;
    }

    Clock &operator+=(int x)
    {
      *this = *this + x;
      return *this;
    }

    std::string ToStr() const
    {
      std::string ans;
      ans += (hour < 10 ? '0' + std::to_string(hour) : std::to_string(hour)) + ':';
      ans += (minute < 10 ? '0' + std::to_string(minute) : std::to_string(minute));
      return ans;
    }

    int GetMinute() const
    {
      return day * 24 * 60 + hour * 60 + minute;
    }
  };

  struct Time
  {
    Calendar calendar_time;
    Clock clock_time;
    explicit Time(int mm_ = 0, int dd_ = 0, int hour_ = 0, int minute_ = 0) : calendar_time(mm_, dd_), clock_time(hour_, minute_) {}
    Time(const std::string &str, int num)
    {
      if (num == 1)
        calendar_time = str;
      else if (num == 2)
        clock_time = str;
      else
      {
        int len_b = 0, len = int(str.length());
        for (int i = 0; i < len && str[len_b] != ' '; ++i, ++len_b)
          ;
        calendar_time = str.substr(0, len_b);
        clock_time = str.substr(len_b + 1, len);
      }
    }

    Time operator+(int x)
    {
      Time lhs;
      int sum = 24 * 60 * (calendar_time.day + mon_daysum[calendar_time.month - (calendar_time.month != 0)]) + 60 * clock_time.hour + clock_time.minute;
      sum += x;
      int day = sum / (24 * 60);
      lhs.calendar_time.month = int(std::lower_bound(mon_daysum, mon_daysum + 12, day) - mon_daysum);
      lhs.calendar_time.day = day - mon_daysum[lhs.calendar_time.month - (lhs.calendar_time.month != 0)];
      sum -= day * 24 * 60;
      lhs.clock_time.hour = sum / 60;
      lhs.clock_time.minute = sum - lhs.clock_time.hour * 60;
      return lhs;
    }

    Time &operator+=(int x)
    {
      *this = *this + x;
      return *this;
    }

    std::string ToStr() const
    {
      std::string ans(11, '\0');
      ans[0] = calendar_time.month / 10 + '0';
      ans[1] = calendar_time.month % 10 + '0';
      ans[2] = '-';
      ans[3] = calendar_time.day / 10 + '0';
      ans[4] = calendar_time.day % 10 + '0';
      ans[5] = ' ';
      ans[6] = clock_time.hour / 10 + '0';
      ans[7] = clock_time.hour % 10 + '0';
      ans[8] = ':';
      ans[9] = clock_time.minute / 10 + '0';
      ans[10] = clock_time.minute % 10 + '0';
      return ans;
    }
  };
}

#endif