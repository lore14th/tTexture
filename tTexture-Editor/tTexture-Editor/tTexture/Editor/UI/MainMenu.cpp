#include "pch.h"
#include "MainMenu.h"

#include <QFrame>

namespace tTexture::Ui {

	MainMenuUi::MainMenuUi(QWidget* parent)
	{
		m_Ui.setupUi(this);

		// Set up action Menu to return to main menu window
		connect(m_Ui.actionMenu, &QAction::triggered, [=]() {
			ShowMainMenu();
		});

		// TODO: consider removing sub menus from member variables
		m_ConvertWidget = std::make_unique<ConvertMenuUi>(this);
		m_ConvertWidget->SetBackAction(m_Ui.actionMenu);
		m_Ui.stackedWidget->addWidget(m_ConvertWidget.get());

		m_CubemapWidget = std::make_unique<CubemapMenuUi>(this);
		m_Ui.stackedWidget->addWidget(m_CubemapWidget.get());

		m_CreateWidget = std::make_unique<CreateMenuUi>(this);
		m_Ui.stackedWidget->addWidget(m_CreateWidget.get());
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
