#pragma once

#include "Settings.hpp"

class Professor;
class Course;

class Section
{
public:

    const std::string &get_id() const 
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
    const Designar::SortedArraySet<Days> &get_days() const 
    { 
        return time_slot.first; 
    }
    const Designar::SortedArraySet<uint> &get_hours() const 
    { 
        return time_slot.second;
    }

    bool set_id(const std::string &id)
    {
        if (id.empty())
        {
            return false;
        }
        this->id = id;
        return true;
    }
    bool set_professor(Professor* professor)
    {
        if (professor == nullptr)
        {
            return false;
        }
        this->professor = professor;
        return true;
    }
    bool set_course(Course* course)
    {
        if (course == nullptr)
        {
            return false;
        }
        this->course = course;
        return true;
    }
    bool add_day(const Days &day)
    {
        if (day < Days::MONDAY || 
            day > Days::FRIDAY ||
            time_slot.first.contains(day))
        {
            return false;
        }
        time_slot.first.append(day);
        return true;
    }
    bool add_hour(const uint &hour)
    {
        //if (time_slot.second.size() == professor.MAX_DAILY_HOURS ||
        if (hour < 1 || 
            hour > 7 || 
            time_slot.second.size() == 7 || 
            time_slot.second.contains(hour))
        {
            return false;
        }
        time_slot.second.append(hour);
        return true;
    }

private:
    std::string id;
    Professor* professor;
    Course* course;
    std::pair<Designar::SortedArraySet<Days>, Designar::SortedArraySet<uint>> time_slot;
};