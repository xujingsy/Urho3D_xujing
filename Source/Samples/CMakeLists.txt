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

# Set project name
project (Urho3D-Samples)

# Find Urho3D library
find_package (Urho3D REQUIRED)
include_directories (${URHO3D_INCLUDE_DIRS})

# Include common to all samples
set (COMMON_SAMPLE_H_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Sample.h" "${CMAKE_CURRENT_SOURCE_DIR}/Sample.inl")

# Define dependency libs
set (INCLUDE_DIRS_ONLY ${CMAKE_CURRENT_SOURCE_DIR})

# Add samples
add_project_dir (01_HelloWorld Samples)
add_project_dir (02_HelloGUI Samples)
add_project_dir (03_Sprites Samples)
add_project_dir (04_StaticScene Samples)
add_project_dir (05_AnimatingScene Samples)
add_project_dir (06_SkeletalAnimation Samples)
add_project_dir (07_Billboards Samples)
add_project_dir (08_Decals Samples)
add_project_dir (09_MultipleViewports Samples)
add_project_dir (10_RenderToTexture Samples)
add_project_dir (11_Physics Samples)
add_project_dir (12_PhysicsStressTest Samples)
add_project_dir (13_Ragdolls Samples)
add_project_dir (14_SoundEffects Samples)
add_project_dir (15_Navigation Samples)
add_project_dir (16_Chat Samples)
add_project_dir (17_SceneReplication Samples)
add_project_dir (18_CharacterDemo Samples)
add_project_dir (19_VehicleDemo Samples)
add_project_dir (20_HugeObjectCount Samples)
if (URHO3D_ANGELSCRIPT)
    add_project_dir (21_AngelScriptIntegration Samples)
endif ()
if (URHO3D_LUA)
    add_project_dir (22_LuaIntegration Samples)
endif ()

add_project_dir (23_Water Samples)
add_project_dir (24_Urho2DSprite Samples)
add_project_dir (25_Urho2DParticle Samples)
add_project_dir (26_ConsoleInput Samples)
add_project_dir (27_Urho2DPhysics Samples)
add_project_dir (28_Urho2DPhysicsRope Samples)
add_project_dir (29_SoundSynthesis Samples)
add_project_dir (30_LightAnimation Samples)
add_project_dir (31_MaterialAnimation Samples)
add_project_dir (32_Urho2DConstraints Samples)
add_project_dir (33_Urho2DSpriterAnimation Samples)
add_project_dir (34_DynamicGeometry Samples)
add_project_dir (35_SignedDistanceFieldText Samples)
