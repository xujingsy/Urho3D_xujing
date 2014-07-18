#include "stdafx.h"
#include "TerrainEditor.h"
#include "Skybox.h"
#include "DebugRenderer.h"
#include "Sprite2D.h"
#include "Drawable2D.h"
#include "StaticSprite2D.h"
#include "Animation.h"
#include "AnimatedModel.h"
#include "AnimationState.h"
#include "Mover.h"
#include "../Manager/SceneHelper.h"
#include "ValueAnimation.h"

TerrainEditor::TerrainEditor(Context* context) : Object(context)
{
	brush_ = NULL;
	brushNode_ = NULL;

	modIndex = 0;	//用来管理物件的命名

	context->RegisterFactory<Mover>();
}

void TerrainEditor::OnMouseMove(float x,float y,unsigned int buttons)
{
	//if(EditorsRoot::Instance()->ActiveTool != TOOL_DEFORM)
	//	return;

	EditorsRoot* editorRoot_ = EditorsRoot::Instance();

	IntVector2 pos(x,y);
	vector<SceneHitResult> results = SceneHelper::Instance()->QueryCurrentMousePosObjects(250.f,&pos);
	Vector3 hitPos;
	Drawable* hitDrawable = NULL;

	for(int i = 0;i < results.size();i ++)
	{
		if(results[i].object->GetTypeName() == "TerrainPatch")
		{
			hitDrawable = results[i].object;
			hitPos = results[i].hitPos;

			//测试
			Node* lightNode = editorRoot_->scene_->GetChild("DirectionalLight");

			Vector3 lightPos = hitPos;
			lightPos.y_ = editorRoot_->terrain_->GetHeight(hitPos) + 1;

			lightNode->SetWorldPosition(lightPos);
			break;
		}
	}

	if(hitDrawable != NULL)
	{
		if(brushNode_ == NULL)
		{
			brushNode_ = editorRoot_->scene_->CreateChild("BrushNode");
			brush_ = brushNode_->CreateComponent<TerrainBrush>();

			ResourceCache* cache = GetSubsystem<ResourceCache>();
			brush_->SetMaterial(cache->GetResource<Material>("Materials/UrhoDecal.xml"));
			brush_->SetOwner(editorRoot_->terrain_);
			brush_->SetBrushTexture(cache->GetResource<Texture2D>("Images/brushes/noisy_circular2.png"));
		}

		brush_->UpdatePos(hitPos);
		brushNode_->SetPosition(hitPos);	//放在当前鼠标位置
	}
	else
	{

	}
}

void TerrainEditor::OnMouseLeftDown(float x,float y,unsigned int buttons)
{
	on_viewport_click(x,y);
}

void TerrainEditor::OnKeyDown(unsigned int key)
{
	if(key == 'P')
	{
		if(brush_)
		{
			brush_->SetBrushSize(brush_->GetBrushSize() + 1);
		}
	}
	else if(key == 'O')
	{
		if(brush_)
		{
			brush_->SetBrushSize(brush_->GetBrushSize() - 1);
		}
	}
}

void TerrainEditor::OnKeyUp(unsigned int key)
{

}

void TerrainEditor::on_viewport_click(float x,float y)
{
	vector<SceneHitResult> results = SceneHelper::Instance()->QueryCurrentMousePosObjects(250.f);
	for(int i = 0;i < results.size();i ++)
	{
		SceneHitResult& obj = results[i];
		if(obj.object->GetTypeName() == "TerrainPatch")
		{
			on_terrain_click(obj.hitPos);
		}
	}
}

