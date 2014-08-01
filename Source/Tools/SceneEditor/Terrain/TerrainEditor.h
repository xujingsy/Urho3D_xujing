#pragma once
#include "Ptr.h"
#include "Object.h"
using namespace Urho3D;
#include "../EditorInfo/BaseEditor.h"
#include "../EditorAssist/TerrainBrush.h"
#include <vector>
using namespace std;

enum EditMode
{
	EM_NONE = 0,	//没有任何编辑
	EM_DEFORM,		//地形拉高，降低
	EM_SPLAT,		//刷地表贴图
	EM_PAINT,		//顶点色，占用一张贴图 todo,暂时不支持
	EM_SMOOTH,		//平滑地表
	EM_SPLATGRASS	//刷草
};

namespace Urho3D
{
	class Context;
	class Engine;

	class Sprite;
	class Node;
	class Scene;
	class Terrain;
}

//地形编辑器
class TerrainEditor : public BaseEditor,public Object
{
	OBJECT(TerrainEditor)
public:
	TerrainEditor(Context* context);

	virtual void OnMouseMove(float x,float y,unsigned int buttons);
	virtual void OnMouseLeftDown(float x,float y,unsigned int buttons);

	virtual void OnKeyDown(unsigned int key);
	virtual void OnKeyUp(unsigned int key);

	void startEdit();
	void stopEdit();

	//设置当前编辑模式
	void setEditMode(unsigned int mode);

	TerrainBrush* GetBrush()
	{
		return brush_;
	}

	Node* add_mesh_test(const char* name,float x,float y,float z,const char* modelUrl);
private:
	EditMode mEditMode;		//当前编辑模式
	float mBrushIntensity;	//画刷强度

	Node* brushNode_;
	TerrainBrush* brush_;

	//当
	void on_viewport_click(float x,float y);

	void on_terrain_click(Vector3& worldPos);

	void test_2d_scene();
	Node* tiledNode;

	int modIndex;
};
