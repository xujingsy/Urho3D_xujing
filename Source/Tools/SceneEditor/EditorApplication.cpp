#include "stdafx.h"
#include "EditorApplication.h"
#include "ProcessUtils.h"
#include "MainWindow.h"
#include "DebugHud.h"
#include "Skybox.h"
#include "EditorSceneWidget.h"
#include "ComponentEditor/Rotator.h"
#include "DebugRenderer.h"
#include "ComponentEditor/StaticModelWidget.h"

EditorApplication::EditorApplication(int argc, char** argv,Context* context) : QApplication(argc, argv), Object(context)
{
	EditorRoot::Instance()->context_ = context;

	SetStyleSheet("makehuman.qss");

    mainWindow_ = new MainWindow();
}

EditorApplication::~EditorApplication()
{
    delete mainWindow_;
}

void EditorApplication::SetStyleSheet(const char* qssPath)
{
	QFile file(qssPath);
	file.open(QFile::ReadOnly);
	QString styleSheet = QLatin1String(file.readAll());
	qApp->setStyleSheet(styleSheet);
	file.close();
}

void EditorApplication::HandleLogMessage(StringHash eventType, VariantMap& eventData)
{
	using namespace LogMessage;
	int level = eventData[P_LEVEL].GetInt();

	Vector<String> rows = eventData[P_MESSAGE].GetString().Split('\n');

	for(int i = 0;i < rows.Size();i ++)
	{
		mainWindow_->AddLog(level,rows[i].CString());
	}
}

int EditorApplication::Run()
{
    if (!mainWindow_)
        return -1;

	//注册自定义类
	context_->RegisterFactory<Rotator>();
	context_->RegisterFactory<TerrainBrush>();

	gEditorGlobalInfo = new EditorGlobalInfo(context_);

    engine_ = new Engine(context_);

	editorRoot_ = EditorRoot::Instance();

	editorRoot_->engine_ = engine_;

    VariantMap engineParameters;
	engineParameters["Headless"] = false;
	engineParameters["FullScreen"] = false;
	engineParameters["WindowWidth"] = 1024;
	engineParameters["WindowHeight"] = 768;

    engineParameters["FrameLimiter"] = false;
    engineParameters["LogName"] = "Urho3DEditor.log";    
    engineParameters["ExternalWindow"] = mainWindow_->GetSceneWidget()->winId();
    if (!engine_->Initialize(engineParameters))
        return -1;

	Input* input = GetSubsystem<Input>();
	input->SetMouseVisible(true);

	//调试信息面板
	CreateDebugHud();

	//注册事件
	SubscribeToEvents();

	//初始化编辑器
	editorRoot_->InitEditors(context_);
	editorRoot_->InitEditorUI(context_);

	//创建默认场景
	EditorRoot::Instance()->NewScene();

	mainWindow_->GetProjectView()->Init(EditorRoot::Instance()->scene_);

    QTimer timer;
    connect(&timer, SIGNAL(timeout()), this, SLOT(OnTimeout()));
    timer.start(1000 / 60);


    return QApplication::exec();
}

void EditorApplication::OnTimeout()
{
    if (engine_ && !engine_->IsExiting())
        engine_->RunFrame();
}

void EditorApplication::CreateDebugHud()
{
    // Get default style
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    XMLFile* xmlFile = cache->GetResource<XMLFile>("UI/DefaultStyle.xml");

    // Create debug HUD.
    DebugHud* debugHud = engine_->CreateDebugHud();
    debugHud->SetDefaultStyle(xmlFile);
}

void EditorApplication::InitUI()
{
	ResourceCache* cache = GetSubsystem<ResourceCache>();

	UI* ui = context_->GetSubsystem<UI>();
	

	UIElement* pRoot = ui->GetRoot();
//	pRoot->AddChild(toolBox);
	//toolBox->SetAlignment(HA_RIGHT,VA_TOP);
}

