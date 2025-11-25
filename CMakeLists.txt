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


set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_SOURCE_DIR}/ui")


# -------------------------------------------------------------
# Qt
# -------------------------------------------------------------
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core)

# -------------------------------------------------------------
# Descargar / Compilar Designar
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
# Archivos del proyecto reorganizados
# -------------------------------------------------------------

set(SOURCES_GUI
    src/gui/mainwindow.cpp
    src/gui/professorform.cpp
    src/gui/courseform.cpp
    src/gui/sectionwindow.cpp
)

set(UI_FILES
    ui/mainwindow.ui
    ui/professorform.ui
    ui/courseform.ui
    ui/sectionwindow.ui
)

set(SOURCES_CORE
    src/core/data_manager.cpp
    src/core/course.cpp
    src/core/professor.cpp
    src/core/section.cpp
    src/core/preference.cpp
)

set(SOURCES_FLOW
    src/flow/flow_node.cpp
    src/flow/flow_network.cpp
    src/flow/section_demand_node.cpp
    src/flow/professor_time_node.cpp
    src/flow/section_time_node.cpp
)

set(HEADERS_PUBLIC
    include/core/data_manager.hpp
    include/core/course.hpp
    include/core/professor.hpp
    include/core/section.hpp
    include/core/preference.hpp
    include/flow/flow_node.hpp
    include/flow/flow_network.hpp
    include/flow/section_demand_node.hpp
    include/flow/professor_time_node.hpp
    include/flow/section_time_node.hpp
)

# -------------------------------------------------------------
# Ejecutable Qt
# -------------------------------------------------------------
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(TimeTabling
        MANUAL_FINALIZATION
        src/main.cpp
        ${SOURCES_GUI}
        ${SOURCES_CORE}
        ${SOURCES_FLOW}
        ${UI_FILES}
        ${HEADERS_PUBLIC}
    )
else()
    add_executable(TimeTabling
        src/main.cpp
        ${SOURCES_GUI}
        ${SOURCES_CORE}
        ${SOURCES_FLOW}
        ${UI_FILES}
        ${HEADERS_PUBLIC}
    )
endif()

add_dependencies(TimeTabling Designar)

# -------------------------------------------------------------
# INCLUDES
# -------------------------------------------------------------
target_include_directories(TimeTabling PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# -------------------------------------------------------------
# Enlazar librerías
# -------------------------------------------------------------
target_link_libraries(TimeTabling PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Designar
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(TimeTabling)
endif()
