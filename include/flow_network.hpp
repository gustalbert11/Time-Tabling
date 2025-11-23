#include "professor_time_node.hpp"
#include "section_time_node.hpp"
#include <graphalgorithms.hpp>
#include "data_manager.hpp"

class FlowNetwork
{
public:
    uint get_section_demand_nodes_count() const;
    uint get_professor_time_nodes_count() const;
    uint get_section_time_nodes_count() const;

    bool add_node(const std::shared_ptr<FlowNode> &node);

private:
    Designar::Digraph<std::shared_ptr<FlowNode>, uint> network;
    uint num_section_demand_nodes = 0;
    uint num_professor_time_nodes = 0;
    uint num_section_time_nodes = 0;

    void create_nodes();
};

// class FlowNetwork
// {
// public:
//     uint get_section_demand_nodes_count() const;
//     uint get_professor_time_nodes_count() const;
//     uint get_section_time_nodes_count() const;

//     bool add_node(const FlowNode &node);

// private:
//     Designar::Digraph<FlowNode, uint> network;
//     uint num_section_demand_nodes;
//     uint num_professor_time_nodes;
//     uint num_section_time_nodes;

//     void create_nodes();
// };