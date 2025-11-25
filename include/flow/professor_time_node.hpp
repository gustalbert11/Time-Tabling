#pragma once

#include "flow_node.hpp"
#include "professor.hpp"

class ProfessorTimeNode : public FlowNode 
{
public:
    ProfessorTimeNode(std::string id, Professor* professor, Days day, uint start_hour, uint end_hour);

    Professor* get_professor() const;
    const Days &get_day() const;
    const std::pair<uint, uint> &get_hour_interval() const;

    bool set_professor(Professor* professor);
    bool set_day(const Days &day);
    bool set_hour_interval(const uint &start_hour, const uint &end_hour);

private:
    Professor* professor;
    Days day;
    std::pair<uint, uint> hour_interval;
};