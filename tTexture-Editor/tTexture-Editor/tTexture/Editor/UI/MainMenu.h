#pragma once
#include <QMainWindow>

#include "Editor/UI/ConversionMenu.h"
#include "Editor/UI/CubemapMenu.h"
#include "Editor/UI/CreateMenu.h"

#include "ui_MainMenu.h"

namespace tTexture {

	class MainMenu : public QMainWindow
	{
	Q_OBJECT
	public:
		MainMenu(QWidget* parent = Q_NULLPTR);

	private slots:
		void on_ConvertButton_clicked();
		void on_CubemapButton_clicked();
		void on_CreateButton_clicked();

	private:
		Ui::MainWindow m_Ui;

		std::unique_ptr<ConversionMenuUi> m_ConversionMenu;
		std::unique_ptr<CubemapMenuUi> m_CubemapMenu;
		std::unique_ptr<CreateMenuUi> m_CreateMenu;
	};

}
