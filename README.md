# Timetabling

#  **Time-Tabling Installation and Compilation Guide**

This tutorial will guide you through the steps to **install all the necessary dependencies** and **compile** the **Time-Tabling** project from scratch on **Ubuntu** or **Debian**.

---

##  **1. Prerequisites**

### **Install Build Essentials**

First, you need to install the essential build tools on your system, such as the `g++` compiler, `make`, and other utilities required to compile the project.

Run the following command in your terminal:

```bash
sudo apt update
sudo apt upgrade
sudo apt install -y build-essential cmake git
```
What does build-essential include?

This package installs:

g++: The C++ compiler (version 11 or higher in Ubuntu 22.04).

make: A tool to automate the build process.

cmake: The build system we use for this project.

git: A tool for managing source code.

## Install Qt6

Next, you need to install Qt6, which is the graphical user interface library used in this project.

Run the following command to install Qt6:

```bash
sudo apt install -y qt6-base-dev qt6-base-dev-tools
```
This will install the main Qt6 modules.

## 2. Download the Project Code
Download a ZIP of the correct branch
Once the dependencies are installed, the next step is to obtain the source code for the project. To do this, you can download the project as a ZIP from the GitHub repository.

Visit the repository on GitHub by clicking [here](https://github.com/gustalbert11/Time-Tabling)

Click on "Code" and select "Download ZIP".

Download the ZIP file and extract it in your working directory.

Alternative: If you prefer using Git, you can clone the repository with the following command:

```bash
git clone -b main --single-branch https://github.com/username/Time-Tabling.git
```
## 3. Create a Build Folder
Inside the directory where you downloaded the project, create a build folder. This is important to keep the build files separate from the source code.

Run:

```bash
cd Time-Tabling 
mkdir build
cd build
```

## 4. Configure the Project with CMake
Inside the build folder, run the following command to configure the project with CMake:

```bash
cmake ..
```
This command will do the following:

Look for Qt6 and other necessary dependencies.

Prepare the project for compilation in the build folder.

## 5. Compile the Project
Once the project is configured, you can compile it using make.

Run:

```bash
make 
```


## 6. Run the Project
Finally, after the project has been successfully compiled, you can run it.

To run the project, simply execute:

```bash
./TimeTabling
```
