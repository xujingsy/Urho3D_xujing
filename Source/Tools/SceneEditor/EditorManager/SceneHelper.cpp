#include "stdafx.h"
#include "SceneHelper.h"
#include "Viewport.h"
#include "RenderPath.h"
#include "Vector3.h"
#include "DebugRenderer.h"

SceneHelper::SceneHelper() : Object(EditorRoot::Instance()->context_)
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
	EditorRoot* pEditorRoot = EditorRoot::Instance();
	RectSelectionFrame* frame = pEditorRoot->RectSelectionFrame_;
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
	EditorRoot* pEditorRoot = EditorRoot::Instance();

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
	query_node_in_rect(EditorRoot::Instance()->scene_,vResults,vecStart,vecEnd);

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
				Camera* pCamera = EditorRoot::Instance()->cameraNode_->GetComponent<Camera>();
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


Node* SceneHelper::AddMesh(const char* name,float x,float y,float z,const char* modelUrl)
{
	EditorRoot* editorRoot_ = EditorRoot::Instance();

	ResourceCache* cache = editorRoot_->context_->GetSubsystem<ResourceCache>();

	y = editorRoot_->terrain_->GetHeight(Vector3(x,0,z));

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
