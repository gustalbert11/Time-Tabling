#include "flow_node.hpp"
#include "section.hpp"

class SectionDemandNode : public FlowNode 
{
public:
    SectionDemandNode(const std::string& id, Section* section, const std::pair<uint, uint>& hour);

    Section* get_section() const;
    const std::pair<uint, uint> &get_hour() const;

    bool set_section(Section* section);
    bool set_hour(const uint &start, const uint &end);

private:
    Section* section;
    std::pair<uint, uint> hour;
};