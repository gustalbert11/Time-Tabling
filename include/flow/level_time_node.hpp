#pragma once

#include "flow_node.hpp"

class LevelTimeNode : public FlowNode 
{
public:
    LevelTimeNode(std::string id, uint level, Days day, uint start_hour, uint end_hour);
    
    uint get_level() const;
    Days get_day() const;
    std::pair<uint, uint> get_hour_interval() const;

    bool set_level(const uint &level);
    bool set_day(const Days &day);
    bool set_hour_interval(const uint &start_hour, const uint &end_hour);

private:
    uint level;
    Days day;
    std::pair<uint, uint> hour_interval;
};