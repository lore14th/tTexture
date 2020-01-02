#pragma once

#include <QDialog>

#include "ui_CreateMenu.h"

namespace tTexture {

	class CreateMenuUi : public QDialog
	{
	Q_OBJECT
	public:
		CreateMenuUi(QWidget* parent = Q_NULLPTR);

	private:
		Ui::CreateMenu m_Ui;
	};

}
