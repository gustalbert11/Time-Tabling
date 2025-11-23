#include "../include/data_manager.hpp"
#include <fstream>
#include <sstream>

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

DataManager *dm_ptr_instance = DataManager::get_ptr_instance();
DataManager &dm_instance = DataManager::get_instance();

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

bool DataManager::import_professors_from_csv(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        return false;
    }

    std::string line;

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
        std::string name_str, num_sec_str, max_daily_str, max_consec_str;

        std::getline(ss, name_str, ';');
        std::getline(ss, num_sec_str, ';');
        std::getline(ss, max_daily_str, ';');
        std::getline(ss, max_consec_str, ';');

        if (name_str.empty())
        {
            continue;
        }

        auto prof = std::make_unique<Professor>();
        prof->set_name(name_str);
        prof->set_num_sections(static_cast<uint>(std::stoul(num_sec_str)));
        prof->set_max_daily_hours(static_cast<uint>(std::stoul(max_daily_str)));
        prof->set_max_consecutive_hours(static_cast<uint>(std::stoul(max_consec_str)));

        add_professor(std::move(prof));
    }

    file.close();
    return true;
}

bool DataManager::import_courses_from_csv(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        return false;
    }

    std::string line;

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
        std::string name_str, level_str, credits_str, num_sec_str, weekly_str, max_daily_str;

        std::getline(ss, name_str, ';');
        std::getline(ss, level_str, ';');
        std::getline(ss, credits_str, ';');
        std::getline(ss, num_sec_str, ';');
        std::getline(ss, weekly_str, ';');
        std::getline(ss, max_daily_str, ';');

        if (name_str.empty())
        {
            continue;
        }

        auto course = std::make_unique<Course>();
        course->set_name(name_str);
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

bool DataManager::import_from_json(const std::string &filename)
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
            if (name_str.isEmpty()) 
            {
                continue;
            }

            int num_sections = prof_obj.value("num_sections").toInt(0);
            int max_daily_hours = prof_obj.value("max_daily_hours").toInt(0);
            int max_consecutive_hours = prof_obj.value("max_consecutive_hours").toInt(0);

            auto professor = std::make_unique<Professor>();
            professor->set_name(name_str.toStdString());
            professor->set_num_sections(static_cast<uint>(num_sections));
            professor->set_max_daily_hours(static_cast<uint>(max_daily_hours));
            professor->set_max_consecutive_hours(static_cast<uint>(max_consecutive_hours));

            if (prof_obj.contains("preference") && 
                prof_obj["preference"].isObject())
            {
                QJsonObject pref_obj = prof_obj["preference"].toObject();
                auto preference = process_preference_from_json(pref_obj);
                if (preference)
                {
                    professor->set_preference(std::move(preference));
                }
            }

            add_professor(std::move(professor));
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
            if (name_str.isEmpty()) 
            {
                continue;
            }

            int level = course_obj.value("level").toInt(0);
            int num_credits = course_obj.value("num_credits").toInt(0);
            int num_sections = course_obj.value("num_sections").toInt(0);
            int num_weekly_hours = course_obj.value("num_weekly_hours").toInt(0);
            int max_daily_hours = course_obj.value("max_daily_hours").toInt(0);

            auto course = std::make_unique<Course>();
            course->set_name(name_str.toStdString());
            course->set_level(static_cast<uint>(level));
            course->set_num_credits(static_cast<uint>(num_credits));
            course->set_num_sections(static_cast<uint>(num_sections));
            course->set_num_weekly_hours(static_cast<uint>(num_weekly_hours));
            course->set_max_daily_hours(static_cast<uint>(max_daily_hours));

            add_course(std::move(course));
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

            if (sect_obj.contains("professor") && 
                sect_obj["professor"].isString())
            {
                QString prof_id = sect_obj["professor"].toString();
                Professor* professor = get_professor(prof_id.toStdString());
                if (professor)
                {
                    section->set_professor(professor);
                    professor->add_section(section.get());
                }
            }

            if (sect_obj.contains("course") && 
                sect_obj["course"].isString())
            {
                QString course_id = sect_obj["course"].toString();
                Course* course = get_course(course_id.toStdString());
                if (course)
                {
                    section->set_course(course);
                    course->add_section(section.get());
                }
            }

            add_section(std::move(section));
        }
    }     

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

void DataManager::remove_professor(std::string id)
{
    professors.erase(id);
}

std::unique_ptr<Preference> DataManager::process_preference_from_json(const QJsonObject& pref_obj)
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