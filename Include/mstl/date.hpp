#pragma once

#include <mstl/types.hpp>

#include <ctime>
#include <sstream>
#include <iomanip>

#define asctime_r asctime
#define localtime_r localtime_s

namespace mstl
{

  class Date final
  {
  public:
    Date(void) : timestamp(time(nullptr)) {}
    Date(const std::time_t unix_timestamp) : timestamp(unix_timestamp) {}

    inline s32 DayOfMonth(void) const { return get_tm().tm_mday; }
    inline s32 Month(void) const { return get_tm().tm_mon + 1; }
    inline s32 Year(void) const { return get_tm().tm_year + 1900; }
    inline s32 DayOfWeek(void) const { return get_tm().tm_wday; }
    s32 WeekNumber(void) const
    {
      std::tm tm_date = get_tm();

      int day_of_year = tm_date.tm_yday;
      int week_day    = (tm_date.tm_wday == 0) ? 6 : tm_date.tm_wday - 1;
      int week_number = (day_of_year + 7 - week_day) / 7;

      return week_number;
    }

    string _asctime(void) const
    {
      std::tm tm_date = get_tm();

      char* buffer = asctime_r(&tm_date);// , buffer);
      auto str = std::string(buffer);

      return str;
    }

    string str(const string& _format) const
    {
      std::stringstream ss{};

      u8 i = 0;
      while (_format.size() > i)
        format(ss, _format[i++]);

      return ss.str();
    }

  private:
    std::time_t timestamp;

    inline std::tm get_tm(void) const
    {
      std::tm tm_date;
      localtime_r(&tm_date, &timestamp);

      return tm_date;
    }

    inline void format(std::stringstream& ss, const char _c) const
    {
      switch (_c)
      {
      case 'Y': ss << Year();                                              break;
      case 'y': ss << (Year() % 100);                                      break;
      case 'm': ss << std::setw(2) << std::setfill('0') << (Month());      break;
      case 'd': ss << std::setw(2) << std::setfill('0') << (DayOfMonth()); break;
      case 'w': ss << std::setw(2) << std::setfill('0') << (WeekNumber()); break;
      case 'D': ss << (DayOfWeek());                                       break;

      default: ss << _c; break;
      }
    }

  };

}
