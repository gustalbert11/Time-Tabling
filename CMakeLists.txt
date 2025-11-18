cmake_minimum_required(VERSION 3.16)

project(TimeTabling VERSION 0.1 LANGUAGES CXX)

# --- INICIO: Configuración DeSiGNAR ---
# 1. Define la ruta a la biblioteca DeSiGNAR,
# set(DESIGNAR_PATH ruta/a/tu/carpeta/DeSiGNAR) 
set(DESIGNAR_PATH /home/gustavo/pr/DeSiGNAR-2.0.0)
# --- FIN: Configuración DeSiGNAR ---

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        src/data_manager.cpp
        src/professor.cpp
        src/course.cpp
        src/section.cpp
        src/preference.cpp 
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(TimeTabling
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(TimeTabling SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(TimeTabling
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# --- INICIO: Enlazar DeSiGNAR y Headers del Proyecto ---

# 2. Añade los directorios de "include"
target_include_directories(TimeTabling PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include  
    ${DESIGNAR_PATH}/include             
)

# 3. Añade la ruta donde CMake buscará las bibliotecas (.so o .a)
target_link_directories(TimeTabling PRIVATE 
    ${DESIGNAR_PATH}/lib                 
)

# 4. Enlaza las bibliotecas
target_link_libraries(TimeTabling PRIVATE 
    Qt${QT_VERSION_MAJOR}::Widgets
    Designar                             
)
# --- FIN: Enlazar DeSiGNAR y Headers del Proyecto ---

# --- INICIO: Configuración para pruebas del DataManager ---

# Agregar el ejecutable de prueba con todos los archivos necesarios
add_executable(test_data_manager 
    test_data_manager.cpp
    src/data_manager.cpp
    src/professor.cpp
    src/course.cpp
    src/section.cpp
    src/preference.cpp  # Si existe este archivo
)

# Configurar las inclusiones
target_include_directories(test_data_manager PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include  
    ${DESIGNAR_PATH}/include             
)

target_link_directories(test_data_manager PRIVATE 
    ${DESIGNAR_PATH}/lib                 
)

target_link_libraries(test_data_manager PRIVATE 
    Qt${QT_VERSION_MAJOR}::Core
    Designar                             
)

# Opcional: Configurar propiedades para el ejecutable de prueba
# set_target_properties(test_data_manager PROPERTIES
#     RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
# )

# --- FIN: Configuración para pruebas del DataManager ---

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.TimeTabling)
endif()
set_target_properties(TimeTabling PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS TimeTabling
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(TimeTabling)
endif()