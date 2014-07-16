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
#include "CardApplication.h"
#include "AnimatedModel.h"
#include "DebugNew.h"

DEFINE_APPLICATION_MAIN(CardApplication)

CardApplication::CardApplication(Context* context) :
    Sample(context),
    yaw_(0.0f),
    pitch_(0.0f),
    drawDebug_(false)
{
}

void CardApplication::Start()
{
	Input* input = GetSubsystem<Input>();
	input->SetMouseVisible(true);

    // Execute base class startup
    Sample::Start();

    // Create the scene content
    CreateScene();
    
    // Create the UI content
    CreateUI();
    
    // Setup the viewport for displaying the scene
    SetupViewport();

    // Hook up to the frame update and render post-update events
    SubscribeToEvents();
}

void CardApplication::CreateScene()
{
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    
    scene_ = new Scene(context_);
    
    // Create octree, use default volume (-1000, -1000, -1000) to (1000, 1000, 1000)
    // Also create a DebugRenderer component so that we can draw debug geometry
    scene_->CreateComponent<Octree>();
    scene_->CreateComponent<DebugRenderer>();
    
    // Create scene node & StaticModel component for showing a static plane
    Node* planeNode = scene_->CreateChild("Plane");
    planeNode->SetScale(Vector3(100.0f, 1.0f, 100.0f));
    StaticModel* planeObject = planeNode->CreateComponent<StaticModel>();
    planeObject->SetModel(cache->GetResource<Model>("Models/Plane.mdl"));
    planeObject->SetMaterial(cache->GetResource<Material>("Materials/StoneTiled.xml"));
	
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
    lightNode->SetDirection(Vector3(0.6f, -0.5f, 0.8f));
    Light* light = lightNode->CreateComponent<Light>();
    light->SetLightType(LIGHT_DIRECTIONAL);
	light->SetSpecularIntensity(3);
	light->SetBrightness(3);
    light->SetCastShadows(true);
    light->SetShadowBias(BiasParameters(0.0001f, 0.5f));
    // Set cascade splits at 10, 50 and 200 world units, fade shadows out at 80% of maximum shadow distance
    light->SetShadowCascade(CascadeParameters(10.0f, 50.0f, 200.0f, 0.0f, 0.8f));

	Node* pThroneNode = add_object(scene_,"throne",enObjectType_StaticModel,0, 0, 3,"Models/throne.mdl","Materials/throne.xml");
	pThroneNode->SetScale(0.5);

	add_object(scene_,"DiZuo",enObjectType_StaticModel,0, 0, -5.0f, "Models/dizuo.mdl","Materials/dizuo.xml");
	add_object(scene_,"Card",enObjectType_StaticModel,0, 0, 0,"Models/card.mdl","Materials/_01.xml");
	Node* pCheNode = add_object(scene_,"Che",enObjectType_AnimationModel,0,0,0,"Models/che1.mdl","Materials/che.xml");
	pCheNode->SetScale(0.1);
    
    // Create the camera. Limit far clip distance to match the fog
    cameraNode_ = scene_->CreateChild("Camera");
    Camera* camera = cameraNode_->CreateComponent<Camera>();
	//float fZoom = camera->GetFov();
	//camera->SetFov(fZoom);

    camera->SetFarClip(300.0f);
    
    // Set an initial position for the camera scene node above the plane
	cameraNode_->SetPosition(Vector3(0.225,11.2,-8));
	yaw_ = - 1.29;
	pitch_ = 66.7;

 //   cameraNode_->SetPosition(Vector3(5.0f, 5.0f, -15.0f));
	//pitch_ = 19;
	cameraNode_->SetRotation(Quaternion(pitch_, yaw_, 0.0f));
}

void CardApplication::CreateUI()
{
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
    Text* instructionText = ui->GetRoot()->CreateChild<Text>();
    //instructionText->SetText(
    //    "Use WASD keys to move\n"
    //    "LMB to paint decals, RMB to rotate view\n"
    //    "Space to toggle debug geometry\n"
    //    "7 to toggle occlusion culling"
    //);
    instructionText->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 15);
    // The text has multiple rows. Center them in relation to each other
    instructionText->SetTextAlignment(HA_CENTER);
    
    // Position the text relative to the screen center
    instructionText->SetHorizontalAlignment(HA_CENTER);
    instructionText->SetVerticalAlignment(VA_CENTER);
    instructionText->SetPosition(0, ui->GetRoot()->GetHeight() / 4);
}

