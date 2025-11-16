#pragma once

#include <singleton.hpp>
#include "professor.hpp"
#include "course.hpp"
#include "section.hpp"
#include <unordered_map>
#include <memory>

class DataManager : public Designar::Singleton<DataManager>
{
    friend class Designar::Singleton<DataManager>;

public:
    bool add_professor(std::unique_ptr<Professor> professor);   
    bool add_course(std::unique_ptr<Course> course);
    bool add_section(std::unique_ptr<Section> section);
    
    Professor* get_professor(const std::string& id) const;
    Course* get_course(const std::string& id) const;
    Section* get_section(const std::string& id) const;
    
    bool import_professors_from_CSV(const std::string& filename);
    bool import_courses_from_CSV(const std::string& filename);
    //bool export_to_CSV(const std::string& filename) const;
    bool import_from_JSON(const std::string& filename);
    bool import_from_JSON2(const std::string& filename);
    //bool export_to_JSON(const std::string& filename) const;
    
    void clear_all_data();
    size_t get_professor_count() const;
    size_t get_course_count() const;
    size_t get_section_count() const;

protected:
    DataManager() {}
    std::unordered_map<std::string, std::unique_ptr<Professor>> professors;
    std::unordered_map<std::string, std::unique_ptr<Course>> courses;
    std::unordered_map<std::string, std::unique_ptr<Section>> sections;
};

DataManager *ptr_instance = DataManager::get_ptr_instance();
DataManager &instance = DataManager::get_instance();

// *  Usage example:
//    *
//    *  \code{.cpp}
//    *  class MySingletonClass : public Singleton<MySingletonClass>
//    *  {
//    *    friend class Singleton<MySingletonClass>;
//    *
//    *    // If you need default constructor, make it protected.
//    *  protected:
//    *    MySingletonClass() { }
//    *
//    *    // Any attributes or methods;
//    *  };
//    *
//    *  MySingletonClass * ptr_instance = MySingletonClass::get_ptr_instance();
//    *
//    *  MySingletonClass & instance = MySingletonClass::get_instance();
//    *  \endcode