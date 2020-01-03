#include "pch.h"

#include <QApplication>
#include "Editor/UI/MainMenu.h"

int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	tTexture::Ui::MainMenuUi menu;
	menu.show();

	return app.exec();
}