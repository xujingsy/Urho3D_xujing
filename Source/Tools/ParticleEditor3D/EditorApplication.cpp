#include "stdafx.h"
#include "EditorApplication.h"

#include "DebugRenderer.h"
#include "DebugHud.h"
#include "Console.h"
#include "ParticleEffect.h"
#include "ParticleEmitter.h"
#include "Text3D.h"

#include <QTimer>
#include <QFile>

EditorApplication::EditorApplication(int argc, char** argv, Context* context) :
	QApplication(argc, argv),
	Object(context),
	particleNode_(NULL)
{
	engine_ = new Engine(context);
	scene_ = new Scene(context);

	SetStyleSheet(":/qss_icons/style.qss");
	mainWindow_ = new MainWindow(context);

	axisLen = 30;

	SubscribeToEvent(E_UPDATE, HANDLER(EditorApplication, HandleUpdate));
	SubscribeToEvent(E_KEYDOWN, HANDLER(EditorApplication, HandleKeyDown));
	SubscribeToEvent(E_MOUSEWHEEL, HANDLER(EditorApplication, HandleMouseWheel));
	SubscribeToEvent(E_RENDERUPDATE, HANDLER(EditorApplication, HandleRenderUpdate));
}

EditorApplication::~EditorApplication()
{

}

void EditorApplication::SetStyleSheet(const char* qssPath)
{
	QFile file(qssPath);
	file.open(QFile::ReadOnly);
	QString styleSheet = QLatin1String(file.readAll());
	qApp->setStyleSheet(styleSheet);
	file.close();
}

//init engine --> run
int EditorApplication::Run()
{
	VariantMap engineParams;
	engineParams["Headless"] = false;
	engineParams["FrameLimiter"] = false;
	engineParams["WindowWidth"] = 1024;
	engineParams["WindowHeight"] = 768;
	engineParams["LogName"] = "ParticleEditor3D.log";
	engineParams["ExternalWindow"] = (void*)(mainWindow_->centralWidget()->winId());
	
	if( !engine_->Initialize(engineParams))
		return -1;

	mainWindow_->CreateWidgets();

	CreateScene();
	CreateConsole();
	CreateDebugHud();

	QTimer timer;
	connect(&timer, SIGNAL(timeout()), this, SLOT(OnTimer()));
	timer.start(16);

	return QApplication::exec();
}

void EditorApplication::New()
{
	Open("Particle//Smoke.xml");
	fileName_.Clear();	//new file
}

void EditorApplication::Open(const String& fileName)
{
	if(particleNode_)
	{
		particleNode_->Remove();
		particleNode_ = 0;
	}

	ResourceCache* cache = GetSubsystem<ResourceCache>();
	effect = cache->GetResource<ParticleEffect>(fileName);
	if(effect == NULL)
		return;

	particleNode_ = scene_->CreateChild("ParticleNode");

	ParticleEmitter* emitter = particleNode_->CreateComponent<ParticleEmitter>();
	emitter->SetEffect(effect);

	mainWindow_->UpdateWidgets();
}

void EditorApplication::Save(const String& fileName)
{
	ParticleEffect* effect = GetEffect();
	if(!effect)
		return;

	File file(context_);
	if(!file.Open(fileName, FILE_WRITE))
	{
		LOGERROR("Open file failed " + fileName);
		return;
	}

	effect->Save(file);

	fileName_ = fileName;
}

ParticleEffect* EditorApplication::GetEffect() const
{
	ParticleEmitter* emitter = GetEmitter();
	if (!emitter)
		return 0;

	return emitter->GetEffect();
}

ParticleEmitter* EditorApplication::GetEmitter() const
{
	return particleNode_->GetComponent<ParticleEmitter>();
}

void EditorApplication::OnTimer()
{
	if (engine_ && !engine_->IsExiting())
		engine_->RunFrame();
}

EditorApplication* EditorApplication::Get()
{
	return qobject_cast<EditorApplication*>(qApp);
}

Camera* EditorApplication::GetCamera() const
{
	return cameraNode_->GetComponent<Camera>();
}

