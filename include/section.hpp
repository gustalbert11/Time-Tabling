#pragma once

#include "settings.hpp"

class Professor;
class Course;

class Section
{
public:
    Section();
    Section(const Section&);

    const std::string &get_id() const;
    Professor* get_professor() const;
    Course* get_course() const;
    const Designar::ArraySet<Days> &get_days() const;
    const Designar::ArraySet<std::pair<uint, uint>> &get_hours() const;

    bool set_professor(Professor* professor);
    bool set_course(Course* course);
    
    bool add_day(const Days &day);
    bool add_hour(const uint &start, const uint &end);

private:
    std::string id;
    Professor* professor;
    Course* course;
    std::pair<Designar::ArraySet<Days>, Designar::ArraySet<std::pair<uint, uint>>> time_slots;

    static uint num_sections;

    bool set_id();
};
