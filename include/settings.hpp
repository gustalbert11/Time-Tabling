#pragma once

#include <string>
#include <vector>
#include <map.hpp>

enum Days 
{ 
    MONDAY = 1, 
    TUESDAY = 2, 
    WEDNESDAY = 3, 
    THURSDAY = 4, 
    FRIDAY = 5 
};

inline std::string day_to_string(Days day)
{
    switch (day)
    {
        case MONDAY: 
            return "MONDAY";

        case TUESDAY: 
            return "TUESDAY";

        case WEDNESDAY: 
            return "WEDNESDAY";

        case THURSDAY: 
            return "THURSDAY";

        case FRIDAY: 
            return "FRIDAY";

        default: 
            return "UNKNOWN";
    }
}

inline Days string_to_day(const std::string& str)
{
    if (str == "MONDAY") 
    {
        return MONDAY;
    }
    else if (str == "TUESDAY") 
    {
        return TUESDAY;
    }
    else if (str == "WEDNESDAY") 
    {
        return WEDNESDAY;
    }
    else if (str == "THURSDAY") 
    {
        return THURSDAY;
    }
    else if (str == "FRIDAY") 
    {
        return FRIDAY;
    }
    return MONDAY;
}

const uint MAX_NUM_DAYS = 5;
const uint MAX_NUM_LEVEL = 10;
const uint MAX_NUM_CREDITS = 6;
const uint MAX_NUM_SECTIONS = 5;
const uint MAX_CONSECUTIVE_HOURS = 2;
const uint MAX_DAILY_HOURS = 7;
const uint MAX_WEEKLY_HOURS = 10;
