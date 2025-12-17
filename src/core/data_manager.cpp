#include "core/data_manager.hpp"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTextStream>
#include <QStringList>

DataManager *dm_ptr_instance = DataManager::get_ptr_instance();
DataManager &dm_instance = DataManager::get_instance();

Professor* DataManager::get_professor(const std::string &id) const
{
    auto it = professors.find(id);
    return it != professors.end() ? it->second.get() : nullptr;
}
Course* DataManager::get_course(const std::string &id) const
{
    auto it = courses.find(id);
    return it != courses.end() ? it->second.get() : nullptr;
}
Section* DataManager::get_section(const std::string &id) const
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

const std::unordered_map<std::string, std::unique_ptr<Professor>>& DataManager::get_professors() const
{
    return professors;
}
const std::unordered_map<std::string, std::unique_ptr<Course>>& DataManager::get_courses() const
{
    return courses;
}
const std::unordered_map<std::string, std::unique_ptr<Section>>& DataManager::get_sections() const
{
    return sections;
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
    //export_professors_to_csv("professors.csv");
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
    //export_courses_to_csv("courses.csv");
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
    //export_sections_to_csv("sections.csv");
    return true;
}

void DataManager::remove_professor(std::string id)
{
    professors.erase(id);
}
void DataManager::remove_course(std::string id)
{
    courses.erase(id);
}
void DataManager::remove_section(std::string id)
{
    sections.erase(id);
}

