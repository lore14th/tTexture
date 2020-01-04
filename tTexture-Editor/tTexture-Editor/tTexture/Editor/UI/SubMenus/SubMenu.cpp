#include "pch.h"
#include "SubMenu.h"

namespace tTexture::Ui {

	SubMenu::SubMenu(QAction* backAction, QWidget* parent)
		: m_ParentWidget(parent), m_BackAction(backAction)
	{
	}

	void SubMenu::BackToMainMenu() const
	{
		// Return to main menu, by triggering the action
		if (m_BackAction)
		{
			ResetControllerData();
			ResetUi();

			m_BackAction->trigger();
		}
	}

}
