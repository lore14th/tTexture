#pragma once

#include <QDialog>

#include "ui_CubemapMenuui.h"

namespace tTexture {

	class CubemapMenuUi : public QDialog
	{
	Q_OBJECT
	public:
		CubemapMenuUi(QWidget* parent = Q_NULLPTR);

	private:
		Ui::CubemapMenu m_Ui;
	};

}
