#include "flow/section_time_node.hpp"

SectionTimeNode::SectionTimeNode(std::string id, Section* section, Days day, uint start_hour, uint end_hour)
: FlowNode(id, FlowNodeType::SECTION_TIME)
{
    set_section(section);
    set_day(day);
    set_hours_interval(start_hour, end_hour);
}

const Section* SectionTimeNode::get_section() const
{
    return section;
}
Days SectionTimeNode::get_day() const
{
    return day;
}
std::pair<uint, uint> SectionTimeNode::get_hours_interval() const
{
    return hours_interval;
}

bool SectionTimeNode::set_section(Section* section)
{
    if (!section)
    {
        return false;
    }
    
    this->section = section;
    return true;
}
bool SectionTimeNode::set_day(const Days &day)
{
    if (day < Days::MONDAY || 
        day > Days::FRIDAY)
    {
        return false;
    }
    
    this->day = day;
    return true;
}
bool SectionTimeNode::set_hours_interval(const uint &start_hour, const uint &end_hour)
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