void TerrainEditor::on_terrain_click(Vector3& worldPos)
{
	EditorsRoot* pEditorRoot = EditorsRoot::Instance();
	if(pEditorRoot->ActiveTool == TOOL_DEFORM)
	{
		//转换成相对整个Terrain起始坐标
		Vector3 startPos = pEditorRoot->terrain_->GetPatch(0)->GetNode()->GetPosition();
		worldPos -= startPos;

		//每个格子的大小
		Vector3 spacing = pEditorRoot->terrain_->GetSpacing();
		int tileX = worldPos.x_ / spacing.x_;
		int tileZ = worldPos.z_ / spacing.z_;

		Image* pHeightMap = pEditorRoot->terrain_->GetHeightMap();

		//图像是上下颠倒的
		tileZ = pHeightMap->GetHeight() - tileZ;

		int nRange = floor(brush_->GetBrushSize() / 2.);
		for(int xx = - nRange;xx < nRange;xx ++)
		{
			for(int yy = - nRange;yy < nRange;yy ++)
			{
				//点击的坐标与Heightmap坐标对应关系
				Color color = pHeightMap->GetPixel(tileX + xx,tileZ + yy);
				color.r_ += 0.02 * (abs(xx) + abs(yy)) / nRange;
				color.g_ += 0.02 ;
				pHeightMap->SetPixel(tileX + xx,tileZ + yy,color);
			}
		}

		//判断点击涉及到的TerrainPatch
		int patchX = tileX / pEditorRoot->terrain_->GetPatchSize();
		int patchZ = tileZ / pEditorRoot->terrain_->GetPatchSize();

		TerrainPatch* pPatch = pEditorRoot->terrain_->GetPatch(patchX,patchZ);//
		if(pPatch)
		{
			//pEditorRoot->terrain_->SyncHeightmapData();
			pEditorRoot->terrain_->CreatePatchGeometry(pPatch);
			if(pPatch->GetNorthPatch())
			{
				pEditorRoot->terrain_->CreatePatchGeometry(pPatch->GetNorthPatch());
			}
			if(pPatch->GetSouthPatch())
			{
				pEditorRoot->terrain_->CreatePatchGeometry(pPatch->GetSouthPatch());
			}
			if(pPatch->GetWestPatch())
			{
				pEditorRoot->terrain_->CreatePatchGeometry(pPatch->GetWestPatch());
			}
			if(pPatch->GetEastPatch())
			{
				pEditorRoot->terrain_->CreatePatchGeometry(pPatch->GetEastPatch());
			}
		}
	}
	else
	{
		//add_mesh_test(worldPos.x_,worldPos.y_,worldPos.z_);
	}

	//terrain->SetHeightMap(pHeightMap);
}

