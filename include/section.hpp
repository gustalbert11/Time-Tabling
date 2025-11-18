#pragma once

#include "settings.hpp"

extern uint num_sections;

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
    const Designar::SortedArraySet<std::pair<Days, uint>> &get_time_slots() const;

    bool set_professor(Professor* professor);
    bool set_course(Course* course);
    bool add_time_slot(const Days& day, const uint& hour);

private:
    std::string id;
    Professor* professor;
    Course* course;
    Designar::SortedArraySet<std::pair<Days, uint>> time_slots;

    bool set_id();
};
