#include "pch.h"
#include "MainMenu.h"

#include <iostream>

namespace tTexture {

	MainMenu::MainMenu(QWidget* parent)
	{
		m_Ui.setupUi(this);

		m_ConversionMenu = std::make_unique<ConversionMenuUi>(this);
		m_ConversionMenu->setModal(true);

		m_CubemapMenu = std::make_unique<CubemapMenuUi>(this);
		m_CubemapMenu->setModal(true);

		m_CreateMenu = std::make_unique<CreateMenuUi>(this);
		m_CreateMenu->setModal(true);
	}

	void MainMenu::on_ConvertButton_clicked()
	{
		m_ConversionMenu->exec();
	}

	void MainMenu::on_CubemapButton_clicked()
	{
		m_CubemapMenu->exec();
	}

	void MainMenu::on_CreateButton_clicked()
	{
		m_CreateMenu->exec();
	}

}
