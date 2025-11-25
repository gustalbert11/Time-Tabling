#!/bin/bash

echo "📦 Reorganizando estructura del proyecto Time-Tabling..."

# Crear nuevas carpetas
echo "📁 Creando carpetas..."
mkdir -p src/core
mkdir -p src/flow
mkdir -p src/gui
mkdir -p ui
mkdir -p include/core
mkdir -p include/flow

# ------------------------------
# MOVER ARCHIVOS GUI
# ------------------------------

echo "📁 Moviendo archivos GUI..."
mv mainwindow.cpp src/gui/ 2>/dev/null
mv mainwindow.h src/gui/ 2>/dev/null
mv professorform.cpp src/gui/ 2>/dev/null
mv professorform.h src/gui/ 2>/dev/null
mv courseform.cpp src/gui/ 2>/dev/null
mv courseform.h src/gui/ 2>/dev/null
mv sectionwindow.cpp src/gui/ 2>/dev/null
mv sectionwindow.h src/gui/ 2>/dev/null

# UI files
mv mainwindow.ui ui/ 2>/dev/null
mv professorform.ui ui/ 2>/dev/null
mv courseform.ui ui/ 2>/dev/null
mv sectionwindow.ui ui/ 2>/dev/null

# ------------------------------
# MOVER ARCHIVOS CORE
# ------------------------------

echo "📁 Moviendo archivos núcleo (core)..."
mv src/data_manager.cpp src/core/ 2>/dev/null
mv src/course.cpp src/core/ 2>/dev/null
mv src/professor.cpp src/core/ 2>/dev/null
mv src/section.cpp src/core/ 2>/dev/null
mv src/preference.cpp src/core/ 2>/dev/null

mv include/data_manager.hpp include/core/ 2>/dev/null
mv include/course.hpp include/core/ 2>/dev/null
mv include/professor.hpp include/core/ 2>/dev/null
mv include/section.hpp include/core/ 2>/dev/null
mv include/preference.hpp include/core/ 2>/dev/null


# ------------------------------
# MOVER ARCHIVOS FLOW NETWORK
# ------------------------------

echo "🔗 Moviendo archivos de flow network..."
mv src/flow_node.cpp src/flow/ 2>/dev/null
mv src/flow_network.cpp src/flow/ 2>/dev/null
mv src/section_demand_node.cpp src/flow/ 2>/dev/null
mv src/professor_time_node.cpp src/flow/ 2>/dev/null
mv src/section_time_node.cpp src/flow/ 2>/dev/null
mv src/level_time_node.cpp src/flow/ 2>/dev/null

mv include/flow_node.hpp include/flow/ 2>/dev/null
mv include/flow_network.hpp include/flow/ 2>/dev/null
mv include/section_demand_node.hpp include/flow/ 2>/dev/null
mv include/professor_time_node.hpp include/flow/ 2>/dev/null
mv include/section_time_node.hpp include/flow/ 2>/dev/null
mv include/level_time_node.hpp include/flow/ 2>/dev/null
# ------------------------------
# MOVER MAIN
# ------------------------------

echo "📄 Moviendo main.cpp..."
mv main.cpp src/ 2>/dev/null

# ------------------------------
# LIMPIEZA
# ------------------------------

echo "🧹 Limpiando directorios vacíos..."
find . -type d -empty -delete

<<<<<<< HEAD
echo "✅ Reorganización completada con éxito!"
=======
echo "✅ Reorganización completada con éxito!"
>>>>>>> e101d6e (generator schedule)
