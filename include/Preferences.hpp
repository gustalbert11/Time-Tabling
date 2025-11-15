#pragma once

#include "Settings.hpp"

enum PreferenceType { NO_PREFERENCE, DAYS, HOURS, DAYS_HOURS };

class Preferences
{
public:

    const std::string &get_description() const 
    { 
        return description; 
    }
    const PreferenceType &get_type() const 
    { 
        return type; 
    }
    const Designar::SortedArraySet<Days> &get_days() const 
    { 
        return days; 
    }
    const Designar::SortedArraySet<uint> &get_hours() const 
    { 
        return hours; 
    }

    bool set_description(const std::string &description)
    {
        if (description.empty())
        {
            return false;
        }
        this->description = description;
        return true;
    }
    bool set_type(const PreferenceType &type)
    {
        if (type < NO_PREFERENCE || 
            type > DAYS_HOURS)
        {
            return false;
        }
        this->type = type;
        return true;
    }
    bool add_day(const Days &day)
    {
        if (day < Days::MONDAY || 
            day > Days::FRIDAY || 
            days.contains(day))
        {
            return false;
        }
        days.append(day);
        return true;
    }
    bool add_hour(const uint &hour)
    {
        if (hour < 1 || 
            hour > 7 || 
            hours.size() == 7 || 
            hours.contains(hour))
        {
            return false;
        }
        hours.append(hour);
        return true;
    }

private:
    std::string description;
    PreferenceType type;
    Designar::SortedArraySet<Days> days;
    Designar::SortedArraySet<uint> hours;
};