void TerrainEditor::InitDefaultScene()
{
	ResourceCache* cache = GetSubsystem<ResourceCache>();

	EditorsRoot* editorRoot_ = EditorsRoot::Instance();

	editorRoot_->scene_ = new Scene(context_);
	editorRoot_->scene_->CreateComponent<DebugRenderer>();
	// Create the Octree component to the scene. This is required before adding any drawable components, or else nothing will
	// show up. The default octree volume will be from (-1000, -1000, -1000) to (1000, 1000, 1000) in world coordinates; it
	// is also legal to place objects outside the volume but their visibility can then not be checked in a hierarchically
	// optimizing manner
	editorRoot_->scene_->CreateComponent<Octree>();
	//scene_->CreateComponent<PhysicsWorld>();

	Node* zoneNode = editorRoot_->scene_->CreateChild("Zone");
	Zone* zone = zoneNode->CreateComponent<Zone>();
	zone->SetBoundingBox(BoundingBox(-1000.0f, 1000.0f));
	zone->SetAmbientColor(Color(250./255.,208./255.,160./255.));

	zone->SetOccluder(true);
	zone->SetAmbientGradient(true);
	zone->SetFogColor(Color(0.1f, 0.2f, 0.3f));
	zone->SetFogStart(10.0f);
	zone->SetFogEnd(100.0f);

	//天空盒
	Node* skyNode = editorRoot_->scene_->CreateChild("Sky");
	skyNode->SetScale(500.0f); // The scale actually does not matter
	Skybox* skybox = skyNode->CreateComponent<Skybox>();
	skybox->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
	skybox->SetMaterial(cache->GetResource<Material>("Materials/Skybox.xml"));

	// Create heightmap terrain with collision
	editorRoot_->terrainNode = editorRoot_->scene_->CreateChild("Terrain");
	editorRoot_->terrainNode->SetPosition(Vector3::ZERO);
	editorRoot_->terrain_ = editorRoot_->terrainNode->CreateComponent<Terrain>();
	editorRoot_->terrain_->SetPatchSize(64);
	editorRoot_->terrain_->SetSpacing(Vector3(2.0f, 0.1f, 2.0f)); // Spacing between vertices and vertical resolution of the height map
	editorRoot_->terrain_->SetSmoothing(false);
	editorRoot_->terrain_->SetHeightMap(cache->GetResource<Image>("Textures/HeightMap.png"));
	editorRoot_->terrain_->SetMaterial(cache->GetResource<Material>("Materials/Terrain.xml"));
	// The terrain consists of large triangles, which fits well for occlusion rendering, as a hill can occlude all
	// terrain patches and other objects behind it
	editorRoot_->terrain_->SetOccluder(true);

	//RigidBody* body = editorRoot_->terrainNode->CreateComponent<RigidBody>();
	//body->SetCollisionLayer(2); // Use layer bitmask 2 for static geometry
	//CollisionShape* shape = editorRoot_->terrainNode->CreateComponent<CollisionShape>();
	//shape->SetTerrain();

	// Create a directional light to the world so that we can see something. The light scene node's orientation controls the
	// light direction; we will use the SetDirection() function which calculates the orientation from a forward direction vector.
	// The light will use default settings (white light, no shadows)
	Node* lightNode = editorRoot_->scene_->CreateChild("DirectionalLight");
	lightNode->SetDirection(Vector3(1.6f, -1.0f, 0.8f)); // The direction vector does not need to be normalized

	//创建默认灯光
	Light* light = lightNode->CreateComponent<Light>();
	light->SetLightType(LIGHT_DIRECTIONAL);
	float fBri = light->GetBrightness();
	light->SetBrightness(1.0f);
	light->SetCastShadows(true);
	//light->SetShadowBias(BiasParameters(0.00025f, 0.5f));
	light->SetShadowCascade(CascadeParameters(10.0f, 50.0f, 200.0f, 0.0f, 0.8f));
	light->SetShadowIntensity(1.0);

	// Create light position animation
	SharedPtr<ValueAnimation> positionAnimation(new ValueAnimation(context_));
	// Use spline interpolation method
	positionAnimation->SetInterpolationMethod(IM_SPLINE);
	// Set spline tension
	positionAnimation->SetSplineTension(0.7f);
	positionAnimation->SetKeyFrame(0.0f, Vector3(-30.0f, 5.0f, -30.0f));
	positionAnimation->SetKeyFrame(1.0f, Vector3( 30.0f, 5.0f, -30.0f));
	positionAnimation->SetKeyFrame(2.0f, Vector3( 30.0f, 5.0f,  30.0f));
	positionAnimation->SetKeyFrame(3.0f, Vector3(-30.0f, 5.0f,  30.0f));
	positionAnimation->SetKeyFrame(4.0f, Vector3(-30.0f, 5.0f, -30.0f));
	//lightNode->SetAttributeAnimation("Position", positionAnimation);

	// Create light color animation
	SharedPtr<ValueAnimation> colorAnimation(new ValueAnimation(context_));
	colorAnimation->SetKeyFrame(0.0f, Color::WHITE);
	colorAnimation->SetKeyFrame(1.0f, Color::RED);
	colorAnimation->SetKeyFrame(2.0f, Color::YELLOW);
	colorAnimation->SetKeyFrame(3.0f, Color::GREEN);
	colorAnimation->SetKeyFrame(4.0f, Color::WHITE);
	//light->SetAttributeAnimation("Color", colorAnimation);

	// Create a scene node for the camera, which we will move around
	// The camera will use default settings (1000 far clip distance, 45 degrees FOV, set aspect ratio automatically)
	editorRoot_->cameraNode_ = editorRoot_->scene_->CreateChild("Camera");
	editorRoot_->cameraNode_->CreateComponent<Camera>();

	// Set an initial position for the camera scene node above the plane
	editorRoot_->cameraNode_->SetPosition(Vector3(0.0f, 15.0f, 0.0f));

	Camera* pCamera = editorRoot_->cameraNode_->GetComponent<Camera>();
	pCamera->SetFarClip(300.0f);

	//设置ViewPort
	setupViewport();

	//
	Node* planeNode = editorRoot_->scene_->CreateChild("Plane");
	planeNode->SetScale(Vector3(100.0f, 1.0f, 100.0f));
	StaticModel* planeObject = planeNode->CreateComponent<StaticModel>();
	planeObject->SetModel(cache->GetResource<Model>("Models/Plane.mdl"));
	
	Material* mushroomMat = cache->GetResource<Material>("Materials/Mushroom.xml");
	// Apply shader parameter animation to material
	SharedPtr<ValueAnimation> specColorAnimation(new ValueAnimation(context_));
	specColorAnimation->SetKeyFrame(0.0f, Color(0.1f, 0.1f, 0.1f, 16.0f));
	specColorAnimation->SetKeyFrame(1.0f, Color(1.0f, 0.0f, 0.0f, 2.0f));
	specColorAnimation->SetKeyFrame(2.0f, Color(1.0f, 1.0f, 0.0f, 2.0f));
	specColorAnimation->SetKeyFrame(3.0f, Color(0.1f, 0.1f, 0.1f, 16.0f));
	mushroomMat->SetShaderParameterAnimation("MatSpecColor", specColorAnimation);

	planeObject->SetMaterial(mushroomMat);

	for(int i = 0; i < 3; i ++)
	{
		char szName[128];
		sprintf(szName,"che_%d",i);
		Node* pNode = add_mesh_test(szName,i * 1,0,i * 1,"Models/che1.mdl");
		pNode->SetWorldScale(0.1);
	}
}

