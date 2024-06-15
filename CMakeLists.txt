# Versión mínima de CMake requerida
cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto
project(CNotes)

# Establecer el estándar de C
set(CMAKE_C_STANDARD 11)

# Incluir los directorios src y nuklear para que CMake pueda encontrar los archivos .h
include_directories(src nuklear)

# Buscar y vincular las librerías de X11
find_package(X11 REQUIRED)
include_directories(${X11_INCLUDE_DIR})

# Specify the source files
set(SOURCES

   src/app.c
   src/style.c
   src/Gui_Text_Title.c
   src/Gui_Archive.c
   src/Gui_Layout.c
   src/Gui_Connector.c
)

# Añadir los archivos fuente al ejecutable
add_executable(CNotes ${SOURCES})

# Vincular las librerías de X11
target_link_libraries(CNotes ${X11_LIBRARIES})
