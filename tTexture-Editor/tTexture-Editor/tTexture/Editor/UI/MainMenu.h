#pragma once

#include <QMainWindow>
#include "ui_MainWindow.h"

#include "Editor/Core-Editor/EditorApplication.h"

namespace tTexture::Ui {

#define MAIN_MENU		0
#define CONVERT_MENU	1
#define CUBEMAP_MENU	2
#define CREATE_MENU		3

	class MainMenuUi : public QMainWindow
	{
	Q_OBJECT
	public:
		MainMenuUi(QWidget* parent = Q_NULLPTR);

		void ShowMainMenu() const;
	private:
		void SwapWidget(uint32_t widgetIndex) const;

	private slots: // Ui signals
		void on_ConvertButton_clicked();
		void on_CubemapButton_clicked();
		void on_CreateButton_clicked();

	private:
		::Ui::MainWindow m_Ui;
		std::shared_ptr<::tTexture::EditorApplication> m_Application;
	};

}
