#include "flow/professor_time_node.hpp"

ProfessorTimeNode::ProfessorTimeNode(std::string id, Professor* professor, Days day, uint start_hour, uint end_hour)
: FlowNode(id, FlowNodeType::PROFESSOR_TIME)
{
    set_professor(professor);
    set_day(day);
    set_hours_interval(start_hour, end_hour);
}

const Professor* ProfessorTimeNode::get_professor() const
{
    return professor;
}
Days ProfessorTimeNode::get_day() const
{
    return day;
}
std::pair<uint, uint> ProfessorTimeNode::get_hours_interval() const
{
    return hours_interval;
}

bool ProfessorTimeNode::set_professor(Professor* professor)
{
    if (!professor)
    {
        return false;
    }
    
    this->professor = professor;
    return true;
}
bool ProfessorTimeNode::set_day(const Days &day)
{
    if (day < Days::MONDAY || 
        day > Days::FRIDAY)
    {
        return false;
    }
    
    this->day = day;
    return true;
}
bool ProfessorTimeNode::set_hours_interval(const uint &start_hour, const uint &end_hour)
{
    if (start_hour < MIN_START_HOUR || 
        start_hour >= end_hour ||
        end_hour > MAX_DAILY_HOURS)
    {
        return false;
    }

    this->hours_interval = std::make_pair(start_hour, end_hour);
    return true;
}