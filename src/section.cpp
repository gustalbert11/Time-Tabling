#include "../include/section.hpp"

const std::string& Section::get_id() const 
{ 
    return id; 
}
Professor* Section::get_professor() const 
{ 
    return professor; 
}
Course* Section::get_course() const 
{ 
    return course; 
}
const Designar::SortedArraySet<Days>& Section::get_days() const 
{ 
    return time_slot.first; 
}
const Designar::SortedArraySet<uint>& Section::get_hours() const 
{ 
    return time_slot.second;
}

bool Section::set_id(const std::string &id)
{
    if (id.empty())
    {
        return false;
    }
    this->id = id;
    return true;
}
bool Section::set_professor(Professor* professor)
{
    if (!professor)
    {
        return false;
    }
    this->professor = professor;
    return true;
}
bool Section::set_course(Course* course)
{
    if (!course)
    {
        return false;
    }
    this->course = course;
    return true;
}
bool Section::add_day(const Days &day)
{
    if (day < Days::MONDAY || 
        day > Days::FRIDAY ||
        time_slot.first.contains(day))
    {
        return false;
    }
    time_slot.first.append(day);
    return true;
}
bool Section::add_hour(const uint &hour)
{
    if (hour < 1 || 
        hour > MAX_DAILY_HOURS ||
        time_slot.second.size() == MAX_DAILY_HOURS || 
        time_slot.second.contains(hour))
    {
        return false;
    }
    time_slot.second.append(hour);
    return true;
}