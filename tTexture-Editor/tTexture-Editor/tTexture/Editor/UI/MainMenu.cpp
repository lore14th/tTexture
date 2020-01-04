#include "pch.h"
#include "MainMenu.h"

#include "Editor/UI/SubMenus/Convert.h"
#include "Editor/UI/SubMenus/Cubemap.h"
#include "Editor/UI/SubMenus/Create.h"

namespace tTexture::Ui {

	MainMenuUi::MainMenuUi(QWidget* parent)
		: m_Application(std::make_shared<::tTexture::EditorApplication>())
	{
		m_Ui.setupUi(this);

		// Set up action Menu to return to main menu window
		connect(m_Ui.actionMenu, &QAction::triggered, [=]() {
			ShowMainMenu();
		});

		m_Ui.stackedWidget->addWidget(new ConvertMenuUi(m_Application, m_Ui.actionMenu, this));
		m_Ui.stackedWidget->addWidget(new CubemapMenuUi(m_Application, m_Ui.actionMenu, this));
		m_Ui.stackedWidget->addWidget(new CreateMenuUi(m_Application, m_Ui.actionMenu, this));
	}

	void MainMenuUi::ShowMainMenu() const
	{
		SwapWidget(MAIN_MENU);
	}

	void MainMenuUi::SwapWidget(uint32_t widgetIndex) const
	{
		m_Ui.stackedWidget->setCurrentIndex(widgetIndex);
	}

	// -- Ui "events" -----------------------------------------
	void MainMenuUi::on_ConvertButton_clicked()
	{
		SwapWidget(CONVERT_MENU);
	}

	void MainMenuUi::on_CubemapButton_clicked()
	{
		SwapWidget(CUBEMAP_MENU);
	}

	void MainMenuUi::on_CreateButton_clicked()
	{
		SwapWidget(CREATE_MENU);
	}

}
