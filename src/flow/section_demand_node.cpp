#include "flow/section_demand_node.hpp"

SectionDemandNode::SectionDemandNode(std::string id, Section* section, uint hour_index)
: FlowNode(id, FlowNodeType::SECTION_DEMAND)
{
    set_section(section);
    set_hour_index(hour_index);
}

Section* SectionDemandNode::get_section() const
{
    return section;
}
uint SectionDemandNode::get_hour_index() const
{
    return hour_index;
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
bool SectionDemandNode::set_hour_index(const uint& hour_index)
{
    this->hour_index = hour_index;
    return true;
}