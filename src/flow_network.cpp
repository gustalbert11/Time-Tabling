#include "../include/flow_network.hpp"

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

const std::vector<ScheduleEntry>& FlowNetwork::get_final_schedule() const
{
    return final_schedule;
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

            for (uint h = MIN_START_HOUR + 1; h <= MAX_END_HOUR - 3; h += 2)
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
            }
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

            for (uint h = MIN_START_HOUR + 1; h <= MAX_END_HOUR - 3; h += 2)
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
        }
    }
}




void FlowNetwork::create_arcs() 
{
    network.reset_counters();
    
    auto source_it = graph_node_map.find("SOURCE");
    auto sink_it = graph_node_map.find("SINK");
    if (source_it == graph_node_map.end() || sink_it == graph_node_map.end()) {
        return;
    }
    
    GraphNode* source = source_it->second;
    GraphNode* sink = sink_it->second;
    
    // 1. Arcos: FUENTE → SECTION_DEMAND
    for (auto demand_node : section_demand_nodes) {
        network.insert_arc(source, demand_node, ArcData(1, 0));
    }
    
    // 2. Arcos: SECTION_DEMAND → PROFESSOR_TIME
    std::unordered_map<std::string, std::vector<GraphNode*>> professor_time_nodes;
    
    // Agrupar nodos de profesor por ID de profesor (CORREGIDO)
    for (const auto& pair : graph_node_map) {
        auto node_info = pair.second->get_info();
        if (node_info && node_info->get_type() == FlowNodeType::PROFESSOR_TIME) {
            auto prof_time = std::dynamic_pointer_cast<ProfessorTimeNode>(node_info);
            if (prof_time && prof_time->get_professor()) {
                std::string prof_id = prof_time->get_professor()->get_id();
                professor_time_nodes[prof_id].push_back(pair.second);
            }
        }
    }
    
    // Conectar demandas con profesores (CORREGIDO)
    for (auto demand_graph_node : section_demand_nodes) 
    {
        auto demand_info = std::dynamic_pointer_cast<SectionDemandNode>(
            demand_graph_node->get_info());
        if (!demand_info) continue;
        
        Section* section = demand_info->get_section();
        if (!section) continue;
        
        Professor* professor = section->get_professor();
        if (!professor) continue;
        
        std::string prof_id = professor->get_id();
        auto prof_nodes_it = professor_time_nodes.find(prof_id);
        if (prof_nodes_it == professor_time_nodes.end()) continue;
        
        for (auto prof_graph_node : prof_nodes_it->second) {
            auto prof_time_info = std::dynamic_pointer_cast<ProfessorTimeNode>(
                prof_graph_node->get_info());
            if (!prof_time_info) continue;
            
            uint cost = calculate_preference_cost(
                professor,
                prof_time_info->get_day(),
                prof_time_info->get_hour_interval().first,
                prof_time_info->get_hour_interval().second
            );
            
            network.insert_arc(demand_graph_node, prof_graph_node, ArcData(1, cost));
        }
    }
    
    // 3. Arcos: PROFESSOR_TIME → SECTION_TIME (CORREGIDO)
    std::unordered_map<std::string, std::vector<GraphNode*>> section_time_nodes;
    
    // Agrupar nodos de sección tiempo por ID de sección
    for (const auto& pair : graph_node_map) {
        auto node_info = pair.second->get_info();
        if (node_info && node_info->get_type() == FlowNodeType::SECTION_TIME) {
            auto sect_time = std::dynamic_pointer_cast<SectionTimeNode>(node_info);
            if (sect_time && sect_time->get_section()) {
                std::string sect_id = sect_time->get_section()->get_id();
                section_time_nodes[sect_id].push_back(pair.second);
            }
        }
    }
    
    // Conectar profesor_time a section_time (CORREGIDO)
    for (const auto& pair : graph_node_map) {
        auto node_info = pair.second->get_info();
        if (!node_info || node_info->get_type() != FlowNodeType::PROFESSOR_TIME) continue;
        
        auto prof_time_info = std::dynamic_pointer_cast<ProfessorTimeNode>(node_info);
        if (!prof_time_info) continue;
        
        Days prof_day = prof_time_info->get_day();
        auto prof_hours = prof_time_info->get_hour_interval();
        
        // Buscar section_time que coincidan en día y hora
        for (const auto& sect_pair : section_time_nodes) {
            for (auto sect_graph_node : sect_pair.second) {
                auto sect_time_info = std::dynamic_pointer_cast<SectionTimeNode>(
                    sect_graph_node->get_info());
                if (!sect_time_info) continue;
                
                if (sect_time_info->get_day() == prof_day &&
                    sect_time_info->get_hour_interval() == prof_hours) {
                    
                    network.insert_arc(pair.second, sect_graph_node, ArcData(1, 0));
                }
            }
        }
    }
    
    // 4. Arcos: SECTION_TIME → SUMIDERO (SIMPLIFICADO - sin colisiones por ahora)
    for (const auto& pair : graph_node_map) {
        auto node_info = pair.second->get_info();
        if (!node_info || node_info->get_type() != FlowNodeType::SECTION_TIME) continue;
        
        network.insert_arc(pair.second, sink, ArcData(1, 0));
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

// En flow_network.cpp
uint FlowNetwork::calculate_preference_cost(Professor* professor, Days day, 
                                          uint start_hour, uint end_hour) {
    if (!professor || !professor->get_preference()) {
        return 0; // Sin preferencias, sin costo
    }
    
    const Preference* preference = professor->get_preference();
    uint cost = 0;
    
    // Verificar preferencias de días
    if (preference->get_type() == DAYS || preference->get_type() == DAYS_HOURS) {
        if (!preference->get_days().contains(day)) {
            cost += 50; // Penalización media por día no preferido
        }
    }
    
    // Verificar preferencias de horas
    if (preference->get_type() == HOURS || preference->get_type() == DAYS_HOURS) {
        auto hour_interval = std::make_pair(start_hour, end_hour);
        if (!preference->get_hours().contains(hour_interval)) {
            cost += 50; // Penalización media por hora no preferida
        }
    }
    
    return cost;
}


// En flow_network.cpp
uint FlowNetwork::calculate_level_collision_cost(Section* current_section, Days day, uint start_hour, uint end_hour) 
{
    Course* current_course = current_section->get_course();
    if (!current_course) return 0;
    
    uint current_level = current_course->get_level();
    uint collision_penalty = 1000; // Penalización MUY alta
        
    auto time_slot = std::make_tuple(current_level, day, start_hour, end_hour);
    
    // Verificar si ya hay una sección del mismo nivel en este horario
    if (level_assignments.find(time_slot) != level_assignments.end()) {
        // ¡COLISIÓN DETECTADA!
        return collision_penalty;
    }
    
    return 0; // Sin colisión
}

bool FlowNetwork::solve_min_cost_max_flow() 
{
    total_flow = 0;
    total_cost = 0;
    final_schedule.clear();
    
    // Inicializar flujos a 0 en todos los arcos (CORREGIDO)
    network.for_each_arc([&](auto arc) {
        arc->get_info().flow = 0;
    });
    
    ResidualGraph residual_graph;
    build_residual_graph(residual_graph);
    
    uint iteration = 0;
    const uint MAX_ITERATIONS = 1000;
    
    while (iteration++ < MAX_ITERATIONS) {
        auto path = find_augmenting_path_bellman(residual_graph);
        if (path.empty()) break;
        
        uint path_flow = INF;
        uint path_cost = 0;
        
        // Calcular flujo y costo del camino
        for (size_t i = 0; i < path.size() - 1; ++i) {
            GraphNode* u = path[i];
            GraphNode* v = path[i+1];
            
            // Buscar el arco en el grafo residual
            auto& arcs = residual_graph[u];
            for (auto& arc : arcs) {
                if (arc.target == v && arc.capacity > 0) {
                    path_flow = std::min(path_flow, arc.capacity);
                    path_cost += arc.cost;
                    break;
                }
            }
        }
        
        if (path_flow == 0 || path_flow == INF) break;
        
        // Actualizar flujos
        update_flows_and_residuals(path, path_flow, residual_graph);
        
        total_flow += path_flow;
        total_cost += path_cost * path_flow;
        
        std::cout << "Iteración " << iteration << ": flujo=" << path_flow 
                  << ", costo=" << path_cost << ", total=" << total_flow << std::endl;
    }
    
    extract_schedule();
    std::cout << "✅ Algoritmo completado. Total: " << total_flow << " asignaciones, costo: " << total_cost << std::endl;
    return total_flow > 0;
}

std::vector<FlowNetwork::GraphNode*> FlowNetwork::find_augmenting_path_bellman(
    ResidualGraph& residual_graph) {
    
    std::unordered_map<GraphNode*, uint> dist;
    std::unordered_map<GraphNode*, GraphNode*> parent;
    std::unordered_map<GraphNode*, ResidualArc*> parent_arc;
    
    // Inicializar distancias
    for (auto& pair : graph_node_map) {
        dist[pair.second] = INF;
    }
    
    auto source = graph_node_map["SOURCE"];
    dist[source] = 0;
    
    // Bellman-Ford relajación
    for (size_t i = 0; i < graph_node_map.size() - 1; ++i) {
        for (auto& [node, arcs] : residual_graph) {
            if (dist[node] == INF) continue;
            
            for (auto& arc : arcs) {
                if (arc.capacity > 0 && dist[node] + arc.cost < dist[arc.target]) {
                    dist[arc.target] = dist[node] + arc.cost;
                    parent[arc.target] = node;
                    parent_arc[arc.target] = &arc;
                }
            }
        }
    }
    
    // Reconstruir camino si llegamos al sumidero
    auto sink = graph_node_map["SINK"];
    if (dist[sink] == INF) return {};
    
    return reconstruct_path(parent, sink);
}

// En flow_network.cpp
std::vector<FlowNetwork::GraphNode*> FlowNetwork::reconstruct_path(
    const std::unordered_map<GraphNode*, GraphNode*>& parent, 
    GraphNode* sink) {
    
    std::vector<GraphNode*> path;
    
    // Comenzamos desde el sumidero y seguimos los punteros de parent hacia atrás
    GraphNode* current = sink;
    
    while (parent.find(current) != parent.end()) {
        path.push_back(current);
        current = parent.at(current);
    }
    
    // Agregar el nodo fuente (que no tiene parent)
    auto source_it = graph_node_map.find("SOURCE");
    if (source_it != graph_node_map.end()) {
        path.push_back(source_it->second);
    }
    
    // Invertir el camino para que quede: fuente → ... → sumidero
    std::reverse(path.begin(), path.end());
    
    return path;
}


void FlowNetwork::update_flows_and_residuals(const std::vector<GraphNode*>& path, 
                                           uint flow, ResidualGraph& residual_graph) {
    for (size_t i = 0; i < path.size() - 1; ++i) {
        GraphNode* u = path[i];
        GraphNode* v = path[i+1];
        
        // Buscar y actualizar arco original (CORREGIDO)
        bool found_arc = false;
        network.for_each_adjacent_arc(u, [&](auto arc) {
            if (arc->get_tgt_node() == v) {
                arc->get_info().flow += flow;
                found_arc = true;
                return; // Salir del lambda temprano
            }
        });
        
        // Actualizar grafo residual
        for (auto& arc : residual_graph[u]) {
            if (arc.target == v) {
                arc.capacity -= flow;
                
                // Actualizar arco reverso
                for (auto& rev_arc : residual_graph[v]) {
                    if (rev_arc.target == u) {
                        rev_arc.capacity += flow;
                        break;
                    }
                }
                break;
            }
        }
    }
}



void FlowNetwork::build_residual_graph(ResidualGraph& residual_graph) {
    // Inicializar el grafo residual con todos los nodos
    network.for_each_node([&](auto node) {
        residual_graph[node] = std::vector<ResidualArc>();
    });
    
    // Agregar arcos directos y reversos
    network.for_each_arc([&](auto arc) {
        auto source = arc->get_src_node();  // ⚠️ VERIFICAR nombre correcto del método
        auto target = arc->get_tgt_node();  // ⚠️ VERIFICAR nombre correcto del método
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
        residual_graph[target].push_back(reverse_arc);
    });
}

void FlowNetwork::extract_schedule() {
    final_schedule.clear();
    
    // Usar for_each_arc de Designar::Digraph (CORREGIDO)
    network.for_each_arc([&](auto arc) {
        if (arc->get_info().flow > 0) {
            auto from_node = arc->get_src_node()->get_info();
            auto to_node = arc->get_tgt_node()->get_info();
            
            if (!from_node || !to_node) return;
            
            // Buscar arcos entre ProfessorTime y SectionTime
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
        }
    });
}