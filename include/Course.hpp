#pragma once

#include "Settings.hpp"

class Section;

class Course
{
public:
    // Course(const std::string& name, u_int level, u_int credits,
    //        u_int num_sections, const std::vector<Section*>& sections,
    //        u_int num_weekly_hours, u_int max_daily_hours)
    //     : name(name), level(level), num_credits(credits),
    //       num_sections(num_sections), sections(sections),
    //       num_weekly_hours(num_weekly_hours), max_daily_hours(max_daily_hours) 
    //       {}

    const std::string& get_id() const 
    { 
        return id; 
    }
    const uint& get_level() const 
    { 
        return level; 
    }
    const uint& get_num_credits() const 
    { 
        return num_credits;
    }
    const uint& get_num_sections() const 
    { 
        return num_sections; 
    }
    const Designar::ArraySet<Section*>& get_sections() const 
    { 
        return sections; 
    }
    const uint& get_num_weekly_hours() const 
    { 
        return num_weekly_hours; 
    }
    const uint& get_max_daily_hours() const 
    { 
        return max_daily_hours; 
    }

private:
    std::string id;
    uint level;
    uint num_credits;
    uint num_sections;
    Designar::ArraySet<Section*> sections;
    uint num_weekly_hours;
    uint max_daily_hours;
};