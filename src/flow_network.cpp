#include "../include/flow_network.hpp"

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

bool FlowNetwork::add_node(const std::shared_ptr<FlowNode> &node)
{
    if (!node)
    {
        return false;
    }
    
    network.insert_node(node);

    switch (node->get_type())
    {
    case FlowNodeType::SECTION_DEMAND:
        ++num_section_demand_nodes;
        break;
    case FlowNodeType::PROFESSOR_TIME:
        ++num_professor_time_nodes;
        break;
    case FlowNodeType::SECTION_TIME:
        ++num_section_time_nodes;
        break;
    default:
        break;
    }

    return true;
}

void FlowNetwork::create_network()
{
    auto source = std::make_shared<FlowNode>("SOURCE", FlowNodeType::SOURCE);
    auto sink   = std::make_shared<FlowNode>("SINK",   FlowNodeType::SINK);

    add_node(source);
    add_node(sink);

    for (const auto &pair : dm_instance.get_professors())
    {
        Professor* professor = pair.second.get();
        const std::string &prof_id = professor->get_id();    

        for (int d = static_cast<int>(Days::MONDAY);
             d <= static_cast<int>(Days::FRIDAY);
             ++d)
        {
            Days day = static_cast<Days>(d);

            for (uint h = MIN_START_HOUR; h <= MAX_END_HOUR - 2; h += 2)
            {
                uint start_hour = h;
                uint end_hour   = h + 2;

                // Aquí podrías chequear preferencias del profesor si quieres
                // (professor->get_preference()) antes de crear el nodo.

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
        }
    }

    for (const auto &pair : dm_instance.get_sections())
    {
        Section* section = pair.second.get();     
        const std::string &sec_id = section->get_id();

        for (uint h = MIN_START_HOUR; h <= MAX_END_HOUR - 2; h += 2)
        {
            uint start_hour = h;
            uint end_hour   = h + 2;

            std::string demand_id =
                sec_id + "_DEMAND_" +
                std::to_string(start_hour) + "-" + std::to_string(end_hour);

            auto demand_node = std::make_shared<SectionDemandNode>(
                demand_id, section, start_hour, end_hour
            );   

            add_node(demand_node);

            for (int d = static_cast<int>(Days::MONDAY);
                 d <= static_cast<int>(Days::FRIDAY);
                 ++d)
            {
                Days day = static_cast<Days>(d);

                std::string time_id =
                    sec_id + "_TIME_D" + std::to_string(d) +
                    "_H" + std::to_string(start_hour) +
                    "-"  + std::to_string(end_hour);

                auto st_node = std::make_shared<SectionTimeNode>(
                    time_id, section, day, start_hour, end_hour
                );   

                add_node(st_node);
            }
        }
    }

}

// void FlowNetwork::create_network()
// {
//     network.insert_node(FlowNode("SOURCE", FlowNodeType::SOURCE));
//     network.insert_node(FlowNode("SINK", FlowNodeType::SINK));
    
//     for (const auto& pair : dm_instance.get_professors())
//     {
//         auto professor = pair.second.get();
//         std::string id = professor->get_id() + "_TIME"; 
        
//         // for (size_t i = Days::MONDAY; i <= Days::FRIDAY; ++i)
//         // {
//         //     /* code */
//         // }
//         // for (uint i = MIN_START_HOUR; i <= MAX_END_HOUR - 2; i += 2)
//         // {
//         //     /* code */
//         // }
//     }
//     for (const auto& pair : dm_instance.get_sections())
//     {
//         auto section = pair.second.get();
//         std::string demand_id = section->get_id() + "_DEMAND";
//         std::string time_id = section->get_id() + "_TIME";

//         // for (uint j = MIN_START_HOUR; j <= MAX_END_HOUR - 2; j += 2)
//         // {
//         //     auto section_demand = SectionDemandNode(demand_id, section, j, j + 2);
//         //     network.insert_node(section_demand);

//         //     for (size_t i = Days::MONDAY; i <= Days::FRIDAY; ++i)
//         //     {
//         //         auto section_time = SectionTimeNode(time_id, section, i, j, j + 2);
//         //     }
//         // }
//     }
// }