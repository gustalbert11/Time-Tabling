#include "../include/section_demand_node.hpp"

SectionDemandNode::SectionDemandNode(const std::string& id, Section* section, const std::pair<uint, uint>& hour)
: FlowNode(id, FlowNodeType::SECTION_DEMAND),
    section(section), hour(hour)
{

}

Section* SectionDemandNode::get_section() const
{
    return section;
}
const std::pair<uint, uint>& SectionDemandNode::get_hour() const
{
    return hour;
}

bool SectionDemandNode::set_section(Section* section)
{
    if (!section)
    {
        return false;
    }
    this->section = section;
    return true;
}
bool SectionDemandNode::set_hour(const uint &start, const uint &end)
{
    if (start >= end ||
        end > MAX_DAILY_HOURS)
    {
        return false;
    }
    
    auto hours_interval = std::make_pair(start, end);
    if (section->get_time_slots().none([hours_interval](auto pair)
    {
        pair.second == hours_interval;
    }))
    {
        return false;
    }

    this->hour = hours_interval;
    return true;
}