#pragma once

#ifndef _WIN32_WINNT            // 指定要求的最低平台是 Windows Vista
#define _WIN32_WINNT 0x0600     // 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#include "glew.h"

//Urho3d的一些常见头文件
#include "Graphics.h"
#include "Context.h"
#include "Engine.h"
#include "Renderer.h"
#include "Font.h"
#include "Node.h"
#include "Octree.h"
#include "CoreEvents.h"
#include "Model.h"
#include "Camera.h"
#include "Input.h"
#include "IOEvents.h"
#include "Material.h"
#include "ResourceCache.h"
#include "Log.h"
#include "Image.h"
#include "FileSystem.h"

#include "DrawableEvents.h"
#include "VertexBuffer.h"
#include "Geometry.h"
#include "Color.h"

#include "UI.h"
#include "UIEvents.h"
#include "Text.h"
#include "Sprite.h"
#include "Texture2D.h"
#include "Zone.h"
#include "Graphics.h"

#include "Technique.h"
#include "Terrain.h"
#include "TerrainPatch.h"
#include "Scene.h"
#include "StaticModel.h"
#include "AnimatedModel.h"

#include "PhysicsWorld.h"
#include "RigidBody.h"
#include "CollisionShape.h"

#include "DecalSet.h"
using namespace Urho3D;

#include "EditorManager/EditorRoot.h"

//Qt常用头文件
#include <QWidget>
#include <QTextEdit>
#include <QLabel>
#include <QCheckBox>
#include <QRadioButton>
#include <QComboBox>
#include <QPushButton>

#include <QList>
#include <QFrame>
#include <QTreeWidget>
#include <QToolBar>
#include <QStatusBar>
#include <QScrollBar>
#include <QScrollArea>
#include <QMessageBox>
#include <QFileDialog>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QListWidget>

#include <QBoxLayout>
#include <QGridLayout>
#include <QDockWidget>

#include <QImage>
#include <QUrl>
#include <QFile>
#include <QTimer>
#include <QColor>
#include <QFont>
#include <QMouseEvent>
