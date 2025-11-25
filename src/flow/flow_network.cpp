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

    std::cout << "👨‍🏫 Creando nodos para TODOS los profesores..." << std::endl;
    
    // Crear nodos para TODOS los profesores, no solo los que tienen secciones
    for (const auto &prof_pair : dm_instance.get_professors())
    {
        auto professor = prof_pair.second.get();
        if (!professor) continue;

        std::string prof_id = professor->get_id();
        std::cout << "   Profesor: " << professor->get_name() 
                  << " (Preferencias: " << (professor->get_preference() ? "Sí" : "No") << ")" << std::endl;

        for (int day_int = static_cast<int>(Days::MONDAY); 
             day_int <= static_cast<int>(Days::FRIDAY); 
             ++day_int)
        {
            Days day = static_cast<Days>(day_int);
            
            // Crear bloques de 2 horas
            for (uint start_hour = MIN_START_HOUR; start_hour <= MAX_END_HOUR - 2; start_hour += 2)
            {
                uint end_hour = start_hour + 2;
                
                if (end_hour > MAX_END_HOUR) continue;

                std::string node_id = prof_id + "_D" + std::to_string(day_int) +
                                    "_H" + std::to_string(start_hour) + "-" + std::to_string(end_hour);

                auto pt_node = std::make_shared<ProfessorTimeNode>(
                    node_id, professor, day, start_hour, end_hour
                );
                
                add_node(pt_node);
            }
        }
    }

    std::cout << "📚 Creando nodos para secciones..." << std::endl;
    
    // Crear nodos para secciones
    for (const auto &section_pair : dm_instance.get_sections())
    {
        auto section = section_pair.second.get();
        if (!section || !section->get_course()) continue;

        std::string sec_id = section->get_id();
        auto course = section->get_course();
        
        std::cout << "   Sección: " << sec_id << " - " << course->get_name() 
                  << " (Prof: " << (section->get_professor() ? section->get_professor()->get_name() : "Ninguno") << ")" << std::endl;

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
                if (end_hour > MAX_END_HOUR) continue;

                std::string time_id = sec_id + "_TIME_D" + std::to_string(day_int) +
                                    "_H" + std::to_string(start_hour) + "-" + std::to_string(end_hour);

                auto st_node = std::make_shared<SectionTimeNode>(time_id, section, day, start_hour, end_hour);
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
    
    std::cout << "🏗️ Construyendo arcos de la red..." << std::endl;
    
    // 1. Arcos: FUENTE → SECTION_DEMAND
    std::cout << "1. Fuente → Secciones demanda (" << section_demand_nodes.size() << " arcos)" << std::endl;
    for (auto demand_node : section_demand_nodes) {
        network.insert_arc(source, demand_node, ArcData(1, 0));
    }
    
    // 2. Arcos: SECTION_DEMAND → PROFESSOR_TIME
    std::cout << "2. Secciones demanda → Profesores tiempo" << std::endl;
    std::unordered_map<std::string, std::vector<GraphNode*>> professor_time_nodes;
    
    // Agrupar nodos de profesor por ID de profesor
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
    
    // Conectar cada sección solo con su profesor asignado (sin costo aquí)
    uint demand_to_prof_arcs = 0;
    for (auto demand_graph_node : section_demand_nodes) 
    {
        auto demand_info = std::dynamic_pointer_cast<SectionDemandNode>(demand_graph_node->get_info());
        if (!demand_info) continue;
        
        Section* section = demand_info->get_section();
        if (!section) continue;
        
        Professor* professor = section->get_professor();
        if (!professor) continue;
        
        std::string prof_id = professor->get_id();
        auto prof_nodes_it = professor_time_nodes.find(prof_id);
        if (prof_nodes_it == professor_time_nodes.end()) continue;
        
        for (auto prof_graph_node : prof_nodes_it->second) 
        {
            network.insert_arc(demand_graph_node, prof_graph_node, ArcData(1, 0));
            demand_to_prof_arcs++;
        }
    }
    std::cout << "   Creados " << demand_to_prof_arcs << " arcos" << std::endl;
    
    // 3. Arcos: PROFESSOR_TIME → SECTION_TIME (CON COSTOS DE PREFERENCIAS)
    std::cout << "3. Profesores tiempo → Secciones tiempo (con costos de preferencias)" << std::endl;
    std::unordered_map<std::string, std::vector<GraphNode*>> section_time_nodes;
    
    // Agrupar nodos de sección tiempo por ID de sección
    for (const auto& pair : graph_node_map) 
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
    uint prof_to_sect_arcs = 0;
    for (const auto& pair : graph_node_map) 
    {
        auto node_info = pair.second->get_info();
        if (!node_info || node_info->get_type() != FlowNodeType::PROFESSOR_TIME) continue;
        
        auto prof_time_info = std::dynamic_pointer_cast<ProfessorTimeNode>(node_info);
        if (!prof_time_info) continue;
        
        Professor* professor = prof_time_info->get_professor();
        Days prof_day = prof_time_info->get_day();
        auto prof_hours = prof_time_info->get_hour_interval();
        
        // Para cada sección tiempo que coincida en día y hora
        for (const auto& sect_pair : section_time_nodes) 
        {
            for (auto sect_graph_node : sect_pair.second) 
            {
                auto sect_time_info = std::dynamic_pointer_cast<SectionTimeNode>(sect_graph_node->get_info());
                if (!sect_time_info) continue;
                
                Section* section = sect_time_info->get_section();
                if (!section) continue;
                
                // Verificar coincidencia de día y hora Y que el profesor sea el correcto
                if (sect_time_info->get_day() == prof_day &&
                    sect_time_info->get_hour_interval() == prof_hours &&
                    section->get_professor() == professor) 
                {
                    // ✅ CALCULAR COSTO SOLO UNA VEZ - aquí es donde importa
                    uint cost = calculate_preference_cost(
                        professor,
                        prof_day,
                        prof_hours.first,
                        prof_hours.second
                    );
                    
                    network.insert_arc(pair.second, sect_graph_node, ArcData(1, cost));
                    prof_to_sect_arcs++;
                }
            }
        }
    }
    std::cout << "   Creados " << prof_to_sect_arcs << " arcos con costos" << std::endl;
    
    // 4. Arcos: SECTION_TIME → SUMIDERO
    std::cout << "4. Secciones tiempo → Sumidero" << std::endl;
    uint sect_to_sink_arcs = 0;
    for (const auto& pair : graph_node_map) 
    {
        auto node_info = pair.second->get_info();
        if (!node_info || node_info->get_type() != FlowNodeType::SECTION_TIME) continue;
        
        network.insert_arc(pair.second, sink, ArcData(1, 0));
        sect_to_sink_arcs++;
    }
    std::cout << "   Creados " << sect_to_sink_arcs << " arcos" << std::endl;
    
    std::cout << "✅ Red construida: " 
              << demand_to_prof_arcs << " + " << prof_to_sect_arcs << " + " << sect_to_sink_arcs 
              << " = " << (demand_to_prof_arcs + prof_to_sect_arcs + sect_to_sink_arcs) << " arcos totales" << std::endl;
}


