#pragma once
#include "Object.h"
#include "Node.h"
using namespace Urho3D;
#include "../EditorInfo/BaseEditor.h"
#include <vector>
using namespace std;
#include "../EditorAPI/SceneHelper.h"
#include "../EditorAPI/OperationData.h"

/*
点击坐标轴实现拖动物体的算法：
点击时，记录鼠标Ray在Axis上投影点到Axis原点的距离，之后鼠标Ray不断变化，导致这个距离也在变化，根据坐标差进行修正坐标轴及对应Object的坐标
*/

//物件移动，缩放等操作
class ObjectPositionEditor : public BaseEditor,public Object
{
	OBJECT(ObjectPositionEditor)
public:
	ObjectPositionEditor(Context* context);

	void OnMouseLeftDown(float x,float y,unsigned int buttons);
	void OnMouseLeftUp(float x,float y,unsigned int buttons);

	void OnMouseMove(float x,float y,unsigned int buttons);

	bool OnKeyDown(unsigned short key);
	bool OnKeyUp(unsigned short key);

	void OnObjectSelect(Node* pNode);

	void OnNodesCut();

	//复制
	void OnNodesCopy();

	//粘贴
	void OnNodesPaste();

	//是否为本编辑器编辑的对象
	static bool IsEditObject(Drawable* obj);
private:
	void HandlePostRenderUpdate(StringHash eventType, VariantMap& eventData);

	Drawable* get_edit_object(const vector<SceneHitResult>& result);
	bool is_axis_object(const vector<SceneHitResult>& result);

	bool isRectSelectionMode;
	Vector2 rectSelectionStart;

	Drawable* CurrentHoverObject;
	list<NodeData> nodes_clipboard_data;	//复制的节点数据
};
