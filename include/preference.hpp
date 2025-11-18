#pragma once

#include "settings.hpp"

enum PreferenceType 
{ 
    NO_PREFERENCE, 
    DAYS, 
    HOURS, 
    DAYS_HOURS 
};

inline std::string preference_type_to_string(PreferenceType type)
{
    switch (type)
    {            
        case DAYS: 
            return "DAYS";
        
        case HOURS: 
            return "HOURS";

        case DAYS_HOURS: 
            return "DAYS_HOURS";

        default: 
            return "NO_PREFERENCE";
    }
}

inline PreferenceType string_to_preference_type(const std::string& str)
{
    if (str == "DAYS") 
    {
        return DAYS;
    }
    else if (str == "HOURS") 
    {
        return HOURS;
    }
    else if (str == "DAYS_HOURS") 
    {
        return DAYS_HOURS;
    }

    return NO_PREFERENCE;
}

class Preference
{
public:
    Preference() = default;
    Preference(const Preference&) = default;
    Preference(Preference&&) = default;

    const std::string &get_description() const;
    const PreferenceType &get_type() const;
    const Designar::SortedArraySet<Days> &get_days() const;
    const Designar::SortedArraySet<uint> &get_hours() const;

    bool set_description(const std::string &description);
    bool set_type(const PreferenceType &type);
    
    bool add_day(const Days &day);
    bool add_hour(const uint &hour);

private:
    std::string description;
    PreferenceType type;
    Designar::SortedArraySet<Days> days;
    Designar::SortedArraySet<uint> hours;
};