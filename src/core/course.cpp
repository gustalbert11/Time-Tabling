#include "core/course.hpp"

uint Course::num_courses = 0;

Course::Course(const std::string &id,
               const std::string &name,
               const uint &level,
               const uint &num_credits,
               const uint &num_weekly_hours,
               const uint &max_daily_hours)
{
    ++num_courses;
    set_id(id);
    set_name(name);
    set_level(level);
    set_num_credits(num_credits);
    set_num_weekly_hours(num_weekly_hours);
    set_max_daily_hours(max_daily_hours);
}
Course::Course(const Course &other)
: id(id),
  sections(other.sections)
{
    set_name(other.name);
    set_level(other.level);
    set_num_credits(other.num_credits);
    set_num_weekly_hours(other.num_weekly_hours);
    set_max_daily_hours(other.max_daily_hours);
}

std::string Course::get_id() const 
{ 
    return id; 
}
std::string Course::get_name() const 
{ 
    return name; 
}
uint Course::get_level() const 
{ 
    return level; 
}
uint Course::get_num_credits() const 
{ 
    return num_credits;
}
uint Course::get_num_sections() const 
{  
    //return num_sections;
    return sections.size();
}
const Designar::ArraySet<Section*>& Course::get_sections() const 
{ 
    return sections; 
}
uint Course::get_num_weekly_hours() const 
{ 
    return num_weekly_hours; 
}
uint Course::get_max_daily_hours() const 
{ 
    return max_daily_hours; 
}

bool Course::set_name(const std::string &name)
{
    if (name.empty())
    {
        return false;
    }
    this->name = name;
    return true;
}
bool Course::set_level(const uint &level)
{
    if (level < 1 || 
        level > MAX_NUM_LEVELS)
    {
        return false;
    }
    this->level = level;
    return true;
}
bool Course::set_num_credits(const uint &num_credits)
{
    if (num_credits < 1 ||
        num_credits > MAX_NUM_CREDITS)
    {
        return false;
    }
    this->num_credits = num_credits;
    return true;
}
bool Course::set_num_weekly_hours(const uint &num_weekly_hours)
{
    if (num_weekly_hours < 4 || 
        num_weekly_hours > MAX_WEEKLY_HOURS)
    {
        return false;
    }
    this->num_weekly_hours = num_weekly_hours;
    return true;
}
bool Course::set_max_daily_hours(const uint &max_daily_hours)
{
    if (max_daily_hours < 2 || 
        max_daily_hours > MAX_DAILY_HOURS)
    {
        return false;
    }
    this->max_daily_hours = max_daily_hours;
    return true;
}

bool Course::add_section(Section *section)
{
    if (!section ||
        sections.contains(section) ||
        sections.size() >= COURSE_MAX_NUM_SECTIONS)
    {
        return false;
    }
    sections.append(section);
    return true;
}

// bool Course::set_id()
// {
//     this->id = "COURSE_" + std::to_string(++num_courses);
//     return true;
// }
bool Course::set_id(const std::string &forced_id)
{
    //++num_courses;
    if (!forced_id.empty()) 
    {
        this->id = forced_id;
    } 
    else 
    {
        this->id = "COURSE_" + std::to_string(num_courses);
    }
    return true;
}