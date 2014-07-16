#include "Camera.h"
#include "CoreEvents.h"
#include "Cursor.h"
#include "DebugRenderer.h"
#include "DecalSet.h"
#include "Engine.h"
#include "Font.h"
#include "Graphics.h"
#include "Input.h"
#include "Light.h"
#include "Material.h"
#include "Model.h"
#include "Octree.h"
#include "Renderer.h"
#include "Terrain.h"
#include "ResourceCache.h"
#include "Scene.h"
#include "StaticModel.h"
#include "Text.h"
#include "UI.h"
#include "XMLFile.h"
#include "Zone.h"
#include "PhysicsWorld.h"
#include "RigidBody.h"
#include "CollisionShape.h"
#include "GameApplication.h"
#include "AnimatedModel.h"
#include "DebugNew.h"
#include <glew.h>
#include "Console.h"
#include "DebugHud.h"
#include "Texture2D.h"
#include "Button.h"
#include "UIEvents.h"
#include "UI/UIButton.h"

DEFINE_APPLICATION_MAIN(GameApplication)

GameApplication::GameApplication(Context* context) :
    Application(context),
    yaw_(0.0f),
    pitch_(0.0f),
    drawDebug_(false)
{
	scoreText = NULL;
}

void GameApplication::Setup()
{
	// Modify engine startup parameters
	engineParameters_["WindowTitle"] = "3d 2048";
	engineParameters_["LogName"]     = "game.log";
	engineParameters_["FullScreen"]  = false;
	engineParameters_["Headless"]    = false;
}

void GameApplication::Start()
{
    // Execute base class startup
    CreateConsoleAndDebugHud();

	glLineWidth(1);

    // Create the scene content
    CreateScene();
    
    // Create the UI content
    CreateUI();
    
    // Setup the viewport for displaying the scene
    SetupViewport();

    // Hook up to the frame update and render post-update events
    SubscribeToEvents();
}

void GameApplication::CreateConsoleAndDebugHud()
{
	// Get default style
	ResourceCache* cache = GetSubsystem<ResourceCache>();
	XMLFile* xmlFile = cache->GetResource<XMLFile>("UI/DefaultStyle.xml");
	if (!xmlFile)
		return;

	// Create console
	Console* console = engine_->CreateConsole();
	console->SetDefaultStyle(xmlFile);

	// Create debug HUD.
	DebugHud* debugHud = engine_->CreateDebugHud();
	debugHud->SetDefaultStyle(xmlFile);
}

