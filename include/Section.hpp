#pragma once

#include "Settings.hpp"

class Professor;
class Course;

class Section
{
public:
    // Section(const std::string& id, Professor* professor, Course* course,
    //         const std::pair<u_int, u_int>& time_slot)
    //     : id(id), professor(professor), course(course), time_slot(time_slot)
    //       {}

    const std::string& get_id() const 
    { 
        return id; 
    }
    Professor* get_professor() const 
    { 
        return professor; 
    }
    Course* get_course() const 
    { 
        return course; 
    }
    const Designar::SortedArraySet<Days>& get_days() const 
    { 
        return time_slot.first; 
    }
    const Designar::SortedArraySet<uint>& get_hours() const 
    { 
        return time_slot.second;
    }

private:
    std::string id;
    Professor* professor;
    Course* course;
    std::pair<Designar::SortedArraySet<Days>, Designar::SortedArraySet<uint>> time_slot;
};