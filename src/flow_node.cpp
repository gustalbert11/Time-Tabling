#include "../include/flow_node.hpp"

FlowNode::FlowNode(std::string id, FlowNodeType type)  
{
    set_id(id);
    set_type(type);
}

std::string FlowNode::get_id() const
{
    return id;
}
FlowNodeType FlowNode::get_type() const
{
    return type;
}

bool FlowNode::set_id(const std::string &id)
{
    if (id.empty())
    {
        return false;
    }
    
    this->id = id;
    return true;
}
bool FlowNode::set_type(const FlowNodeType &type)
{
    if (type < FlowNodeType::SOURCE ||
        type > FlowNodeType::SINK)
    {
        return false;
    }
    
    this->type = type;
    return true;
}