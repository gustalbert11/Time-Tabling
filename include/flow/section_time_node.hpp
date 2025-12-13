#pragma once

#include "flow/section_demand_node.hpp"

class SectionTimeNode : public FlowNode 
{
public:
    SectionTimeNode(std::string id, Section* section, Days day, uint start_hour, uint end_hour);

    const Section* get_section() const;
    Days get_day() const;
    std::pair<uint, uint> get_hours_interval() const;

    bool set_section(Section* section);
    bool set_day(const Days &day);
    bool set_hours_interval(const uint &start_hour, const uint &end_hour);

private:
    Section* section;
    Days day;
    std::pair<uint, uint> hours_interval;
};