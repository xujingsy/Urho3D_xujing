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
	//if(EditorRoot::Instance()->ActiveTool != TOOL_DEFORM)
	//	return;

	EditorRoot* editorRoot_ = EditorRoot::Instance();

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
			brush_->SetMaterial(cache->GetResource<Material>("Materials/Editor/GreenUnlit.xml"));
			brush_->SetOwner(editorRoot_->terrain_);
			//brush_->SetBrushTexture(cache->GetResource<Texture2D>(":/Images/Brushes/noisy_circular2.png"));
			//brush_->SetViewMask(0xfffffffe);
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
	EditorRoot* pEditorRoot = EditorRoot::Instance();
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
				Color color = pHeightMap->GetPixel(tileX + xx,tileZ + yy).Lerp(Color(1.0, 0, 0),pHeightMap->GetPixel(tileX, tileZ).r_ * 0.1);
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

		//重新刷新所有地表，这样效率太低
		pEditorRoot->terrain_->SetHeightMap(pHeightMap);
	}
	else
	{
		//add_mesh_test(worldPos.x_,worldPos.y_,worldPos.z_);
	}
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
	EditorRoot* editorRoot_ = EditorRoot::Instance();

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

	
	//AnimatedModel* modelObject = mushroomNode->CreateComponent<AnimatedModel>();
	//Material* matObj = cache->GetResource<Material>("Materials/che.xml");
	//SharedPtr<ValueAnimation> specColorAnimation(new ValueAnimation(context_));
	//specColorAnimation->SetKeyFrame(0.0f, Color(0.1f, 0.1f, 0.1f, 16.0f));
	//specColorAnimation->SetKeyFrame(1.0f, Color(1.0f, 0.0f, 0.0f, 2.0f));
	//specColorAnimation->SetKeyFrame(2.0f, Color(1.0f, 1.0f, 0.0f, 2.0f));
	//specColorAnimation->SetKeyFrame(3.0f, Color(0.1f, 0.1f, 0.1f, 16.0f));
	//matObj->SetShaderParameterAnimation("MatSpecColor", specColorAnimation);

	//modelObject->SetModel(cache->GetResource<Model>(modelUrl));	//"Akali/Akali.mdl"
	//modelObject->SetMaterial(0,matObj);
	//modelObject->SetCastShadows(true);

	//指定一个动作
	//Animation* walkAnimation = cache->GetResource<Animation>("Models/che1_Bone01.ani");
	//AnimationState* state = modelObject->AddAnimationState(walkAnimation);
	//if(state)
	//{
	//	state->SetWeight(1.0f);
	//	state->SetLooped(true);
	//}

	//const float MODEL_MOVE_SPEED = 2.0f;
	//const float MODEL_ROTATE_SPEED = 100.0f;
	//const BoundingBox bounds(Vector3(-47.0f, 0.0f, -47.0f), Vector3(47.0f, 0.0f, 47.0f));
	//Mover* mover = mushroomNode->CreateComponent<Mover>();
	//mover->SetParameters(MODEL_MOVE_SPEED, MODEL_ROTATE_SPEED, bounds);

	return mushroomNode;
}