void GameApplication::CreateScene()
{
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    
    scene_ = new Scene(context_);
    
    // Create octree, use default volume (-1000, -1000, -1000) to (1000, 1000, 1000)
    // Also create a DebugRenderer component so that we can draw debug geometry
    scene_->CreateComponent<Octree>();
    scene_->CreateComponent<DebugRenderer>();
    
    // Create scene node & StaticModel component for showing a static plane
    Node* planeNode = scene_->CreateChild("Plane");
    //planeNode->SetScale(Vector3(100.0f, 1.0f, 100.0f));
    StaticModel* planeObject = planeNode->CreateComponent<StaticModel>();
    planeObject->SetModel(cache->GetResource<Model>("Models/dikuang.mdl"));

	Material* material = cache->GetResource<Material>("Materials/Water.xml");
	//SharedPtr<ValueAnimation> specColorAnimation(new ValueAnimation(context_));
	//specColorAnimation->SetKeyFrame(0.0f, Color(0.1f, 0.1f, 0.1f, 16.0f));
	//specColorAnimation->SetKeyFrame(1.0f, Color(1.0f, 0.0f, 0.0f, 2.0f));
	//specColorAnimation->SetKeyFrame(2.0f, Color(1.0f, 1.0f, 0.0f, 2.0f));
	//specColorAnimation->SetKeyFrame(3.0f, Color(0.1f, 0.1f, 0.1f, 16.0f));
	//material->SetShaderParameterAnimation("MatSpecColor", specColorAnimation);

    planeObject->SetMaterial(material);	//dikuang
	
    // Create a Zone component for ambient lighting & fog control
    Node* zoneNode = scene_->CreateChild("Zone");
    Zone* zone = zoneNode->CreateComponent<Zone>();
    zone->SetBoundingBox(BoundingBox(-1000.0f, 1000.0f));
    zone->SetAmbientColor(Color(0.3f, 0.3f, 0.3f));
    zone->SetFogColor(Color(0.5f, 0.5f, 0.7f));
    zone->SetFogStart(100.0f);
    zone->SetFogEnd(300.0f);
    
    // Create a directional light to the world. Enable cascaded shadows on it
    Node* lightNode = scene_->CreateChild("DirectionalLight");
    lightNode->SetDirection(Vector3(0.2f, -0.5f, -0.1f));
    Light* light = lightNode->CreateComponent<Light>();
    light->SetLightType(LIGHT_DIRECTIONAL);
//	light->SetSpecularIntensity(3);
	light->SetBrightness(1);
    light->SetCastShadows(true);
    light->SetShadowBias(BiasParameters(0.0001f, 0.5f));
    // Set cascade splits at 10, 50 and 200 world units, fade shadows out at 80% of maximum shadow distance
    light->SetShadowCascade(CascadeParameters(10.0f, 50.0f, 200.0f, 0.0f, 0.8f));

	InitGridModels();
    
    // Create the camera. Limit far clip distance to match the fog
    cameraNode_ = scene_->CreateChild("Camera");
    Camera* camera = cameraNode_->CreateComponent<Camera>();
	//float fZoom = camera->GetFov();
	//camera->SetFov(fZoom);

    camera->SetFarClip(300.0f);
    
    // Set an initial position for the camera scene node above the plane
	cameraNode_->SetPosition(Vector3(0.2318,7.5248,-0.2721));
	yaw_ = 0.10003410;
	pitch_ = 90;

 //   cameraNode_->SetPosition(Vector3(5.0f, 5.0f, -15.0f));
	//pitch_ = 19;
	cameraNode_->SetRotation(Quaternion(pitch_, yaw_, 0.0f));
}

void GameApplication::CreateUI()
{
	Input* input = GetSubsystem<Input>();
	input->SetMouseVisible(true);

    ResourceCache* cache = GetSubsystem<ResourceCache>();
    UI* ui = GetSubsystem<UI>();
    
    // Create a Cursor UI element because we want to be able to hide and show it at will. When hidden, the mouse cursor will
    // control the camera, and when visible, it will point the raycast target
    XMLFile* style = cache->GetResource<XMLFile>("UI/DefaultStyle.xml");
    SharedPtr<Cursor> cursor(new Cursor(context_));
    cursor->SetStyleAuto(style);
    ui->SetCursor(cursor);
    // Set starting position of the cursor at the rendering window center
    Graphics* graphics = GetSubsystem<Graphics>();
    cursor->SetPosition(graphics->GetWidth() / 2, graphics->GetHeight() / 2);
    
    // Construct new Text object, set string to display and font to use
    scoreText = ui->GetRoot()->CreateChild<Text>();
    scoreText->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 15);
    // The text has multiple rows. Center them in relation to each other
    scoreText->SetTextAlignment(HA_CENTER);
	scoreText->SetColor(Color::BLUE);
	scoreText->SetTextEffect(TextEffect::TE_SHADOW);
    // Position the text relative to the screen center
    scoreText->SetHorizontalAlignment(HA_CENTER);
    scoreText->SetVerticalAlignment(VA_CENTER);
    scoreText->SetPosition(0, ui->GetRoot()->GetHeight() / 4 + 80);

	//开始按钮
	//UIButton* btnStart = new UIButton(context_);
	//btnStart->SetPosition(100,100);
	//btnStart->SetSize(120,50);

	//ui->GetRoot()->AddChild(btnStart);

	Button* btnStart = CreateButton(80,100,120,50,"Start");// ui->GetRoot()->CreateChild<Button>();
	btnStart->SetName("btnStart");
	btnStart->SetMinHeight(24);
	btnStart->SetStyleAuto();
	btnStart->SetPosition(100,100);
	btnStart->SetSize(120,50);

	Text* buttonText = btnStart->CreateChild<Text>();
	buttonText->SetAlignment(HA_CENTER, VA_CENTER);
	buttonText->SetColor(Color::BLUE);
	Font* font = cache->GetResource<Font>("Fonts/Anonymous Pro.ttf");
	buttonText->SetFont(font, 12);
	buttonText->SetText("Start");

	SubscribeToEvent(btnStart,E_PRESSED,HANDLER(GameApplication,OnButtonStartClick));

	UpdateScore();
}

