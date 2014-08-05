#pragma once
#include "../Single.h"
#include <vector>
using namespace std;
#include "Vector3.h"
#include "Drawable.h"
#include "Object.h"
#include "Node.h"
using namespace Urho3D;

struct SceneHitResult 
{
	Drawable* object;
	Vector3 hitPos;
};

class SceneHelper : public Singleton<SceneHelper>,public Object
{
	OBJECT(SceneHelper)
public:
	SceneHelper();

	void Init2DScene();

	//返回当前鼠标点查询出来的对象列表
	vector<SceneHitResult> QueryCurrentMousePosObjects(float maxDistance,IntVector2* screenPos = NULL);

	//查询屏幕矩形区域的对应的Node(里面必须有可视化对象)
	vector<Node*> QueryScreenRectNodes(const Vector2& vecStart,const Vector2& vecEnd);

	Scene* scene2D;

	void HandleUpdate(StringHash eventType, VariantMap& eventData);

	Vector2 GetScreenPos(const Vector2& viewPos);
	Vector2 GetViewPos(const Vector2& screenPos);
private:
	//递归函数，判断节点是否落在区域内
	void query_node_in_rect(Node* node,vector<Node*>& outNodes,const Vector2& vecStart,const Vector2& vecEnd);
};
