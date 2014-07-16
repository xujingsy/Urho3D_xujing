#pragma once
#include "CustomGeometry.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Vector2.h"
#include "Texture.h"
using namespace Urho3D;

namespace Urho3D
{
	class Terrain;
};

//大小为单位1^，通过Scale设置笔刷大小
class TerrainBrush : public CustomGeometry
{
	OBJECT(TerrainBrush);
public:
	TerrainBrush(Context* context);
	~TerrainBrush();

	void SetOwner(Terrain* terrain)
	{
		owner_ = terrain;
	}

	void UpdatePos(const Vector3& worldPosition);

	void SetBrushSize(int brushSize);
	int GetBrushSize();

	void SetBrushTexture(Texture* texture);
private:
	float brushSize_;
	Vector3 currentPos;	//当前在世界坐标中的位置
	Terrain* owner_;
	int pieces_;	//长宽有多少个顶点，越多越细腻，但是计算量会变大
	float fPerSize;
	float fPerUv;
	int numVerts;
	float curPosHeight;

	void getVector(int x,int y,Vector3& position,Vector2& uv);
	void createGeometry();
};