Button* GameApplication::CreateButton(int x,int y,int xSize,int ySize,const String& text)
{
	UIElement* root = GetSubsystem<UI>()->GetRoot();
	ResourceCache* cache = GetSubsystem<ResourceCache>();
	Font* font = cache->GetResource<Font>("Fonts/Anonymous Pro.ttf");

	// Create the button and center the text onto it
	Button* button = root->CreateChild<Button>();
	button->SetTexture(cache->GetResource<Texture2D>("Urho2D/Box.png"));
	button->SetStyleAuto();
	button->SetPosition(x, y);
	button->SetSize(xSize, ySize);

	Text* buttonText = button->CreateChild<Text>();
	buttonText->SetAlignment(HA_CENTER, VA_CENTER);
	buttonText->SetFont(font, 12);
	buttonText->SetColor(Color::BLUE);
	buttonText->SetText(text);

	return button;
}

void GameApplication::OnButtonStartClick(StringHash eventType, VariantMap& eventData)
{
	list<CHANGE_ORDER> changes;
	gameData_.OnNewGame(changes);

	UpdateShow();
}

void GameApplication::UpdateScore()
{
	char szScore[128];
	sprintf(szScore,"Score:%d",gameData_.Score);

	if(scoreText != NULL)
		scoreText->SetText(szScore);
}

void GameApplication::InitGridModels()
{
	for(int x = 0;x < 4;x ++)
	{
		for(int y = 0;y < 4;y ++)
		{
			char szName[128];
			sprintf(szName,"Node_%d_%d",x,y);

			Node* pNode = add_object(scene_,szName, enObjectType_StaticModel,0,0,0,"Models/Box.mdl",NULL);
			pNode->SetPosition(Vector3(x - 2 + 0.5,0,y - 2 + 0.5));
			pNode->SetRotation(Quaternion(0.2,0,0));

			gridNodes[y][x] = pNode;
		}
	}

	UpdateShow();
}

void GameApplication::SetupViewport()
{
    Renderer* renderer = GetSubsystem<Renderer>();
    
    // Set up a viewport to the Renderer subsystem so that the 3D scene can be seen
    SharedPtr<Viewport> viewport(new Viewport(context_, scene_, cameraNode_->GetComponent<Camera>()));
    renderer->SetViewport(0, viewport);
}

void GameApplication::SubscribeToEvents()
{
    // Subscribe HandleUpdate() function for processing update events
    SubscribeToEvent(E_UPDATE, HANDLER(GameApplication, HandleUpdate));
    
    // Subscribe HandlePostRenderUpdate() function for processing the post-render update event, during which we request
    // debug geometry
    SubscribeToEvent(E_POSTRENDERUPDATE, HANDLER(GameApplication, HandlePostRenderUpdate));

	SubscribeToEvent(E_MOUSEMOVE,HANDLER(GameApplication,HandleMouseMove));

	// Subscribe key down event
	SubscribeToEvent(E_KEYDOWN, HANDLER(GameApplication, HandleKeyDown));
	SubscribeToEvent(E_KEYUP,HANDLER(GameApplication,HandleKeyUp));
}

void GameApplication::HandleKeyUp(StringHash eventType, VariantMap& eventData)
{
	using namespace KeyUp;
	int key = eventData[P_KEY].GetInt();
}

void GameApplication::HandleKeyDown(StringHash eventType, VariantMap& eventData)
{
	using namespace KeyDown;

	int key = eventData[P_KEY].GetInt();
	list<CHANGE_ORDER> changes;
	//显示与内存中上下相反
	if (key == 'W')
	{
		gameData_.OnDown(changes);
		UpdateShow();
	}
	if (key == 'S')
	{
		gameData_.OnUp(changes);
		UpdateShow();
	}
	if (key == 'A')
	{
		gameData_.OnLeft(changes);
		UpdateShow();
	}
	if (key == 'D')
	{
		gameData_.OnRight(changes);
		UpdateShow();
	}

	// Close console (if open) or exit when ESC is pressed
	if (key == KEY_ESC)
	{
		Console* console = GetSubsystem<Console>();
		if (!console->IsVisible())
			engine_->Exit();
		else
			console->SetVisible(false);
	}

	// Toggle console with F1
	else if (key == KEY_F1)
		GetSubsystem<Console>()->Toggle();

	// Toggle debug HUD with F2
	else if (key == KEY_F2)
		GetSubsystem<DebugHud>()->ToggleAll();

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
			renderer->SetDynamicInstancing(!renderer->GetDynamicInstancing());
	}
}

