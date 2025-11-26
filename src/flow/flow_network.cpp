#include "flow/flow_network.hpp"
#include <unordered_set>
FlowNetwork *fn_ptr_instance = FlowNetwork::get_ptr_instance();
FlowNetwork &fn_instance = FlowNetwork::get_instance();

const uint INF = std::numeric_limits<uint>::max();

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

const std::vector<ScheduleEntry> &FlowNetwork::get_final_schedule() const
{
    return final_schedule;
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

FlowNetwork::FlowNetwork()
{
}

void FlowNetwork::create_nodes()
{
    auto source = std::make_shared<FlowNode>("SOURCE", FlowNodeType::SOURCE);
    auto sink = std::make_shared<FlowNode>("SINK", FlowNodeType::SINK);

    add_node(source);
    add_node(sink);

    
    for (const auto &prof_pair : dm_instance.get_professors())
    {
        auto professor = prof_pair.second.get();
        if (!professor)
            continue;

        std::string prof_id = professor->get_id();
        for (int day_int = static_cast<int>(Days::MONDAY);
             day_int <= static_cast<int>(Days::FRIDAY);
             ++day_int)
        {
            Days day = static_cast<Days>(day_int);

            for (uint start_hour = MIN_START_HOUR; start_hour <= MAX_END_HOUR - 2; start_hour += 2)
            {
                uint end_hour = start_hour + 2;

                if (end_hour > MAX_END_HOUR)
                    continue;

                std::string node_id = prof_id + "_D" + std::to_string(day_int) +
                                      "_H" + std::to_string(start_hour) + "-" + std::to_string(end_hour);

                auto pt_node = std::make_shared<ProfessorTimeNode>(
                    node_id, professor, day, start_hour, end_hour);

                add_node(pt_node);
            }
        }
    }

    // Crear nodos para secciones
    for (const auto &section_pair : dm_instance.get_sections())
    {
        auto section = section_pair.second.get();
        if (!section || !section->get_course())
            continue;

        std::string sec_id = section->get_id();
        auto course = section->get_course();
        // Nodos de demanda
        uint num_weekly_hours = course->get_num_weekly_hours();
        for (uint hour_index = 2; hour_index <= num_weekly_hours; hour_index += 2)
        {
            std::string demand_id = sec_id + "_DEMAND_" + std::to_string(hour_index);
            auto demand_node = std::make_shared<SectionDemandNode>(demand_id, section, hour_index);
            add_node(demand_node);
        }

        // Nodos de tiempo
        for (int day_int = static_cast<int>(Days::MONDAY);
             day_int <= static_cast<int>(Days::FRIDAY);
             ++day_int)
        {
            Days day = static_cast<Days>(day_int);

            for (uint start_hour = MIN_START_HOUR; start_hour <= MAX_END_HOUR - 2; start_hour += 2)
            {
                uint end_hour = start_hour + 2;
                if (end_hour > MAX_END_HOUR)
                    continue;

                std::string time_id = sec_id + "_TIME_D" + std::to_string(day_int) +
                                      "_H" + std::to_string(start_hour) + "-" + std::to_string(end_hour);

                auto st_node = std::make_shared<SectionTimeNode>(time_id, section, day, start_hour, end_hour);
                add_node(st_node);
            }
        }
    }

    
    for (uint level = 1; level <= MAX_NUM_LEVEL; ++level)
    {
        for (int day_int = static_cast<int>(Days::MONDAY);
             day_int <= static_cast<int>(Days::FRIDAY); ++day_int)
        {
            Days day = static_cast<Days>(day_int);
            for (uint start_hour = MIN_START_HOUR; start_hour <= MAX_END_HOUR - 2; start_hour += 2)
            {
                uint end_hour = start_hour + 2;
                if (end_hour > MAX_END_HOUR)
                    continue;

                std::string node_id = "LEVEL_" + std::to_string(level) +
                                      "_D" + std::to_string(day_int) +
                                      "_H" + std::to_string(start_hour) + "-" + std::to_string(end_hour);

                auto level_node = std::make_shared<FlowNode>(node_id, FlowNodeType::LEVEL_TIME_SLOT);
                add_node(level_node);
            }
        }
    }
}

void FlowNetwork::create_arcs()
{
    network.reset_counters();

    auto source_it = graph_node_map.find("SOURCE");
    auto sink_it = graph_node_map.find("SINK");
    if (source_it == graph_node_map.end() || sink_it == graph_node_map.end())
    {
        return;
    }

    GraphNode *source = source_it->second;
    GraphNode *sink = sink_it->second;


    
    for (auto demand_node : section_demand_nodes)
    {
        network.insert_arc(source, demand_node, ArcData(1, 0));
    }


    std::unordered_map<std::string, std::vector<GraphNode *>> professor_time_nodes;


    for (const auto &pair : graph_node_map)
    {
        auto node_info = pair.second->get_info();
        if (node_info && node_info->get_type() == FlowNodeType::PROFESSOR_TIME)
        {
            auto prof_time = std::dynamic_pointer_cast<ProfessorTimeNode>(node_info);
            if (prof_time && prof_time->get_professor())
            {
                std::string prof_id = prof_time->get_professor()->get_id();
                professor_time_nodes[prof_id].push_back(pair.second);
            }
        }
    }

    // Conectar cada sección solo con su profesor asignado
    uint demand_to_prof_arcs = 0;
    for (auto demand_graph_node : section_demand_nodes)
    {
        auto demand_info = std::dynamic_pointer_cast<SectionDemandNode>(demand_graph_node->get_info());
        if (!demand_info)
            continue;

        Section *section = demand_info->get_section();
        if (!section)
            continue;

        Professor *professor = section->get_professor();
        if (!professor)
            continue;

        std::string prof_id = professor->get_id();
        auto prof_nodes_it = professor_time_nodes.find(prof_id);
        if (prof_nodes_it == professor_time_nodes.end())
            continue;

        for (auto prof_graph_node : prof_nodes_it->second)
        {
            network.insert_arc(demand_graph_node, prof_graph_node, ArcData(1, 0));
            demand_to_prof_arcs++;
        }
    }

   
    std::unordered_map<std::string, std::vector<GraphNode *>> section_time_nodes;
    uint prof_to_sect_arcs = 0;

   
    for (const auto &pair : graph_node_map)
    {
        auto node_info = pair.second->get_info();
        if (node_info && node_info->get_type() == FlowNodeType::SECTION_TIME)
        {
            auto sect_time = std::dynamic_pointer_cast<SectionTimeNode>(node_info);
            if (sect_time && sect_time->get_section())
            {
                std::string sect_id = sect_time->get_section()->get_id();
                section_time_nodes[sect_id].push_back(pair.second);
            }
        }
    }

    // Conectar profesor_time a section_time que coincidan en día y hora
    for (const auto &pair : graph_node_map)
    {
        auto node_info = pair.second->get_info();
        if (!node_info)
            continue;
        if (node_info->get_type() == FlowNodeType::PROFESSOR_TIME)
        {
            auto prof_time_info = std::dynamic_pointer_cast<ProfessorTimeNode>(node_info);
            if (!prof_time_info)
                continue;

            Professor *professor = prof_time_info->get_professor();
            Days prof_day = prof_time_info->get_day();
            auto prof_hours = prof_time_info->get_hour_interval();


            for (const auto &sect_pair : section_time_nodes)
            {
                for (auto sect_graph_node : sect_pair.second)
                {
                    auto sect_time_info = std::dynamic_pointer_cast<SectionTimeNode>(sect_graph_node->get_info());
                    if (!sect_time_info)
                        continue;

                    Section *section = sect_time_info->get_section();
                    if (!section)
                        continue;

                    
                    if (sect_time_info->get_day() == prof_day &&
                        sect_time_info->get_hour_interval() == prof_hours &&
                        section->get_professor() == professor)
                    {
                        uint cost = calculate_preference_cost(
                            professor, prof_day, prof_hours.first, prof_hours.second, section);

                        network.insert_arc(pair.second, sect_graph_node, ArcData(1, cost));
                        prof_to_sect_arcs++;
                    }
                }
            }
        }
    }

  
    uint sect_to_level_arcs = 0;
    for (const auto &pair : graph_node_map)
    {
        auto node_info = pair.second->get_info();
        if (!node_info || node_info->get_type() != FlowNodeType::SECTION_TIME)
            continue;

        auto sect_time = std::dynamic_pointer_cast<SectionTimeNode>(node_info);
        if (!sect_time || !sect_time->get_section() || !sect_time->get_section()->get_course())
            continue;

        auto section = sect_time->get_section();
        auto course = section->get_course();
        uint level = course->get_level();
        Days day = sect_time->get_day();
        auto hours = sect_time->get_hour_interval();

        std::string level_node_id = "LEVEL_" + std::to_string(level) +
                                    "_D" + std::to_string(static_cast<int>(day)) +
                                    "_H" + std::to_string(hours.first) + "-" + std::to_string(hours.second);

        auto level_node_it = graph_node_map.find(level_node_id);
        if (level_node_it == graph_node_map.end())
            continue;

        network.insert_arc(pair.second, level_node_it->second, ArcData(1, 0));
        sect_to_level_arcs++;
    }

 
    uint level_to_sink_arcs = 0;
    for (const auto &pair : graph_node_map)
    {
        auto node_info = pair.second->get_info();
        if (!node_info || node_info->get_type() != FlowNodeType::LEVEL_TIME_SLOT)
            continue;

        network.insert_arc(pair.second, sink, ArcData(1, 0));
        level_to_sink_arcs++;
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

uint FlowNetwork::calculate_preference_cost(Professor *professor, Days day, uint start_hour, uint end_hour, Section *current_section)
{
    uint cost = 0;

    if (!professor)
    {
        return cost;
    }

    if (professor->get_preference())
    {
        const Preference *preference = professor->get_preference();

        if (preference->get_type() == DAYS || preference->get_type() == DAYS_HOURS)
        {
            const auto &preferred_days = preference->get_days();
            if (!preferred_days.is_empty() && !preferred_days.contains(day))
            {
                cost += PREFERENCE_PENALTY;
            }
        }

        if (preference->get_type() == HOURS || preference->get_type() == DAYS_HOURS)
        {
            const auto &preferred_hours = preference->get_hours();
            if (!preferred_hours.is_empty())
            {
                bool found_preferred_time = false;
                for (const auto &preferred : preferred_hours)
                {
                    if (start_hour >= preferred.first && end_hour <= preferred.second)
                    {
                        found_preferred_time = true;
                        break;
                    }
                }
                if (!found_preferred_time)
                {
                    cost += PREFERENCE_PENALTY;
                }
            }
        }
    }

    //penalización por colisiones de nivel
    if (current_section && current_section->get_course())
    {
        uint collision_penalty = calculate_collision_penalty(current_section, day, start_hour, end_hour);
        cost += collision_penalty;
    }

    return cost;
}

uint FlowNetwork::calculate_collision_penalty(Section *current_section, Days day, uint start_hour, uint end_hour)
{
    if (!current_section || !current_section->get_course())
    {
        return 0;
    }

    Course *current_course = current_section->get_course();
    uint current_level = current_course->get_level();
    uint collision_count = 0;

    for (const auto &section_pair : dm_instance.get_sections())
    {
        Section *other_section = section_pair.second.get();


        if (other_section == current_section || !other_section->get_course())
        {
            continue;
        }

        Course *other_course = other_section->get_course();

       
        if (other_course->get_level() != current_level)
        {
            continue;
        }

        // Verificar si la otra sección ya tiene asignado este horario
        const auto &other_time_slots = other_section->get_time_slots();
        for (const auto &time_slot : other_time_slots)
        {
            Days other_day = time_slot.first;
            uint other_start = time_slot.second.first;
            uint other_end = time_slot.second.second;

           
            if (other_day == day && time_overlap(start_hour, end_hour, other_start, other_end))
            {
                collision_count++;
                break; 
            }
        }
    }

    return collision_count * COLLISION_PENALTY;
}

//detectar superposición
bool FlowNetwork::time_overlap(uint start1, uint end1, uint start2, uint end2)
{
    return (start1 < end2) && (end1 > start2);
}

bool FlowNetwork::solve_min_cost_max_flow()
{
    total_flow = 0;
    total_cost = 0;
    final_schedule.clear();

    
    uint arcs_with_flow = 0;
    network.for_each_arc([&](auto arc)
                         {
        arc->get_info().flow = 0;
        arcs_with_flow++; });

    ResidualGraph residual_graph;
    build_residual_graph(residual_graph);

    uint iteration = 0;
    const uint MAX_ITERATIONS = 1000;

    while (iteration++ < MAX_ITERATIONS)
    {

        auto path = find_augmenting_path_bellman(residual_graph);
        if (path.empty())
        {
            break;
        }

        uint path_flow = INF;
        uint path_cost = 0;

        // Calcular flujo y costo del camino
        for (size_t i = 0; i < path.size() - 1; ++i)
        {
            GraphNode *u = path[i];
            GraphNode *v = path[i + 1];

            // Buscar el arco en el grafo residual
            auto &arcs = residual_graph[u];
            for (auto &arc : arcs)
            {
                if (arc.target == v && arc.capacity > 0)
                {
                    path_flow = std::min(path_flow, arc.capacity);
                    path_cost += arc.cost;

                    break;
                }
            }
        }

        if (path_flow == 0 || path_flow == INF)
        {
            break;
        }
        // Actualizar flujos
        update_flows_and_residuals(path, path_flow, residual_graph);

        total_flow += path_flow;
        total_cost += path_cost * path_flow;
    }
    extract_schedule();

    return total_flow > 0;
}


std::vector<FlowNetwork::GraphNode *> FlowNetwork::find_augmenting_path_bellman(
    ResidualGraph &residual_graph)
{

    std::unordered_map<GraphNode *, uint> dist;
    std::unordered_map<GraphNode *, GraphNode *> parent;
    std::unordered_map<GraphNode *, ResidualArc *> parent_arc;

    for (auto &pair : graph_node_map)
    {
        dist[pair.second] = INF;
    }

    auto source = graph_node_map["SOURCE"];
    dist[source] = 0;


    // Bellman-Ford relajación
    for (size_t i = 0; i < graph_node_map.size() - 1; ++i)
    {
        bool updated = false;

        for (auto &[node, arcs] : residual_graph)
        {
            if (dist[node] == INF)
                continue;

            for (auto &arc : arcs)
            {
                if (arc.capacity > 0 && dist[node] + arc.cost < dist[arc.target])
                {
                    dist[arc.target] = dist[node] + arc.cost;
                    parent[arc.target] = node;
                    parent_arc[arc.target] = &arc;
                    updated = true;
                }
            }
        }

        if (!updated)
        {
            break;
        }
    }

    auto sink = graph_node_map["SINK"];
    if (dist[sink] == INF)
    {
        return {};
    }
    return reconstruct_path(parent, sink);
}

std::vector<FlowNetwork::GraphNode *> FlowNetwork::reconstruct_path(
    const std::unordered_map<GraphNode *, GraphNode *> &parent,
    GraphNode *sink)
{

    std::vector<GraphNode *> path;

    // Comenzamos desde el sumidero y seguimos los punteros de parent hacia atrás
    GraphNode *current = sink;

    while (parent.find(current) != parent.end())
    {
        path.push_back(current);
        current = parent.at(current);
    }

    // Agregar el nodo fuente 
    auto source_it = graph_node_map.find("SOURCE");
    if (source_it != graph_node_map.end())
    {
        path.push_back(source_it->second);
    }

    // Invertir el camino 
    std::reverse(path.begin(), path.end());

    return path;
}

void FlowNetwork::update_flows_and_residuals(const std::vector<GraphNode *> &path,
                                             uint flow, ResidualGraph &residual_graph)
{
    for (size_t i = 0; i < path.size() - 1; ++i)
    {
        GraphNode *u = path[i];
        GraphNode *v = path[i + 1];

        // Buscar y actualizar arco original 
        bool found_arc = false;
        network.for_each_adjacent_arc(u, [&](auto arc)
                                      {
            if (arc->get_tgt_node() == v) {
                arc->get_info().flow += flow;
                found_arc = true;
                return; 
            } });

        // Actualizar grafo residual
        for (auto &arc : residual_graph[u])
        {
            if (arc.target == v)
            {
                arc.capacity -= flow;

                // Actualizar arco reverso
                for (auto &rev_arc : residual_graph[v])
                {
                    if (rev_arc.target == u)
                    {
                        rev_arc.capacity += flow;
                        break;
                    }
                }
                break;
            }
        }
    }
}

void FlowNetwork::build_residual_graph(ResidualGraph &residual_graph)
{
    network.for_each_node([&](auto node)
                          { residual_graph[node] = std::vector<ResidualArc>(); });

   
    network.for_each_arc([&](auto arc)
                         {
        auto source = arc->get_src_node();
        auto target = arc->get_tgt_node();  
        auto arc_data = arc->get_info();
        
        ResidualArc forward_arc;
        forward_arc.target = target;
        forward_arc.capacity = arc_data.capacity;
        forward_arc.cost = arc_data.cost;
        forward_arc.reverse_index = residual_graph[target].size();
        
        ResidualArc reverse_arc;
        reverse_arc.target = source;
        reverse_arc.capacity = 0;
        reverse_arc.cost = -arc_data.cost;
        reverse_arc.reverse_index = residual_graph[source].size();
        
        residual_graph[source].push_back(forward_arc);
        residual_graph[target].push_back(reverse_arc); });
}

void FlowNetwork::extract_schedule()
{
    final_schedule.clear();


    network.for_each_arc([&](auto arc)
                         {
        if (arc->get_info().flow > 0) {
            auto from_node = arc->get_src_node()->get_info();
            auto to_node = arc->get_tgt_node()->get_info();
            
            if (!from_node || !to_node) return;
            
     
            if (from_node->get_type() == FlowNodeType::PROFESSOR_TIME &&
                to_node->get_type() == FlowNodeType::SECTION_TIME) {
                
                auto prof_time = std::dynamic_pointer_cast<ProfessorTimeNode>(from_node);
                auto sect_time = std::dynamic_pointer_cast<SectionTimeNode>(to_node);
                
                if (prof_time && sect_time && 
                    prof_time->get_professor() && 
                    sect_time->get_section() && 
                    sect_time->get_section()->get_course()) {
                    
                    ScheduleEntry entry;
                    entry.professor_id = prof_time->get_professor()->get_id();
                    entry.professor_name = prof_time->get_professor()->get_name();
                    entry.course_id = sect_time->get_section()->get_course()->get_id();
                    entry.course_name = sect_time->get_section()->get_course()->get_name();
                    entry.section_id = sect_time->get_section()->get_id();
                    entry.day = prof_time->get_day();
                    entry.start_hour = prof_time->get_hour_interval().first;
                    entry.end_hour = prof_time->get_hour_interval().second;
                    entry.cost = arc->get_info().cost;
                    
                    final_schedule.push_back(entry);
                }
            }
        } });
}