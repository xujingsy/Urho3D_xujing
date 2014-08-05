#include "stdafx.h"
#include "EditorSceneWidget.h"
#include "EditorAPI/SceneHelper.h"
#include "MainWindow.h"

EditorSceneWidget::EditorSceneWidget(QWidget* parent, Qt::WindowFlags flags) : QWidget(parent, flags)
{
    // Set mininimum size
    setMinimumSize(640, 480);
    // Disable update
    setUpdatesEnabled(false);
	setFocusPolicy(Qt::WheelFocus);
	setMouseTracking(true);
	setAcceptDrops(true);	//允许拖放的Drop

	m_Captured = false;

	MouseMovedSincePress = false;
}

void EditorSceneWidget::dragEnterEvent(QDragEnterEvent* event)
{
	if(event->mimeData()->hasFormat("text/uri-list"))
	{
		event->acceptProposedAction();
	}
}

//拖动释放时
void EditorSceneWidget::dropEvent(QDropEvent *event)
{
	QList<QUrl> urls = event->mimeData()->urls();
	if(urls.isEmpty())
		return;

	QString fileName = urls.first().path();
	if(fileName.isEmpty())
	{
		return;
	}

	IntVector2 pos(event->pos().x(),event->pos().y());
	vector<SceneHitResult> results = SceneHelper::Instance()->QueryCurrentMousePosObjects(250.f,&pos);
	if(results.size() == 0)
		return;

	fileName = fileName.right(fileName.size() - 1);
	if(fileName.indexOf(".mdl") > 0)
	{
		for(int i = 0;i < results.size();i ++)
		{
			const SceneHitResult& obj = results[i];
			if(obj.object->GetTypeName() == "TerrainPatch")
			{
				EditorRoot::Instance()->GetTerrainEditor()->add_mesh_test(fileName.toStdString().c_str(),obj.hitPos.x_,obj.hitPos.y_,obj.hitPos.z_,fileName.toStdString().c_str());
				break;
			}
		}
	}
	else if(fileName.indexOf(".xml") > 0)	//todo:进一步识别是否是Material
	{
		const SceneHitResult& obj = results[0];
		if(obj.object->GetTypeName() == "StaticModel" || obj.object->GetTypeName() == "AnimatedModel")
		{
			StaticModel* pModel = (StaticModel*)obj.object;

			ResourceCache* cache = EditorRoot::Instance()->engine_->GetContext()->GetSubsystem<ResourceCache>();

			//选择材质序号
			int index = 0;
			
			if(pModel->GetBatches().Size() > 1)
			{

			}

			pModel->SetMaterial(index,cache->GetResource<Material>(fileName.toStdString().c_str()));
		}
	}
}

void EditorSceneWidget::mouseMoveEvent(QMouseEvent *evt)
{
	if(!hasFocus())
		setFocus();

	EditorRoot* editorRoot_ = EditorRoot::Instance();
	int x = evt->x();
	int y = evt->y();

	//状态栏显示鼠标坐标
	char szPos[128];
	sprintf(szPos,"x:%d y:%d",x,y);
	editorRoot_->GetMainWindow()->statusBar()->showMessage(szPos);
	
	EditorRoot::Instance()->OnMouseMove(x,y,0);

	//右键旋转
	if(m_Captured)
	{
		int relx = x - m_oldx;
		int rely = y - m_oldy;

		// Mouse sensitivity as degrees per pixel
		const float MOUSE_SENSITIVITY = 0.1f;

		editorRoot_->yaw_ += MOUSE_SENSITIVITY * relx;
		editorRoot_->pitch_ += MOUSE_SENSITIVITY * rely;
		editorRoot_->pitch_ = Clamp(editorRoot_->pitch_, -90.0f, 90.0f);
		// Construct new orientation for the camera scene node from yaw and pitch. Roll is fixed to zero
		editorRoot_->cameraNode_->SetRotation(Quaternion(editorRoot_->pitch_, editorRoot_->yaw_, 0.0f));
	}

	//鼠标中间拖拽场景
	if(evt->buttons() & Qt::MidButton)
	{
		editorRoot_->cameraNode_->Translate(Vector3((x - m_oldx) * 0.01,(y - m_oldy) * 0.01,0));
	}

	//记录上次的坐标，为了计算鼠标偏移
	m_oldx = x;
	m_oldy = y;

	//发生了鼠标移动
	MouseMovedSincePress = true;
}

