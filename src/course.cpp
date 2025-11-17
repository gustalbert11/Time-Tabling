#include "../include/course.hpp"

const std::string& Course::get_id() const 
{ 
    return id; 
}
const uint& Course::get_level() const 
{ 
    return level; 
}
const uint& Course::get_num_credits() const 
{ 
    return num_credits;
}
const uint& Course::get_num_sections() const 
{ 
    return num_sections; 
}
const Designar::ArraySet<Section*>& Course::get_sections() const 
{ 
    return sections; 
}
const uint& Course::get_num_weekly_hours() const 
{ 
    return num_weekly_hours; 
}
const uint& Course::get_max_daily_hours() const 
{ 
    return max_daily_hours; 
}

bool Course::set_id(const std::string &id)
{
    if (id.empty())
    {
        return false;
    }
    this->id = id;
    return true;
}
bool Course::set_level(const uint &level)
{
    if (level < 1 || 
        level > MAX_NUM_LEVEL)
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
        num_sections > MAX_NUM_SECTIONS)
    {
        return false;
    }
    this->num_sections = num_sections;
    return true;
}
bool Course::set_num_weekly_hours(const uint &num_weekly_hours)
{
    if (num_weekly_hours < 1 || 
        num_weekly_hours > MAX_WEEKLY_HOURS)
    {
        return false;
    }
    this->num_weekly_hours = num_weekly_hours;
    return true;
}
bool Course::set_max_daily_hours(const uint &max_daily_hours)
{
    if (max_daily_hours < 1 || 
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
        sections.size() == num_sections || 
        sections.contains(section))
    {
        return false;
    }
    sections.append(section);
    return true;
}
// bool Course::remove_section(Section* section)
// {
//     if (section == nullptr)
//     {
//         return false;
//     }
//     for (nat_t i = 0; i < sections.size(); ++i)
//     {
//         if (sections.nth(i) == section)
//         {
//             sections.remove_pos(i);
//             return true;
//         }
//     }
//     return false;
// }