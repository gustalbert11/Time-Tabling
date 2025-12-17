#pragma once

#include <singleton.hpp>
#include "professor.hpp"
#include "course.hpp"
#include "section.hpp"
#include <unordered_map>

class QString;
class QJsonObject;

class DataManager : public Designar::Singleton<DataManager>
{
    friend class Designar::Singleton<DataManager>;

public:
    Professor* get_professor(const std::string &id) const;
    Course* get_course(const std::string &id) const;
    Section* get_section(const std::string &id) const;
    size_t get_professor_count() const;
    size_t get_course_count() const;
    size_t get_section_count() const;

    const std::unordered_map<std::string, std::unique_ptr<Professor>> &get_professors() const;
    const std::unordered_map<std::string, std::unique_ptr<Course>> &get_courses() const;
    const std::unordered_map<std::string, std::unique_ptr<Section>> &get_sections() const;

    bool add_professor(std::unique_ptr<Professor> professor);   
    bool add_course(std::unique_ptr<Course> course);
    bool add_section(std::unique_ptr<Section> section);

    void remove_professor(std::string id);
    void remove_course(std::string id);
    void remove_section(std::string id);
    
    bool import_from_json(const std::string &filename, bool update_existing = false);
    bool import_professors_from_csv(const std::string &filename, bool update_existing = false);
    bool import_courses_from_csv(const std::string &filename, bool update_existing = false);
    bool import_sections_from_csv(const std::string &filename, bool update_existing = false);
    
    bool export_to_json(const std::string &filename) const;
    bool export_professors_to_csv(const std::string &filename) const;
    bool export_courses_to_csv(const std::string &filename) const;
    bool export_sections_to_csv(const std::string &filename) const;
    
    void clear_all_data();

protected:
    DataManager();
    std::unordered_map<std::string, std::unique_ptr<Professor>> professors;
    std::unordered_map<std::string, std::unique_ptr<Course>> courses;
    std::unordered_map<std::string, std::unique_ptr<Section>> sections;

private:
    std::string days_to_string(const Designar::ArraySet<Days> &days) const;
    Designar::ArraySet<Days> string_to_days(const QString &str) const;
    
    std::string hours_interval_to_string(const Designar::ArraySet<std::pair<uint, uint>>& intervals) const;
    Designar::ArraySet<std::pair<uint, uint>> string_to_hours_interval(const QString &str) const;

    std::unique_ptr<Preference> process_preference_from_json(const QJsonObject &pref_obj);
};

extern DataManager *dm_ptr_instance;
extern DataManager &dm_instance;
