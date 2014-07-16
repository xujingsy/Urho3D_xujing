#include "stdafx.h"
#include "RTTScene.h"

RTTScene::RTTScene(Context* context,int width,int height) : Object(context)
{
	_width = width;
	_height = height;

	resultImage = new Image(context);
	renderTexture = NULL;

	CreateScene();
}

void RTTScene::CreateScene()
{
	rttScene_ = new Scene(context_);
	rttScene_->CreateComponent<Octree>();

	Node* zoneNode = rttScene_->CreateChild("Zone");
	Zone* zone = zoneNode->CreateComponent<Zone>();
	zone->SetBoundingBox(BoundingBox(-1000.0f, 1000.0f));
	zone->SetAmbientColor(Color(0.05f, 0.1f, 0.15f));
	zone->SetFogColor(Color(0.1f, 0.2f, 0.3f));
	zone->SetFogStart(10.0f);
	zone->SetFogEnd(100.0f);

	//用来指定ViewPort时的定位
	rttCameraNode_ = rttScene_->CreateChild("Camera");
	Camera* camera = rttCameraNode_->CreateComponent<Camera>();
	
	Light* light = rttCameraNode_->CreateComponent<Light>();
	light->SetLightType(LIGHT_POINT);

	renderTexture = new Texture2D(context_);
	//设置当前场景的ViewPort
	renderTexture->SetSize(_width,_height,Graphics::GetRGBFormat(),TEXTURE_RENDERTARGET);
	RenderSurface* surface = renderTexture->GetRenderSurface();
	renderTexture->SetFilterMode(FILTER_BILINEAR);

	Viewport* rttViewPort = new Viewport(context_,rttScene_,camera);
	surface->SetViewport(0,rttViewPort);
}

//恢复到初始化场景
void RTTScene::Clear()
{
	rttScene_->RemoveAllChildren();
}

Node* RTTScene::CreateChildNode(const String& nodeName)
{
	Node* pChild = rttScene_->CreateChild(nodeName);
	return pChild;
}

void RTTScene::SaveJpg(const char* pPath,int quality)
{
	resultImage->SetSize(_width,_height,3);
	renderTexture->GetData(0,resultImage->GetData());
	resultImage->FlipVertical();

	resultImage->SaveJPG(pPath,quality);
}
