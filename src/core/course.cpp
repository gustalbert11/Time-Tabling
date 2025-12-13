#include "core/course.hpp"

uint Course::num_courses = 0;

Course::Course()
: name(""),
  level(MAX_NUM_LEVELS),
  num_credits(MAX_NUM_CREDITS),
  num_sections(COURSE_MAX_NUM_SECTIONS),
  sections(COURSE_MAX_NUM_SECTIONS),
  num_weekly_hours(MAX_WEEKLY_HOURS),
  max_daily_hours(MAX_DAILY_HOURS)
{
    set_id();
}

Course::Course(const Course& other)
: id(id),
  sections(other.sections)
{
    set_name(other.name);
    set_level(other.level);
    set_num_credits(other.num_credits);
    set_num_sections(other.num_sections);
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
    return sections.size(); 
}
const Designar::ArraySet<const Section*>& Course::get_sections() const 
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
bool Course::set_num_sections(const uint &num_sections)
{
    if (num_sections < 1 || 
        num_sections > COURSE_MAX_NUM_SECTIONS)
    {
        return false;
    }
    this->num_sections = num_sections;
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

bool Course::add_section(Section* section)
{
    if (!section ||
        sections.contains(section))
    {
        return false;
    }
    sections.append(section);
    return true;
}

bool Course::set_id()
{
    this->id = "COURSE_" + std::to_string(++num_courses);
    return true;
}