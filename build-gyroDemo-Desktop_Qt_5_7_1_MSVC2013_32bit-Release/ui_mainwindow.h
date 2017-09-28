/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QFrame *frame;
    QGridLayout *gridLayout;
    QPushButton *pushButtonStart;
    QLabel *labelFreq;
    QSlider *horizontalSliderScaleValue;
    QPushButton *pushButtonIntegration;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1000, 500);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        frame = new QFrame(centralWidget);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(0, 0, 231, 111));
        frame->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(frame);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        pushButtonStart = new QPushButton(frame);
        pushButtonStart->setObjectName(QStringLiteral("pushButtonStart"));

        gridLayout->addWidget(pushButtonStart, 0, 0, 1, 1);

        labelFreq = new QLabel(frame);
        labelFreq->setObjectName(QStringLiteral("labelFreq"));
        labelFreq->setStyleSheet(QStringLiteral(""));

        gridLayout->addWidget(labelFreq, 1, 0, 1, 1);

        horizontalSliderScaleValue = new QSlider(frame);
        horizontalSliderScaleValue->setObjectName(QStringLiteral("horizontalSliderScaleValue"));
        horizontalSliderScaleValue->setMinimum(1);
        horizontalSliderScaleValue->setMaximum(100);
        horizontalSliderScaleValue->setPageStep(2);
        horizontalSliderScaleValue->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(horizontalSliderScaleValue, 0, 1, 1, 1);

        pushButtonIntegration = new QPushButton(frame);
        pushButtonIntegration->setObjectName(QStringLiteral("pushButtonIntegration"));
        pushButtonIntegration->setCheckable(true);
        pushButtonIntegration->setChecked(true);

        gridLayout->addWidget(pushButtonIntegration, 1, 1, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1000, 21));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        pushButtonStart->setText(QApplication::translate("MainWindow", "Start", Q_NULLPTR));
        labelFreq->setText(QApplication::translate("MainWindow", "Tan so do:", Q_NULLPTR));
        pushButtonIntegration->setText(QApplication::translate("MainWindow", "Integration", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
