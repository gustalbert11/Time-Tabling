#include "../include/section_time_node.hpp"

SectionTimeNode::SectionTimeNode(std::string id, Section* section, Days day, uint start_hour, uint end_hour)
: FlowNode(id, FlowNodeType::SECTION_TIME),
  section(section), day(day)
{
    set_hour(start_hour, end_hour);
}

Section* SectionTimeNode::get_section() const
{
    return section;
}
const Days& SectionTimeNode::get_day() const
{
    return day;
}
const std::pair<uint, uint>& SectionTimeNode::get_hour() const
{
    return hour;
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
bool SectionTimeNode::set_hour(const uint &start_hour, const uint &end_hour)
{
    if (start_hour >= end_hour ||
        end_hour > MAX_DAILY_HOURS)
    {
        return false;
    }

    this->hour = std::make_pair(start_hour, end_hour);
    return true;
}