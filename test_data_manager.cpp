#include <QCoreApplication>
#include <iostream>

// Incluye el Singleton y tus modelos
#include "../include/data_manager.hpp" 
#include "../include/professor.hpp"
#include "../include/course.hpp"

using namespace std;

// Función principal para la prueba
int main(int argc, char *argv[])
{
    // 1. Inicializar QCoreApplication
    // Esto es NECESARIO para que las clases de Qt (QFile, QJsonDocument, etc.) funcionen.
    QCoreApplication a(argc, argv);
    
    cout << "--- INICIANDO PRUEBA DE DATA MANAGER (IMPORTAR JSON) ---" << endl;
    
    const string json_filename = "../test_data.json";
    
    // (Paso opcional: crear el archivo JSON aquí programáticamente para la prueba)
    // Para simplificar, asumiremos que 'test_data.json' ya existe en el directorio de ejecución.
    
    // Limpiar antes de importar para asegurar la prueba
    dm_instance.clear_all_data(); 

    // 3. Realizar la importación
    cout << "Intentando importar datos desde " << json_filename << "..." << endl;
    bool import_successful = dm_instance.import_from_JSON(json_filename);
    
    // 4. Verificación de resultados
    if (import_successful) 
    {
        cout << "Importación JSON: OK." << endl;
    } 
    else 
    {
        cerr << "Importación JSON: ERROR. La función import_from_JSON devolvió FALSE." << endl;
    }
    
    cout << "\n--- RESULTADOS DE LA VERIFICACIÓN ---" << endl;
    cout << "Profesores importados: " << dm_instance.get_professor_count() << endl;
    cout << "Materias importadas: " << dm_instance.get_course_count() << endl;
    cout << "Secciones importadas: " << dm_instance.get_section_count() << endl;

    if (dm_instance.get_professor_count() == 3 && 
    dm_instance.get_course_count() == 2 &&
    dm_instance.get_section_count() == 3)
    {
        cout << "VERIFICACIÓN CONTEO: PASSED. Se importaron 3 profesores, 2 materias y 3 secciones." << endl;

        for (const auto &professor_pair : dm_instance.get_professors()) 
        {
            auto professor = professor_pair.second.get();
            
            cout << "Profesor ID: " << professor->get_id() << endl;
            cout << "Número de Secciones: " << professor->get_num_sections() << endl;

            auto pref = professor->get_preference();

            if (!pref)
            {
                continue;
            }

            cout << "Preferencia descripción: " << pref->get_description() << endl;

            auto pref_type = pref->get_type();

            cout << "Preferencia tipo: " << preference_type_to_string(pref_type) << endl;
            
            if (pref_type == PreferenceType::NO_PREFERENCE)
            {
                continue;
            }
            
            cout << "Preferencia días: " << endl;
            for (const auto &day : pref->get_days()) 
            {
                cout << "  Día: " << day_to_string(day) << endl;
            }
            cout << "Preferencia horas: " << endl;
            for (const auto &hour : pref->get_hours())
            {
                cout << "  Hora: " << hour << endl;
            }
        }
        for (const auto &course_pair : dm_instance.get_courses()) 
        {
           auto course = course_pair.second.get();
           
            cout << "Materia ID: " << course->get_id() << endl;
           cout << "  Nivel: " << course->get_level() << endl;
           cout << "  Créditos: " << course->get_num_credits() << endl;
           cout << "  Número de Secciones: " << course->get_num_sections() << endl;
        }
        for (const auto &section_pair : dm_instance.get_sections())
        {
           auto section = section_pair.second.get();
           
            cout << "Sección ID: " << section->get_id() << endl;
        }
        return 0; // Retorna 0 si la prueba es exitosa
    } 
    else 
    {
        cerr << "VERIFICACIÓN CONTEO: FAILED. El conteo no coincide con los datos esperados." << endl;
        return 1; // Retorna 1 si la prueba falla
    }
}