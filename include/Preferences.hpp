#pragma once

#include "Settings.hpp"

enum PreferenceType { NO_PREFERENCE, DAYS, HOURS, DAYS_HOURS };

class Preferences
{
public:
    // Preferences(const std::string& description, PreferenceType type,
    //             const std::vector<Days>& days,
    //             const std::vector<uint>& hours)
    //     : description(description), type(type), days(days), hours(hours)
    //       {}

    const std::string& get_description() const 
    { 
        return description; 
    }
    const PreferenceType& get_type() const 
    { 
        return type; 
    }
    const std::vector<Days>& get_days() const 
    { 
        return days; 
    }
    const std::vector<uint>& get_hours() const 
    { 
        return hours; 
    }

private:
    std::string description;
    PreferenceType type;
    std::vector<Days> days;
    std::vector<uint> hours;
    //Use a container from DeSiGNAR library
};