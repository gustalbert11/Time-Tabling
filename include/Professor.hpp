#pragma once

#include "Settings.hpp"

class Section;
class Preferences;

class Professor
{
public:
    // Professor(const std::string& name, u_int num_sections,
    //           const std::vector<Section*>& sections,
    //           u_int max_daily_hours, u_int max_consecutive_hours)
    //     : name(name), num_sections(num_sections), sections(sections),
    //       max_daily_hours(max_daily_hours), max_consecutive_hours(max_consecutive_hours)
    //       {}

    const std::string& get_id() const 
    { 
        return id; 
    }
    const uint& get_num_sections() const 
    { 
        return num_sections; 
    }
    const Designar::ArraySet<Section*>& get_sections() const 
    { 
        return sections; 
    }
    const uint& get_max_daily_hours() const 
    { 
        return max_daily_hours; 
    }
    const uint& get_max_consecutive_hours() const 
    { 
        return max_consecutive_hours; 
    }
    const Designar::ArraySet<Preferences*>& get_preferences() const 
    { 
        return preferences; 
    }

private:
    std::string id;
    uint num_sections;
    Designar::ArraySet<Section*> sections;
    uint max_daily_hours;
    uint max_consecutive_hours;
    Designar::ArraySet<Preferences*> preferences;
};