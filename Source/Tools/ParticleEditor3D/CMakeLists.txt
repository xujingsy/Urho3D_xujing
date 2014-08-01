# Define target name
set (TARGET_NAME ParticleEditor3D)

if (WIN32)
    set (CMAKE_DEBUG_POSTFIX _d)
endif ()

# Qt
find_package (Qt4 REQUIRED)
include (${QT_USE_FILE})
add_definitions (${QT_DEFINITIONS})

# Add Include Path
INCLUDE_DIRECTORIES (${CMAKE_SOURCE_DIR}/ThirdParty/GLEW)

# Define Project Files
FILE (GLOB_RECURSE H_FILES *.h)
FILE (GLOB_RECURSE CPP_FILES *.cpp)

SET (SOURCE_FILES ${H_FILES} ${CPP_FILES} app.rc)

file (GLOB ROOT_FILES *.*)
source_group (Main FILES ${ROOT_FILES})

# Moccing h Files
qt4_wrap_cpp (MOC_FILES ${H_FILES})

# Rccing qrc files
file (GLOB QRC_FILES *.qrc)
qt4_add_resources( RCC_FILES ${QRC_FILES} )

qt4_add_resources( app.rc)

# Group Moc Generated Cpp Files
source_group (QtGenCpps FILES ${MOC_FILES} ${RCC_FILES})

# PreCompile
if (MSVC)
    foreach (FILE ${CPP_FILES})
        if (FILE MATCHES ".*stdafx.*")
            set_source_files_properties (${FILE} PROPERTIES COMPILE_FLAGS "/Ycstdafx.h")
        else ()
            set_source_files_properties (${FILE} PROPERTIES COMPILE_FLAGS "/Yustdafx.h")
        endif ()
    endforeach ()
endif ()

# vs group
SET (SubDirs Widgets)
add_project_dirs (${SubDirs})

SET (SOURCE_FILES ${SOURCE_FILES} ${MOC_FILES} ${RCC_FILES})

# Windows SubSystem
setup_executable (WIN32)
target_link_libraries(${TARGET_NAME} ${QT_LIBRARIES})
