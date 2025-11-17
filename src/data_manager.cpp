#include "../include/data_manager.hpp"
#include <fstream>
#include <sstream>

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

// DataManager *ptr_instance = DataManager::get_ptr_instance();
// DataManager &instance = DataManager::get_instance();

Professor* DataManager::get_professor(const std::string& id) const
{
    auto it = professors.find(id);
    return it != professors.end() ? it->second.get() : nullptr;
}
Course* DataManager::get_course(const std::string& id) const
{
    auto it = courses.find(id);
    return it != courses.end() ? it->second.get() : nullptr;
}
Section* DataManager::get_section(const std::string& id) const
{
    auto it = sections.find(id);
    return it != sections.end() ? it->second.get() : nullptr;
}
size_t DataManager::get_professor_count() const
{
    return professors.size();
}
size_t DataManager::get_course_count() const
{
    return courses.size();
}
size_t DataManager::get_section_count() const
{
    return sections.size();
}

bool DataManager::add_professor(std::unique_ptr<Professor> professor)
{
    if (!professor) 
    {
        return false;
    }
    
    std::string id = professor->get_id();
    if (professors.find(id) != professors.end()) 
    {
        return false;
    }
    
    professors[id] = std::move(professor);
    return true;
}
bool DataManager::add_course(std::unique_ptr<Course> course)
{
    if (!course) 
    {
        return false;
    }
    
    std::string id = course->get_id();
    if (courses.find(id) != courses.end()) 
    {
        return false;
    }
    
    courses[id] = std::move(course);
    return true;
}
bool DataManager::add_section(std::unique_ptr<Section> section)
{
    if (!section) 
    {
        return false;
    }
    
    std::string id = section->get_id();
    if (sections.find(id) != sections.end()) 
    {
        return false;
    }
    
    sections[id] = std::move(section);
    return true;
}

bool DataManager::import_professors_from_CSV(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        return false;
    }

    std::string line;

    // Si el archivo tiene encabezado, lo leemos y lo ignoramos.
    if (!std::getline(file, line))
    {
        return false; // archivo vacío
    }

    while (std::getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }

        std::stringstream ss(line);
        std::string id_str, num_sec_str, max_daily_str, max_consec_str;

        std::getline(ss, id_str, ';');
        std::getline(ss, num_sec_str, ';');
        std::getline(ss, max_daily_str, ';');
        std::getline(ss, max_consec_str, ';');

        if (id_str.empty())
        {
            continue;
        }

        auto prof = std::make_unique<Professor>();
        prof->set_id(id_str);
        prof->set_num_sections(static_cast<uint>(std::stoul(num_sec_str)));
        prof->set_max_daily_hours(static_cast<uint>(std::stoul(max_daily_str)));
        prof->set_max_consecutive_hours(static_cast<uint>(std::stoul(max_consec_str)));

        add_professor(std::move(prof));
    }

    file.close();
    return true;
}

bool DataManager::import_courses_from_CSV(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        return false;
    }

    std::string line;

    // Encabezado
    if (!std::getline(file, line))
    {
        return false;
    }

    while (std::getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }

        std::stringstream ss(line);
        std::string id_str, level_str, credits_str, num_sec_str, weekly_str, max_daily_str;

        std::getline(ss, id_str, ';');
        std::getline(ss, level_str, ';');
        std::getline(ss, credits_str, ';');
        std::getline(ss, num_sec_str, ';');
        std::getline(ss, weekly_str, ';');
        std::getline(ss, max_daily_str, ';');

        if (id_str.empty())
        {
            continue;
        }

        auto course = std::make_unique<Course>();
        course->set_id(id_str);
        course->set_level(static_cast<uint>(std::stoul(level_str)));
        course->set_num_credits(static_cast<uint>(std::stoul(credits_str)));
        course->set_num_sections(static_cast<uint>(std::stoul(num_sec_str)));
        course->set_num_weekly_hours(static_cast<uint>(std::stoul(weekly_str)));
        course->set_max_daily_hours(static_cast<uint>(std::stoul(max_daily_str)));

        add_course(std::move(course));
    }

    file.close();
    return true;
}

