#pragma once
#include <queue>
#include <functional>
#include <limits>
#include <unordered_set>
#include <tuple>

#include "professor_time_node.hpp"
#include "section_time_node.hpp"
#include <graphalgorithms.hpp>
#include "core/data_manager.hpp"

// En flow_network.hpp - modificar ArcData
struct ArcData 
{
    uint capacity;
    uint cost;
    uint flow; 
    
    ArcData(uint cap = 0, uint c = 0, uint f = 0) : capacity(cap), cost(c), flow(f) {}
};

struct ScheduleEntry 
{
    std::string professor_id;
    std::string professor_name;
    std::string course_id;
    std::string course_name;
    std::string section_id;
    Days day;
    uint start_hour;
    uint end_hour;
    uint cost;  // Para ver qué tan "buena" es esta asignación
};

class FlowNetwork : public Designar::Singleton<FlowNetwork>
{
    friend class Designar::Singleton<FlowNetwork>;

public:
    using NetworkType = Designar::Digraph<std::shared_ptr<FlowNode>, ArcData>;

    void init();
    
    uint get_section_demand_nodes_count() const;
    uint get_professor_time_nodes_count() const;
    uint get_section_time_nodes_count() const;
    uint get_arcs_count() const;
    
    bool add_node(const std::shared_ptr<FlowNode> &node);

    bool solve_min_cost_max_flow();
    
    // Métodos para acceder a la solución
    const std::vector<ScheduleEntry>& get_final_schedule() const;
    uint get_total_flow() const;
    uint get_total_cost() const;
    
    void extract_schedule();
    void print_schedule() const;
    void debug_preferences_loading();


    uint calculate_preference_cost(Professor* professor, Days day, uint start_hour, uint end_hour);

protected:
    FlowNetwork();

    NetworkType network;
    uint num_section_demand_nodes;
    uint num_professor_time_nodes;
    uint num_section_time_nodes;
    bool is_initialized;

    using GraphNode = NetworkType::Node;
    std::unordered_map<std::string, GraphNode*> graph_node_map;
    std::vector<GraphNode*> section_demand_nodes;

    void create_nodes();
    void create_arcs();
    void create_network();
    
    struct ResidualArc 
    {
        GraphNode* target;
        uint capacity;
        uint cost;
        int reverse_index; // Índice al arco reverso
    };
    using ResidualGraph = std::unordered_map<GraphNode*, std::vector<ResidualArc>>;
    
    // Métodos del algoritmo
    void build_residual_graph(ResidualGraph& residual_graph);

    std::vector<GraphNode*>find_augmenting_path_bellman(ResidualGraph& residual_graph);

    std::vector<GraphNode*> reconstruct_path(const std::unordered_map<GraphNode*, GraphNode*>& parent,
                                            GraphNode* sink);

    void update_flows_and_residuals(const std::vector<GraphNode*>& path, 
                                           uint flow, ResidualGraph& residual_graph);

    void analyze_preference_compliance();
    
    // Variables de solución
    uint total_flow;
    uint total_cost;
    std::vector<ScheduleEntry> final_schedule;
};

extern FlowNetwork *fn_ptr_instance;
extern FlowNetwork &fn_instance;