bool DataManager::import_from_json(const std::string &filename, bool update_existing)
{
    QFile file(QString::fromStdString(filename));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) 
    {
        return false;
    }
    QByteArray raw_data = file.readAll();
    file.close();

    QJsonParseError parse_error;
    QJsonDocument doc = QJsonDocument::fromJson(raw_data, &parse_error);
    if (parse_error.error != QJsonParseError::NoError || 
        !doc.isObject()) 
    {
        return false;
    }

    QJsonObject root_obj = doc.object();

    // Mapas temporales para traducir IDs del JSON a punteros reales en memoria
    // Esto es CRUCIAL si los IDs internos se autogeneran y difieren del archivo.
    std::unordered_map<std::string, Professor*> json_id_to_prof;
    std::unordered_map<std::string, Course*> json_id_to_course;

    if (root_obj.contains("professors") && 
        root_obj["professors"].isArray()) 
    {
        QJsonArray prof_array = root_obj["professors"].toArray();
        for (const QJsonValue &value : prof_array) 
        {
            if (!value.isObject()) 
            {
                continue;
            }
            QJsonObject prof_obj = value.toObject();

            QString name_str = prof_obj.value("name").toString();
            QString json_id = prof_obj.value("id").toString();

            auto professor = std::make_unique<Professor>();
            professor->set_name(name_str.toStdString());
            //professor->set_num_sections(static_cast<uint>(prof_obj.value("num_sections").toInt()));
            professor->set_max_daily_hours(static_cast<uint>(prof_obj.value("max_daily_hours").toInt()));
            professor->set_max_consecutive_hours(static_cast<uint>(prof_obj.value("max_consecutive_hours").toInt()));

            if (prof_obj.contains("preference") && prof_obj["preference"].isObject()) 
            {
                auto preference = process_preference_from_json(prof_obj["preference"].toObject());
                if (preference) 
                {
                    professor->set_preference(std::move(preference));
                }
            }

            // Guardamos el puntero antes de mover el unique_ptr
            Professor* prof_ptr = professor.get();
            if(add_professor(std::move(professor))) 
            {
                // Mapeamos ID JSON -> Puntero real
                if(!json_id.isEmpty()) 
                {
                    json_id_to_prof[json_id.toStdString()] = prof_ptr;
                }
            }
        }
    }

    if (root_obj.contains("courses") && 
        root_obj["courses"].isArray()) 
    {
        QJsonArray course_array = root_obj["courses"].toArray();
        for (const QJsonValue &value : course_array) 
        {
            if (!value.isObject()) 
            {
                continue;
            }
            QJsonObject course_obj = value.toObject();

            QString name_str = course_obj.value("name").toString();
            QString json_id = course_obj.value("id").toString();

            auto course = std::make_unique<Course>();
            course->set_name(name_str.toStdString());
            course->set_level(static_cast<uint>(course_obj.value("level").toInt()));
            course->set_num_credits(static_cast<uint>(course_obj.value("num_credits").toInt()));
            //course->set_num_sections(static_cast<uint>(course_obj.value("num_sections").toInt()));
            course->set_num_weekly_hours(static_cast<uint>(course_obj.value("num_weekly_hours").toInt()));
            course->set_max_daily_hours(static_cast<uint>(course_obj.value("max_daily_hours").toInt()));

            Course* course_ptr = course.get();
            if(add_course(std::move(course))) 
            {
                if(!json_id.isEmpty()) 
                {
                    json_id_to_course[json_id.toStdString()] = course_ptr;
                }
            }
        }
    }

    if (root_obj.contains("sections") && 
        root_obj["sections"].isArray()) 
    {
        QJsonArray sect_array = root_obj["sections"].toArray();
        for (const QJsonValue &value : sect_array) 
        {
            if (!value.isObject()) 
            {
                continue;
            }
            QJsonObject sect_obj = value.toObject();
            
            auto section = std::make_unique<Section>();

            // Vinculación segura usando el mapa
            if (sect_obj.contains("professor")) 
            {
                std::string prof_id = sect_obj["professor"].toString().toStdString();
                if (json_id_to_prof.count(prof_id)) 
                {
                    Professor* prof = json_id_to_prof[prof_id];
                    section->set_professor(prof);
                    prof->add_section(section.get());
                }
            }

            if (sect_obj.contains("course")) 
            {
                std::string course_id = sect_obj["course"].toString().toStdString();
                if (json_id_to_course.count(course_id)) 
                {
                    Course* course = json_id_to_course[course_id];
                    section->set_course(course);
                    course->add_section(section.get());
                }
            }

            if (sect_obj.contains("time_slots") && 
            sect_obj["time_slots"].isArray()) 
            {
                QJsonArray time_slots_array = sect_obj["time_slots"].toArray();
                for(const QJsonValue& time_slot_val : time_slots_array) 
                {
                    QJsonObject time_slot_obj = time_slot_val.toObject();
                    std::string day_str = time_slot_obj["day"].toString().toStdString();
                    uint start = time_slot_obj["start"].toInt();
                    uint end = time_slot_obj["end"].toInt();
                    
                    // Asumiendo que string_to_day existe y funciona como en Preferences
                    Days day = string_to_day(day_str); 
                    section->add_time_slot(day, start, end);
                }
            }

            add_section(std::move(section));
        }
    }     
    return true;
}
bool DataManager::import_professors_from_csv(const std::string &filename, bool update_existing)
{
    QFile file(QString::fromStdString(filename));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) 
    {
        return false;
    }

    QTextStream in(&file);
    if(!in.atEnd()) 
    {
        in.readLine();
    }

    while (!in.atEnd()) 
    {
        QString line = in.readLine();
        // Nota: Esto es un split simple. Si hay comas DENTRO de las comillas, fallará.
        // Para robustez total se requiere un parser regex, pero para uso académico/interno esto suele bastar.
        QStringList parts = line.split(","); 

        // if (parts.size() < 5) 
        // {
        //     continue;
        // }
        if (parts.size() < 4) 
        {
            continue;
        } 

        // Limpiar comillas de los nombres
        QString name = parts[1];
        name.remove("\"");

        std::string id = parts[0].toStdString();
        
        // Verificamos si ya existe
        Professor* existing_prof = get_professor(id);

        if (existing_prof && update_existing)
        {
            existing_prof->set_name(name.toStdString());
            // existing_prof->set_num_sections(parts[2].toUInt());
            // existing_prof->set_max_daily_hours(parts[3].toUInt());
            // existing_prof->set_max_consecutive_hours(parts[4].toUInt());

            // // Procesar Preferencias si existen
            // if (parts.size() >= 6) 
            // {
            //     auto preference = std::make_unique<Preference>();
            //     std::string type_str = parts[5].toStdString();
            //     preference->set_type(string_to_preference_type(type_str));
                
            //     if (parts.size() >= 7) 
            //     {
            //         QString desc = parts[6];
            //         desc.remove("\"");
            //         preference->set_description(desc.toStdString());
            //     }

            //     if (parts.size() >= 8 && !parts[7].isEmpty()) 
            //     {
            //         auto days = string_to_days(parts[7]);
            //         for(const auto& d : days) 
            //         {
            //             preference->add_day(d);
            //         }
            //     }
                
            //     if (parts.size() >= 9 && !parts[8].isEmpty()) 
            //     {
            //         auto hours = string_to_hours_interval(parts[8]);
            //         for(const auto& h : hours) 
            //         {
            //             preference->add_hour(h.first, h.second);
            //         }
            //     }
            //     existing_prof->set_preference(std::move(preference));
            // }
            existing_prof->set_max_daily_hours(parts[2].toUInt());
            existing_prof->set_max_consecutive_hours(parts[3].toUInt());

            // Procesar Preferencias si existen
            if (parts.size() >= 5) 
            {
                auto preference = std::make_unique<Preference>();
                std::string type_str = parts[4].toStdString();
                preference->set_type(string_to_preference_type(type_str));
                
                if (parts.size() >= 6) 
                {
                    QString desc = parts[5];
                    desc.remove("\"");
                    preference->set_description(desc.toStdString());
                }

                if (parts.size() >= 7 && !parts[6].isEmpty()) 
                {
                    auto days = string_to_days(parts[6]);
                    for(const auto& d : days) 
                    {
                        preference->add_day(d);
                    }
                }
                
                if (parts.size() >= 8 && !parts[7].isEmpty()) 
                {
                    auto hours = string_to_hours_interval(parts[7]);
                    for(const auto& h : hours) 
                    {
                        preference->add_hour(h.first, h.second);
                    }
                }
                existing_prof->set_preference(std::move(preference));
            }
        }
        else if (!existing_prof)
        {
            auto professor = std::make_unique<Professor>();
            professor->set_name(name.toStdString());
            // professor->set_num_sections(parts[2].toUInt());
            // professor->set_max_daily_hours(parts[3].toUInt());
            // professor->set_max_consecutive_hours(parts[4].toUInt());

            // if (parts.size() >= 6) 
            // {
            //     auto preference = std::make_unique<Preference>();
            //     std::string type_str = parts[5].toStdString();
            //     preference->set_type(string_to_preference_type(type_str));
                
            //     if (parts.size() >= 7) 
            //     {
            //         QString desc = parts[6];
            //         desc.remove("\"");
            //         preference->set_description(desc.toStdString());
            //     }

            //     if (parts.size() >= 8 && !parts[7].isEmpty()) 
            //     {
            //         auto days = string_to_days(parts[7]);
            //         for(const auto& d : days) 
            //         {
            //             preference->add_day(d);
            //         }
            //     }
                
            //     if (parts.size() >= 9 && !parts[8].isEmpty()) 
            //     {
            //         auto hours = string_to_hours_interval(parts[8]);
            //         for(const auto& h : hours) 
            //         {
            //             preference->add_hour(h.first, h.second);
            //         }
            //     }
            //     professor->set_preference(std::move(preference));
            // }
            professor->set_max_daily_hours(parts[2].toUInt());
            professor->set_max_consecutive_hours(parts[3].toUInt());

            if (parts.size() >= 5) 
            {
                auto preference = std::make_unique<Preference>();
                std::string type_str = parts[4].toStdString();
                preference->set_type(string_to_preference_type(type_str));
                
                if (parts.size() >= 6) 
                {
                    QString desc = parts[5];
                    desc.remove("\"");
                    preference->set_description(desc.toStdString());
                }

                if (parts.size() >= 7 && !parts[6].isEmpty()) 
                {
                    auto days = string_to_days(parts[6]);
                    for(const auto& d : days) 
                    {
                        preference->add_day(d);
                    }
                }
                
                if (parts.size() >= 8 && !parts[7].isEmpty()) 
                {
                    auto hours = string_to_hours_interval(parts[7]);
                    for(const auto& h : hours) 
                    {
                        preference->add_hour(h.first, h.second);
                    }
                }
                professor->set_preference(std::move(preference));
            }

            // Add verifica si el ID ya existe internamente
            add_professor(std::move(professor));
        }
    }
    file.close();
    return true;
}
bool DataManager::import_courses_from_csv(const std::string &filename, bool update_existing)
{
    QFile file(QString::fromStdString(filename));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) 
    {
        return false;
    }

    QTextStream in(&file);
    if(!in.atEnd()) 
    {
        in.readLine();
    }

    while (!in.atEnd()) 
    {
        QString line = in.readLine();
        QStringList parts = line.split(",");
        
        // if (parts.size() < 7) 
        // {
        //     continue;
        // }
        if (parts.size() < 6) 
        {
            continue;
        }

        QString name = parts[1];
        name.remove("\"");

        std::string id = parts[0].toStdString();
        
        Course* existing_course = get_course(id);

        if (existing_course && update_existing)
        {
            existing_course->set_name(name.toStdString());
            existing_course->set_level(parts[2].toUInt());
            existing_course->set_num_credits(parts[3].toUInt());
            // existing_course->set_num_sections(parts[4].toUInt());
            // existing_course->set_num_weekly_hours(parts[5].toUInt());
            // existing_course->set_max_daily_hours(parts[6].toUInt());
            existing_course->set_num_weekly_hours(parts[4].toUInt());
            existing_course->set_max_daily_hours(parts[5].toUInt());
        }
        else if (!existing_course)
        {
            auto course = std::make_unique<Course>();
            course->set_name(name.toStdString());
            course->set_level(parts[2].toUInt());
            course->set_num_credits(parts[3].toUInt());
            // course->set_num_sections(parts[4].toUInt());
            // course->set_num_weekly_hours(parts[5].toUInt());
            // course->set_max_daily_hours(parts[6].toUInt());
            course->set_num_weekly_hours(parts[4].toUInt());
            course->set_max_daily_hours(parts[5].toUInt());

            add_course(std::move(course));
        }
    }
    file.close();
    return true;
}
bool DataManager::import_sections_from_csv(const std::string &filename, bool update_existing)
{
    QFile file(QString::fromStdString(filename));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) 
    {
        return false;
    }

    QTextStream in(&file);
    if(!in.atEnd()) 
    {
        in.readLine();
    } 

    while (!in.atEnd()) 
    {
        QString line = in.readLine();
        QStringList parts = line.split(",");

        if (parts.size() < 3) 
        {
            continue;
        }

        QString prof_id = parts[1];
        QString course_id = parts[2];

        std::string id = parts[0].toStdString();
        
        Section* existing_section = get_section(id);

        if (existing_section && update_existing)
        {
            if (!prof_id.isEmpty()) 
            {
                Professor* prof = get_professor(prof_id.toStdString());
                if (prof) 
                {
                    existing_section->set_professor(prof);
                    prof->add_section(existing_section);
                }
            }

            if (!course_id.isEmpty()) 
            {
                Course* course = get_course(course_id.toStdString());
                if (course) 
                {
                    existing_section->set_course(course);
                    course->add_section(existing_section);
                }
            }

            // Parsear TimeSlots si existen (Columna 3)
            if (parts.size() > 3 && !parts[3].isEmpty()) 
            {
                QStringList time_slots = parts[3].split(";", Qt::SkipEmptyParts);
                for(const QString& time_slot : time_slots) 
                {
                    // Formato esperado: DAY:START-END (ej: 1:8-10)
                    QStringList data = time_slot.split(":");
                    if(data.size() == 2) 
                    {
                        QStringList hours = data[1].split("-");
                        if(hours.size() == 2) 
                        {
                            // Aquí deberías usar string_to_day si guardaste texto, o cast si guardaste int
                            Days day = static_cast<Days>(data[0].toInt());
                            existing_section->add_time_slot(day, hours[0].toUInt(), hours[1].toUInt());
                        }
                    }
                }
            }
        }
        else if (!existing_section)
        {
            auto section = std::make_unique<Section>();
            
            if (!prof_id.isEmpty()) 
            {
                Professor* prof = get_professor(prof_id.toStdString());
                if (prof) 
                {
                    section->set_professor(prof);
                    prof->add_section(section.get());
                }
            }

            if (!course_id.isEmpty()) 
            {
                Course* course = get_course(course_id.toStdString());
                if (course) 
                {
                    section->set_course(course);
                    course->add_section(section.get());
                }
            }

            // Parsear TimeSlots si existen (Columna 3)
            if (parts.size() > 3 && !parts[3].isEmpty()) 
            {
                QStringList time_slots = parts[3].split(";", Qt::SkipEmptyParts);
                for(const QString& time_slot : time_slots) 
                {
                    // Formato esperado: DAY:START-END (ej: 1:8-10)
                    QStringList data = time_slot.split(":");
                    if(data.size() == 2) 
                    {
                        QStringList hours = data[1].split("-");
                        if(hours.size() == 2) 
                        {
                            // Aquí deberías usar string_to_day si guardaste texto, o cast si guardaste int
                            Days day = static_cast<Days>(data[0].toInt());
                            section->add_time_slot(day, hours[0].toUInt(), hours[1].toUInt());
                        }
                    }
                }
            }

            add_section(std::move(section));
        }
    }
    file.close();
    return true;
}

