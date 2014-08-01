#pragma once
#include "Object.h"

//选择内容发生变化
EVENT(E_NODE_SELECTION_CHANGE, NodeSelectionChange)
{
}

//Node的显示发生变化
EVENT(E_NODE_UPDATE_SHOW, NodeUpdateShow)
{
	PARAM(P_NODE, Node);
}

//整个场景重置
EVENT (E_SCENE_RESET, SceneReset)
{

}