void GameApplication::HandleMouseMove(StringHash eventType, VariantMap& eventData)
{
	PaintDecal();
}

void GameApplication::MoveCamera(float timeStep)
{
    // Right mouse button controls mouse cursor visibility: hide when pressed
    UI* ui = GetSubsystem<UI>();
    Input* input = GetSubsystem<Input>();
    ui->GetCursor()->SetVisible(!input->GetMouseButtonDown(MOUSEB_RIGHT));
    
    // Do not move if the UI has a focused element (the console)
    if (ui->GetFocusElement())
        return;
    
    // Movement speed as world units per second
    const float MOVE_SPEED = 20.0f;
    // Mouse sensitivity as degrees per pixel
    const float MOUSE_SENSITIVITY = 0.1f;
    
    // Use this frame's mouse motion to adjust camera node yaw and pitch. Clamp the pitch between -90 and 90 degrees
    // Only move the camera when the cursor is hidden
    if (!ui->GetCursor()->IsVisible())
    {
        IntVector2 mouseMove = input->GetMouseMove();
        yaw_ += MOUSE_SENSITIVITY * mouseMove.x_;
        pitch_ += MOUSE_SENSITIVITY * mouseMove.y_;
        pitch_ = Clamp(pitch_, -90.0f, 90.0f);
        
        // Construct new orientation for the camera scene node from yaw and pitch. Roll is fixed to zero
        cameraNode_->SetRotation(Quaternion(pitch_, yaw_, 0.0f));
    }
    
    // Read WASD keys and move the camera scene node to the corresponding direction if they are pressed
    //if (input->GetKeyDown('W'))
    //    cameraNode_->Translate(Vector3::FORWARD * MOVE_SPEED * timeStep);
    //if (input->GetKeyDown('S'))
    //    cameraNode_->Translate(Vector3::BACK * MOVE_SPEED * timeStep);
    //if (input->GetKeyDown('A'))
    //    cameraNode_->Translate(Vector3::LEFT * MOVE_SPEED * timeStep);
    //if (input->GetKeyDown('D'))
    //    cameraNode_->Translate(Vector3::RIGHT * MOVE_SPEED * timeStep);
    
    // Toggle debug geometry with space
    if (input->GetKeyPress(KEY_SPACE))
        drawDebug_ = !drawDebug_;
    
    // Paint decal with the left mousebutton; cursor must be visible
    if (ui->GetCursor()->IsVisible() && input->GetMouseButtonPress(MOUSEB_LEFT))
        PaintDecal();
}

void GameApplication::PaintDecal()
{
	return;
    Vector3 hitPos;
    Drawable* hitDrawable;
    
    if (Raycast(250.0f, hitPos, hitDrawable))
    {
        // Check if target scene node already has a DecalSet component. If not, create now
        Node* targetNode = hitDrawable->GetNode();
        DecalSet* decal = targetNode->GetComponent<DecalSet>();
        if (!decal)
        {
            ResourceCache* cache = GetSubsystem<ResourceCache>();
            
            decal = targetNode->CreateComponent<DecalSet>();
            decal->SetMaterial(cache->GetResource<Material>("Materials/UrhoDecal.xml"));
        }
		decal->RemoveAllDecals();
        // Add a square decal to the decal set using the geometry of the drawable that was hit, orient it to face the camera,
        // use full texture UV's (0,0) to (1,1). Note that if we create several decals to a large object (such as the ground
        // plane) over a large area using just one DecalSet component, the decals will all be culled as one unit. If that is
        // undesirable, it may be necessary to create more than one DecalSet based on the distance
        decal->AddDecal(hitDrawable, hitPos, cameraNode_->GetRotation(), 1.5f, 1.0f, 1.0f, Vector2::ZERO,
            Vector2::ONE);
    }
}

