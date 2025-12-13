#pragma once

#include "flow/flow_node.hpp"
#include "core/professor.hpp"

class ProfessorTimeNode : public FlowNode 
{
public:
    ProfessorTimeNode(std::string id, Professor* professor, Days day, uint start_hour, uint end_hour);

    const Professor* get_professor() const;
    Days get_day() const;
    std::pair<uint, uint> get_hours_interval() const;

    bool set_professor(Professor* professor);
    bool set_day(const Days &day);
    bool set_hours_interval(const uint &start_hour, const uint &end_hour);

private:
    Professor* professor;
    Days day;
    std::pair<uint, uint> hours_interval;
};