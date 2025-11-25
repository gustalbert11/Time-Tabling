#include "flow/professor_time_node.hpp"

ProfessorTimeNode::ProfessorTimeNode(std::string id, Professor* professor, Days day, uint start_hour, uint end_hour)
: FlowNode(id, FlowNodeType::PROFESSOR_TIME),
  professor(professor), day(day)
{
    set_hour_interval(start_hour, end_hour);
}

Professor* ProfessorTimeNode::get_professor() const
{
    return professor;
}
const Days& ProfessorTimeNode::get_day() const
{
    return day;
}
const std::pair<uint, uint>& ProfessorTimeNode::get_hour_interval() const
{
    return hour_interval;
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
bool ProfessorTimeNode::set_hour_interval(const uint &start_hour, const uint &end_hour)
{
    if (start_hour >= end_hour ||
        end_hour > MAX_DAILY_HOURS)
    {
        return false;
    }

    this->hour_interval = std::make_pair(start_hour, end_hour);
    return true;
}