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
set (TARGET_NAME T2048)
if (URHO3D_OPENGL)
	INCLUDE_DIRECTORIES (${CMAKE_SOURCE_DIR}/ThirdParty/GLEW)
endif ()

# Define source files
file (GLOB_RECURSE H_FILES *.h)
file (GLOB_RECURSE CPP_FILES *.cpp)

define_source_files (EXTRA_H_FILES ${COMMON_SAMPLE_H_FILES} ${H_FILES} ${CPP_FILES})

set (SubDirs Game UI)
foreach (SubDir ${SubDirs})
	FILE (GLOB ${SubDir}_Files ${SubDir}/*.*)
	source_group (${SubDir} FILES ${${SubDir}_Files})
endforeach ()

# Setup target with resource copying
setup_main_executable ()

# Setup test cases
add_test (NAME ${TARGET_NAME} COMMAND ${TARGET_NAME} -timeout ${URHO3D_TEST_TIME_OUT})
