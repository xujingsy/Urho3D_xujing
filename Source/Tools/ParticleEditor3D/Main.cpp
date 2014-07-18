#include "stdafx.h"
#include "Application.h"
#include "EditorApplication.h"
#include <QFile>

int Main()
{
    int argc = 0;
    char** argv = 0;
    Urho3D::SharedPtr<Urho3D::Context> context(new Urho3D::Context());
    EditorApplication editor(argc, argv, context);

    QFile file(":/qdarkstyle/style.qss");
    if (file.open(QFile::ReadOnly | QFile::Text))
        editor.setStyleSheet(QLatin1String(file.readAll()));

    return editor.Run();
}

DEFINE_MAIN(Main());
