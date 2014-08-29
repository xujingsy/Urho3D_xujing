//
// Copyright (c) 2008-2014 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include "Precompiled.h"
#include "Context.h"
#include "Deserializer.h"
#include "FileSystem.h"
#include "Log.h"
#include "ResourceCache.h"
#include "Serializer.h"
#include "Sprite2D.h"
#include "SpriteSheet2D.h"
#include "Texture2D.h"
#include "XMLFile.h"

#include "DebugNew.h"


namespace Urho3D
{

SpriteSheet2D::SpriteSheet2D(Context* context) :
    Resource(context)
{
	plistFile_ = NULL;
}

SpriteSheet2D::~SpriteSheet2D()
{
	if(plistFile_)
		delete plistFile_;
}

void SpriteSheet2D::RegisterObject(Context* context)
{
    context->RegisterFactory<SpriteSheet2D>();
}

bool SpriteSheet2D::BeginLoad(Deserializer& source)
{
    if (GetName().Empty())
        SetName(source.GetName());

    loadTextureName_.Clear();
    spriteMapping_.Clear();

    loadXMLFile_ = new XMLFile(context_);
    if(!loadXMLFile_->Load(source))
    {
        LOGERROR("Could not load sprite sheet");
        loadXMLFile_.Reset();
        return false;
    }

    SetMemoryUse(source.GetSize());

	//Starling Sparrow Format
    XMLElement rootElem = loadXMLFile_->GetRoot("TextureAtlas");
    if (rootElem)
    {
		texFormat_ = enTexPackFormat_StarlingSparrow;

		// If we're async loading, request the texture now. Finish during EndLoad().
		loadTextureName_ = GetParentPath(GetName()) + rootElem.GetAttribute("imagePath");
		if (GetAsyncLoadState() == ASYNC_LOADING)
			GetSubsystem<ResourceCache>()->BackgroundLoadResource<Texture2D>(loadTextureName_, true, this);

		return true;
	}
	
	//plist Format
	rootElem = loadXMLFile_->GetRoot("plist");
	if(rootElem)
	{
		texFormat_ = enTexPackFormat_Plist;

		unsigned dataSize = source.GetSize();
		SharedArrayPtr<char> buffer(new char[dataSize + 1]);	//string end with \0
		source.Seek(0);
		if (source.Read(buffer.Get(), dataSize) != dataSize)
			return false;

		buffer[dataSize] = 0;

		plistFile_ = new PlistFile();
		if(!plistFile_->Init((const unsigned char*)buffer.Get(), dataSize + 1))
		{
			LOGERROR("Could not load plist");
			loadXMLFile_.Reset();
			return false;
		}

		//1.metadata
		PlistElement* metadata = plistFile_->data.GetChild("metadata");
		if(metadata == NULL)
		{
			return false;
		}

		loadTextureName_ = GetParentPath(GetName()) + metadata->GetChild("textureFileName")->strValue.c_str();
		if (GetAsyncLoadState() == ASYNC_LOADING)
			GetSubsystem<ResourceCache>()->BackgroundLoadResource<Texture2D>(loadTextureName_, true, this);

		return true;
    }

	// Only support sparrow or plist
	LOGERROR("Invalid sprite sheet");
	loadXMLFile_.Reset();

	return false;
}

bool SpriteSheet2D::EndLoad()
{
    if (!loadXMLFile_)
        return false;

    ResourceCache* cache = GetSubsystem<ResourceCache>();
    texture_ = cache->GetResource<Texture2D>(loadTextureName_);
    if (!texture_)
    {
        LOGERROR("Could not load texture " + loadTextureName_);
        loadXMLFile_.Reset();
        loadTextureName_.Clear();
        return false;
    }

	if (texFormat_ == enTexPackFormat_StarlingSparrow)
	{
		XMLElement rootElem = loadXMLFile_->GetRoot("TextureAtlas");
		XMLElement subTextureElem = rootElem.GetChild("SubTexture");
		while (subTextureElem)
		{
			String name = subTextureElem.GetAttribute("name");

			int x = subTextureElem.GetInt("x");
			int y = subTextureElem.GetInt("y");
			int width = subTextureElem.GetInt("width");
			int height = subTextureElem.GetInt("height");
			IntRect rectangle(x, y, x + width, y + height);

			Vector2 hotSpot(0.5f, 0.5f);
			IntVector2 offset(0, 0);
			if (subTextureElem.HasAttribute("frameWidth") && subTextureElem.HasAttribute("frameHeight"))
			{
				offset.x_ = subTextureElem.GetInt("frameX");
				offset.y_ = subTextureElem.GetInt("frameY");
				int frameWidth = subTextureElem.GetInt("frameWidth");
				int frameHeight = subTextureElem.GetInt("frameHeight");
				hotSpot.x_ = ((float)offset.x_ + frameWidth / 2) / width;
				hotSpot.y_ = 1.0f - ((float)offset.y_ + frameHeight / 2) / height;
			}

			DefineSprite(name, rectangle, hotSpot, offset);

			subTextureElem = subTextureElem.GetNext("SubTexture");
		}
	}
	else if(texFormat_ == enTexPackFormat_Plist)
	{
		PlistElement* frames = plistFile_->data.GetChild("frames");

		int index = 1;
		for each(PlistElement* pic in frames->dict)
		{
			String name = pic->key.c_str();

			vector<SizeElement> frame = pic->GetChild("frame")->ValueToSizeArray();
			int x = frame[0].X;
			int y = frame[0].Y;
			int width = frame[1].X;
			int height = frame[1].Y;
			IntRect rectangle(x, y, x + width, y + height);

			vector<SizeElement> hotOffset = pic->GetChild("offset")->ValueToSizeArray();
			Vector2 hotSpot(0.5f, 0.5f);
			IntVector2 offset(0, 0);
			{
				offset.x_ = hotOffset[0].X;
				offset.y_ = hotOffset[0].Y;

				vector<SizeElement> sourceSize = pic->GetChild("sourceSize")->ValueToSizeArray();
				int frameWidth = sourceSize[0].X;
				int frameHeight = sourceSize[0].Y;
				hotSpot.x_ = ((float)offset.x_ + frameWidth / 2) / width;
				hotSpot.y_ = 1.0f - ((float)offset.y_ + frameHeight / 2) / height;
			}
			
			string rotated = pic->GetChild("rotated")->strValue;
			if(rotated == "true")	// rotate CW 90,todo:now not support
			{
			
			}
			//vector<SizeElement> sourceColorRect = pic->GetChild("sourceColorRect")->ValueToSizeArray(); not support now

			DefineSprite(name, rectangle, hotSpot, offset);
		}
	}

    loadXMLFile_.Reset();
    loadTextureName_.Clear();

    return true;
}

Sprite2D* SpriteSheet2D::GetSprite(const String& name) const
{
    HashMap<String, SharedPtr<Sprite2D> >::ConstIterator i = spriteMapping_.Find(name);
    if (i == spriteMapping_.End())
        return 0;

    return i->second_;
}

void SpriteSheet2D::DefineSprite(const String& name, const IntRect& rectangle, const Vector2& hotSpot, const IntVector2& offset)
{
    if (!texture_)
        return;

    if (GetSprite(name))
        return;

    SharedPtr<Sprite2D> sprite(new Sprite2D(context_));
    sprite->SetName(name);
    sprite->SetTexture(texture_);
    sprite->SetRectangle(rectangle);
    sprite->SetHotSpot(hotSpot);
    sprite->SetOffset(offset);
    sprite->SetSpriteSheet(this);

    spriteMapping_[name] = sprite;
}

}
