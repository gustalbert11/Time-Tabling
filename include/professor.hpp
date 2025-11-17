#pragma once

#include "preferences.hpp"

class Section;

class Professor
{
public:
    Professor() = default;
    Professor(const Professor&) = default;
    Professor(Professor&&) = default;

    const std::string &get_id() const;
    const uint &get_num_sections() const;
    const Designar::ArraySet<Section*> &get_sections() const;
    const uint &get_max_daily_hours() const;
    const uint &get_max_consecutive_hours() const;
    const Designar::ArraySet<Preferences*> &get_preferences() const;

    bool set_id(const std::string &id);
    bool set_num_sections(const uint &num_sections);
    bool set_max_daily_hours(const uint &max_daily_hours);
    bool set_max_consecutive_hours(const uint &max_consecutive_hours);
    
    bool add_section(Section* section);

private:
    std::string id;
    uint num_sections;
    Designar::ArraySet<Section*> sections;
    uint max_daily_hours;
    uint max_consecutive_hours;
    Designar::ArraySet<Preferences*> preferences;
};