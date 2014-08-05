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
#include "../EditorManager/SceneHelper.h"
#include "ValueAnimation.h"

TerrainEditor::TerrainEditor(Context* context) : Object(context)
{
	brush_ = NULL;
	brushNode_ = NULL;

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
			create_brush_node();
		}

		brush_->UpdatePos(hitPos);
		brushNode_->SetPosition(hitPos);	//放在当前鼠标位置
	}
}

void TerrainEditor::create_brush_node()
{
	brushNode_ = EditorRoot::Instance()->scene_->CreateChild("BrushNode");
	brush_ = brushNode_->CreateComponent<TerrainBrush>();

	ResourceCache* cache = GetSubsystem<ResourceCache>();
	brush_->SetMaterial(cache->GetResource<Material>("Materials/Editor/GreenUnlit.xml"));
	brush_->SetOwner(EditorRoot::Instance()->terrain_);
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
}

void TerrainEditor::OnSceneReset()
{
	create_brush_node();
}
