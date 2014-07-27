#pragma once
#include <QScrollArea>
#include <QHBoxLayout>
#include <QVBoxLayout>

class ScrollAreaWidget : public QScrollArea
{
    Q_OBJECT

public:
    ScrollAreaWidget();
    virtual ~ScrollAreaWidget();

protected:
    QHBoxLayout* AddHBoxLayout();

    QVBoxLayout* vBoxLayout_;
};