void EditorApplication::HandleKeyDown(StringHash eventType, VariantMap& eventData)
{
    using namespace KeyDown;

    int key = eventData[P_KEY].GetInt();

    // Close console (if open) or exit when ESC is pressed
    if (key == KEY_ESC)
    {
		//取消选择等操作 todo:
    }
    else if (key == KEY_F2)    // Toggle debug HUD with F2
	{
        GetSubsystem<DebugHud>()->ToggleAll();
	}

    // Common rendering quality controls, only when UI has no focused element
    else if (!GetSubsystem<UI>()->GetFocusElement())
    {
        Renderer* renderer = GetSubsystem<Renderer>();

        // Texture quality
        if (key == '1')
        {
            int quality = renderer->GetTextureQuality();
            ++quality;
            if (quality > QUALITY_HIGH)
                quality = QUALITY_LOW;
            renderer->SetTextureQuality(quality);
        }
        // Material quality
        else if (key == '2')
        {
            int quality = renderer->GetMaterialQuality();
            ++quality;
            if (quality > QUALITY_HIGH)
                quality = QUALITY_LOW;
            renderer->SetMaterialQuality(quality);
        }
        // Specular lighting
        else if (key == '3')
            renderer->SetSpecularLighting(!renderer->GetSpecularLighting());
        // Shadow rendering
        else if (key == '4')
            renderer->SetDrawShadows(!renderer->GetDrawShadows());
        // Shadow map resolution
        else if (key == '5')
        {
            int shadowMapSize = renderer->GetShadowMapSize();
            shadowMapSize *= 2;
            if (shadowMapSize > 2048)
                shadowMapSize = 512;
            renderer->SetShadowMapSize(shadowMapSize);
        }
        // Shadow depth and filtering quality
        else if (key == '6')
        {
            int quality = renderer->GetShadowQuality();
            ++quality;
            if (quality > SHADOWQUALITY_HIGH_24BIT)
                quality = SHADOWQUALITY_LOW_16BIT;
            renderer->SetShadowQuality(quality);
        }
        // Occlusion culling
        else if (key == '7')
        {
            bool occlusion = renderer->GetMaxOccluderTriangles() > 0;
            occlusion = !occlusion;
            renderer->SetMaxOccluderTriangles(occlusion ? 5000 : 0);
        }
        // Instancing
        else if (key == '8')
        {
			renderer->SetDynamicInstancing(!renderer->GetDynamicInstancing());
        }
		else
		{
			//todo:
			editorRoot_->OnKeyDown(key);
		}
    }
}

void EditorApplication::MoveCamera(float timeStep)
{
    // Do not move if the UI has a focused element (the console)
    if (GetSubsystem<UI>()->GetFocusElement())
        return;

    Input* input = GetSubsystem<Input>();
    // Movement speed as world units per second
    const float MOVE_SPEED = 18.0f;

    //上下左右Camera行走
    if (input->GetKeyDown('W'))
        editorRoot_->cameraNode_->Translate(Vector3::FORWARD * MOVE_SPEED * timeStep);
    if (input->GetKeyDown('S'))
        editorRoot_->cameraNode_->Translate(Vector3::BACK * MOVE_SPEED * timeStep);
    if (input->GetKeyDown('A'))
        editorRoot_->cameraNode_->Translate(Vector3::LEFT * MOVE_SPEED * timeStep);
    if (input->GetKeyDown('D'))
        editorRoot_->cameraNode_->Translate(Vector3::RIGHT * MOVE_SPEED * timeStep);
}

void EditorApplication::SubscribeToEvents()
{
	//显示日志
	SubscribeToEvent(E_LOGMESSAGE,HANDLER(EditorApplication,HandleLogMessage));

	//处理一些快捷键
	SubscribeToEvent(E_KEYDOWN, HANDLER(EditorApplication, HandleKeyDown));

    //每一帧更新事件
    SubscribeToEvent(E_UPDATE, HANDLER(EditorApplication, HandleUpdate));
}

void EditorApplication::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
    using namespace Update;

    // Take the frame time step, which is stored as a float
    float timeStep = eventData[P_TIMESTEP].GetFloat();

    // Move the camera, scale movement with time step
    MoveCamera(timeStep);
}