void EditorSceneWidget::mousePressEvent(QMouseEvent* evt)
{
	if(!hasFocus())
		setFocus();

	int x = evt->x();
	int y = evt->y();

	if(evt->button() == Qt::LeftButton)
	{
		EditorRoot::Instance()->OnMouseLeftDown(x,y,getMouseButton(evt->buttons(),evt->button()));
	}
	else if(evt->button() == Qt::RightButton)
	{
		if(m_Captured == false)
		{
			SetCapture(this->winId());
			ShowCursor(false);
			m_Captured = true;

			GetCursorPos(&m_savedpos);
		}
		
		EditorRoot::Instance()->OnMouseRightDown(evt->x(),evt->y(),getMouseButton(evt->buttons(),evt->button()));
	}
	else if(evt->button() == Qt::MiddleButton)
	{
		EditorRoot::Instance()->OnMouseMiddleDown(evt->x(),evt->y(),getMouseButton(evt->buttons(),evt->button()));
	}

	MouseMovedSincePress = false;
}

void EditorSceneWidget::mouseReleaseEvent(QMouseEvent *evt)
{
	if(!hasFocus())
		setFocus();
	if(evt->button() == Qt::LeftButton)
	{
		EditorRoot::Instance()->OnMouseLeftUp(evt->x(),evt->y(),getMouseButton(evt->buttons(),evt->button()));
	}
	else if(evt->button() == Qt::RightButton)
	{
		EditorRoot::Instance()->OnMouseRightUp(evt->x(),evt->y(),getMouseButton(evt->buttons(),evt->button()));
	}
	else if(evt->button() == Qt::MiddleButton)
	{
		EditorRoot::Instance()->OnMouseMiddleUp(evt->x(),evt->y(),getMouseButton(evt->buttons(),evt->button()));
	}

	if(m_Captured == true)	//
	{
		SetCursorPos(m_savedpos.x,m_savedpos.y);
		
		ShowCursor(true);
		m_Captured = false;

		ReleaseCapture();
	}

	if(!MouseMovedSincePress && evt->button() == Qt::RightButton)
	{
		showObjectMenu();
	}
}

void EditorSceneWidget::showObjectMenu()
{
	EditorRoot* pEditorRoot = EditorRoot::Instance();
	if(pEditorRoot->SelectionNodes.empty())
		return;

	MainWindow* mMainWindow = pEditorRoot->GetMainWindow();

	QMenu* contextMenu = new QMenu(this);
	if(pEditorRoot->SelectionNodes.size() == 1)
	{
		Node* pNode = pEditorRoot->SelectionNodes[0];
		contextMenu->setTitle(tr("Object Menu : ") + QString(pNode->GetName().CString()));
	}
	else
	{
		contextMenu->setTitle(tr("Object Count : ") + QString::number(pEditorRoot->SelectionNodes.size()));
	}

	contextMenu->addAction(mMainWindow->renameAction_);
	contextMenu->addSeparator();

	contextMenu->addAction(mMainWindow->copyAction_);
	contextMenu->addAction(mMainWindow->cutAction_);
	contextMenu->addSeparator();

	contextMenu->addAction(mMainWindow->deleteAction_);

	contextMenu->exec(QCursor::pos());
}

void EditorSceneWidget::leaveEvent(QEvent *evt)
{
	
}

void EditorSceneWidget::wheelEvent(QWheelEvent *evt)
{
	EditorRoot::Instance()->OnMouseWheel(evt->x(),evt->y(),0);
}

unsigned int EditorSceneWidget::getMouseButton(Qt::MouseButtons buttons,Qt::MouseButton button)
{
	unsigned int flags = 0;
	buttons |= button;

	if(buttons.testFlag(Qt::LeftButton))
		flags |= MOUSEB_LEFT;
	if(buttons.testFlag(Qt::RightButton))
		flags |= MOUSEB_RIGHT;
	if(buttons.testFlag(Qt::MidButton))
		flags |= MOUSEB_MIDDLE;

	return flags;
}

void EditorSceneWidget::keyPressEvent(QKeyEvent *evt)
{
	unsigned int key = evt->key();
	if(key > 255)
		key = (key & 0xFFF) + 0xFF;

	EditorRoot::Instance()->OnKeyDown(key);
}
