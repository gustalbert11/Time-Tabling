#pragma once

#include "settings.hpp"

class Section;

class Course
{
public:
    Course() = default;
    Course(const Course&) = default;
    Course(Course&&) = default;

    const std::string &get_id() const;
    const uint &get_level() const;
    const uint &get_num_credits() const;
    const uint &get_num_sections() const;
    const Designar::ArraySet<Section*> &get_sections() const;
    const uint &get_num_weekly_hours() const;
    const uint &get_max_daily_hours() const;

    bool set_id(const std::string &id);
    bool set_level(const uint &level);
    bool set_num_credits(const uint &num_credits);
    bool set_num_sections(const uint &num_sections);
    
    bool set_num_weekly_hours(const uint &num_weekly_hours);
    bool set_max_daily_hours(const uint &max_daily_hours);

    bool add_section(Section* section);
    // bool remove_section(Section* section);

private:

    std::string id;
    uint level;
    uint num_credits;
    uint num_sections;
    Designar::ArraySet<Section*> sections;
    uint num_weekly_hours;
    uint max_daily_hours;
};