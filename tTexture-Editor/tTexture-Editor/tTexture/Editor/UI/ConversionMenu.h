#pragma once

#include <QDialog>
#include "ui_ConversionMenu.h"

namespace tTexture {

	class ConversionMenuUi : public QDialog
	{
	Q_OBJECT
	public:
		ConversionMenuUi(QWidget* parent = Q_NULLPTR);

	private:
		Ui::ConversionMenu m_Ui;
	};

}
