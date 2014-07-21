#include "stdafx.h"
#include "EditorApplication.h"
#include <qsplashscreen.h>

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   char*    lpCmdLine,
					   int       nCmdShow)
{
	Context* context = new Context();

	EditorApplication editorApp(1, &lpCmdLine,context);

	QSplashScreen* splash = new QSplashScreen();
	splash->setPixmap(QPixmap("Images/Splash.png"));
	splash->show();
	Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;
	splash->showMessage(QObject::tr("start editor..."),topRight,Qt::red);
	Sleep(1000);
	delete splash;

   return editorApp.Run();
}