void CardApplication::SetupViewport()
{
    Renderer* renderer = GetSubsystem<Renderer>();
    
    // Set up a viewport to the Renderer subsystem so that the 3D scene can be seen
    SharedPtr<Viewport> viewport(new Viewport(context_, scene_, cameraNode_->GetComponent<Camera>()));
    renderer->SetViewport(0, viewport);
}

void CardApplication::SubscribeToEvents()
{
    // Subscribe HandleUpdate() function for processing update events
    SubscribeToEvent(E_UPDATE, HANDLER(CardApplication, HandleUpdate));
    
    // Subscribe HandlePostRenderUpdate() function for processing the post-render update event, during which we request
    // debug geometry
    SubscribeToEvent(E_POSTRENDERUPDATE, HANDLER(CardApplication, HandlePostRenderUpdate));

	SubscribeToEvent(E_MOUSEMOVE,HANDLER(CardApplication,HandleMouseMove));
}

void CardApplication::HandleMouseMove(StringHash eventType, VariantMap& eventData)
{
	PaintDecal();
}

void CardApplication::MoveCamera(float timeStep)
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
    if (input->GetKeyDown('W'))
        cameraNode_->Translate(Vector3::FORWARD * MOVE_SPEED * timeStep);
    if (input->GetKeyDown('S'))
        cameraNode_->Translate(Vector3::BACK * MOVE_SPEED * timeStep);
    if (input->GetKeyDown('A'))
        cameraNode_->Translate(Vector3::LEFT * MOVE_SPEED * timeStep);
    if (input->GetKeyDown('D'))
        cameraNode_->Translate(Vector3::RIGHT * MOVE_SPEED * timeStep);
    
    // Toggle debug geometry with space
    if (input->GetKeyPress(KEY_SPACE))
        drawDebug_ = !drawDebug_;
    
    // Paint decal with the left mousebutton; cursor must be visible
    if (ui->GetCursor()->IsVisible() && input->GetMouseButtonPress(MOUSEB_LEFT))
        PaintDecal();
}

void CardApplication::PaintDecal()
{
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

bool CardApplication::Raycast(float maxDistance, Vector3& hitPos, Drawable*& hitDrawable)
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

void CardApplication::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
    using namespace Update;

    // Take the frame time step, which is stored as a float
    float timeStep = eventData[P_TIMESTEP].GetFloat();
    
    // Move the camera, scale movement with time step
    MoveCamera(timeStep);
}

void CardApplication::HandlePostRenderUpdate(StringHash eventType, VariantMap& eventData)
{
    // If draw debug mode is enabled, draw viewport debug geometry. Disable depth test so that we can see the effect of occlusion
    if (drawDebug_)
        GetSubsystem<Renderer>()->DrawDebugGeometry(false);
}

Node* CardApplication::add_object(Node* pParentNode, const String& nodeName,enObjectType type,float x,float y,float z,const char* modelUrl,const char* material)
{
	ResourceCache* cache = GetSubsystem<ResourceCache>();

	Node* pNode = pParentNode->CreateChild(nodeName);
	pNode->SetPosition(Vector3(x, y, z));

	if(type == enObjectType_StaticModel)
	{
		StaticModel* pModel = pNode->CreateComponent<StaticModel>();
		pModel->SetModel(cache->GetResource<Model>(modelUrl));
		pModel->SetMaterial(0,cache->GetResource<Material>(material));

		pModel->SetCastShadows(true);
	}
	else
	{
		AnimatedModel* pAniModel = pNode->CreateComponent<AnimatedModel>();
		pAniModel->SetModel(cache->GetResource<Model>(modelUrl));
		pAniModel->SetMaterial(0,cache->GetResource<Material>(material));

		pAniModel->SetCastShadows(true);
	}

	return pNode;
}
