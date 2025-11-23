#pragma once

#include "professor_time_node.hpp"
#include "section_time_node.hpp"
#include <graphalgorithms.hpp>
#include "data_manager.hpp"

class FlowNetwork : public Designar::Singleton<FlowNetwork>
{
    friend class Designar::Singleton<FlowNetwork>;

public:
    void init();
    
    uint get_section_demand_nodes_count() const;
    uint get_professor_time_nodes_count() const;
    uint get_section_time_nodes_count() const;

    bool add_node(const std::shared_ptr<FlowNode> &node);

protected:
    FlowNetwork();

    Designar::Digraph<std::shared_ptr<FlowNode>, uint> network;
    uint num_section_demand_nodes;
    uint num_professor_time_nodes;
    uint num_section_time_nodes;
    bool is_initialized;

    void create_nodes();
    void create_network();
};

extern FlowNetwork *fn_ptr_instance;
extern FlowNetwork &fn_instance;