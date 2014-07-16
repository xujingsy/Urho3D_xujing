#pragma once
#include "Game/DataGrid.h"
#include "Application.h"
#include <map>
using namespace std;

namespace Urho3D
{
class Drawable;
class Node;
class Scene;
class Terrain;
class Button;
class Text;
}

enum enObjectType
{
	enObjectType_StaticModel,
	enObjectType_AnimationModel,
};

class GameApplication : public Application
{
    OBJECT(GameApplication);

public:
    /// Construct.
    GameApplication(Context* context);

	void Setup();

    /// Setup after engine initialization and before running the main loop.
    virtual void Start();

private:
    /// Construct the scene content.
    void CreateScene();
    /// Construct user interface elements.
    void CreateUI();
    /// Set up a viewport for displaying the scene.
    void SetupViewport();
    /// Subscribe to application-wide logic update and post-render update events.
    void SubscribeToEvents();
	void HandleMouseMove(StringHash eventType, VariantMap& eventData);
    /// Reads input and moves the camera.
    void MoveCamera(float timeStep);
    /// Paint a decal using a ray cast from the mouse cursor.
    void PaintDecal();
    /// Utility function to raycast to the cursor position. Return true if hit
    bool Raycast(float maxDistance, Vector3& hitPos, Drawable*& hitDrawable);
    /// Handle the logic update event.
    void HandleUpdate(StringHash eventType, VariantMap& eventData);
    /// Handle the post-render update event.
    void HandlePostRenderUpdate(StringHash eventType, VariantMap& eventData);
    
    /// Scene.
    SharedPtr<Scene> scene_;
    /// Camera scene node.
    SharedPtr<Node> cameraNode_;
    /// Camera yaw angle.
    float yaw_;
    /// Camera pitch angle.
    float pitch_;
    /// Flag for drawing debug geometry.
    bool drawDebug_;

	void InitGridModels();

	void SetElementNumber(int x,int y,int number);

	DataGrid gameData_;

	void UpdateShow();
	Node* gridNodes[4][4];
	map<int,Material*> materials;

	Material* GetMaterial(int number);

	void UpdateScore();
private:
	Node* add_object(Node* pParentNode, const String& nodeName,enObjectType type,float x,float y,float z,const char* modelUrl,const char* material);

	void HandleKeyDown(StringHash eventType, VariantMap& eventData);
	void HandleKeyUp(StringHash eventType, VariantMap& eventData);

	void OnButtonStartClick(StringHash eventType, VariantMap& eventData);

	void CreateConsoleAndDebugHud();

	Button* CreateButton(int x,int y,int xSize,int ySize,const String& text);

	Text* scoreText;	//·ÖÊýÅÆ
};