void TerrainEditor::setupViewport()
{
	Renderer* renderer = GetSubsystem<Renderer>();
	EditorsRoot* editorRoot_ = EditorsRoot::Instance();

	// Set up a viewport to the Renderer subsystem so that the 3D scene can be seen. We need to define the scene and the camera
	// at minimum. Additionally we could configure the viewport screen size and the rendering path (eg. forward / deferred) to
	// use, but now we just use full screen and default render path configured in the engine command line options
	SharedPtr<Viewport> viewport(new Viewport(context_, editorRoot_->scene_, editorRoot_->cameraNode_->GetComponent<Camera>()));
	renderer->SetViewport(0, viewport);
}

void TerrainEditor::InitDefault2DScene()
{
	EditorsRoot* editorRoot_ = EditorsRoot::Instance();
	tiledNode = editorRoot_->scene_->CreateChild("TiledNode");
	//test_2d_scene();

	//创建2d Scene Node
	//"E:\\TileMaps\\examples","desert.tmx"
	//bool success = tiledScene.Load(tiledNode,context_,"D:\\Editor\\Editor\\Debug\\Urho2D\\tiled","example.tmx");

	/*
	//一个2d骨骼动画需要这两个配置，需要的图片在atlas中指定，相当于plist
	SkeletonAnimation* pSkeleton = new SkeletonAnimation("goblins.json","goblins.atlas");
	pSkeleton->setAnimation(0,"run",true);

	//在每次更新时获得每个骨骼的位置，交给绘图
	spSkeleton* ske = pSkeleton->skeleton;
	for(int i = 0;i < ske->boneCount;i ++)
	{
		spBone* bone = ske->bones[i];
		bone->worldX;
		bone->worldY;
	}
	*/
}

void TerrainEditor::test_2d_scene()
{
	ResourceCache* cache = context_->GetSubsystem<ResourceCache>();

	for(int x = 0;x < 10;x ++)
	{
		for(int y = 7;y >= 0;y --)
		{
			char nodeName[128];
			sprintf(nodeName,"tileNode%d-%d",y,x);
			Node* tileNode = tiledNode->CreateChild(nodeName);
			tileNode->SetPosition(Vector3(x*5.12,y*5.12,0));	//一个格子是1*1

			//载入图片
			char szFullPath[512];
			sprintf(szFullPath,"D:\\1\\%d_%d.jpg",7-y,x);
			Sprite2D* pNewSprite = cache->GetResource<Sprite2D>(szFullPath);
			pNewSprite->SetRectangle(IntRect(0,0,1.f/PIXEL_SIZE*5.12,1.f/PIXEL_SIZE*5.12));
			pNewSprite->SetHotSpot(Vector2(0,0));

			StaticSprite2D* tile = tileNode->CreateComponent<StaticSprite2D>();
			tile->SetSprite(pNewSprite);
		}
	}
}

