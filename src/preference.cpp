#include "../include/preference.hpp"

const std::string& Preference::get_description() const 
{ 
    return description; 
}
const PreferenceType& Preference::get_type() const 
{ 
    return type; 
}
const Designar::ArraySet<Days>& Preference::get_days() const 
{ 
    return days_hours_pair.first; 
}
const Designar::ArraySet<std::pair<uint, uint>>& Preference::get_hours() const 
{ 
    return days_hours_pair.second; 
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
        type != DAYS ||
        type != DAYS_HOURS)
    {
        return false;
    }
    days_hours_pair.first.append(day);
    return true;
}
bool Preference::add_hour(const uint &start, const uint &end)
{
    if (type != HOURS ||
        type != DAYS_HOURS)
    {
        return false;
    }

    auto hours_interval = std::make_pair(start, end);
    if (days_hours_pair.second.contains(hours_interval))
    {
        return false;
    }
    
    days_hours_pair.second.append(hours_interval);
    return true;
}
