#include "../include/flow_node.hpp"

FlowNode::FlowNode(const std::string &id, const FlowNodeType &type) 
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