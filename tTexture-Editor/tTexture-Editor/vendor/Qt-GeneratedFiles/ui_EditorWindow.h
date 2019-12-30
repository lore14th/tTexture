/********************************************************************************
** Form generated from reading UI file 'EditorWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EDITORWINDOW_H
#define UI_EDITORWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EditorWindow
{
public:
    QWidget *MainWindow;
    QWidget *widget;
    QVBoxLayout *MainWindowLayout;
    QHBoxLayout *ConversionTypeSectionLayout;
    QLabel *ConversionTypeLabel;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *ConversionTypeLayout;
    QVBoxLayout *Texture2DLayout;
    QPushButton *Texture2DButton;
    QLabel *Texture2DLabel;
    QVBoxLayout *HCrossLayout;
    QPushButton *HCrossButton;
    QLabel *HCrossLabel;
    QVBoxLayout *VCrossLayout;
    QPushButton *VCrossButton;
    QLabel *VCrossLabel;
    QVBoxLayout *EquirectangularLayout;
    QPushButton *EquirectangularButton;
    QLabel *EquirectangularLabel;
    QHBoxLayout *horizontalLayout;
    QLabel *InputLabel;
    QVBoxLayout *InputLayout;
    QHBoxLayout *InputFilepathLayout;
    QLabel *InputFilepathLabel;
    QLabel *InputFilepathValue;
    QToolButton *InputFilepathButton;
    QHBoxLayout *InputChannelLayout;
    QLabel *InputChannelLabel;
    QComboBox *InputChannelBox;
    QHBoxLayout *FlipOnLoadLayout;
    QCheckBox *FlipOnLoadCheckbox;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *OutputSectionLayout;
    QLabel *OutputLabel;
    QVBoxLayout *OutputLayout;
    QHBoxLayout *OutputFilepathLayout;
    QLabel *OutputFilepathLabel;
    QLabel *OutputFilepathValue;
    QToolButton *OutputFilepathButton;
    QHBoxLayout *PrefilterLayout;
    QCheckBox *PrefilterCheckbox;
    QSpacerItem *PrefilterSpacer;
    QHBoxLayout *FooterLayout;
    QHBoxLayout *ExportLayout;
    QLabel *ExportLabel;
    QLabel *ExportValue;
    QDialogButtonBox *ButtonBox;
    QMenuBar *menubar;
    QMenu *menuFile;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *EditorWindow)
    {
        if (EditorWindow->objectName().isEmpty())
            EditorWindow->setObjectName(QStringLiteral("EditorWindow"));
        EditorWindow->resize(887, 462);
        MainWindow = new QWidget(EditorWindow);
        MainWindow->setObjectName(QStringLiteral("MainWindow"));
        widget = new QWidget(MainWindow);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(12, 12, 861, 401));
        MainWindowLayout = new QVBoxLayout(widget);
        MainWindowLayout->setObjectName(QStringLiteral("MainWindowLayout"));
        MainWindowLayout->setContentsMargins(0, 0, 0, 0);
        ConversionTypeSectionLayout = new QHBoxLayout();
        ConversionTypeSectionLayout->setObjectName(QStringLiteral("ConversionTypeSectionLayout"));
        ConversionTypeLabel = new QLabel(widget);
        ConversionTypeLabel->setObjectName(QStringLiteral("ConversionTypeLabel"));
        ConversionTypeLabel->setMinimumSize(QSize(100, 0));
        ConversionTypeLabel->setMaximumSize(QSize(100, 16777215));

        ConversionTypeSectionLayout->addWidget(ConversionTypeLabel);

        horizontalSpacer = new QSpacerItem(150, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        ConversionTypeSectionLayout->addItem(horizontalSpacer);

        ConversionTypeLayout = new QHBoxLayout();
        ConversionTypeLayout->setObjectName(QStringLiteral("ConversionTypeLayout"));
        Texture2DLayout = new QVBoxLayout();
        Texture2DLayout->setObjectName(QStringLiteral("Texture2DLayout"));
        Texture2DButton = new QPushButton(widget);
        Texture2DButton->setObjectName(QStringLiteral("Texture2DButton"));

        Texture2DLayout->addWidget(Texture2DButton);

        Texture2DLabel = new QLabel(widget);
        Texture2DLabel->setObjectName(QStringLiteral("Texture2DLabel"));
        Texture2DLabel->setMaximumSize(QSize(100, 30));

        Texture2DLayout->addWidget(Texture2DLabel);


        ConversionTypeLayout->addLayout(Texture2DLayout);

        HCrossLayout = new QVBoxLayout();
        HCrossLayout->setObjectName(QStringLiteral("HCrossLayout"));
        HCrossButton = new QPushButton(widget);
        HCrossButton->setObjectName(QStringLiteral("HCrossButton"));

        HCrossLayout->addWidget(HCrossButton);

        HCrossLabel = new QLabel(widget);
        HCrossLabel->setObjectName(QStringLiteral("HCrossLabel"));
        HCrossLabel->setMaximumSize(QSize(100, 30));

        HCrossLayout->addWidget(HCrossLabel);


        ConversionTypeLayout->addLayout(HCrossLayout);

        VCrossLayout = new QVBoxLayout();
        VCrossLayout->setObjectName(QStringLiteral("VCrossLayout"));
        VCrossButton = new QPushButton(widget);
        VCrossButton->setObjectName(QStringLiteral("VCrossButton"));

        VCrossLayout->addWidget(VCrossButton);

        VCrossLabel = new QLabel(widget);
        VCrossLabel->setObjectName(QStringLiteral("VCrossLabel"));
        VCrossLabel->setMaximumSize(QSize(100, 30));

        VCrossLayout->addWidget(VCrossLabel);


        ConversionTypeLayout->addLayout(VCrossLayout);

        EquirectangularLayout = new QVBoxLayout();
        EquirectangularLayout->setObjectName(QStringLiteral("EquirectangularLayout"));
        EquirectangularButton = new QPushButton(widget);
        EquirectangularButton->setObjectName(QStringLiteral("EquirectangularButton"));

        EquirectangularLayout->addWidget(EquirectangularButton);

        EquirectangularLabel = new QLabel(widget);
        EquirectangularLabel->setObjectName(QStringLiteral("EquirectangularLabel"));
        EquirectangularLabel->setMaximumSize(QSize(100, 30));

        EquirectangularLayout->addWidget(EquirectangularLabel);


        ConversionTypeLayout->addLayout(EquirectangularLayout);


        ConversionTypeSectionLayout->addLayout(ConversionTypeLayout);


        MainWindowLayout->addLayout(ConversionTypeSectionLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        InputLabel = new QLabel(widget);
        InputLabel->setObjectName(QStringLiteral("InputLabel"));
        InputLabel->setMinimumSize(QSize(100, 0));
        InputLabel->setMaximumSize(QSize(100, 16777215));

        horizontalLayout->addWidget(InputLabel);

        InputLayout = new QVBoxLayout();
        InputLayout->setObjectName(QStringLiteral("InputLayout"));
        InputFilepathLayout = new QHBoxLayout();
        InputFilepathLayout->setObjectName(QStringLiteral("InputFilepathLayout"));
        InputFilepathLabel = new QLabel(widget);
        InputFilepathLabel->setObjectName(QStringLiteral("InputFilepathLabel"));
        InputFilepathLabel->setMaximumSize(QSize(100, 16777215));

        InputFilepathLayout->addWidget(InputFilepathLabel);

        InputFilepathValue = new QLabel(widget);
        InputFilepathValue->setObjectName(QStringLiteral("InputFilepathValue"));

        InputFilepathLayout->addWidget(InputFilepathValue);

        InputFilepathButton = new QToolButton(widget);
        InputFilepathButton->setObjectName(QStringLiteral("InputFilepathButton"));

        InputFilepathLayout->addWidget(InputFilepathButton);


        InputLayout->addLayout(InputFilepathLayout);

        InputChannelLayout = new QHBoxLayout();
        InputChannelLayout->setObjectName(QStringLiteral("InputChannelLayout"));
        InputChannelLabel = new QLabel(widget);
        InputChannelLabel->setObjectName(QStringLiteral("InputChannelLabel"));
        InputChannelLabel->setMaximumSize(QSize(100, 16777215));

        InputChannelLayout->addWidget(InputChannelLabel);

        InputChannelBox = new QComboBox(widget);
        InputChannelBox->setObjectName(QStringLiteral("InputChannelBox"));
        InputChannelBox->setEditable(false);

        InputChannelLayout->addWidget(InputChannelBox);


        InputLayout->addLayout(InputChannelLayout);

        FlipOnLoadLayout = new QHBoxLayout();
        FlipOnLoadLayout->setObjectName(QStringLiteral("FlipOnLoadLayout"));
        FlipOnLoadCheckbox = new QCheckBox(widget);
        FlipOnLoadCheckbox->setObjectName(QStringLiteral("FlipOnLoadCheckbox"));
        FlipOnLoadCheckbox->setMinimumSize(QSize(20, 0));
        FlipOnLoadCheckbox->setMaximumSize(QSize(1677215, 16777215));
        FlipOnLoadCheckbox->setLayoutDirection(Qt::RightToLeft);

        FlipOnLoadLayout->addWidget(FlipOnLoadCheckbox);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        FlipOnLoadLayout->addItem(horizontalSpacer_2);


        InputLayout->addLayout(FlipOnLoadLayout);


        horizontalLayout->addLayout(InputLayout);


        MainWindowLayout->addLayout(horizontalLayout);

        OutputSectionLayout = new QHBoxLayout();
        OutputSectionLayout->setObjectName(QStringLiteral("OutputSectionLayout"));
        OutputLabel = new QLabel(widget);
        OutputLabel->setObjectName(QStringLiteral("OutputLabel"));
        OutputLabel->setMinimumSize(QSize(100, 0));
        OutputLabel->setMaximumSize(QSize(100, 16777215));
        OutputLabel->setBaseSize(QSize(0, 0));

        OutputSectionLayout->addWidget(OutputLabel);

        OutputLayout = new QVBoxLayout();
        OutputLayout->setObjectName(QStringLiteral("OutputLayout"));
        OutputFilepathLayout = new QHBoxLayout();
        OutputFilepathLayout->setObjectName(QStringLiteral("OutputFilepathLayout"));
        OutputFilepathLabel = new QLabel(widget);
        OutputFilepathLabel->setObjectName(QStringLiteral("OutputFilepathLabel"));
        OutputFilepathLabel->setMaximumSize(QSize(100, 16777215));

        OutputFilepathLayout->addWidget(OutputFilepathLabel);

        OutputFilepathValue = new QLabel(widget);
        OutputFilepathValue->setObjectName(QStringLiteral("OutputFilepathValue"));

        OutputFilepathLayout->addWidget(OutputFilepathValue);

        OutputFilepathButton = new QToolButton(widget);
        OutputFilepathButton->setObjectName(QStringLiteral("OutputFilepathButton"));

        OutputFilepathLayout->addWidget(OutputFilepathButton);


        OutputLayout->addLayout(OutputFilepathLayout);

        PrefilterLayout = new QHBoxLayout();
        PrefilterLayout->setObjectName(QStringLiteral("PrefilterLayout"));
        PrefilterCheckbox = new QCheckBox(widget);
        PrefilterCheckbox->setObjectName(QStringLiteral("PrefilterCheckbox"));
        PrefilterCheckbox->setMinimumSize(QSize(20, 0));
        PrefilterCheckbox->setMaximumSize(QSize(100, 16777215));
        PrefilterCheckbox->setLayoutDirection(Qt::RightToLeft);

        PrefilterLayout->addWidget(PrefilterCheckbox);

        PrefilterSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        PrefilterLayout->addItem(PrefilterSpacer);


        OutputLayout->addLayout(PrefilterLayout);


        OutputSectionLayout->addLayout(OutputLayout);


        MainWindowLayout->addLayout(OutputSectionLayout);

        FooterLayout = new QHBoxLayout();
        FooterLayout->setObjectName(QStringLiteral("FooterLayout"));
        ExportLayout = new QHBoxLayout();
        ExportLayout->setObjectName(QStringLiteral("ExportLayout"));
        ExportLabel = new QLabel(widget);
        ExportLabel->setObjectName(QStringLiteral("ExportLabel"));
        ExportLabel->setMinimumSize(QSize(100, 0));
        ExportLabel->setMaximumSize(QSize(100, 16777215));

        ExportLayout->addWidget(ExportLabel);

        ExportValue = new QLabel(widget);
        ExportValue->setObjectName(QStringLiteral("ExportValue"));

        ExportLayout->addWidget(ExportValue);


        FooterLayout->addLayout(ExportLayout);

        ButtonBox = new QDialogButtonBox(widget);
        ButtonBox->setObjectName(QStringLiteral("ButtonBox"));
        ButtonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        FooterLayout->addWidget(ButtonBox);


        MainWindowLayout->addLayout(FooterLayout);

        EditorWindow->setCentralWidget(MainWindow);
        menubar = new QMenuBar(EditorWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 887, 26));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        EditorWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(EditorWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        EditorWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());

        retranslateUi(EditorWindow);

        QMetaObject::connectSlotsByName(EditorWindow);
    } // setupUi

    void retranslateUi(QMainWindow *EditorWindow)
    {
        EditorWindow->setWindowTitle(QApplication::translate("EditorWindow", "tTexture", Q_NULLPTR));
        ConversionTypeLabel->setText(QApplication::translate("EditorWindow", "Conversion Type", Q_NULLPTR));
        Texture2DButton->setText(QApplication::translate("EditorWindow", "Texture2D", Q_NULLPTR));
        Texture2DLabel->setText(QApplication::translate("EditorWindow", "Texture 2D", Q_NULLPTR));
        HCrossButton->setText(QApplication::translate("EditorWindow", "H-Cross", Q_NULLPTR));
        HCrossLabel->setText(QApplication::translate("EditorWindow", "H-Cross", Q_NULLPTR));
        VCrossButton->setText(QApplication::translate("EditorWindow", "V-Cross", Q_NULLPTR));
        VCrossLabel->setText(QApplication::translate("EditorWindow", "V-Cross", Q_NULLPTR));
        EquirectangularButton->setText(QApplication::translate("EditorWindow", "Equirectangular", Q_NULLPTR));
        EquirectangularLabel->setText(QApplication::translate("EditorWindow", "Equirectangular", Q_NULLPTR));
        InputLabel->setText(QApplication::translate("EditorWindow", "Input", Q_NULLPTR));
        InputFilepathLabel->setText(QApplication::translate("EditorWindow", "Filepath", Q_NULLPTR));
        InputFilepathValue->setText(QString());
        InputFilepathButton->setText(QApplication::translate("EditorWindow", "...", Q_NULLPTR));
        InputChannelLabel->setText(QApplication::translate("EditorWindow", "Channels", Q_NULLPTR));
        InputChannelBox->clear();
        InputChannelBox->insertItems(0, QStringList()
         << QApplication::translate("EditorWindow", "3", Q_NULLPTR)
         << QApplication::translate("EditorWindow", "4", Q_NULLPTR)
        );
        FlipOnLoadCheckbox->setText(QApplication::translate("EditorWindow", "FlipOnLoad", Q_NULLPTR));
        OutputLabel->setText(QApplication::translate("EditorWindow", "Output", Q_NULLPTR));
        OutputFilepathLabel->setText(QApplication::translate("EditorWindow", "Filepath", Q_NULLPTR));
        OutputFilepathValue->setText(QString());
        OutputFilepathButton->setText(QApplication::translate("EditorWindow", "...", Q_NULLPTR));
        PrefilterCheckbox->setText(QApplication::translate("EditorWindow", "Prefilter", Q_NULLPTR));
        ExportLabel->setText(QApplication::translate("EditorWindow", "Export", Q_NULLPTR));
        ExportValue->setText(QString());
        menuFile->setTitle(QApplication::translate("EditorWindow", "File", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class EditorWindow: public Ui_EditorWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITORWINDOW_H
