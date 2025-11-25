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

    case FlowNodeType::LEVEL_TIME:
        level_time_nodes.push_back(graph_node);
        ++num_level_time_nodes;
        break;

    default:
        break;
    }

    return true;
}

FlowNetwork::FlowNetwork()
{
    
}

uint calculate_preference_penalty(Professor* professor, const std::shared_ptr<ProfessorTimeNode>& prof_time) 
{
    const Preference* pref = professor->get_preference();
    if (!pref || 
        pref->get_type() == PreferenceType::NO_PREFERENCE) 
    {
        return 0;
    }
    
    auto day = prof_time->get_day();
    auto hour_interval = prof_time->get_hour_interval();
    uint penalty = 0;
    
    if (pref->get_type() == PreferenceType::DAYS || 
        pref->get_type() == PreferenceType::DAYS_HOURS) 
    {
        auto preferred_days = pref->get_days();
        bool day_match = false;
        
        for (const auto& preferred_day : preferred_days) 
        {
            if (preferred_day == day) 
            {
                day_match = true;
                break;
            }
        }
        if (!day_match) 
        {
            penalty += 100;
        }
    }
    
    if (pref->get_type() == PreferenceType::HOURS || 
        pref->get_type() == PreferenceType::DAYS_HOURS) 
    {
        auto preferred_hours = pref->get_hours();
        bool hour_match = false;

        for (const auto& preferred_hour : preferred_hours) 
        {
            if (hour_interval.first >= preferred_hour.first && 
                hour_interval.second <= preferred_hour.second) 
            {
                hour_match = true;
                break;
            }
        }
        if (!hour_match) 
        {
            penalty += 100;
        }
    }
    
    return penalty;
}

