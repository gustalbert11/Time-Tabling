#pragma once

#include "settings.hpp"

enum PreferenceType 
{ 
    NO_PREFERENCE, 
    DAYS, 
    HOURS, 
    DAYS_HOURS 
};

class Preferences
{
public:
    Preferences() = default;
    Preferences(const Preferences&) = default;
    Preferences(Preferences&&) = default;

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