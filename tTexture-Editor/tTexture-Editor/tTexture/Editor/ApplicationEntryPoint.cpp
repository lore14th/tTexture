#include "pch.h"

#include <QApplication>

#include "Editor/UI/MainMenu.h"

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	QApplication app(__argc, __argv);
	tTexture::Ui::MainMenuUi menu;
	menu.show();
	return app.exec();
}