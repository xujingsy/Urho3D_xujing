#include "stdafx.h"
#include "AddComponentWidget.h"

AddComponentWidget::AddComponentWidget(QWidget* parent)
{
	QVBoxLayout* vLayout = new QVBoxLayout();
	setLayout(vLayout);

	String strResult;

	const HashMap<StringHash, SharedPtr<ObjectFactory> >& factories = EditorRoot::Instance()->context_->GetObjectFactories();
	for (HashMap<StringHash, SharedPtr<ObjectFactory> >::ConstIterator i = factories.Begin(); i != factories.End(); ++i)
	{
		SharedPtr<Object> object = i->second_->CreateObject();
		
		QPushButton* btn = new QPushButton();
		btn->setText(object->GetTypeName().CString());

		strResult += object->GetTypeName() + "\r\n";

		vLayout->addWidget(btn);
	}
}
