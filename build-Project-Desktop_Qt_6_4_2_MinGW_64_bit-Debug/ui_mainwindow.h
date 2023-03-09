/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGraphicsView *antMindView;
    QPushButton *StepButton;
    QPushButton *PlayButton;
    QPushButton *PauseButton;
    QGraphicsView *statisticsView;
    QLabel *TurnBar;
    QLabel *PercentageBar;
    QPushButton *ResetButton;
    QLabel *AliveBar;
    QLabel *Slow;
    QLabel *Fast;
    QLabel *CurrentSpeed;
    QSlider *horizontalSlider;
    QGraphicsView *antArmyView;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(971, 973);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        antMindView = new QGraphicsView(centralwidget);
        antMindView->setObjectName("antMindView");
        antMindView->setGeometry(QRect(90, 70, 600, 311));
        StepButton = new QPushButton(centralwidget);
        StepButton->setObjectName("StepButton");
        StepButton->setGeometry(QRect(30, 400, 99, 28));
        PlayButton = new QPushButton(centralwidget);
        PlayButton->setObjectName("PlayButton");
        PlayButton->setGeometry(QRect(30, 440, 99, 28));
        PauseButton = new QPushButton(centralwidget);
        PauseButton->setObjectName("PauseButton");
        PauseButton->setGeometry(QRect(30, 480, 99, 28));
        statisticsView = new QGraphicsView(centralwidget);
        statisticsView->setObjectName("statisticsView");
        statisticsView->setGeometry(QRect(150, 400, 440, 100));
        statisticsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        statisticsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        TurnBar = new QLabel(centralwidget);
        TurnBar->setObjectName("TurnBar");
        TurnBar->setGeometry(QRect(30, 10, 291, 18));
        PercentageBar = new QLabel(centralwidget);
        PercentageBar->setObjectName("PercentageBar");
        PercentageBar->setGeometry(QRect(30, 40, 321, 18));
        ResetButton = new QPushButton(centralwidget);
        ResetButton->setObjectName("ResetButton");
        ResetButton->setGeometry(QRect(630, 420, 99, 28));
        AliveBar = new QLabel(centralwidget);
        AliveBar->setObjectName("AliveBar");
        AliveBar->setGeometry(QRect(290, 10, 241, 18));
        Slow = new QLabel(centralwidget);
        Slow->setObjectName("Slow");
        Slow->setGeometry(QRect(230, 510, 41, 18));
        Fast = new QLabel(centralwidget);
        Fast->setObjectName("Fast");
        Fast->setGeometry(QRect(450, 510, 81, 18));
        CurrentSpeed = new QLabel(centralwidget);
        CurrentSpeed->setObjectName("CurrentSpeed");
        CurrentSpeed->setGeometry(QRect(250, 530, 251, 18));
        horizontalSlider = new QSlider(centralwidget);
        horizontalSlider->setObjectName("horizontalSlider");
        horizontalSlider->setGeometry(QRect(280, 510, 160, 22));
        horizontalSlider->setOrientation(Qt::Horizontal);
        antArmyView = new QGraphicsView(centralwidget);
        antArmyView->setObjectName("antArmyView");
        antArmyView->setGeometry(QRect(90, 550, 600, 311));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 971, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        StepButton->setText(QCoreApplication::translate("MainWindow", "Step", nullptr));
        PlayButton->setText(QCoreApplication::translate("MainWindow", "Play", nullptr));
        PauseButton->setText(QCoreApplication::translate("MainWindow", "Pause", nullptr));
        TurnBar->setText(QCoreApplication::translate("MainWindow", "Turn: 0", nullptr));
        PercentageBar->setText(QCoreApplication::translate("MainWindow", "Population: ", nullptr));
        ResetButton->setText(QCoreApplication::translate("MainWindow", "Reset", nullptr));
        AliveBar->setText(QCoreApplication::translate("MainWindow", "Alive > 2:", nullptr));
        Slow->setText(QCoreApplication::translate("MainWindow", "Slow", nullptr));
        Fast->setText(QCoreApplication::translate("MainWindow", "Fast", nullptr));
        CurrentSpeed->setText(QCoreApplication::translate("MainWindow", "Current Speed: 500 ms/turn", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
