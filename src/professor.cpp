#include "../include/professor.hpp"

const std::string& Professor::get_id() const 
{ 
    return id; 
}
const uint& Professor::get_num_sections() const 
{ 
    return num_sections; 
}
const Designar::ArraySet<Section*>& Professor::get_sections() const 
{ 
    return sections; 
}
const uint& Professor::get_max_daily_hours() const 
{ 
    return max_daily_hours; 
}
const uint& Professor::get_max_consecutive_hours() const 
{ 
    return max_consecutive_hours; 
}
const Designar::ArraySet<Preferences*>& Professor::get_preferences() const 
{ 
    return preferences; 
}

bool Professor::set_id(const std::string &id)
{
    if (id.empty())
    {
        return false;
    }
    this->id = id;
    return true;
}
bool Professor::set_num_sections(const uint &num_sections)
{
    if (num_sections < 1 || 
        num_sections > MAX_NUM_SECTIONS)
    {
        return false;
    }
    this->num_sections = num_sections;
    return true;
}
bool Professor::set_max_daily_hours(const uint &max_daily_hours)
{
    if (max_daily_hours < 1 || 
        max_daily_hours > MAX_DAILY_HOURS)
    {
        return false;
    }
    this->max_daily_hours = max_daily_hours;
    return true;
}
bool Professor::set_max_consecutive_hours(const uint &max_consecutive_hours)
{
    if (max_consecutive_hours < 1 || 
        max_consecutive_hours >= this->max_daily_hours ||
        max_consecutive_hours > MAX_CONSECUTIVE_HOURS)
    {
        return false;
    }
    this->max_consecutive_hours = max_consecutive_hours;
    return true;
}

bool Professor::add_section(Section* section)
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