bool DataManager::export_to_json(const std::string &filename) const
{
    QFile file(QString::fromStdString(filename));
    if (!file.open(QIODevice::WriteOnly)) 
    {
        return false;
    }

    QJsonObject root_obj;

    QJsonArray prof_array;
    for (const auto& [id, prof_ptr] : professors) 
    {
        const Professor* prof = prof_ptr.get();
        QJsonObject prof_obj;

        // Guardamos el ID para mantener referencias, aunque se regenere al importar
        prof_obj["id"] = QString::fromStdString(prof->get_id());
        prof_obj["name"] = QString::fromStdString(prof->get_name());
        //prof_obj["num_sections"] = static_cast<int>(prof->get_num_sections());
        prof_obj["max_daily_hours"] = static_cast<int>(prof->get_max_daily_hours());
        prof_obj["max_consecutive_hours"] = static_cast<int>(prof->get_max_consecutive_hours());

        // Preferencias
        const Preference* pref = prof->get_preference();
        if (pref) 
        {
            QJsonObject pref_obj;
            pref_obj["type"] = QString::fromStdString(preference_type_to_string(pref->get_type()));
            pref_obj["description"] = QString::fromStdString(pref->get_description());

            QJsonArray days_array;
            for (const auto& day : pref->get_days()) 
            {
                days_array.append(QString::fromStdString(day_to_string(day)));
            }
            pref_obj["days"] = days_array;

            QJsonArray hours_array;
            for (const auto& interval : pref->get_hours()) 
            {
                QJsonArray interval_arr;
                interval_arr.append(static_cast<int>(interval.first));
                interval_arr.append(static_cast<int>(interval.second));
                hours_array.append(interval_arr);
            }
            pref_obj["hours"] = hours_array;

            prof_obj["preference"] = pref_obj;
        }
        prof_array.append(prof_obj);
    }
    root_obj["professors"] = prof_array;

    QJsonArray course_array;
    for (const auto& [id, course_ptr] : courses) 
    {
        const Course* course = course_ptr.get();
        QJsonObject course_obj;

        course_obj["id"] = QString::fromStdString(course->get_id());
        course_obj["name"] = QString::fromStdString(course->get_name());
        course_obj["level"] = static_cast<int>(course->get_level());
        course_obj["num_credits"] = static_cast<int>(course->get_num_credits());
        //course_obj["num_sections"] = static_cast<int>(course->get_num_sections());
        course_obj["num_weekly_hours"] = static_cast<int>(course->get_num_weekly_hours());
        course_obj["max_daily_hours"] = static_cast<int>(course->get_max_daily_hours());

        course_array.append(course_obj);
    }
    root_obj["courses"] = course_array;

    QJsonArray sect_array;
    for (const auto& [id, sect_ptr] : sections) 
    {
        const Section* section = sect_ptr.get();
        QJsonObject sect_obj;

        // Referencias por ID (clave para revincular)
        auto professor = section->get_professor();
        if (professor) 
        {
            sect_obj["professor"] = QString::fromStdString(professor->get_id());
        }

        auto course = section->get_course();
        if (course) 
        {
            sect_obj["course"] = QString::fromStdString(course->get_id());
        }

        // Exportar Horarios
        QJsonArray time_slots_array;
        for (const auto& time_slot : section->get_time_slots())
        {
            QJsonObject time_slot_obj;
            time_slot_obj["day"] = QString::fromStdString(day_to_string(time_slot.first));
            time_slot_obj["start"] = static_cast<int>(time_slot.second.first);
            time_slot_obj["end"] = static_cast<int>(time_slot.second.second);
            time_slots_array.append(time_slot_obj);
        }
        if (!time_slots_array.isEmpty())
        {
            sect_obj["time_slots"] = time_slots_array;
        }

        sect_array.append(sect_obj);
    }
    root_obj["sections"] = sect_array;

    QJsonDocument doc(root_obj);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    return true;
}
bool DataManager::export_professors_to_csv(const std::string &filename) const
{
    QFile file(QString::fromStdString(filename));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) 
    {
        return false;
    }

    QTextStream out(&file);
    // out << "ID,Name,NumSections,MaxDaily,MaxConsecutive,PrefType,PrefDesc,PrefDays,PrefHours\n";
    out << "ID,Name,MaxDaily,MaxConsecutive,PrefType,PrefDesc,PrefDays,PrefHours\n";

    for (const auto& [id, prof_ptr] : professors) 
    {
        const Professor* prof = prof_ptr.get();
        const Preference* pref = prof->get_preference();

        QString pref_type = "NO_PREFERENCE";
        QString pref_desc = "";
        QString pref_days = "";
        QString pref_hours = "";

        if (pref) 
        {
            pref_type = QString::fromStdString(preference_type_to_string(pref->get_type()));
            pref_desc = QString::fromStdString(pref->get_description());
            pref_days = QString::fromStdString(days_to_string(pref->get_days()));
            pref_hours = QString::fromStdString(hours_interval_to_string(pref->get_hours()));
        }

        // Manejo básico de comillas para evitar romper el CSV si el nombre tiene comas
        out << QString::fromStdString(prof->get_id()) << ","
            << "\"" << QString::fromStdString(prof->get_name()) << "\","
            //<< prof->get_num_sections() << ","
            << prof->get_max_daily_hours() << ","
            << prof->get_max_consecutive_hours() << ","
            << pref_type << ","
            << "\"" << pref_desc << "\","
            << pref_days << ","
            << pref_hours << "\n";
    }
    file.close();
    return true;
}
bool DataManager::export_courses_to_csv(const std::string &filename) const
{
    QFile file(QString::fromStdString(filename));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) 
    {
        return false;
    }

    QTextStream out(&file);
    // out << "ID,Name,Level,Credits,NumSections,WeeklyHours,MaxDaily\n";
    out << "ID,Name,Level,Credits,WeeklyHours,MaxDaily\n";

    for (const auto& [id, course_ptr] : courses) 
    {
        const Course* course = course_ptr.get();
        out << QString::fromStdString(course->get_id()) << ","
            << "\"" << QString::fromStdString(course->get_name()) << "\","
            << course->get_level() << ","
            << course->get_num_credits() << ","
            //<< course->get_num_sections() << ","
            << course->get_num_weekly_hours() << ","
            << course->get_max_daily_hours() << "\n";
    }
    file.close();
    return true;
}
bool DataManager::export_sections_to_csv(const std::string &filename) const
{
    QFile file(QString::fromStdString(filename));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) 
    {
        return false;
    }

    QTextStream out(&file);
    // El ID de sección se autogenera al importar, pero podemos guardarlo si es necesario.
    // Lo crucial es guardar las relaciones (ID Profe, ID Curso).
    out << "ID,ProfessorID,CourseID,TimeSlots\n"; 

    for (const auto& [id, section_ptr] : sections) 
    {
        const Section* section = section_ptr.get();
        
        QString prof_id = section->get_professor() ? QString::fromStdString(section->get_professor()->get_id()) : "";
        QString course_id = section->get_course() ? QString::fromStdString(section->get_course()->get_id()) : "";
        
        // Serializar TimeSlots (Day, Start, End)
        QStringList slots_list;

        for(const auto& time_slot : section->get_time_slots()) 
        {
            // Formato: DAY_STRING:START-END
            QString dayStr = QString::number(static_cast<int>(time_slot.first)); // Simplificado a int para el ejemplo
            slots_list << QString("%1:%2-%3").arg(dayStr).arg(time_slot.second.first).arg(time_slot.second.second);
        }

        out << QString::fromStdString(section->get_id()) << ","
            << prof_id << ","
            << course_id << ","
            << slots_list.join(";") << "\n";
    }
    file.close();
    return true;
}