void GameApplication::SetElementNumber(int x,int y,int number)
{
	char szNodeName[128];
	Node* pNode = scene_->GetChild(szNodeName);
	Texture2D* pTexture;
}

bool GameApplication::Raycast(float maxDistance, Vector3& hitPos, Drawable*& hitDrawable)
{
    hitDrawable = 0;
    
    UI* ui = GetSubsystem<UI>();
    IntVector2 pos = ui->GetCursorPosition();
    // Check the cursor is visible and there is no UI element in front of the cursor
    if (!ui->GetCursor()->IsVisible() || ui->GetElementAt(pos, true))
        return false;
    
    Graphics* graphics = GetSubsystem<Graphics>();
    Camera* camera = cameraNode_->GetComponent<Camera>();
    Ray cameraRay = camera->GetScreenRay((float)pos.x_ / graphics->GetWidth(), (float)pos.y_ / graphics->GetHeight());
    // Pick only geometry objects, not eg. zones or lights, only get the first (closest) hit
    PODVector<RayQueryResult> results;
    RayOctreeQuery query(results, cameraRay, RAY_TRIANGLE, maxDistance, DRAWABLE_GEOMETRY);
    scene_->GetComponent<Octree>()->RaycastSingle(query);
    if (results.Size())
    {
        RayQueryResult& result = results[0];
        
        // Calculate hit position in world space
        hitPos = cameraRay.origin_ + cameraRay.direction_ * result.distance_;
        hitDrawable = result.drawable_;
        return true;
    }
    
    return false;
}

void GameApplication::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
    using namespace Update;

    // Take the frame time step, which is stored as a float
    float timeStep = eventData[P_TIMESTEP].GetFloat();
    
    // Move the camera, scale movement with time step
    MoveCamera(timeStep);
}

void GameApplication::HandlePostRenderUpdate(StringHash eventType, VariantMap& eventData)
{
    // If draw debug mode is enabled, draw viewport debug geometry. Disable depth test so that we can see the effect of occlusion
    if (drawDebug_)
	{
        GetSubsystem<Renderer>()->DrawDebugGeometry(false);
	}

	DebugRenderer* debug = scene_->GetComponent<DebugRenderer>();

	//4
	for(int i = 0;i < 5;i ++)
	{
		//横线
		debug->AddLine(Vector3(- 2,0,i - 2),Vector3(2,0,i - 2),Color::YELLOW);
		//竖线
		debug->AddLine(Vector3(i - 2,0,- 2),Vector3(i - 2,0,2),Color::GREEN);
	}
}

Node* GameApplication::add_object(Node* pParentNode, const String& nodeName,enObjectType type,float x,float y,float z,const char* modelUrl,const char* material)
{
	ResourceCache* cache = GetSubsystem<ResourceCache>();

	Node* pNode = pParentNode->CreateChild(nodeName);
	pNode->SetPosition(Vector3(x, y, z));

	if(type == enObjectType_StaticModel)
	{
		StaticModel* pModel = pNode->CreateComponent<StaticModel>();
		pModel->SetModel(cache->GetResource<Model>(modelUrl));
		if(material != NULL)
			pModel->SetMaterial(0,cache->GetResource<Material>(material));

		pModel->SetCastShadows(true);
	}
	else
	{
		AnimatedModel* pAniModel = pNode->CreateComponent<AnimatedModel>();
		pAniModel->SetModel(cache->GetResource<Model>(modelUrl));
		if(material != NULL)
			pAniModel->SetMaterial(0,cache->GetResource<Material>(material));

		pAniModel->SetCastShadows(true);
	}

	return pNode;
}

void GameApplication::UpdateShow()
{
	UpdateScore();

	ResourceCache* cache = GetSubsystem<ResourceCache>();

	for(int x = 0;x < 4;x ++)
	{
		for(int y = 0;y < 4;y ++)
		{
			Node* pNode = gridNodes[y][x];

			if(gameData_.data[y][x] != 0)
			{
				pNode->SetEnabled(true);

				//设置贴图
				StaticModel* pModel = pNode->GetComponent<StaticModel>();

				char szPath[256];
				sprintf(szPath,"Materials/number/%d.xml",gameData_.data[y][x]);
				pModel->SetMaterial(0, cache->GetResource<Material>(szPath));
			}
			else
			{
				pNode->SetEnabled(false);
			}
		}
	}
}
