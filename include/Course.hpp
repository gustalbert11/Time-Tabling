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
    const u_int& get_level() const 
    { 
        return level; 
    }
    const u_int& get_num_credits() const 
    { 
        return num_credits;
    }
    const u_int& get_num_sections() const 
    { 
        return num_sections; 
    }
    const std::vector<Section*>& get_sections() const 
    { 
        return sections; 
    }
    const u_int& get_num_weekly_hours() const 
    { 
        return num_weekly_hours; 
    }
    const u_int& get_max_daily_hours() const 
    { 
        return max_daily_hours; 
    }

private:
    std::string id;
    u_int level;
    u_int num_credits;
    u_int num_sections;
    std::vector<Section*> sections;
    //Use a container from DeSiGNAR library
    u_int num_weekly_hours;
    u_int max_daily_hours;
};