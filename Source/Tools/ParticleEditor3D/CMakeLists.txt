# Define target name
set (TARGET_NAME ParticleEditor3D)

# Qt
find_package (Qt4 REQUIRED)
include (${QT_USE_FILE})
add_definitions (${QT_DEFINITIONS})

# Add Include Path
INCLUDE_DIRECTORIES (${CMAKE_SOURCE_DIR}/ThirdParty/GLEW)

# Define Project Files
FILE (GLOB_RECURSE H_FILES *.h)
FILE (GLOB_RECURSE CPP_FILES *.cpp)

SET (SOURCE_FILES ${H_FILES} ${CPP_FILES})

# Moccing h Files
qt4_wrap_cpp (MOC_FILES ${H_FILES})
# Group Moc Generated Cpp Files
source_group (moc FILES ${MOC_FILES})

# Rccing qrc files
file (GLOB QRC_FILES *.qrc)
qt4_add_resources( RCC_FILES ${QRC_FILES} )

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

# group
SET (SubDirs Widgets)
foreach (SubDir ${SubDirs})
	FILE (GLOB ${SubDir}_Files ${SubDir}/*.*)
	source_group (${SubDir} FILES ${${SubDir}_Files})
endforeach ()

SET (SOURCE_FILES ${SOURCE_FILES} ${MOC_FILES} ${RCC_FILES})

# Windows SubSystem
setup_executable (WIN32)
target_link_libraries(${TARGET_NAME} ${QT_LIBRARIES})