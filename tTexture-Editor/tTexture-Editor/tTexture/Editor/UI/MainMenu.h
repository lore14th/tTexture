#pragma once

#include "Editor/Core-Editor/EditorApplication.h"

#include "Editor/UI/Convert.h"
#include "Editor/UI/Cubemap.h"
#include "Editor/UI/Create.h"

#include <QMainWindow>
#include "ui_MainWindow.h"

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

		std::unique_ptr<ConvertMenuUi> m_ConvertWidget;
		std::unique_ptr<CubemapMenuUi> m_CubemapWidget;
		std::unique_ptr<CreateMenuUi> m_CreateWidget;

		std::shared_ptr<::tTexture::EditorApplication> m_Application;
	};

}
