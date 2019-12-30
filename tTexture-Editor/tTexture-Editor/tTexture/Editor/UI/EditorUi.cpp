#include "pch.h"

#include <QtWidgets/QApplication>
#include "Editor/UI/EditorWindow.h"

int main(int argc, char** argv)
{
	QApplication a(argc, argv);
	tTexture::EditorWindow w;
	w.show();
	return a.exec();
}