#pragma once

#include "Vector3.h"
using namespace Urho3D;
#include <QGroupBox>
#include "FloatEditor.h"

class Vector3Editor : public QGroupBox
{
	Q_OBJECT
public:
	Vector3Editor(const QString& text);
	virtual ~Vector3Editor();

public:
	void setValue(const Vector3& value);

	Vector3 value() const;

	void setRange(const Vector3& min, const Vector3& max);

signals:
	//内部抛出的事件
	void valueChanged(const Vector3&);
protected slots:
	void editorValueChanged();
private:
	FloatEditor* xEditor;
	FloatEditor* yEditor;
	FloatEditor* zEditor;
};
