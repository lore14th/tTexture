#pragma once

#include <QWidget>
#include "ui_CreateWidget.h"

namespace tTexture::Ui {

	class CreateMenuUi : public QWidget
	{
	Q_OBJECT
	public:
		CreateMenuUi(QWidget* parent = Q_NULLPTR);

	private:
		::Ui::CreateWidget m_Ui;
	};
}
