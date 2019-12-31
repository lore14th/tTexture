#include "pch.h"
#include "EditorWindow.h"

#include <QMessageBox>

namespace tTexture {

	// TODO: temp, add support for 1-2 channels textures
	uint32_t InputIndexToImageChannels(int32_t index) 
	{
		if (index == 0)
			return 3;
		else 
			return 4;
	}

	EditorWindow::EditorWindow(QWidget* parent)
		: QMainWindow(parent), m_Converter(std::make_unique<Converter>()), m_FileDialog(std::make_unique<QFileDialog>(this))
	{
		m_Ui.setupUi(this);
		ResetUi();
	}

	void EditorWindow::Reset() const
	{
		ResetUi();
		m_Converter->ResetConversionData();
	}

	void EditorWindow::ResetUi() const
	{
		UpdateUiLabel(m_Ui.InputFilepathValue, "");
		UpdateUiLabel(m_Ui.OutputFilepathValue, "");
		UpdateUiLabel(m_Ui.ExportValue, "");

		UpdateButtonColor(m_Ui.Texture2DButton, QColor(Qt::red));
		UpdateButtonColor(m_Ui.HCrossButton, QColor(Qt::blue));
		UpdateButtonColor(m_Ui.VCrossButton, QColor(Qt::blue));
		UpdateButtonColor(m_Ui.EquirectangularButton, QColor(Qt::blue));

		m_Ui.InputChannelBox->setCurrentIndex(0);
	}

	void EditorWindow::UpdateConversionType(InputFlag newFlag) const
	{
		// reset the color of the current conversion type
		UpdateButtonColor(GetConversionTypeButton(m_Converter->GetData()->InputType), QColor(Qt::blue));
		// set the color of the selected conversion type
		UpdateButtonColor(GetConversionTypeButton(newFlag), QColor(Qt::red));
		// updates the converter data
		m_Converter->GetData()->InputType = newFlag;
	}

	QPushButton* EditorWindow::GetConversionTypeButton(InputFlag flag) const
	{
		switch (flag)
		{
			case tTexture::InputFlag::Texture2D:		return m_Ui.Texture2DButton;
			case tTexture::InputFlag::H_Cross:			return m_Ui.HCrossButton;
			case tTexture::InputFlag::V_Cross:			return m_Ui.VCrossButton;
			case tTexture::InputFlag::Equirectangular:	return m_Ui.EquirectangularButton;
		}
		return nullptr;
	}

	void EditorWindow::UpdateButtonColor(QPushButton* button, QColor color) const
	{
		if (button != nullptr)
		{
			QPalette pal = button->palette();
			pal.setColor(QPalette::Button, color);
			button->setAutoFillBackground(true);
			button->setPalette(pal);
			button->update();
		}
		else 
			TTEX_WARN("QPushButton is nullptr");
	}
	
	void EditorWindow::UpdateUiLabel(QLabel* label, const QString& message) const
	{
		label->setText(message);
	}

	void EditorWindow::ShowMessageBox(const char* message) const
	{
		QMessageBox* messageBox = new QMessageBox();
		messageBox->setText(message); 
		messageBox->show();
	}

	// -- Ui Event handlers ---------------------------------------------

	void EditorWindow::on_Texture2DButton_clicked()
	{
		UpdateConversionType(InputFlag::Texture2D);
	}

	void EditorWindow::on_HCrossButton_clicked()
	{
		UpdateConversionType(InputFlag::H_Cross);
	}

	void EditorWindow::on_VCrossButton_clicked()
	{
		UpdateConversionType(InputFlag::V_Cross);
	}

	void EditorWindow::on_EquirectangularButton_clicked()
	{
		UpdateConversionType(InputFlag::Equirectangular);
	}

	void EditorWindow::on_InputFilepathButton_clicked()
	{
		// open file dialog
		QString filepath = m_FileDialog->getOpenFileName(this);

		// we need to check that the filepath is not empty to avoid errors
		if (!filepath.isEmpty())
		{
			// show the filepath to the ui
			m_Ui.InputFilepathValue->setText(filepath);
			// update the converter data
			m_Converter->GetData()->InputFilepath = filepath.toStdString();

			//m_Converter->SetInputFilepath(filepath.toStdString());
		}
	}

	void EditorWindow::on_InputChannelBox_currentIndexChanged()
	{
		// update the converter data
		// TODO: when one and two channel textures are supported, we'll change this to pass the inputBoxIndex directly
		m_Converter->GetData()->InputChannels = InputIndexToImageChannels(m_Ui.InputChannelBox->currentIndex());
	}

	void EditorWindow::on_FlipOnLoadCheckbox_stateChanged()
	{
		// update the converter data
		m_Converter->GetData()->FlipOnLoad = m_Ui.FlipOnLoadCheckbox->isChecked();
	}

	void EditorWindow::on_OutputFilepathButton_clicked()
	{
		// Open a window to enter the output filepath
		QString filepath = m_FileDialog->getSaveFileName(this);

		// we need to check that the filepath is not empty to avoid error in case the user doesn't enter a filepath
		if (!filepath.isEmpty())
		{
			// show the output filepath to the ui
			UpdateUiLabel(m_Ui.OutputFilepathValue, filepath);
			UpdateUiLabel(m_Ui.ExportValue, filepath);

			// update the converter data
			m_Converter->GetData()->OutputFilepath = filepath.toStdString();
		}
	}

	void EditorWindow::on_PrefilterCheckbox_stateChanged()
	{
		// update the converter data
		m_Converter->GetData()->Prefilter = m_Ui.PrefilterCheckbox->isChecked();
	}

	void EditorWindow::on_ButtonBox_accepted()
	{
		// activates the Converter
		std::string conversionMessage = m_Converter->PerformConversion();
		// shows the conversion message to the screen
		ShowMessageBox(conversionMessage.c_str());

		Reset();
	}

	void EditorWindow::on_ButtonBox_rejected()
	{
		Reset();
	}

}
