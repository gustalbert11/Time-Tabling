#pragma once

#include "Settings.hpp"

class Section;
class Preferences;

class Professor
{
public:
    const uint MAX_NUM_SECTIONS = 5;
    const uint MAX_DAILY_HOURS = 7;
    const uint MAX_CONSECUTIVE_HOURS = 2;

    const std::string &get_id() const 
    { 
        return id; 
    }
    const uint &get_num_sections() const 
    { 
        return num_sections; 
    }
    const Designar::ArraySet<Section*> &get_sections() const 
    { 
        return sections; 
    }
    const uint &get_max_daily_hours() const 
    { 
        return max_daily_hours; 
    }
    const uint &get_max_consecutive_hours() const 
    { 
        return max_consecutive_hours; 
    }
    const Designar::ArraySet<Preferences*> &get_preferences() const 
    { 
        return preferences; 
    }

    bool set_id(const std::string &id)
    {
        if (id.empty())
        {
            return false;
        }
        this->id = id;
        return true;
    }
    bool set_num_sections(const uint &num_sections)
    {
        if (num_sections < 1 || 
            num_sections > MAX_NUM_SECTIONS)
        {
            return false;
        }
        this->num_sections = num_sections;
        return true;
    }
    bool add_section(Section* section)
    {
        if (section == nullptr || 
            sections.size() == num_sections || 
            sections.contains(section))
        {
            return false;
        }
        sections.append(section);
        return true;
    }
    bool set_max_daily_hours(const uint &max_daily_hours)
    {
        if (max_daily_hours < 1 || 
            max_daily_hours > MAX_DAILY_HOURS)
        {
            return false;
        }
        this->max_daily_hours = max_daily_hours;
        return true;
    }
    bool set_max_consecutive_hours(const uint &max_consecutive_hours)
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

private:
    std::string id;
    uint num_sections;
    Designar::ArraySet<Section*> sections;
    uint max_daily_hours;
    uint max_consecutive_hours;
    Designar::ArraySet<Preferences*> preferences;
};