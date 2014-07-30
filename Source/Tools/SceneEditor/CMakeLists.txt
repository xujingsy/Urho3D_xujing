#
# Copyright (c) 2008-2014 the Urho3D project.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#

# Define target name
set (TARGET_NAME SceneEditor)

# Qt
find_package (Qt4 REQUIRED)
include (${QT_USE_FILE})
add_definitions (${QT_DEFINITIONS})

# Add Include Path
INCLUDE_DIRECTORIES (${CMAKE_SOURCE_DIR}/ThirdParty/GLEW)
INCLUDE_DIRECTORIES (${CMAKE_SOURCE_DIR}/ThirdParty/Assimp/include)

# Define source files
# define_source_files ()

# define Project Files
file (GLOB_RECURSE H_FILES *.h)
file (GLOB_RECURSE CPP_FILES *.cpp)

#foreach (FILE ${CPP_TMPS})
#	list (APPEND ${CPP_FILES} string (REPLACE ${FILE} (${CMAKE_SOURCE_DIR}) ("") (${FILE}) ))
#endforeach()

# foreach (FILE ${H_TMPS})
#	list (APPEND ${H_FILES} string (REPLACE ${FILE} (${CMAKE_SOURCE_DIR}) ("") (${FILE}) ))
# endforeach()

set (SOURCE_FILES ${H_FILES} ${CPP_FILES} app.rc)

# Compile ui files
# file (GLOB UI_FILES *.ui)
# qt4_wrap_ui (UI_HEADERS ${UI_FILES})

# Rccing qrc files
file (GLOB QRC_FILES *.qrc)
qt4_add_resources (RCC_FILES ${QRC_FILES})

QT4_ADD_RESOURCES(app.rc)

# Moccing h files
qt4_wrap_cpp (MOC_FILES ${H_FILES} )
source_group (QtGenCpps FILES ${MOC_FILES} ${RCC_FILES})

# PreCompile
if (MSVC)
    foreach (FILE ${CPP_FILES})
        if (FILE MATCHES ".*stdafx.cpp.*")
            set_source_files_properties (${FILE} PROPERTIES COMPILE_FLAGS "/Ycstdafx.h")
        else ()
            set_source_files_properties (${FILE} PROPERTIES COMPILE_FLAGS "/Yustdafx.h")
        endif ()
    endforeach ()
endif ()

#file(WRITE SceneGenLog.txt "message to write")

# group
set (SubDirs Controls EditorAPI EditorAssist EditorInfo EditorWindow Manager ObjectEditor OtherTools Terrain)
add_project_dirs (${SubDirs})

set (SOURCE_FILES ${SOURCE_FILES} ${MOC_FILES} ${UI_HEADERS} ${RCC_FILES})

# Windows SubSystem
setup_executable (WIN32)
target_link_libraries(${TARGET_NAME} ${QT_LIBRARIES})
