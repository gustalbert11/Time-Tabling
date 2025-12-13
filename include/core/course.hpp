#pragma once

#include "settings.hpp"

class Section;

class Course
{
public:
    Course();
    Course(const Course&);

    std::string get_id() const;
    std::string get_name() const;
    uint get_level() const;
    uint get_num_credits() const;
    uint get_num_sections() const;
    const Designar::ArraySet<const Section*> &get_sections() const;
    uint get_num_weekly_hours() const;
    uint get_max_daily_hours() const;

    bool set_name(const std::string &name);
    bool set_level(const uint &level);
    bool set_num_credits(const uint &num_credits);
    bool set_num_sections(const uint &num_sections);
    bool set_num_weekly_hours(const uint &num_weekly_hours);
    bool set_max_daily_hours(const uint &max_daily_hours);

    bool add_section(Section* section);
    // bool remove_section(Section* section);

private:
    std::string id;
    std::string name;
    uint level;
    uint num_credits;
    uint num_sections;
    Designar::ArraySet<const Section*> sections;
    uint num_weekly_hours;
    uint max_daily_hours;

    static uint num_courses;

    bool set_id();
};
