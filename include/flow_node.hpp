#pragma once

#include <graphalgorithms.hpp>

enum class FlowNodeType 
{
    SOURCE,
    SECTION_DEMAND,
    PROFESSOR_TIME,
    SECTION_TIME,
    SINK
};

class FlowNode 
{
public:
    FlowNode() = default;
    FlowNode(const FlowNode &) = default;

    FlowNode(const std::string &id, const FlowNodeType &type);

    const std::string &get_id() const;
    const FlowNodeType &get_type() const;

    bool set_type(const FlowNodeType &type);

private:
    std::string id;
    FlowNodeType type;
};