void FlowNetwork::create_network()
{
    num_section_demand_nodes = 0;
    num_professor_time_nodes = 0;
    num_section_time_nodes = 0;

    create_nodes();
    create_arcs();
}


uint FlowNetwork::calculate_preference_cost(Professor* professor, Days day, uint start_hour, uint end_hour) 
{

    if (!professor || !professor->get_preference()) {
        return 0;
    }
    
    const Preference* preference = professor->get_preference();
    uint cost = 0;
    
    std::cout << "🔍 Analizando preferencias de " << professor->get_name() 
              << " para " << day_to_string(day) << " " << start_hour << "-" << end_hour << std::endl;
    
    // Verificar si la preferencia tiene tipo válido
    if (preference->get_type() == PreferenceType::NO_PREFERENCE) {
        std::cout << "   ℹ️ Sin preferencias específicas" << std::endl;
        return 0;
    }
    
    // Verificar preferencias de días
    if (preference->get_type() == DAYS || preference->get_type() == DAYS_HOURS) 
    {
        const auto& preferred_days = preference->get_days();
        if (!preferred_days.is_empty()) 
        {
            std::cout << "   Días preferidos: ";
            for (const auto& d : preferred_days) {
                std::cout << day_to_string(d) << " ";
            }
            std::cout << std::endl;
            
            if (!preferred_days.contains(day)) 
            {
                cost += 50;
                std::cout << "   ⚠️ Día no preferido: +50 costo" << std::endl;
            }
            else {
                std::cout << "   ✅ Día preferido" << std::endl;
            }
        } else {
            std::cout << "   ℹ️ No hay días preferidos definidos" << std::endl;
        }
    }
    
    // Verificar preferencias de horas - CORREGIDO
    if (preference->get_type() == HOURS || preference->get_type() == DAYS_HOURS) 
    {
        const auto& preferred_hours = preference->get_hours();
        if (!preferred_hours.is_empty()) {
            std::cout << "   Horas preferidas del profesor: ";
            for(const auto& pref : preferred_hours) {
                std::cout << pref.first << "-" << pref.second << " ";
            }
            std::cout << std::endl;
            
            bool found_preferred_time = false;
            
            // Verificar si el horario actual está dentro de algún intervalo preferido
            for(const auto& preferred : preferred_hours) {
                // El horario actual está COMPLETAMENTE dentro del intervalo preferido
                if (start_hour >= preferred.first && end_hour <= preferred.second) {
                    found_preferred_time = true;
                    std::cout << "   ✅ Horario dentro de preferencia: " 
                              << preferred.first << "-" << preferred.second << std::endl;
                    break;
                }
            }
            
            if (!found_preferred_time) {
                cost += 50;
                std::cout << "   ⚠️ Horario no preferido: +50 costo" << std::endl;
            }
        } else {
            std::cout << "   ℹ️ No hay horas preferidas definidas" << std::endl;
        }
    }
    
    std::cout << "   💰 Costo total: " << cost << std::endl;
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
    level_assignments.clear();
    
    std::cout << "\n🎯 INICIANDO ALGORITMO DE FLUJO MÁXIMO COSTO MÍNIMO" << std::endl;
    std::cout << "==================================================" << std::endl;
    
    // Inicializar flujos a 0 en todos los arcos
    uint arcs_with_flow = 0;
    network.for_each_arc([&](auto arc) 
    {
        arc->get_info().flow = 0;
        arcs_with_flow++;
    });
    std::cout << "📊 " << arcs_with_flow << " arcos inicializados" << std::endl;
    
    ResidualGraph residual_graph;
    build_residual_graph(residual_graph);
    
    uint iteration = 0;
    const uint MAX_ITERATIONS = 1000;
    
    while (iteration++ < MAX_ITERATIONS) {
        std::cout << "\n🔄 Iteración " << iteration << ":" << std::endl;
        
        auto path = find_augmenting_path_bellman(residual_graph);
        if (path.empty()) {
            std::cout << "   💡 No hay más caminos aumentantes" << std::endl;
            break;
        }
        
        std::cout << "   📍 Camino encontrado con " << path.size() << " nodos" << std::endl;
        
        uint path_flow = INF;
        uint path_cost = 0;
        
        // Calcular flujo y costo del camino
        for (size_t i = 0; i < path.size() - 1; ++i) 
        {
            GraphNode* u = path[i];
            GraphNode* v = path[i+1];
            
            // Buscar el arco en el grafo residual
            auto& arcs = residual_graph[u];
            for (auto& arc : arcs) 
            {
                if (arc.target == v && arc.capacity > 0) 
                {
                    path_flow = std::min(path_flow, arc.capacity);
                    path_cost += arc.cost;
                    
                    // Depuración del arco utilizado
                    if (arc.cost > 0) {
                        std::cout << "   💰 Arco con costo " << arc.cost 
                                  << " de " << u->get_info()->get_id() 
                                  << " a " << v->get_info()->get_id() << std::endl;
                    }
                    break;
                }
            }
        }
        
        if (path_flow == 0 || path_flow == INF) {
            std::cout << "   💡 Flujo del camino es cero o infinito" << std::endl;
            break;
        }
        
        std::cout << "   📦 Enviando flujo: " << path_flow << " unidades, costo: " << path_cost << std::endl;
        
        // Actualizar flujos
        update_flows_and_residuals(path, path_flow, residual_graph);
        
        total_flow += path_flow;
        total_cost += path_cost * path_flow;
        
        std::cout << "   📊 Acumulado: " << total_flow << " flujo, " << total_cost << " costo" << std::endl;
    }
    
    std::cout << "\n✅ ALGORITMO COMPLETADO" << std::endl;
    std::cout << "   Total iteraciones: " << iteration - 1 << std::endl;
    std::cout << "   Flujo total: " << total_flow << std::endl;
    std::cout << "   Costo total: " << total_cost << std::endl;
    
    extract_schedule();
    
    // Verificar si se cumplieron las preferencias
    analyze_preference_compliance();
    
    return total_flow > 0;
}



