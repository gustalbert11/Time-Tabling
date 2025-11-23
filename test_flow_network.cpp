#include <iostream>
#include <cassert>
#include <QCoreApplication>

// Inclusiones de tu proyecto
#include "../include/flow_network.hpp" 

// Nota: Asegúrate de que settings.hpp defina MIN_START_HOUR y MAX_END_HOUR
// para poder calcular los valores esperados manualmente.

using namespace std;

void setup_dummy_data()
{
    dm_instance.clear_all_data();

    // 1. Crear un Profesor
    auto prof = make_unique<Professor>();
    prof->set_name("Alan Turing");
    // ID se genera auto: PROF_1
    dm_instance.add_professor(std::move(prof));

    auto prof2 = make_unique<Professor>();
    prof2->set_name("Alejandro Mujica");
    // ID se genera auto: PROF_2
    dm_instance.add_professor(std::move(prof2));

    // 2. Crear una Materia (4 horas semanales -> Debería generar 2 nodos de demanda)
    auto course = make_unique<Course>();
    course->set_name("Algoritmos");
    course->set_num_weekly_hours(4); 
    // ID se genera auto: COURSE_1
    dm_instance.add_course(std::move(course));

    auto course2 = make_unique<Course>();
    course2->set_name("AyDA");
    course2->set_num_weekly_hours(4); 
    // ID se genera auto: COURSE_1
    dm_instance.add_course(std::move(course2));

    auto course3 = make_unique<Course>();
    course3->set_name("PR3");
    course3->set_num_weekly_hours(6); 
    // ID se genera auto: COURSE_1
    dm_instance.add_course(std::move(course3));

    // 3. Crear una Sección
    auto section = make_unique<Section>();
    
    // Vincular (Obtenemos los punteros raw del dm_instance)
    Professor* p_ptr = dm_instance.get_professor("PROF_1");
    Course* c_ptr = dm_instance.get_course("COURSE_1");

    if (p_ptr && c_ptr) 
    {
        section->set_professor(p_ptr);
        section->set_course(c_ptr);
        
        // Link inverso necesario para la lógica de importación/integridad
        p_ptr->add_section(section.get());
        c_ptr->add_section(section.get());
    }

    dm_instance.add_section(std::move(section));

    auto section2 = make_unique<Section>();
    
    Professor* p_ptr2 = dm_instance.get_professor("PROF_2");
    Course* c_ptr2 = dm_instance.get_course("COURSE_2");

    if (p_ptr2 && c_ptr2) 
    {
        section2->set_professor(p_ptr2);
        section2->set_course(c_ptr2);
        
        // Link inverso necesario para la lógica de importación/integridad
        p_ptr2->add_section(section2.get());
        c_ptr2->add_section(section2.get());
    }

    dm_instance.add_section(std::move(section2));

    auto section3 = make_unique<Section>();
    
    Course* c_ptr3 = dm_instance.get_course("COURSE_3");

    if (p_ptr2 && c_ptr3) 
    {
        section3->set_professor(p_ptr2);
        section3->set_course(c_ptr3);
        
        // Link inverso necesario para la lógica de importación/integridad
        p_ptr2->add_section(section3.get());
        c_ptr3->add_section(section3.get());
    }

    dm_instance.add_section(std::move(section3));

    cout << "Datos Dummy Cargados: 2 Prof, 3 Curso (14h), 3 Sección." << endl;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    cout << "--- TEST FLOW NETWORK ---" << endl;

    // 1. Preparar datos
    setup_dummy_data();

    // 2. Inicializar la Red de Flujo
    
    // Opcional: Reiniciar la red si tu clase tuviera un método clear(), 
    // pero como es singleton y init() tiene un flag, asumimos ejecución limpia.
    cout << "Inicializando red..." << endl;
    fn_instance.init();

    // 3. Obtener conteos
    uint demand_nodes = fn_instance.get_section_demand_nodes_count();
    uint prof_time_nodes = fn_instance.get_professor_time_nodes_count();
    uint section_time_nodes = fn_instance.get_section_time_nodes_count();

    cout << "\n--- RESULTADOS ---" << endl;
    cout << "Nodos de Demanda (Sección): " << demand_nodes << endl;
    cout << "Nodos de Tiempo (Profesor): " << prof_time_nodes << endl;
    cout << "Nodos de Tiempo (Sección):  " << section_time_nodes << endl;

    // 4. Verificaciones (Aserciones)
    
    // A. Verificación de Demanda
    // El curso tiene 4 horas semanales. Los bloques son de 2 horas.
    // Esperamos 4 / 2 = 2 nodos de demanda.
    if (demand_nodes == 7) {
        cout << "[OK] Conteo de Nodos de Demanda correcto (7)." << endl;
    } else {
        cout << "[FAIL] Error en Nodos de Demanda. Esperado: 2, Obtenido: " << demand_nodes << endl;
    }

    // B. Verificación de Nodos de Tiempo (Simetría)
    // Dado que hay 1 profesor y 1 sección, la cantidad de nodos de tiempo
    // disponibles generados por los bucles "for" debería ser idéntica para ambos.
    if (prof_time_nodes == 90 && section_time_nodes == 135) {
        cout << "[OK] Nodos de tiempo correctos (Profesores-tiempo: " << prof_time_nodes << " nodos, " << "Secciones-tiempo: " << section_time_nodes << " nodos)." << endl;
    } else {
        cout << "[FAIL] Los nodos de tiempo no coinciden o son cero." << endl;
    }

    return 0;
}