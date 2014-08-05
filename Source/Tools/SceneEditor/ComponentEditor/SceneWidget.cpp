#include "stdafx.h"
#include "SceneWidget.h"
#include "DebugRenderer.h"

SceneWidget::SceneWidget()
{
	SetIcon(":/Images/Components/Root.png");
	SetName("Scene");

	chkOctree = new QCheckBox("Octree");
	chkOctree->setEnabled(false);	//Octree是必须的，不允许编辑

	chkPhysics = new QCheckBox("PhysicsWorld");
	chkDebugRender = new QCheckBox("DebugRenderer");

	frameLayout_->addWidget(chkOctree);
	frameLayout_->addWidget(chkPhysics);
	frameLayout_->addWidget(chkDebugRender);

	connect(chkPhysics,SIGNAL(stateChanged(int)),this,SLOT(on_physics_click(int)));
	connect(chkDebugRender,SIGNAL(stateChanged(int)),this,SLOT(on_debugrender_click(int)));
}

void SceneWidget::Init(Node* pNode)
{
	pOwnerNode = pNode;

	//Octree
	if(pNode->GetComponent<Octree>() != NULL)
	{
		chkOctree->setCheckState(Qt::Checked);
	}
	else
	{
		chkOctree->setCheckState(Qt::Unchecked);
	}

	//PhysicsWorld
	if(pNode->GetComponent<PhysicsWorld>() != NULL)
	{
		chkPhysics->setCheckState(Qt::Checked);
	}
	else
	{
		chkPhysics->setCheckState(Qt::Unchecked);
	}

	//DebugRenderer
	if(pNode->GetComponent<DebugRenderer>() != NULL)
	{
		chkDebugRender->setCheckState(Qt::Checked);
	}
	else
	{
		chkDebugRender->setCheckState(Qt::Unchecked);
	}
}

void SceneWidget::on_physics_click(int state)
{
	PhysicsWorld* phys = pOwnerNode->GetComponent<PhysicsWorld>();

	if(state == Qt::Checked)
	{
		if(phys == NULL)
		{
			pOwnerNode->CreateComponent<PhysicsWorld>();
		}
	}
	else
	{
		if(phys != NULL)
		{
			pOwnerNode->RemoveComponent(phys);
		}
	}
}

void SceneWidget::on_debugrender_click(int state)
{
	DebugRenderer* renderer = pOwnerNode->GetComponent<DebugRenderer>();

	if(state == Qt::Checked)
	{
		if(renderer == NULL)
		{
			pOwnerNode->CreateComponent<DebugRenderer>();
		}
	}
	else
	{
		if(renderer != NULL)
		{
			pOwnerNode->RemoveComponent(renderer);
		}
	}
}
