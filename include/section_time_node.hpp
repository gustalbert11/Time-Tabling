#include "section_demand_node.hpp"

class SectionTimeNode : public FlowNode 
{
public:
    SectionTimeNode(std::string id, Section* section, Days day, uint start_hour, uint end_hour);

    Section* get_section() const;
    const Days &get_day() const;
    const std::pair<uint, uint> &get_hour() const;

    bool set_section(Section* section);
    bool set_day(const Days &day);
    bool set_hour(const uint &start_hour, const uint &end_hour);

private:
    Section* section;
    Days day;
    std::pair<uint, uint> hour;
};