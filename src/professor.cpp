#include "../include/professor.hpp"

uint Professor::num_professors = 0;

Professor::Professor()
: name(""),
  num_sections(PROF_MAX_NUM_SECTIONS),
  sections(PROF_MAX_NUM_SECTIONS),
  max_daily_hours(MAX_DAILY_HOURS),
  max_consecutive_hours(MAX_CONSECUTIVE_HOURS),
  preference(nullptr)
{
    set_id();
}
Professor::Professor(const Professor& other)
: id(other.id),
  sections(other.sections)
{
    set_name(other.name);
    set_num_sections(other.num_sections);
    set_max_daily_hours(other.max_daily_hours);
    set_max_consecutive_hours(other.max_consecutive_hours);
    //set_preference(std::make_unique<Preference>(*other.preference));
}

const std::string& Professor::get_id() const 
{ 
    return id; 
}
const std::string& Professor::get_name() const 
{ 
    return name; 
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
Preference* Professor::get_preference() const 
{ 
    return preference.get(); 
}

bool Professor::set_name(const std::string &name)
{
    if (name.empty())
    {
        return false;
    }
    this->name = name;
    return true;
}
bool Professor::set_num_sections(const uint &num_sections)
{
    if (num_sections < 1 || 
        num_sections > PROF_MAX_NUM_SECTIONS)
    {
        return false;
    }
    this->num_sections = num_sections;
    return true;
}
bool Professor::set_max_daily_hours(const uint &max_daily_hours)
{
    if (max_daily_hours < 2 || 
        max_daily_hours > MAX_DAILY_HOURS)
    {
        return false;
    }
    this->max_daily_hours = max_daily_hours;
    return true;
}
bool Professor::set_max_consecutive_hours(const uint &max_consecutive_hours)
{
    if (max_consecutive_hours < 2 || 
        max_consecutive_hours >= this->max_daily_hours ||
        max_consecutive_hours > MAX_CONSECUTIVE_HOURS)
    {
        return false;
    }
    this->max_consecutive_hours = max_consecutive_hours;
    return true;
}
bool Professor::set_preference(std::unique_ptr<Preference> preference)
{
    if (!preference)
    {
        return false;
    }
    
    this->preference = std::move(preference);
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

bool Professor::set_id()
{
    this->id = "PROF_" + std::to_string(++num_professors);
    return true;
}
