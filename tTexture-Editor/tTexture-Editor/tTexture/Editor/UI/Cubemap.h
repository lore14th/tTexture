#pragma once

#include <QWidget>
#include "ui_CubemapWidget.h"

namespace tTexture::Ui {

	class CubemapMenuUi : public QWidget
	{
	Q_OBJECT
	public:
		CubemapMenuUi(QWidget* parent = Q_NULLPTR);

	private:
		::Ui::CubemapWidget m_Ui;
	};
}
