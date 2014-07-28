#pragma once

#include <QScrollArea>

class QHBoxLayout;
class QVBoxLayout;

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
