#include "stdafx.h"
#include "TerrainBrush.h"
#include "MemoryBuffer.h"

TerrainBrush::TerrainBrush(Context* context) :
	CustomGeometry(context)
{
	pieces_ = 10;
	numVerts = (pieces_ - 1) * (pieces_ - 1) * 6;	//每个格子6个顶点

	brushSize_ = 2.0;
	fPerUv = 1.0 / (pieces_ - 1);	//每个格子uv增量
	owner_ = NULL;

	BeginGeometry(0,TRIANGLE_LIST);
}

TerrainBrush::~TerrainBrush()
{

}


void TerrainBrush::SetBrushSize(int brushSize)
{
	brushSize_ = Clamp(brushSize,1,100);
	createGeometry();
}

int TerrainBrush::GetBrushSize()
{
	return brushSize_;
}

void TerrainBrush::createGeometry()
{
	PODVector<unsigned char> data;
	data.Resize(numVerts * (sizeof(Vector3) + sizeof(Vector2)) + 3 * sizeof(int));	//todo:
	fPerSize = brushSize_ / (pieces_ - 1);	//每个格子的长宽

	MemoryBuffer buffer(data);
	buffer.WriteVLE(1);	//Geometry数量1
	buffer.WriteUInt(MASK_POSITION | MASK_TEXCOORD1);
	buffer.WriteVLE((pieces_ - 1) * (pieces_ - 1) * 6);	//顶点数
	buffer.WriteUByte(TRIANGLE_LIST);

	Vector3 pt;
	Vector2 uv;
	//循环每个格子
	for(int x = 0;x < pieces_ - 1;x ++)
	{
		for(int y = 0;y < pieces_ - 1;y ++)
		{
			//left triangle CCW
			getVector(x,y,pt,uv);
			buffer.WriteVector3(pt);
			buffer.WriteVector2(uv);

			getVector(x,y + 1,pt,uv);
			buffer.WriteVector3(pt);
			buffer.WriteVector2(uv);

			getVector(x + 1,y + 1,pt,uv);
			buffer.WriteVector3(pt);
			buffer.WriteVector2(uv);

			//right triangle
			getVector(x,y,pt,uv);
			buffer.WriteVector3(pt);
			buffer.WriteVector2(uv);

			getVector(x + 1,y + 1,pt,uv);
			buffer.WriteVector3(pt);
			buffer.WriteVector2(uv);

			getVector(x + 1,y,pt,uv);
			buffer.WriteVector3(pt);
			buffer.WriteVector2(uv);
		}
	}

	SetGeometryDataAttr(data);
}

void TerrainBrush::getVector(int x,int y,Vector3& position,Vector2& uv)
{//currentPos.x_
	Vector3 pos;
	pos.x_ = - brushSize_ / 2. + x * fPerSize;
	pos.z_ = - brushSize_ / 2. + y * fPerSize;
	pos.y_ = owner_->GetHeight(pos) - curPosHeight;
	position = pos;

	uv.x_ = x * fPerUv;
	uv.y_ = y * fPerUv;
}

//worldPosition 世界坐标点
//生成各个点所在世界坐标中的位置
void TerrainBrush::UpdatePos(const Vector3& worldPosition)
{
	currentPos = worldPosition;

	curPosHeight = owner_->GetHeight(worldPosition);

	createGeometry();
}

void TerrainBrush::SetBrushTexture(Texture* texture)
{
	for(int i = 0;i < batches_.Size();i ++)
	{
		SourceBatch& sBatch = batches_[i];
		sBatch.material_->SetTexture(TextureUnit::TU_DIFFUSE,texture);
	}
}
