# Define target name
set (TARGET_NAME Recast)

# Define source files
file (GLOB CPP_FILES source/*.cpp)
file (GLOB H_FILES include/*.h)
set (SOURCE_FILES ${CPP_FILES} ${H_FILES})

# Define dependency libs
set (INCLUDE_DIRS_ONLY include)

# Setup target
setup_library ()
add_solution_folder (${TARGET_NAME} ThirdParty)