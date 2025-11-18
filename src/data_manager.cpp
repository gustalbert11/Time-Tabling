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

bool DataManager::import_professors_from_CSV(const std::string& file_name)
{
    std::ifstream file(file_name);
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

bool DataManager::import_courses_from_CSV(const std::string& file_name)
{
    std::ifstream file(file_name);
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

bool DataManager::import_from_JSON(const std::string &file_name)
{
    QFile file(QString::fromStdString(file_name));
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

            QString nameStr = pObj.value("name").toString();
            if (nameStr.isEmpty()) 
            {
                continue;
            }

            int numSections = pObj.value("num_sections").toInt(0);
            int maxDailyHours = pObj.value("max_daily_hours").toInt(0);
            int maxConsecHours = pObj.value("max_consecutive_hours").toInt(0);

            auto professor = std::make_unique<Professor>();
            professor->set_name(nameStr.toStdString());
            professor->set_num_sections(static_cast<uint>(numSections));
            professor->set_max_daily_hours(static_cast<uint>(maxDailyHours));
            professor->set_max_consecutive_hours(static_cast<uint>(maxConsecHours));

            if (pObj.contains("preference") && pObj["preference"].isObject())
            {
                QJsonObject prefObj = pObj["preference"].toObject();
                auto preference = process_preference_from_json(prefObj);
                if (preference)
                {
                    professor->set_preference(std::move(preference));
                }
            }

            add_professor(std::move(professor));
        }
    }

    if (rootObj.contains("courses") && 
        rootObj["courses"].isArray()) 
    {
        QJsonArray courseArray = rootObj["courses"].toArray();

        for (const QJsonValue &value : courseArray) 
        {
            if (!value.isObject()) 
            {
                continue;
            }

            QJsonObject cObj = value.toObject();

            QString nameStr = cObj.value("name").toString();
            if (nameStr.isEmpty()) 
            {
                continue;
            }

            int level = cObj.value("level").toInt(0);
            int numCredits = cObj.value("num_credits").toInt(0);
            int numSections = cObj.value("num_sections").toInt(0);
            int weeklyHours = cObj.value("num_weekly_hours").toInt(0);
            int maxDailyHours = cObj.value("max_daily_hours").toInt(0);

            auto course = std::make_unique<Course>();
            course->set_name(nameStr.toStdString());
            course->set_level(static_cast<uint>(level));
            course->set_num_credits(static_cast<uint>(numCredits));
            course->set_num_sections(static_cast<uint>(numSections));
            course->set_num_weekly_hours(static_cast<uint>(weeklyHours));
            course->set_max_daily_hours(static_cast<uint>(maxDailyHours));

            add_course(std::move(course));
        }
    }

    if (rootObj.contains("sections") && 
        rootObj["sections"].isArray()) 
    {
        QJsonArray sectionArray = rootObj["sections"].toArray();
        for (const QJsonValue &value : sectionArray) 
        {
            if (!value.isObject())
            {
                continue;
            }
            
            QJsonObject sObj = value.toObject();
            
            auto section = std::make_unique<Section>();

            if (sObj.contains("professor") && 
                sObj["professor"].isString())
            {
                QString profId = sObj["professor"].toString();
                Professor* prof = get_professor(profId.toStdString());
                if (prof)
                {
                    section->set_professor(prof);
                    prof->add_section(section.get());
                }
            }

            if (sObj.contains("course") && 
                sObj["course"].isString())
            {
                QString courseId = sObj["course"].toString();
                Course* course = get_course(courseId.toStdString());
                if (course)
                {
                    section->set_course(course);
                    course->add_section(section.get());
                }
            }

            if (sObj.contains("time_slots") && 
                sObj["time_slots"].isArray())
            {
                QJsonArray slotsArray = sObj["time_slots"].toArray();
                for (const QJsonValue &slotValue : slotsArray)
                {
                    if (!slotValue.isObject()) 
                    {
                        continue;
                    }
                    
                    QJsonObject slotObj = slotValue.toObject();
                    
                    QString dayStr = slotObj.value("day").toString();
                    int hour = slotObj.value("hour").toInt(-1);
                    
                    if (!dayStr.isEmpty() && 
                        hour >= 1 && 
                        hour <= MAX_DAILY_HOURS)
                    {
                        Days day = string_to_day(dayStr.toStdString());
                        section->add_time_slot(day, static_cast<uint>(hour));
                    }
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

    num_sections = 0;
    num_professors = 0;
    num_courses = 0;
}

std::unique_ptr<Preference> DataManager::process_preference_from_json(const QJsonObject& prefObj)
{
    auto preference = std::make_unique<Preference>();
        
    if (prefObj.contains("description") && 
        prefObj["description"].isString())
    {
        QString desc = prefObj["description"].toString();
        if (!desc.isEmpty())
        {
            preference->set_description(desc.toStdString());
        }
    }
    
    if (prefObj.contains("type") && 
        prefObj["type"].isString())
    {
        QString typeStr = prefObj["type"].toString();

        PreferenceType type = string_to_preference_type(typeStr.toStdString());

        preference->set_type(type);
    }
    
    if (preference.get()->get_type() == PreferenceType::NO_PREFERENCE)
    {
        return preference;
    }

    if (prefObj.contains("days") && 
        prefObj["days"].isArray())
    {
        QJsonArray daysArray = prefObj["days"].toArray();
        for (const QJsonValue &dayValue : daysArray)
        {
            QString dayStr = dayValue.toString();

            Days day = string_to_day(dayStr.toStdString());

            preference->add_day(day);
        }
    }
    
    if (prefObj.contains("hours") && 
        prefObj["hours"].isArray())
    {
        QJsonArray hoursArray = prefObj["hours"].toArray();
        for (const QJsonValue &hourValue : hoursArray)
        {
            int hour = hourValue.toInt(-1);
            if (hour >= 1 && hour <= MAX_DAILY_HOURS)
            {
                preference->add_hour(static_cast<uint>(hour));
            }
        }
    }
    
    return preference;
}