Node* TerrainEditor::add_mesh_test(const char* name,float x,float y,float z,const char* modelUrl)
{
	EditorsRoot* editorRoot_ = EditorsRoot::Instance();

	ResourceCache* cache = GetSubsystem<ResourceCache>();

	y = editorRoot_->terrain_->GetHeight(Vector3(x,0,z));

	//modIndex ++;
	Node* mushroomNode = editorRoot_->scene_->CreateChild(String(name));
	mushroomNode->SetPosition(Vector3(x,y,z));//Vector3(0,editorRoot_->terrain_->GetHeight(Vector3(0,0,0)),0)
	//mushroomNode->SetPosition(Vector3(x, y + 5,z ));

	//Quaternion tt;
	//tt.FromAngleAxis(145,Vector3(0,1,0));
	//mushroomNode->Rotate(tt);
	//mushroomNode->SetRotation(Quaternion(-45.0f,0.0f , 0));//Random(360.0f)
	//mushroomNode->SetScale(0.010);	//0.9f + Random(1.0f)
	//StaticModel* mushroomObject = mushroomNode->CreateComponent<StaticModel>();
	//mushroomObject->SetModel(cache->GetResource<Model>(modelUrl));
	//mushroomObject->SetMaterial(0,cache->GetResource<Material>("Akali/Akali.xml"));
	//mushroomObject->SetMaterial(1,cache->GetResource<Material>("Akali/taizi-material.xml"));

	//mushroomObject->GetLodGeometry(0,0)->GetVertexBuffer(0)->SetShadowed(true);
	//mushroomObject->SetCastShadows(true);
	//mushroomObject->SetOccluder(true);

	//return mushroomNode;

	//Rotator* rotator = mushroomNode->CreateComponent<Rotator>();
	//rotator->SetRotationSpeed(Vector3(Vector3(0.0f, 20.0f, 0.0f)));

	
	AnimatedModel* modelObject = mushroomNode->CreateComponent<AnimatedModel>();
	Material* matObj = cache->GetResource<Material>("Materials/che.xml");
	SharedPtr<ValueAnimation> specColorAnimation(new ValueAnimation(context_));
	specColorAnimation->SetKeyFrame(0.0f, Color(0.1f, 0.1f, 0.1f, 16.0f));
	specColorAnimation->SetKeyFrame(1.0f, Color(1.0f, 0.0f, 0.0f, 2.0f));
	specColorAnimation->SetKeyFrame(2.0f, Color(1.0f, 1.0f, 0.0f, 2.0f));
	specColorAnimation->SetKeyFrame(3.0f, Color(0.1f, 0.1f, 0.1f, 16.0f));
	matObj->SetShaderParameterAnimation("MatSpecColor", specColorAnimation);

	modelObject->SetModel(cache->GetResource<Model>(modelUrl));	//"Akali/Akali.mdl"
	modelObject->SetMaterial(0,matObj);
	modelObject->SetCastShadows(true);

	//指定一个动作
	Animation* walkAnimation = cache->GetResource<Animation>("Models/che1_Bone01.ani");
	AnimationState* state = modelObject->AddAnimationState(walkAnimation);
	if(state)
	{
		state->SetWeight(1.0f);
		state->SetLooped(true);
	}

	const float MODEL_MOVE_SPEED = 2.0f;
	const float MODEL_ROTATE_SPEED = 100.0f;
	const BoundingBox bounds(Vector3(-47.0f, 0.0f, -47.0f), Vector3(47.0f, 0.0f, 47.0f));
	Mover* mover = mushroomNode->CreateComponent<Mover>();
	mover->SetParameters(MODEL_MOVE_SPEED, MODEL_ROTATE_SPEED, bounds);

	return mushroomNode;
}