void FlowNetwork::analyze_preference_compliance() {
    std::cout << "\n📊 ANÁLISIS DE CUMPLIMIENTO DE PREFERENCIAS" << std::endl;
    std::cout << "========================================" << std::endl;
    
    uint total_assignments = final_schedule.size();
    uint optimal_assignments = 0;
    uint non_optimal_assignments = 0;
    
    for (const auto& entry : final_schedule) {
        auto professor = dm_instance.get_professor(entry.professor_id);
        if (!professor) continue;
        
        uint calculated_cost = calculate_preference_cost(
            professor,
            entry.day,
            entry.start_hour,
            entry.end_hour
        );
        
        if (calculated_cost == 0) {
            optimal_assignments++;
            std::cout << "   ✅ " << professor->get_name() << " -> " << entry.course_name
                      << " (" << day_to_string(entry.day) << " " << entry.start_hour << "-" << entry.end_hour 
                      << ") - ÓPTIMO" << std::endl;
        } else {
            non_optimal_assignments++;
            std::cout << "   ⚠️ " << professor->get_name() << " -> " << entry.course_name
                      << " (" << day_to_string(entry.day) << " " << entry.start_hour << "-" << entry.end_hour 
                      << ") - Costo: " << calculated_cost << std::endl;
        }
    }
    
    std::cout << "\n📈 RESUMEN:" << std::endl;
    std::cout << "   Asignaciones óptimas: " << optimal_assignments << "/" << total_assignments 
              << " (" << (total_assignments > 0 ? (optimal_assignments * 100 / total_assignments) : 0) << "%)" << std::endl;
    std::cout << "   Asignaciones no óptimas: " << non_optimal_assignments << "/" << total_assignments 
              << " (" << (total_assignments > 0 ? (non_optimal_assignments * 100 / total_assignments) : 0) << "%)" << std::endl;
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
    
    std::cout << "🔄 Bellman-Ford iniciado..." << std::endl;
    
    // Bellman-Ford relajación
    for (size_t i = 0; i < graph_node_map.size() - 1; ++i) 
    {
        bool updated = false;
        
        for (auto& [node, arcs] : residual_graph) 
        {
            if (dist[node] == INF) continue;
            
            for (auto& arc : arcs) 
            {
                if (arc.capacity > 0 && dist[node] + arc.cost < dist[arc.target]) {
                    dist[arc.target] = dist[node] + arc.cost;
                    parent[arc.target] = node;
                    parent_arc[arc.target] = &arc;
                    updated = true;
                    
                    std::cout << "   📈 Actualizada distancia para " << arc.target->get_info()->get_id()
                              << ": " << dist[arc.target] << " (costo arco: " << arc.cost << ")" << std::endl;
                }
            }
        }
        
        if (!updated) {
            std::cout << "   ✅ No hay más actualizaciones en iteración " << i << std::endl;
            break;
        }
    }
    
    // Verificar si llegamos al sumidero
    auto sink = graph_node_map["SINK"];
    if (dist[sink] == INF) {
        std::cout << "❌ No se encontró camino al sumidero" << std::endl;
        return {};
    }
    
    std::cout << "✅ Camino encontrado con costo total: " << dist[sink] << std::endl;
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

void FlowNetwork::debug_preferences_loading() {
    std::cout << "\n🔍 DEPURACIÓN DE PREFERENCIAS CARGADAS" << std::endl;
    std::cout << "=====================================" << std::endl;
    
    for (const auto& prof_pair : dm_instance.get_professors()) {
        auto professor = prof_pair.second.get();
        std::cout << "👨‍🏫 Profesor: " << professor->get_name() << std::endl;
        
        if (professor->get_preference()) {
            auto preference = professor->get_preference();
            std::cout << "   Tipo: " << preference_type_to_string(preference->get_type()) << std::endl;
            std::cout << "   Descripción: " << preference->get_description() << std::endl;
            
            std::cout << "   Días: ";
            const auto& days = preference->get_days();
            for (const auto& day : days) {
                std::cout << day_to_string(day) << " ";
            }
            std::cout << "(total: " << days.size() << ")" << std::endl;
            
            std::cout << "   Horas: ";
            const auto& hours = preference->get_hours();
            for (const auto& hour : hours) {
                std::cout << hour.first << "-" << hour.second << " ";
            }
            std::cout << "(total: " << hours.size() << ")" << std::endl;
            
            // Probar cálculo de costo para el horario preferido
            if (!hours.is_empty() && !days.is_empty()) {
                auto sample_hour = *hours.begin();
                auto sample_day = *days.begin();
                uint cost = calculate_preference_cost(professor, sample_day, sample_hour.first, sample_hour.second);
                std::cout << "   ✅ Costo para horario preferido: " << cost << " (debería ser 0)" << std::endl;
            }
        } else {
            std::cout << "   ❌ No tiene preferencias cargadas" << std::endl;
        }
    }
}