bool DataManager::import_from_JSON(const std::string &filename)
{
    // Abrir el archivo con Qt
    QFile file(QString::fromStdString(filename));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) 
    {
        return false;
    }

    QByteArray rawData = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(rawData, &parseError);

    if (parseError.error != QJsonParseError::NoError || 
        !doc.isObject()) 
    {
        return false;
    }

    QJsonObject rootObj = doc.object();

    // --- IMPORTAR PROFESORES ---
    if (rootObj.contains("professors") && 
        rootObj["professors"].isArray()) 
    {
        QJsonArray profArray = rootObj["professors"].toArray();

        for (const QJsonValue &value : profArray) 
        {
            if (!value.isObject()) 
            {
                continue;
            }

            QJsonObject pObj = value.toObject();

            QString idStr = pObj.value("id").toString();
            if (idStr.isEmpty()) 
            {
                continue;
            }

            int numSections = pObj.value("num_sections").toInt(0);
            int maxDailyHours = pObj.value("max_daily_hours").toInt(0);
            int maxConsecHours = pObj.value("max_consecutive_hours").toInt(0);

            auto professor = std::make_unique<Professor>();
            professor->set_id(idStr.toStdString());
            professor->set_num_sections(static_cast<uint>(numSections));
            professor->set_max_daily_hours(static_cast<uint>(maxDailyHours));
            professor->set_max_consecutive_hours(static_cast<uint>(maxConsecHours));

            add_professor(std::move(professor));
        }
    }

    // --- IMPORTAR CURSOS ---
    if (rootObj.contains("courses") && rootObj["courses"].isArray()) 
    {
        QJsonArray courseArray = rootObj["courses"].toArray();

        for (const QJsonValue &value : courseArray) 
        {
            if (!value.isObject()) 
            {
                continue;
            }

            QJsonObject cObj = value.toObject();

            QString idStr = cObj.value("id").toString();
            if (idStr.isEmpty()) 
            {
                continue;
            }

            int level = cObj.value("level").toInt(0);
            int numCredits = cObj.value("num_credits").toInt(0);
            int numSections = cObj.value("num_sections").toInt(0);
            int weeklyHours = cObj.value("num_weekly_hours").toInt(0);
            int maxDailyHours = cObj.value("max_daily_hours").toInt(0);

            auto course = std::make_unique<Course>();
            course->set_id(idStr.toStdString());
            course->set_level(static_cast<uint>(level));
            course->set_num_credits(static_cast<uint>(numCredits));
            course->set_num_sections(static_cast<uint>(numSections));
            course->set_num_weekly_hours(static_cast<uint>(weeklyHours));
            course->set_max_daily_hours(static_cast<uint>(maxDailyHours));

            add_course(std::move(course));
        }
    }

    return true;
}

// bool DataManager::import_from_JSON2(const std::string& filename)
// {
//     // Si quieres borrar todo antes de importar, hazlo desde fuera:
//     // DataManager::get_instance().clear_all_data();

//     QFile file(QString::fromStdString(filename));
//     if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
//     {
//         // No se pudo abrir el archivo
//         return false;
//     }

//     QByteArray rawData = file.readAll();
//     file.close();

//     QJsonParseError parseError;
//     QJsonDocument doc = QJsonDocument::fromJson(rawData, &parseError);

//     if (parseError.error != QJsonParseError::NoError || 
//         !doc.isObject())
//     {
//         // JSON inválido o no es un objeto en la raíz
//         return false;
//     }

//     QJsonObject root = doc.object();

//     // ---------------- PROFESORES ----------------
//     if (root.contains("professors") && 
//         root["professors"].isArray())
//     {
//         QJsonArray profArray = root["professors"].toArray();

//         for (const QJsonValue &val : profArray)
//         {
//             if (!val.isObject())
//             {
//                 continue;
//             }

//             QJsonObject obj = val.toObject();

//             QString qid = obj.value("id").toString();
//             if (qid.isEmpty())
//             {
//                 continue;
//             }

//             auto professor = std::make_unique<Professor>();
//             professor->set_id(qid.toStdString());

//             if (obj.contains("num_sections"))
//             {
//                 professor->set_num_sections(
//                     static_cast<uint>(obj.value("num_sections").toInt())
//                 );
//             }
//             if (obj.contains("max_daily_hours"))
//             { 
//                 professor->set_max_daily_hours(
//                     static_cast<uint>(obj.value("max_daily_hours").toInt())
//                 );
//             }
//             if (obj.contains("max_consecutive_hours"))
//             {
//                 professor->set_max_consecutive_hours(
//                     static_cast<uint>(obj.value("max_consecutive_hours").toInt())
//                 );
//             }

//             add_professor(std::move(professor));
//         }
//     }

//     // ---------------- CURSOS/MATERIAS ----------------
//     if (root.contains("courses") && 
//         root["courses"].isArray())
//     {
//         QJsonArray courseArray = root["courses"].toArray();

//         for (const QJsonValue &val : courseArray)
//         {
//             if (!val.isObject())
//             {
//                 continue;
//             }

//             QJsonObject obj = val.toObject();

//             QString qid = obj.value("id").toString();
//             if (qid.isEmpty())
//             {
//                 continue;
//             }

//             auto course = std::make_unique<Course>();
//             course->set_id(qid.toStdString());

//             if (obj.contains("level"))
//             {
//                 course->set_level(
//                     static_cast<uint>(obj.value("level").toInt())
//                 );
//             }
//             if (obj.contains("num_credits"))
//             {
//                 course->set_num_credits(
//                     static_cast<uint>(obj.value("num_credits").toInt())
//                 );
//             }
//             if (obj.contains("num_sections"))
//             {
//                 course->set_num_sections(
//                     static_cast<uint>(obj.value("num_sections").toInt())
//                 );
//             }
//             if (obj.contains("num_weekly_hours"))
//             {
//                 course->set_num_weekly_hours(
//                     static_cast<uint>(obj.value("num_weekly_hours").toInt())
//                 );
//             }
//             if (obj.contains("max_daily_hours"))
//             {
//                 course->set_max_daily_hours(
//                     static_cast<uint>(obj.value("max_daily_hours").toInt())
//                 );
//             }

//             add_course(std::move(course));
//         }
//     }

//     return true;
// }

void DataManager::clear_all_data()
{
    professors.clear();
    courses.clear();
    sections.clear();
}