void DataManager::clear_all_data()
{
    sections.clear();
    professors.clear();
    courses.clear();

    // num_sections = 0;
    // num_professors = 0;
    // num_courses = 0;
}

DataManager::DataManager()
{

}

std::string DataManager::days_to_string(const Designar::ArraySet<Days> &days) const
{
    QStringList list;

    for(const auto& day : days) 
    {
        // Asumo que existe una función global o estática para convertir Enum a String
        // Si no, deberás implementarla similar a string_to_day
        // Aquí uso una lógica simple basada en tu JSON parser implícito
        //list << QString::number(static_cast<int>(day));
        list << QString::fromStdString(day_to_string(day)); 
    }
    return list.join(";").toStdString();
}
Designar::ArraySet<Days> DataManager::string_to_days(const QString &str) const
{
    Designar::ArraySet<Days> days;
    QStringList list = str.split(";", Qt::SkipEmptyParts);

    for(const auto& s : list) 
    {
        days.insert(string_to_day(s.toStdString())); 
    }
    return days;
}

std::string DataManager::hours_interval_to_string(const Designar::ArraySet<std::pair<uint, uint>> &intervals) const
{
    QStringList list;

    for(const auto& interval : intervals) 
    {
        list << QString("%1-%2").arg(interval.first).arg(interval.second);
    }
    return list.join(";").toStdString();
}
Designar::ArraySet<std::pair<uint, uint>> DataManager::string_to_hours_interval(const QString &str) const
{
    Designar::ArraySet<std::pair<uint, uint>> days;
    QStringList list = str.split(";", Qt::SkipEmptyParts);

    for(const auto& s : list) 
    {
        QStringList pair = s.split("-");
        if(pair.size() == 2) 
        {
            days.insert({pair[0].toUInt(), pair[1].toUInt()});
        }
    }
    return days;
}

