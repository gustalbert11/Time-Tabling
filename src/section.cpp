#include "../include/section.hpp"

uint Section::num_sections = 0;

Section::Section()
: professor(nullptr),
  course(nullptr),
  time_slots(MAX_NUM_DAYS)
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
Designar::ArraySet<std::pair<Days, std::pair<uint, uint>>> const &Section::get_time_slots() const
{
    return time_slots;
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

bool Section::add_time_slot(const Days& day, const uint &start, const uint &end)
{
    if (day < Days::MONDAY || 
        day > Days::FRIDAY ||
        start >= end || 
        end > MAX_DAILY_HOURS)
    {
        return false;
    }

    auto time_slot = std::make_pair(day, std::make_pair(start, end));
    if (time_slots.contains(time_slot))
    {
        return false;
    }
    
    time_slots.append(time_slot);
    return true;
}

bool Section::set_id()
{
    this->id = "SEC_" + std::to_string(++num_sections);
    return true;
}