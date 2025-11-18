#pragma once

#include "preference.hpp"

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
    const Preference* get_preference() const;

    bool set_id(const std::string &id);
    bool set_num_sections(const uint &num_sections);
    bool set_max_daily_hours(const uint &max_daily_hours);
    bool set_max_consecutive_hours(const uint &max_consecutive_hours);
    bool set_preference(std::unique_ptr<Preference> preference);
    
    bool add_section(Section* section);
    //bool add_preference(std::unique_ptr<Preference> preference);

private:
    std::string id;
    uint num_sections;
    Designar::ArraySet<Section*> sections;
    uint max_daily_hours;
    uint max_consecutive_hours;
    std::unique_ptr<Preference> preference;
};