std::unique_ptr<Preference> DataManager::process_preference_from_json(const QJsonObject &pref_obj)
{
    auto preference = std::make_unique<Preference>();
        
    if (pref_obj.contains("description") && 
        pref_obj["description"].isString())
    {
        QString desc = pref_obj["description"].toString();
        if (!desc.isEmpty())
        {
            preference->set_description(desc.toStdString());
        }
    }
    
    if (pref_obj.contains("type") && 
        pref_obj["type"].isString())
    {
        QString type_str = pref_obj["type"].toString();

        PreferenceType type = string_to_preference_type(type_str.toStdString());

        preference->set_type(type);
    }
    
    auto pref_type = preference->get_type();
    if (pref_type == PreferenceType::NO_PREFERENCE)
    {
        return preference;
    }    

    if (pref_obj.contains("days") && 
        pref_obj["days"].isArray())
    {
        QJsonArray days_array = pref_obj["days"].toArray();
        for (const QJsonValue &day_value : days_array)
        {
            QString day_str = day_value.toString();

            Days day = string_to_day(day_str.toStdString());
            preference->add_day(day);
        }
    }
    
    if (pref_obj.contains("hours") && 
        pref_obj["hours"].isArray())
    {
        QJsonArray hours_array = pref_obj["hours"].toArray();

        for (const QJsonValue &val : hours_array)
        {
            if (!val.isArray())
            {
                continue;
            }

            QJsonArray interval = val.toArray();

            if (interval.size() != 2)
            {
                continue;
            }

            int start = interval[0].toInt(-1);
            int end = interval[1].toInt(-1);

            if (start >= 0 && end >= 0)
            {
                preference->add_hour(static_cast<uint>(start), static_cast<uint>(end));
            }
        }
    }
    
    return preference;
}