uint calculate_preference_penalty(Professor* professor, Days day, uint start_hour, uint end_hour) 
{
    auto pref = professor->get_preference();
    if (!pref || 
        pref->get_type() == PreferenceType::NO_PREFERENCE) 
    {
        return 0;
    }
    
    uint penalty = 0;
    
    if (pref->get_type() == PreferenceType::DAYS || 
        pref->get_type() == PreferenceType::DAYS_HOURS) 
    {
        auto preferred_days = pref->get_days();
        bool day_match = false;
        
        for (const auto& preferred_day : preferred_days) 
        {
            if (preferred_day == day) 
            {
                day_match = true;
                break;
            }
        }
        if (!day_match) 
        {
            penalty += 100;
        }
    }
    
    if (pref->get_type() == PreferenceType::HOURS || 
        pref->get_type() == PreferenceType::DAYS_HOURS) 
    {
        auto preferred_hours = pref->get_hours();
        bool hour_match = false;

        for (const auto& preferred_hour : preferred_hours) 
        {
            if (start_hour >= preferred_hour.first && 
                end_hour <= preferred_hour.second) 
            {
                hour_match = true;
                break;
            }
        }
        if (!hour_match) 
        {
            penalty += 100;
        }
    }
    
    return penalty;
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

            //professor_daily_hours[professor][day] = 0;

            for (uint h = MIN_START_HOUR; h <= MAX_END_HOUR - 2; h += 2)
            {
                uint start_hour = h;
                uint end_hour = h + 2;

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
                uint end_hour = h + 2;

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

    for (uint level = 1; level <= MAX_NUM_LEVELS; ++level)
    {
        for (int d = static_cast<int>(Days::MONDAY);
             d <= static_cast<int>(Days::FRIDAY);
             ++d)
        {
            Days day = static_cast<Days>(d);

            for (uint h = MIN_START_HOUR; h <= MAX_END_HOUR - 2; h += 2)
            {
                uint start_hour = h;
                uint end_hour = h + 2;

                std::string node_id = 
                "LEVEL_" + std::to_string(level) + 
                "_D" + std::to_string(d) +
                "_H" + std::to_string(start_hour) + "-" + std::to_string(end_hour);
                
                auto level_node = std::make_shared<LevelTimeNode>(
                    node_id, level, day, start_hour, end_hour
                );
                add_node(level_node);
            }

            // for (uint h = MIN_START_HOUR + 1; h <= MAX_END_HOUR - 3; h += 2)
            // {
            //     uint start_hour = h;
            //     uint end_hour   = h + 2;

                // std::string node_id = 
                // "LEVEL_" + std::to_string(level) + 
                // "_D" + std::to_string(d) +
                // "_H" + std::to_string(start_hour) + "-" + std::to_string(end_hour);
                
                // auto level_node = std::make_shared<LevelTimeNode>(
                //     node_id, level, day, start_hour, end_hour
                // );
                // add_node(level_node);
            // }
        }
    }
    
}
// void FlowNetwork::create_arcs()
// {
//     network.reset_counters();
    
//     auto source_it = graph_node_map.find("SOURCE");
//     auto sink_it = graph_node_map.find("SINK");
//     if (source_it == graph_node_map.end() || sink_it == graph_node_map.end()) 
//     {
//         return;
//     }
    
//     GraphNode* src = source_it->second;
//     GraphNode* sink = sink_it->second;

//     for (auto demand_node : section_demand_nodes)
//     {
//         network.insert_arc(src, demand_node, std::make_pair(static_cast<uint>(1), 0));
//         ++demand_node->counter();

//         auto sect_demand = std::dynamic_pointer_cast<SectionDemandNode>(demand_node->get_info());
//         if (!sect_demand)
//         {
//             continue;
//         }
        
//         auto section = sect_demand->get_section();
//         auto sect_prof = section->get_professor();

//         for (auto prof_time_node : professor_time_nodes)
//         {
//             auto prof_time = std::dynamic_pointer_cast<ProfessorTimeNode>(prof_time_node->get_info());
//             if (!prof_time)
//             {
//                 continue;
//             }
//             if (prof_time->get_professor() != sect_prof)
//             {
//                 continue;
//             }

//             // uint penalty = 0;
//             // if (prof_time_node->counter() > 0)
//             // {
//             //     penalty = 1000 * prof_time_node->counter();
//             // }
            
//             auto day = prof_time->get_day();
//             auto current_daily_hours = professor_daily_hours[sect_prof][day];
//             auto max_daily_hours = sect_prof->get_max_daily_hours();

//             uint daily_hours_penalty = 0;
//             if (current_daily_hours + 2 > max_daily_hours) 
//             {
//                 daily_hours_penalty = 1000 * (current_daily_hours + 2 - max_daily_hours);
//             }
            
//             auto preference_penalty = calculate_preference_penalty(sect_prof, prof_time);
            
//             //auto total_cost = daily_hours_penalty + preference_penalty + penalty;
//             auto total_cost = daily_hours_penalty + preference_penalty;
            
//             network.insert_arc(demand_node, prof_time_node, std::make_pair(static_cast<uint>(1), total_cost));
//             ++prof_time_node->counter();

//             professor_daily_hours[sect_prof][day] += 2;
//         }
        
//     }

//     for (auto prof_time_node : professor_time_nodes)
//     {
//         auto pt_info = std::dynamic_pointer_cast<ProfessorTimeNode>(prof_time_node->get_info());
//         if (!pt_info)
//         {
//             continue;
//         }

//         for (auto section_time_node : section_time_nodes)
//         {
//             auto st_info = std::dynamic_pointer_cast<SectionTimeNode>(section_time_node->get_info());
//             if (!st_info) 
//             {
//                 continue;
//             }
//             if (st_info->get_section()->get_professor()->get_id() != pt_info->get_professor()->get_id() ||
//                 pt_info->get_day() != st_info->get_day() || 
//                 pt_info->get_hour_interval() != st_info->get_hour_interval())
//             {
//                 continue;
//             }
            
//             network.insert_arc(prof_time_node, section_time_node, std::make_pair(static_cast<uint>(1), 0));
//             ++section_time_node->counter();
//         }
//     }

//     for (auto section_time_node : section_time_nodes)
//     {
//         auto st_info = std::dynamic_pointer_cast<SectionTimeNode>(section_time_node->get_info());
//         if (!st_info) 
//         {
//             continue;
//         }

//         for (auto level_time_node : level_time_nodes)
//         {
//             auto lt_info = std::dynamic_pointer_cast<LevelTimeNode>(level_time_node->get_info());
//             if (!lt_info) 
//             {
//                 continue;
//             }
//             if (lt_info->get_level() != st_info->get_section()->get_course()->get_level() ||
//                 lt_info->get_day() != st_info->get_day() ||
//                 lt_info->get_hour_interval() != st_info->get_hour_interval())
//             {
//                 continue;
//             }
            
//             uint penalty = 0;
//             if (level_time_node->counter() > 0)
//             {
//                 penalty = 1000 * level_time_node->counter();
//             }

//             network.insert_arc(section_time_node, level_time_node, std::make_pair(static_cast<uint>(1), penalty));
//             ++level_time_node->counter();
//         }
//     }

//     for (auto level_time_node : level_time_nodes)
//     {
//         if (level_time_node->counter() == 0)
//         {
//             continue;
//         }
        
//         network.insert_arc(level_time_node, sink, std::make_pair(static_cast<uint>(1), 0));
//     }
// }

void FlowNetwork::create_arcs()
{
    auto source_it = graph_node_map.find("SOURCE");
    auto sink_it = graph_node_map.find("SINK");
    if (source_it == graph_node_map.end() || sink_it == graph_node_map.end()) 
    {
        return;
    }
    GraphNode* src = source_it->second;
    GraphNode* sink = sink_it->second;

    // Mapa auxiliar para búsqueda rápida de nodos de tiempo existentes
    // (Asumo que ya los creaste todos en create_nodes y están en graph_node_map)

    // 1. ITERAMOS SOBRE LAS DEMANDAS (LAS CLASES A ASIGNAR)
    for (auto demand_graph_node : section_demand_nodes)
    {
        // ARCO 1: SOURCE -> DEMAND
        // Capacidad 2: Porque dividiremos el flujo en dos ramas (Profe y Nivel)
        network.insert_arc(src, demand_graph_node, std::make_pair(static_cast<uint>(2), 0));
        
        // Obtenemos la información de la demanda
        auto sect_demand = std::dynamic_pointer_cast<SectionDemandNode>(demand_graph_node->get_info());
        if (!sect_demand) 
        {
            continue;
        }

        auto section = sect_demand->get_section();
        auto professor = section->get_professor();
        auto level = section->get_course()->get_level();

        // 2. EXPANSIÓN A LOS POSIBLES HORARIOS (SECTION TIME NODES)
        // Buscamos todos los nodos SectionTime que correspondan a esta sección
        for (auto sec_time_graph_node : section_time_nodes)
        {
            auto st_info = std::dynamic_pointer_cast<SectionTimeNode>(sec_time_graph_node->get_info());
            if (!st_info || 
                st_info->get_section()->get_id() != section->get_id()) 
            {
                continue; 
            }

            // Datos del intervalo de tiempo actual
            auto day = st_info->get_day();
            auto interval = st_info->get_hour_interval();
            auto start_h = interval.first;
            auto end_h = interval.second;

            // ARCO 2: DEMAND -> SECTION_TIME (La Decisión)
            // Capacidad 2: Pasan las dos unidades de flujo.
            // Costo: Aquí aplicamos la penalización por preferencia del profesor.
            
            // Nota: Necesitarás adaptar tu función calculate_preference_penalty para que acepte (Professor, Day, start, end)
            // O crear un objeto temporal, pero idealmente pásale los datos directos.
            uint cost = 0; // Placeholder, implementa tu lógica de preferencias aquí.
            uint cost = calculate_preference_penalty(professor, day, start_h, end_h); 
            
            network.insert_arc(demand_graph_node, sec_time_graph_node, std::make_pair(static_cast<uint>(2), cost));

            // 3. CONECTAR A LAS RESTRICCIONES (BRANCHING)
            
            // A. Buscar el nodo de TIEMPO DEL PROFESOR correspondiente
            std::string prof_node_id = professor->get_id() + "_D" + std::to_string(static_cast<int>(day)) +
                                       "_H" + std::to_string(start_h) + "-" + std::to_string(end_h);
            
            if (graph_node_map.find(prof_node_id) != graph_node_map.end())
            {
                auto prof_time_graph_node = graph_node_map[prof_node_id];
                
                // ARCO 3a: SECTION_TIME -> PROFESSOR_TIME
                // Capacidad 1: Solo enviamos una "sonda" para validar al profesor.
                network.insert_arc(sec_time_graph_node, prof_time_graph_node, std::make_pair(static_cast<uint>(1), 0));
            }

            // B. Buscar el nodo de TIEMPO DEL NIVEL correspondiente
            std::string level_node_id = "LEVEL_" + std::to_string(level) + 
                                        "_D" + std::to_string(static_cast<int>(day)) +
                                        "_H" + std::to_string(start_h) + "-" + std::to_string(end_h);

            if (graph_node_map.find(level_node_id) != graph_node_map.end())
            {
                GraphNode* level_time_graph_node = graph_node_map[level_node_id];

                // ARCO 3b: SECTION_TIME -> LEVEL_TIME
                // Capacidad 1: Solo enviamos una "sonda" para validar al nivel.
                network.insert_arc(sec_time_graph_node, level_time_graph_node, std::make_pair(static_cast<uint>(1), 0));
            }
        }
    }

    // 4. CONECTAR RESTRICCIONES AL SINK (BOTELLAS DE CUELLO)
    
    // A. Professor Time Nodes -> Sink
    for (auto prof_time_node : professor_time_nodes)
    {
        // Capacidad 1: Un profesor solo puede dar 1 clase a la vez.
        // Si llegan dos flujos (dos materias tratando de usar al mismo profe a la misma hora), 
        // solo pasará 1, bloqueando la solución inválida.
        network.insert_arc(prof_time_node, sink, std::make_pair(static_cast<uint>(1), 0));
    }

    // B. Level Time Nodes -> Sink
    for (auto level_time_node : level_time_nodes)
    {
        // Capacidad K: Cuántas secciones de este nivel pueden chocar a la vez?
        // Si es 1 (no puede haber choques de horario en el mismo semestre): Capacidad 1.
        // Si hay 3 aulas disponibles para ese nivel: Capacidad 3.
        uint max_simultaneous_classes_per_level = 1; 
        network.insert_arc(level_time_node, sink, std::make_pair(static_cast<uint>(max_simultaneous_classes_per_level), 0));
    }
}

void FlowNetwork::create_network()
{
    num_section_demand_nodes = 0;
    num_professor_time_nodes = 0;
    num_section_time_nodes = 0;

    create_nodes();
    create_arcs();
}