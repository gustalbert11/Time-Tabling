#pragma once

#include "preference.hpp"

class Section;

class Professor
{
public:
    Professor();
    Professor(const Professor&);

    const std::string &get_id() const;
    const std::string &get_name() const;
    const uint &get_num_sections() const;
    const Designar::ArraySet<Section*> &get_sections() const;
    const uint &get_max_daily_hours() const;
    const uint &get_max_consecutive_hours() const;
    Preference* get_preference() const;

    bool set_name(const std::string &name);
    bool set_num_sections(const uint &num_sections);
    bool set_max_daily_hours(const uint &max_daily_hours);
    bool set_max_consecutive_hours(const uint &max_consecutive_hours);
    bool set_preference(std::unique_ptr<Preference> preference);
    
    bool add_section(Section* section);

private:
    std::string id;
    std::string name;
    uint num_sections;
    Designar::ArraySet<Section*> sections;
    uint max_daily_hours;
    uint max_consecutive_hours;
    std::unique_ptr<Preference> preference;

    static uint num_professors;

    bool set_id();
};
