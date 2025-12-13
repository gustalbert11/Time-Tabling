#pragma once

#include "settings.hpp"

enum class FlowNodeType 
{
    NO_SPECIFIED,
    SOURCE,
    SECTION_DEMAND, 
    PROFESSOR_TIME,
    SECTION_TIME,
    LEVEL_TIME, 
    SINK
};

inline std::string flow_node_type_to_string(FlowNodeType type)
{
    switch (type)
    {
        case FlowNodeType::SOURCE: 
            return "SOURCE";

        case FlowNodeType::SECTION_DEMAND: 
            return "SECTION DEMAND";

        case FlowNodeType::PROFESSOR_TIME: 
            return "PROFESSOR TIME";

        case FlowNodeType::SECTION_TIME: 
            return "SECTION TIME";

        case FlowNodeType::LEVEL_TIME: 
            return "LEVEL TIME";

        case FlowNodeType::SINK: 
            return "SINK";

        default: 
            return "NO SPECIFIED";
    }
}

inline FlowNodeType string_to_flow_node_type(const std::string& str)
{
    if (str == "SOURCE") 
    {
        return FlowNodeType::SOURCE;
    }
    else if (str == "SECTION DEMAND") 
    {
        return FlowNodeType::SECTION_DEMAND;
    }
    else if (str == "PROFESSOR TIME") 
    {
        return FlowNodeType::PROFESSOR_TIME;
    }
    else if (str == "SECTION TIME") 
    {
        return FlowNodeType::SECTION_TIME;
    }
    else if (str == "LEVEL TIME") 
    {
        return FlowNodeType::LEVEL_TIME;
    }
    else if (str == "SINK") 
    {
        return FlowNodeType::SINK;
    }

    return FlowNodeType::NO_SPECIFIED;
}

class FlowNode 
{
public:
    FlowNode() = default;
    FlowNode(const FlowNode &) = default;

    FlowNode(std::string id, FlowNodeType type);

    virtual ~FlowNode() = default;

    std::string get_id() const;
    FlowNodeType get_type() const;

private:
    std::string id;
    FlowNodeType type;

    bool set_id(const std::string &id);
    bool set_type(const FlowNodeType &type);
};