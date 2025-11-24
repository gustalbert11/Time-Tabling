#include "../include/level_time_node.hpp"

LevelTimeNode::LevelTimeNode(std::string id, uint level, Days day, uint start_hour, uint end_hour)
: FlowNode(id, FlowNodeType::LEVEL_TIME)
{
    set_level(level);
    set_day(day);
    set_hour_interval(start_hour, end_hour);
}

uint LevelTimeNode::get_level() const
{
    return level;
}
Days LevelTimeNode::get_day() const
{
    return day;
}
std::pair<uint, uint> LevelTimeNode::get_hour_interval() const
{
    return hour_interval;
}


bool LevelTimeNode::set_level(const uint &level)
{
    if (level < 1 ||
        level > MAX_NUM_LEVELS)
    {
        return false;
    }
    
    this->level = level;
    return true;
}
bool LevelTimeNode::set_day(const Days &day)
{
    if (day < Days::MONDAY || 
        day > Days::FRIDAY)
    {
        return false;
    }
    
    this->day = day;
    return true;
}
bool LevelTimeNode::set_hour_interval(const uint &start_hour, const uint &end_hour)
{
    if (start_hour >= end_hour ||
        end_hour > MAX_DAILY_HOURS)
    {
        return false;
    }

    this->hour_interval = std::make_pair(start_hour, end_hour);
    return true;
}