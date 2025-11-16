#pragma once

#include "settings.hpp"

class Professor;
class Course;

class Section
{
public:
    const std::string &get_id() const;
    Professor* get_professor() const;
    Course* get_course() const;
    const Designar::SortedArraySet<Days> &get_days() const;
    const Designar::SortedArraySet<uint> &get_hours() const;

    bool set_id(const std::string &id);
    bool set_professor(Professor* professor);
    bool set_course(Course* course);
    bool add_day(const Days &day);
    bool add_hour(const uint &hour);

private:
    std::string id;
    Professor* professor;
    Course* course;
    std::pair<Designar::SortedArraySet<Days>, Designar::SortedArraySet<uint>> time_slot;
};