#include "stdafx.h"
#include "SceneHelper.h"
#include "Viewport.h"
#include "RenderPath.h"
#include "Vector3.h"
#include "DebugRenderer.h"

SceneHelper::SceneHelper() : Object(EditorsRoot::Instance()->context_)
{
	scene2D = new Scene(context_);
	scene2D->CreateComponent<Octree>();
	scene2D->CreateComponent<DebugRenderer>();

	Camera* camera2D_ = scene2D->CreateComponent<Camera>();
	camera2D_->SetOrthographic(true);

	Graphics* graphics = context_->GetSubsystem<Graphics>();
	camera2D_->SetOrthoSize(graphics->GetHeight());

	Renderer* renderer = context_->GetSubsystem<Renderer>();
	SharedPtr<RenderPath> forwardNoClear = renderer->GetDefaultRenderPath()->Clone();
	forwardNoClear->RemoveCommand(0);

	Viewport* viewport = new Viewport(context_,scene2D,camera2D_,forwardNoClear);
	renderer->SetViewport(1,viewport);

	scene2D->SubscribeToEvent(E_UPDATE,HANDLER(SceneHelper,HandleUpdate));
}

void SceneHelper::Init2DScene()
{

}

void SceneHelper::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
	EditorsRoot* pEditorsRoot = EditorsRoot::Instance();
	RectSelectionFrame* frame = pEditorsRoot->RectSelectionFrame_;
	if(frame && frame->isBegin)
	{
		DebugRenderer* render = scene2D->GetComponent<DebugRenderer>();
		Graphics* graphics = GetSubsystem<Graphics>();

		Vector2 vecStart = GetScreenPos(frame->vecStart);
		Vector2 vecEnd = GetScreenPos(frame->vecEnd);

		Color color(1,0.784,0.392);
		render->AddLine(Vector3(vecStart.x_, vecStart.y_,0), Vector3(vecEnd.x_, vecStart.y_,0), color);
		render->AddLine(Vector3(vecEnd.x_, vecStart.y_,0), Vector3(vecEnd.x_, vecEnd.y_,0), color);
		render->AddLine(Vector3(vecEnd.x_, vecEnd.y_,0), Vector3(vecStart.x_, vecEnd.y_,0), color);
		render->AddLine(Vector3(vecStart.x_, vecEnd.y_,0), Vector3(vecStart.x_, vecStart.y_,0), color);

		render->Render();
	}
}

Vector2 SceneHelper::GetScreenPos(const Vector2& viewPos)
{
	Graphics* graphics = GetSubsystem<Graphics>();
	Vector2 result;
	result.x_ = viewPos.x_ - graphics->GetWidth() * 0.5;
	result.y_ = graphics->GetHeight() * 0.5 - viewPos.y_;

	return result;
}

Vector2 SceneHelper::GetViewPos(const Vector2& screenPos)
{
	Graphics* graphics = GetSubsystem<Graphics>();
	Vector2 result;
	result.x_ = screenPos.x_ + graphics->GetWidth() * 0.5;
	result.y_ = graphics->GetHeight() * 0.5 - screenPos.y_;

	return result;
}

vector<SceneHitResult> SceneHelper::QueryCurrentMousePosObjects(float maxDistance,IntVector2* screenPos)
{
	EditorsRoot* pEditorRoot = EditorsRoot::Instance();

	vector<SceneHitResult> vecObjects;

	IntVector2 pos;
	if(screenPos == NULL)
	{
		UI* ui = pEditorRoot->context_->GetSubsystem<UI>();
		IntVector2 pos = ui->GetCursorPosition();
		// Check the cursor is visible and there is no UI element in front of the cursor
		if (ui->GetElementAt(pos, true))	//!ui->GetCursor()->IsVisible() || 
			return vecObjects;
	}
	else
	{
		pos = *screenPos;
	}

	Graphics* graphics = pEditorRoot->context_->GetSubsystem<Graphics>();
	Camera* camera = pEditorRoot->cameraNode_->GetComponent<Camera>();
	Ray cameraRay = camera->GetScreenRay((float)pos.x_ / graphics->GetWidth(), (float)pos.y_ / graphics->GetHeight());
	// Pick only geometry objects, not eg. zones or lights, only get the first (closest) hit
	PODVector<RayQueryResult> results;
	RayOctreeQuery query(results, cameraRay, RAY_TRIANGLE, maxDistance, DRAWABLE_GEOMETRY);
	pEditorRoot->scene_->GetComponent<Octree>()->Raycast(query);

	for(int i = 0;i < results.Size();i ++)
	{
		RayQueryResult& result = results[i];

		SceneHitResult obj;
		obj.object = result.drawable_;
		// Calculate hit position in world space
		obj.hitPos = cameraRay.origin_ + cameraRay.direction_ * result.distance_;

		vecObjects.push_back(obj);
	}

	return vecObjects;
}

vector<Node*> SceneHelper::QueryScreenRectNodes(const Vector2& vecStart,const Vector2& vecEnd)
{
	vector<Node*> vResults;
	query_node_in_rect(EditorsRoot::Instance()->scene_,vResults,vecStart,vecEnd);

	return vResults;
}

void SceneHelper::query_node_in_rect(Node* node,vector<Node*>& outNodes,const Vector2& vecStart,const Vector2& vecEnd)
{
	const Vector<SharedPtr<Component> >& components = node->GetComponents();
	for(int j = 0;j < node->GetNumComponents();j ++)
	{
		Drawable* drawable = dynamic_cast<Drawable*>(components[j].Get());
		if(drawable != NULL)
		{
			if(ObjectPositionEditor::IsEditObject(drawable) == true)
			{
				Graphics* graphics = GetSubsystem<Graphics>();
				//判断Node中心点的投影是否落在矩形内
				Camera* pCamera = EditorsRoot::Instance()->cameraNode_->GetComponent<Camera>();
				Vector2 scrPos = pCamera->WorldToScreenPoint(node->GetWorldPosition());
				scrPos.x_ *= graphics->GetWidth();
				scrPos.y_ *= graphics->GetHeight();

				int maxX = max(vecStart.x_,vecEnd.x_);
				int minX = min(vecStart.x_,vecEnd.x_);
				int maxY = max(vecStart.y_,vecEnd.y_);
				int minY = min(vecStart.y_,vecEnd.y_);

				if((scrPos.x_ >= minX && scrPos.x_ <= maxX) && (scrPos.y_ >= minY && scrPos.y_ <= maxY))
				{
					outNodes.push_back(node);
				}
			}

			break;
		}
	}

	for(int i = 0;i < node->GetNumChildren();i ++)
	{
		query_node_in_rect(node->GetChild(i),outNodes,vecStart,vecEnd);
	}
}
