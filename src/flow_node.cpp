#include "../include/flow_node.hpp"

FlowNode::FlowNode(std::string id, FlowNodeType type) 
: id(id), type(type) 
{

}

const std::string& FlowNode::get_id() const
{
    return id;
}

const FlowNodeType& FlowNode::get_type() const
{
    return type;
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