# Timetabling

These instructions will guide you through compiling and running the TimeTabling project on a Linux environment.

---

## 📋 Requirements

Before you begin, please ensure you have the following dependencies installed and configured:

* **CMake** (Version 3.16 or higher)
* **Qt 6** (Development libraries)
* **DeSiGNAR** (The project's dependency library. You can find its repository [here](https://github.com/R3mmurd/DeSiGNAR)).

---

## 🚀 Compiling and Running

The project uses CMake, making the compilation process straightforward.

### 1. Check DeSiGNAR Path

This project links to DeSiGNAR using a path variable. Open the main `CMakeLists.txt` file and ensure the `DESIGNAR_PATH` variable points to the correct location of the DeSiGNAR library on your system:

```cmake
# Verify this path is correct for your machine
set(DESIGNAR_PATH /path/to/your/DeSiGNAR)

### 2. Build the Project
From the root of the project directory, create and navigate into a build directory, configure the project with CMake, and compile using make:

```shell
$ mkdir build
$ cd build
$ cmake .. 
$ make

### 3. Run the Application
After a successful compilation, the executable (named TimeTabling) will be located in the build directory.

```shell
$ ./TimeTabling