void EditorApplication::CreateScene()
{
	scene_ = new Scene(context_);
	scene_->CreateComponent<Octree>();
	scene_->CreateComponent<DebugRenderer>();

	cameraNode_ = scene_->CreateChild("Camera");
	cameraNode_->SetWorldPosition(Vector3(30,30,-80));
	Camera* camera = cameraNode_->CreateComponent<Camera>();
	cameraNode_->SetWorldDirection(Vector3(0,0,0) - cameraNode_->GetWorldPosition());
	camera->SetOrthographic(false);

	Graphics* graphics = GetSubsystem<Graphics>();

	Viewport* viewport = new Viewport(context_, scene_, camera);
	Renderer* renderer = GetSubsystem<Renderer>();
	renderer->SetViewport(0, viewport);

	Node* lightNode = scene_->CreateChild("DirectionalLight");
	lightNode->SetDirection(Vector3(0.6, -1.0, 0.8)); // The direction vector does not need to be normalized
	Light* light = lightNode->CreateComponent<Light>();
	light->SetLightType(LIGHT_DIRECTIONAL);
	light->SetBrightness(2.0f);

	//AxisName Show
	Node* xNode = scene_->CreateChild("X");
	xNode->SetPosition(Vector3(axisLen * 0.9, 0, 0));
	Text3D* flag3D = xNode->CreateComponent<Text3D>();
	flag3D->SetText("X");
	flag3D->SetColor(Color::YELLOW);
	ResourceCache* cache = GetSubsystem<ResourceCache>();
	flag3D->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 105);

	Node* yNode = scene_->CreateChild("Y");
	yNode->SetPosition(Vector3(0, axisLen * 0.9, 0));
	flag3D = yNode->CreateComponent<Text3D>();
	flag3D->SetText("Y");
	flag3D->SetColor(Color::YELLOW);
	flag3D->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 105);

	Node* zNode = scene_->CreateChild("Z");
	zNode->SetPosition(Vector3(0, 0, axisLen * 0.9));
	flag3D = zNode->CreateComponent<Text3D>();
	flag3D->SetText("Z");
	flag3D->SetColor(Color::YELLOW);
	flag3D->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 105);

	New();
}

void EditorApplication::CreateConsole()
{
	// Get default style
	ResourceCache* cache = GetSubsystem<ResourceCache>();
	XMLFile* xmlFile = cache->GetResource<XMLFile>("UI/DefaultStyle.xml");

	// Create console
	Console* console = engine_->CreateConsole();
	console->SetDefaultStyle(xmlFile);
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

void EditorApplication::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
	using namespace Update;

	float timeStep = eventData[P_TIMESTEP].GetFloat();

	MoveCamera(timeStep);
}

void EditorApplication::HandleKeyDown(StringHash eventType, VariantMap& eventData)
{
	using namespace KeyDown;
	int key = eventData[P_KEY].GetInt();
	if (key == KEY_F1)
		GetSubsystem<Console>()->Toggle();
	else if (key == KEY_F2)
		GetSubsystem<DebugHud>()->ToggleAll();
}

void EditorApplication::HandleMouseWheel(StringHash eventType, VariantMap& eventData)
{
	using namespace MouseWheel;

	int wheel = eventData[P_WHEEL].GetInt();

	Camera* camera = cameraNode_->GetComponent<Camera>();

	float fZoom = camera->GetZoom();
	if (wheel > 0)
	{
		fZoom *= 1.25f;
		if(fZoom > 10.f)
			fZoom = 10.f;
		camera->SetZoom(fZoom);
	}
	else
	{
		fZoom *= 0.8f;
		if(fZoom < 0.1f)
			fZoom = 0.1f;
		camera->SetZoom(fZoom);
	}
}

//Axis Line
void EditorApplication::HandleRenderUpdate(StringHash eventType, VariantMap& eventData)
{
	DebugRenderer* debugRenderer = scene_->GetComponent<DebugRenderer>();

	const Color color(0.0f, 0.5f, 0.0f, 0.5f);

	//x
	debugRenderer->AddLine(Vector3(- axisLen, 0.0f, 0.0f), Vector3(axisLen, 0.0f, 0.0f), color);
	//y
	debugRenderer->AddLine(Vector3(0.0f,- axisLen,  0.0f), Vector3(0.0f, axisLen, 0.0f), color);
	//z
	debugRenderer->AddLine(Vector3(0.0f, 0.0f, - axisLen), Vector3(0.0f, 0.0f, axisLen), color);

	debugRenderer->Render();
}

void EditorApplication::MoveCamera(float timeStep)
{
	// Do not move if the UI has a focused element (the console)
	if (GetSubsystem<UI>()->GetFocusElement())
		return;

	Input* input = GetSubsystem<Input>();
	// Movement speed as world units per second
	const float MOVE_SPEED = 18.0f;

	//Camera move
	if (input->GetKeyDown('W'))
		cameraNode_->Translate(Vector3::FORWARD * MOVE_SPEED * timeStep);
	if (input->GetKeyDown('S'))
		cameraNode_->Translate(Vector3::BACK * MOVE_SPEED * timeStep);
	if (input->GetKeyDown('A'))
		cameraNode_->Translate(Vector3::LEFT * MOVE_SPEED * timeStep);
	if (input->GetKeyDown('D'))
		cameraNode_->Translate(Vector3::RIGHT * MOVE_SPEED * timeStep);
}