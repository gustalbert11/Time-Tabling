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
    return time_slots.first; 
}
const Designar::ArraySet<std::pair<uint, uint>>& Preference::get_hours() const 
{ 
    return time_slots.second; 
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

// En preference.cpp - verificar implementaciones
bool Preference::add_day(const Days &day)
{
    if (day < Days::MONDAY || day > Days::FRIDAY) {
        return false;
    }
    
    // CORREGIR: La condición estaba incorrecta
    if (type != DAYS && type != DAYS_HOURS) {
        return false;
    }
    
    time_slots.first.append(day);
    return true;
}

bool Preference::add_hour(const uint &start_hour, const uint &end_hour)
{
    if (start_hour >= end_hour || end_hour > MAX_END_HOUR) {
        return false;
    }
    
    // CORREGIR: La condición estaba incorrecta  
    if (type != HOURS && type != DAYS_HOURS) {
        return false;
    }

    auto hours_interval = std::make_pair(start_hour, end_hour);
    if (time_slots.second.contains(hours_interval)) {
        return false;
    }
    
    time_slots.second.append(hours_interval);
    return true;
}
