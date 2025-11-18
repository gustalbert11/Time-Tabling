#include "../include/preference.hpp"

const std::string& Preference::get_description() const 
{ 
    return description; 
}
const PreferenceType& Preference::get_type() const 
{ 
    return type; 
}
const Designar::SortedArraySet<Days>& Preference::get_days() const 
{ 
    return days; 
}
const Designar::SortedArraySet<uint>& Preference::get_hours() const 
{ 
    return hours; 
}

bool Preference::set_description(const std::string &description)
{
    if (description.empty())
    {
        return false;
    }
    this->description = description;
    return true;
}
bool Preference::set_type(const PreferenceType &type)
{
    if (type < NO_PREFERENCE || 
        type > DAYS_HOURS)
    {
        return false;
    }
    this->type = type;
    return true;
}
bool Preference::add_day(const Days &day)
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
bool Preference::add_hour(const uint &hour)
{
    if (hour < 1 || 
        hour > MAX_DAILY_HOURS || 
        hours.size() == MAX_DAILY_HOURS || 
        hours.contains(hour))
    {
        return false;
    }
    hours.append(hour);
    return true;
}