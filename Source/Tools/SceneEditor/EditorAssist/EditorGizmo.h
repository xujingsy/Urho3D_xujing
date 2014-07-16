#pragma once
#include "Object.h"
#include "StaticModel.h"
#include "Ray.h"
#include "Context.h"
#include <vector>
using namespace std;

namespace Urho3D
{
	class Node;
	class Context;
}

using namespace Urho3D;

enum enEditMode
{
	enEditMode_Move,	//移动
	enEditMode_Rotate,	//旋转
	enEditMode_Scale,	//缩放
};

class Editor3dGizmo;

//单个坐标轴
class GizmoAxis : public Object
{
	OBJECT(GizmoAxis)
public:
	GizmoAxis(Context* context) : Object(context)
	{
		selected = false;
		hovered = false;

		t = d = lastT = lastD = 0;
	}

	//判断与该射线是否选中
	bool QueryRaySelect(const Ray& worldRay);

	bool IsHighShow()
	{
		return selected || hovered;
	}

	Ray axisRay;	//在世界中的指向(Node的偏移方向就是根据这个)
	bool selected;	//是否被选中，当点击坐标轴原点附近时三个坐标轴都会被选中(仅在点击时更新该值，拖动时根据该值判断是否应该拖动)
	bool hovered;	//当前是否悬浮

	float t;
	float d;
	float lastT;
	float lastD;

	Editor3dGizmo* pOwnerGizmo;
};

//点击Node时显示的三个坐标轴
class Editor3dGizmo : public Object
{
	OBJECT(Editor3dGizmo)
public:
	Editor3dGizmo(Context* context);

	void CreateGizmo();
	void ShowGizmo();
	void HideGizmo();

	void SetMode(enEditMode mode);
public:
	GizmoAxis* gizmoAxisX;
	GizmoAxis* gizmoAxisY;
	GizmoAxis* gizmoAxisZ;

	bool IsSelected()
	{
		return gizmoAxisX->selected || gizmoAxisY->selected || gizmoAxisZ->selected;
	}

	Node* GetNode()
	{
		return gizmoNode;
	}

	StaticModel* GetModel()
	{
		return gizmo;
	}

	//根据选择状态来更新
	void Update();

	//给定world射线，返回三个坐标轴的相交情况
	void QuerySelectedStateByWorldRay(const Ray& worldRay,bool& xSelect,bool& ySelect,bool& zSelect);
	//根据当前编辑类型显示不同的模型
	void update_model();

	void BeginDrag();
	void EndDrag();

	void UpdateMovePosition();

	Vector3 posOnClick_;	//记录按下鼠标时的坐标轴原点位置
	bool IsDrag;
private:

	void HandleUpdate(StringHash eventType, VariantMap& eventData);

	//更新三个坐标轴在世界中的指向
	void CalculateGizmoAxes();

	enEditMode lastGizmoMode;

	Node* gizmoNode;
	StaticModel* gizmo;
};
