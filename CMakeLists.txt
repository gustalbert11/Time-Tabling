cmake_minimum_required(VERSION 3.16)
project(TimeTabling VERSION 0.1 LANGUAGES CXX)

# -------------------------------------------------------------
# Configuración general
# -------------------------------------------------------------
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -------------------------------------------------------------
# Qt
# -------------------------------------------------------------
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core)

# -------------------------------------------------------------
# Descargar y compilar Designar
# -------------------------------------------------------------
include(ExternalProject)

set(DESIGNAR_PREFIX "${CMAKE_BINARY_DIR}/external/designar")
set(DESIGNAR_INSTALL_DIR "${DESIGNAR_PREFIX}/install")

ExternalProject_Add(
    Designar_External
    GIT_REPOSITORY https://github.com/R3mmurd/DeSiGNAR.git
    GIT_TAG v2.0.0
    PREFIX "${DESIGNAR_PREFIX}"
    CMAKE_ARGS 
        -DCMAKE_INSTALL_PREFIX=${DESIGNAR_INSTALL_DIR}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    BUILD_ALWAYS OFF
    UPDATE_COMMAND ""
    INSTALL_COMMAND ""  
)

file(MAKE_DIRECTORY "${DESIGNAR_INSTALL_DIR}/include")
file(MAKE_DIRECTORY "${DESIGNAR_INSTALL_DIR}/lib")

add_custom_target(Designar_CopyFiles
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${DESIGNAR_PREFIX}/src/Designar_External/include"
        "${DESIGNAR_INSTALL_DIR}/include"
    COMMAND ${CMAKE_COMMAND} -E copy
        "${DESIGNAR_PREFIX}/src/Designar_External-build/libDesignar.a"
        "${DESIGNAR_INSTALL_DIR}/lib/libDesignar.a"
    DEPENDS Designar_External
    COMMENT "Copiando archivos de Designar..."
)

add_library(Designar STATIC IMPORTED)
set_target_properties(Designar PROPERTIES
    IMPORTED_LOCATION "${DESIGNAR_INSTALL_DIR}/lib/libDesignar.a"
    INTERFACE_INCLUDE_DIRECTORIES "${DESIGNAR_INSTALL_DIR}/include"
)

add_dependencies(Designar Designar_CopyFiles)

# -------------------------------------------------------------
# Archivos del proyecto
# -------------------------------------------------------------
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui

    professorform.cpp
    professorform.h
    professorform.ui

    courseform.cpp
    courseform.h
    courseform.ui

<<<<<<< HEAD
=======
    sectionwindow.cpp
    sectionwindow.h
    sectionwindow.ui

    
>>>>>>> manuel
    src/data_manager.cpp
    src/professor.cpp
    src/course.cpp
    src/section.cpp
    src/preference.cpp

<<<<<<< HEAD
=======
    
    src/flow_node.cpp
    src/section_demand_node.cpp
    src/professor_time_node.cpp
    src/section_time_node.cpp
    src/flow_network.cpp

    
>>>>>>> manuel
    include/data_manager.hpp
    include/professor.hpp
    include/course.hpp
    include/section.hpp
    include/preference.hpp
<<<<<<< HEAD
=======
    include/flow_node.hpp
    include/section_demand_node.hpp
    include/professor_time_node.hpp
    include/section_time_node.hpp
    include/flow_network.hpp
>>>>>>> manuel
)

# -------------------------------------------------------------
# Ejecutable Qt
# -------------------------------------------------------------
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(TimeTabling
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(TimeTabling
        ${PROJECT_SOURCES}
    )
endif()

add_dependencies(TimeTabling Designar)

# -------------------------------------------------------------
# Enlazar librerías
# -------------------------------------------------------------
target_link_libraries(TimeTabling PRIVATE 
    Qt${QT_VERSION_MAJOR}::Widgets
    Designar
)

# ✨ AGREGAR TU CARPETA include/ PARA QUE FUNCIONE AUTOGEN
target_include_directories(TimeTabling PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# -------------------------------------------------------------
# Configuración extra
# -------------------------------------------------------------
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(TimeTabling)
endif()