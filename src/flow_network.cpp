#include "../include/flow_network.hpp"

FlowNetwork *fn_ptr_instance = FlowNetwork::get_ptr_instance();
FlowNetwork &fn_instance = FlowNetwork::get_instance();

void FlowNetwork::init()
{
    if (is_initialized)
    {
        return;
    }
    
    create_network();
    is_initialized = true;
}

uint FlowNetwork::get_section_demand_nodes_count() const
{
    return num_section_demand_nodes;
}
uint FlowNetwork::get_professor_time_nodes_count() const
{
    return num_professor_time_nodes;
}
uint FlowNetwork::get_section_time_nodes_count() const
{
    return num_section_time_nodes;
}
uint FlowNetwork::get_arcs_count() const
{
    return network.get_num_arcs();
}

// bool FlowNetwork::add_node(const std::shared_ptr<FlowNode> &node)
// {
//     if (!node)
//     {
//         return false;
//     }
    
//     network.insert_node(node);

//     switch (node->get_type())
//     {
//     case FlowNodeType::SECTION_DEMAND:
//         ++num_section_demand_nodes;
//         break;

//     case FlowNodeType::PROFESSOR_TIME:
//         ++num_professor_time_nodes;
//         break;

//     case FlowNodeType::SECTION_TIME:
//         ++num_section_time_nodes;
//         break;

//     default:
//         break;
//     }

//     return true;
// }

bool FlowNetwork::add_node(const std::shared_ptr<FlowNode> &node)
{
    if (!node) 
    {
        return false;
    }

    auto graph_node = network.insert_node(node);
    graph_node_map[node->get_id()] = graph_node;

    switch (node->get_type())
    {
    case FlowNodeType::SECTION_DEMAND:
        section_demand_nodes.push_back(graph_node);
        ++num_section_demand_nodes;
        break;

    case FlowNodeType::PROFESSOR_TIME:
        professor_time_nodes.push_back(graph_node);
        ++num_professor_time_nodes;
        break;

    case FlowNodeType::SECTION_TIME:
        section_time_nodes.push_back(graph_node);
        ++num_section_time_nodes;
        break;

    default:
        break;
    }

    return true;
}

FlowNetwork::FlowNetwork()
{
    
}

void FlowNetwork::create_nodes()
{
    auto source = std::make_shared<FlowNode>("SOURCE", FlowNodeType::SOURCE);
    auto sink   = std::make_shared<FlowNode>("SINK",   FlowNodeType::SINK);

    add_node(source);
    add_node(sink);

    for (const auto &pair : dm_instance.get_professors())
    {
        auto professor = pair.second.get();
        auto prof_id = professor->get_id();    

        for (int d = static_cast<int>(Days::MONDAY);
             d <= static_cast<int>(Days::FRIDAY);
             ++d)
        {
            Days day = static_cast<Days>(d);

            for (uint h = MIN_START_HOUR; h <= MAX_END_HOUR - 2; h += 2)
            {
                uint start_hour = h;
                uint end_hour   = h + 2;

                std::string node_id =
                    prof_id + "_D" + std::to_string(d) +
                    "_H" + std::to_string(start_hour) +
                    "-"  + std::to_string(end_hour);

                auto pt_node = std::make_shared<ProfessorTimeNode>(
                    node_id, professor, day, start_hour, end_hour
                );   

                add_node(pt_node);

                // Aquí es buen lugar para guardar el índice/nodo en
                // un mapa auxiliar si luego lo necesitas para crear aristas.
            }

            // for (uint h = MIN_START_HOUR + 1; h <= MAX_END_HOUR - 3; h += 2)
            // {
            //     uint start_hour = h;
            //     uint end_hour   = h + 2;

            //     std::string node_id =
            //         prof_id + "_D" + std::to_string(d) +
            //         "_H" + std::to_string(start_hour) +
            //         "-"  + std::to_string(end_hour);

            //     auto pt_node = std::make_shared<ProfessorTimeNode>(
            //         node_id, professor, day, start_hour, end_hour
            //     );   

            //     add_node(pt_node);
            // }
        }
    }

    for (const auto &pair : dm_instance.get_sections())
    {
        auto section = pair.second.get();     
        auto sec_id = section->get_id();

        auto num_weekly_hours = section->get_course()->get_num_weekly_hours();

        for (uint i = 0; i + 2 <= num_weekly_hours; i += 2)
        {
            uint hour_index = i + 2;
            
            std::string demand_id =
                sec_id + "_DEMAND_" +
                std::to_string(hour_index);

            auto demand_node = std::make_shared<SectionDemandNode>(
                demand_id, section, hour_index
            );   

            add_node(demand_node);
        }

        for (int d = static_cast<int>(Days::MONDAY);
             d <= static_cast<int>(Days::FRIDAY);
             ++d)
        {
            Days day = static_cast<Days>(d);

            for (uint h = MIN_START_HOUR; h <= MAX_END_HOUR - 2; h += 2)
            {
                uint start_hour = h;
                uint end_hour   = h + 2;

                std::string time_id =
                    sec_id + "_TIME_D" + std::to_string(d) +
                    "_H" + std::to_string(start_hour) +
                    "-"  + std::to_string(end_hour);

                auto st_node = std::make_shared<SectionTimeNode>(
                    time_id, section, day, start_hour, end_hour
                );   

                add_node(st_node);
            }

            // for (uint h = MIN_START_HOUR + 1; h <= MAX_END_HOUR - 3; h += 2)
            // {
            //     uint start_hour = h;
            //     uint end_hour   = h + 2;

            //     std::string time_id =
            //         sec_id + "_TIME_D" + std::to_string(d) +
            //         "_H" + std::to_string(start_hour) +
            //         "-"  + std::to_string(end_hour);

            //     auto st_node = std::make_shared<SectionTimeNode>(
            //         time_id, section, day, start_hour, end_hour
            //     );   

            //     add_node(st_node);
            // }
        }
    }
}
void FlowNetwork::create_arcs()
{
    network.reset_counters();
    
    auto it = graph_node_map.find("SOURCE");
    if (it == graph_node_map.end()) 
    {
        return;
    }
    GraphNode* src = it->second;

    for (auto demand_node : section_demand_nodes)
    {
        network.insert_arc(src, demand_node, static_cast<uint>(1));
        ++demand_node->counter();

        auto sect_demand_node_info = demand_node->get_info();
        auto sect_demand = std::dynamic_pointer_cast<SectionDemandNode>(sect_demand_node_info);
        if (!sect_demand)
        {
            continue;
        }
        
        auto section = sect_demand->get_section();
        auto sect_prof = section->get_professor();
        auto max_daily_hours = sect_prof->get_max_daily_hours();
        auto max_consecutive_hours = sect_prof->get_max_consecutive_hours();

        for (auto prof_time_node : professor_time_nodes)
        {
            auto prof_time_node_info = prof_time_node->get_info();
            auto prof_time = std::dynamic_pointer_cast<ProfessorTimeNode>(prof_time_node_info);
            if (!prof_time)
            {
                continue;
            }
            if (prof_time->get_professor() != sect_prof)
            {
                continue;
            }
            
            
        }
        
    }

    // más arcos a crear...
}
void FlowNetwork::create_network()
{
    num_section_demand_nodes = 0;
    num_professor_time_nodes = 0;
    num_section_time_nodes = 0;

    create_nodes();
    create_arcs();
}