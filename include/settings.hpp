#pragma once

#include <string>
#include <vector>
#include <map.hpp>

enum Days 
{ 
    NO_SPECIFIED,
    MONDAY, 
    TUESDAY, 
    WEDNESDAY, 
    THURSDAY, 
    FRIDAY 
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
            return "NO SPECIFIED";
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

    return NO_SPECIFIED;
}

const uint MAX_NUM_DAYS = 5;
const uint MAX_NUM_LEVELS = 10;
const uint MAX_NUM_CREDITS = 6;
const uint PROF_MAX_NUM_SECTIONS = 5;
const uint COURSE_MAX_NUM_SECTIONS = 8;
const uint MAX_CONSECUTIVE_HOURS = 4;
const uint MIN_START_HOUR = 8;
const uint MAX_END_HOUR = 18;
const uint MAX_DAILY_HOURS = 10;
const uint MAX_WEEKLY_HOURS = 5 * MAX_DAILY_HOURS;
