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

    // Verificación de conteo (debería ser 2 y 2 según el JSON de prueba)
    if (dm_instance.get_professor_count() == 2 && 
    dm_instance.get_course_count() == 2 &&
    dm_instance.get_section_count() == 3)
    {
        cout << "VERIFICACIÓN CONTEO: PASSED. Se importaron 2 profesores, 2 materias y 3 secciones." << endl;

        // for (auto professor : instance.get_professors()) 
        // {
        //    cout << "Profesor ID: " << prof_id << endl;
        // }
        // for (auto course : instance.get_courses()) 
        // {
        //    cout << "Materia ID: " << course_id << endl;
        // }
        // for (auto section : instance.get_sections())
        // {
        //    cout << "Sección ID: " << section_id << endl;
        // }
        // return 0; // Retorna 0 si la prueba es exitosa
    } 
    else 
    {
        cerr << "VERIFICACIÓN CONTEO: FAILED. El conteo no coincide con los datos esperados." << endl;
        return 1; // Retorna 1 si la prueba falla
    }
}