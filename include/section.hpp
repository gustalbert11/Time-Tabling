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
    const Designar::ArraySet<std::pair<Days, std::pair<uint, uint>>> &get_time_slots() const;

    bool set_professor(Professor* professor);
    bool set_course(Course* course);

    bool add_time_slot(const Days& day, const uint &start, const uint &end);

private:
    std::string id;
    Professor* professor;
    Course* course;
    Designar::ArraySet<std::pair<Days, std::pair<uint, uint>>> time_slots;

    static uint num_sections;

    bool set_id();
};
