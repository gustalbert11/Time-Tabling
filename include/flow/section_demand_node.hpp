#pragma once

#include "flow_node.hpp"
#include "section.hpp"

class SectionDemandNode : public FlowNode 
{
public:
    SectionDemandNode(std::string id, Section* section, uint hour_index);

    Section* get_section() const;
    uint get_hour_index() const;

    bool set_section(Section* section);
    bool set_hour_index(const uint& hour_index);

private:
    Section* section;
    uint hour_index;
};