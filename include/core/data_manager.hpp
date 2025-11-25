#pragma once

#include <singleton.hpp>
#include "professor.hpp"
#include "course.hpp"
#include "section.hpp"
#include <unordered_map>

class QJsonObject;

class DataManager : public Designar::Singleton<DataManager>
{
    friend class Designar::Singleton<DataManager>;

public:
    Professor* get_professor(const std::string& id) const;
    Course* get_course(const std::string& id) const;
    Section* get_section(const std::string& id) const;
    size_t get_professor_count() const;
    size_t get_course_count() const;
    size_t get_section_count() const;

    const std::unordered_map<std::string, std::unique_ptr<Professor>> &get_professors() const;
    const std::unordered_map<std::string, std::unique_ptr<Course>> &get_courses() const;
    const std::unordered_map<std::string, std::unique_ptr<Section>> &get_sections() const;

    bool add_professor(std::unique_ptr<Professor> professor);   
    bool add_course(std::unique_ptr<Course> course);
    bool add_section(std::unique_ptr<Section> section);
    
    bool import_professors_from_csv(const std::string& filename);
    bool import_courses_from_csv(const std::string& filename);
    //bool import_sections_from_csv(const std::string& file_name);
    //bool export_to_csv(const std::string& filename) const;
    bool import_from_json(const std::string& filename);
    //bool export_to_json(const std::string& filename) const;
    
    void remove_professor(std::string id);
    void remove_course(std::string id);
    void clear_all_data();

protected:
    DataManager();
    std::unordered_map<std::string, std::unique_ptr<Professor>> professors;
    std::unordered_map<std::string, std::unique_ptr<Course>> courses;
    std::unordered_map<std::string, std::unique_ptr<Section>> sections;

private:
    std::unique_ptr<Preference> process_preference_from_json(const QJsonObject& prefObj);
};

extern DataManager *dm_ptr_instance;
extern DataManager &dm_instance;
