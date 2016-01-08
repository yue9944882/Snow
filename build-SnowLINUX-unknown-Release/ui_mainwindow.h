/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGraphicsView>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QScrollArea>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QToolBar>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QFrame *ButtonFrame;
    QToolButton *newButton;
    QToolButton *contButton;
    QToolButton *pauseButton;
    QToolButton *deleteButton;
    QToolButton *restButton;
    QFrame *WavePlot;
    QGraphicsView *graphicsView;
    QLabel *speedLabel;
    QTabWidget *InfoTags;
    QWidget *BasictTag;
    QLabel *labelTN;
    QLabel *labelPath;
    QWidget *ThreadTag;
    QFrame *frame;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(765, 478);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        ButtonFrame = new QFrame(centralWidget);
        ButtonFrame->setObjectName(QString::fromUtf8("ButtonFrame"));
        ButtonFrame->setGeometry(QRect(10, 0, 741, 81));
        ButtonFrame->setFrameShape(QFrame::StyledPanel);
        ButtonFrame->setFrameShadow(QFrame::Raised);
        newButton = new QToolButton(ButtonFrame);
        newButton->setObjectName(QString::fromUtf8("newButton"));
        newButton->setGeometry(QRect(30, 8, 81, 61));
        newButton->setIconSize(QSize(40, 40));
        contButton = new QToolButton(ButtonFrame);
        contButton->setObjectName(QString::fromUtf8("contButton"));
        contButton->setGeometry(QRect(140, 10, 81, 61));
        contButton->setIconSize(QSize(40, 40));
        pauseButton = new QToolButton(ButtonFrame);
        pauseButton->setObjectName(QString::fromUtf8("pauseButton"));
        pauseButton->setGeometry(QRect(250, 10, 81, 61));
        pauseButton->setIconSize(QSize(40, 40));
        deleteButton = new QToolButton(ButtonFrame);
        deleteButton->setObjectName(QString::fromUtf8("deleteButton"));
        deleteButton->setGeometry(QRect(360, 10, 81, 61));
        deleteButton->setIconSize(QSize(40, 40));
        restButton = new QToolButton(ButtonFrame);
        restButton->setObjectName(QString::fromUtf8("restButton"));
        restButton->setGeometry(QRect(470, 10, 81, 61));
        restButton->setIconSize(QSize(40, 40));
        WavePlot = new QFrame(centralWidget);
        WavePlot->setObjectName(QString::fromUtf8("WavePlot"));
        WavePlot->setGeometry(QRect(10, 90, 171, 131));
        WavePlot->setFrameShape(QFrame::StyledPanel);
        WavePlot->setFrameShadow(QFrame::Raised);
        graphicsView = new QGraphicsView(WavePlot);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setGeometry(QRect(10, 10, 150, 110));
        speedLabel = new QLabel(WavePlot);
        speedLabel->setObjectName(QString::fromUtf8("speedLabel"));
        speedLabel->setGeometry(QRect(10, 10, 71, 21));
        speedLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        InfoTags = new QTabWidget(centralWidget);
        InfoTags->setObjectName(QString::fromUtf8("InfoTags"));
        InfoTags->setGeometry(QRect(10, 239, 161, 171));
        BasictTag = new QWidget();
        BasictTag->setObjectName(QString::fromUtf8("BasictTag"));
        labelTN = new QLabel(BasictTag);
        labelTN->setObjectName(QString::fromUtf8("labelTN"));
        labelTN->setGeometry(QRect(10, 10, 121, 31));
        labelPath = new QLabel(BasictTag);
        labelPath->setObjectName(QString::fromUtf8("labelPath"));
        labelPath->setGeometry(QRect(10, 40, 121, 81));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        labelPath->setFont(font);
        InfoTags->addTab(BasictTag, QString());
        ThreadTag = new QWidget();
        ThreadTag->setObjectName(QString::fromUtf8("ThreadTag"));
        InfoTags->addTab(ThreadTag, QString());
        frame = new QFrame(centralWidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(190, 90, 561, 321));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        scrollArea = new QScrollArea(frame);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setGeometry(QRect(0, 0, 561, 321));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 559, 319));
        scrollArea->setWidget(scrollAreaWidgetContents);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 765, 27));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        InfoTags->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        newButton->setText(QApplication::translate("MainWindow", "New", 0, QApplication::UnicodeUTF8));
        contButton->setText(QApplication::translate("MainWindow", "Continue", 0, QApplication::UnicodeUTF8));
        pauseButton->setText(QApplication::translate("MainWindow", "Pause", 0, QApplication::UnicodeUTF8));
        deleteButton->setText(QApplication::translate("MainWindow", "Delete", 0, QApplication::UnicodeUTF8));
        restButton->setText(QApplication::translate("MainWindow", "Restart", 0, QApplication::UnicodeUTF8));
        speedLabel->setText(QApplication::translate("MainWindow", "0KB/s", 0, QApplication::UnicodeUTF8));
        labelTN->setText(QString());
        labelPath->setText(QString());
        InfoTags->setTabText(InfoTags->indexOf(BasictTag), QApplication::translate("MainWindow", "Basics", 0, QApplication::UnicodeUTF8));
        InfoTags->setTabText(InfoTags->indexOf(ThreadTag), QApplication::translate("MainWindow", "Threads", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
