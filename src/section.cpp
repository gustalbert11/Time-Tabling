#include "../include/section.hpp"

uint Section::num_sections = 0;

Section::Section()
: professor(nullptr),
  course(nullptr)
{
    set_id();
}
Section::Section(const Section& other)
: id(other.id),
  professor(other.professor),
  course(other.course),
  time_slots(other.time_slots)
{

}

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
const Designar::ArraySet<Days>& Section::get_days() const 
{ 
    return time_slots.first; 
}
const Designar::ArraySet<std::pair<uint, uint>>& Section::get_hours() const 
{ 
    return time_slots.second; 
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
        day > Days::FRIDAY)
    {
        return false;
    }
    time_slots.first.append(day);
    return true;
}
bool Section::add_hour(const uint &start, const uint &end)
{
    if (start >= end || 
        end > MAX_DAILY_HOURS)
    {
        return false;
    }

    auto hours_interval = std::make_pair(start, end);
    if (time_slots.second.contains(hours_interval))
    {
        return false;
    }
    
    time_slots.second.append(hours_interval);
    return true;
}

bool Section::set_id()
{
    this->id = "SEC_" + std::to_string(++num_sections);
    return true;
}