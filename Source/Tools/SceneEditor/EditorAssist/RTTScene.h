#pragma once
#include "Object.h"
#include "Image.h"

namespace Urho3D
{
	class Node;
	class Scene;
	class Context;
	class Image;
	class Texture2D;
	class Image;
}
using namespace Urho3D;

//一个空的场景，用来辅助RTT
class RTTScene : public Object
{
	OBJECT(RTTScene)
public:
	//参数width,height表示输出的图片尺寸
	RTTScene(Context* context,int width = 1024,int height = 768);

	void CreateScene();

	void Clear();

	Node* CreateChildNode(const String& nodeName);

	Texture2D* GetResultTexture()
	{
		return renderTexture;
	}

	void SaveJpg(const char* pPath,int quality = 80);

	Node* GetCameraNode()
	{
		return rttCameraNode_;
	}
private:
	Scene* rttScene_;
	Node* rttCameraNode_;
	Texture2D* renderTexture;	//实时得到的Texture
	
	Image* resultImage;

	int _width;
	int _height;
};
