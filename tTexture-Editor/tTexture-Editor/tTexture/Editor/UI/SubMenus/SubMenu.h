#pragma once

#include <QWidget>
#include <QAction>

namespace tTexture::Ui {

	class SubMenu : public QWidget
	{
	public:
		SubMenu(QAction* backAction, QWidget* parent = Q_NULLPTR);
		virtual ~SubMenu() = default;

		void BackToMainMenu() const;

		virtual void ResetUi() const = 0;
		virtual void ResetControllerData() const = 0;
	private:
		QWidget* m_ParentWidget;
		QAction* m_BackAction;
	};

}