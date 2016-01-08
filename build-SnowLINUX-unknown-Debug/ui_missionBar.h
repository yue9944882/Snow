/********************************************************************************
** Form generated from reading UI file 'missionBar.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MISSIONBAR_H
#define UI_MISSIONBAR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QProgressBar>

QT_BEGIN_NAMESPACE

class Ui_MissionBar
{
public:
    QProgressBar *progressBar;
    QLabel *label;
    QLabel *timeBar;
    QLabel *nameBar;
    QLabel *sizeBar;

    void setupUi(QDialog *MissionBar)
    {
        if (MissionBar->objectName().isEmpty())
            MissionBar->setObjectName(QString::fromUtf8("MissionBar"));
        MissionBar->setWindowModality(Qt::NonModal);
        MissionBar->resize(540, 35);
        progressBar = new QProgressBar(MissionBar);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(160, 10, 211, 23));
        progressBar->setValue(24);
        label = new QLabel(MissionBar);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(380, 10, 91, 21));
        timeBar = new QLabel(MissionBar);
        timeBar->setObjectName(QString::fromUtf8("timeBar"));
        timeBar->setGeometry(QRect(470, 10, 66, 21));
        nameBar = new QLabel(MissionBar);
        nameBar->setObjectName(QString::fromUtf8("nameBar"));
        nameBar->setGeometry(QRect(10, 10, 71, 21));
        sizeBar = new QLabel(MissionBar);
        sizeBar->setObjectName(QString::fromUtf8("sizeBar"));
        sizeBar->setGeometry(QRect(80, 10, 66, 21));
        sizeBar->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        retranslateUi(MissionBar);

        QMetaObject::connectSlotsByName(MissionBar);
    } // setupUi

    void retranslateUi(QDialog *MissionBar)
    {
        MissionBar->setWindowTitle(QApplication::translate("MissionBar", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MissionBar", "Time to Wait:", 0, QApplication::UnicodeUTF8));
        timeBar->setText(QApplication::translate("MissionBar", "00:00:00", 0, QApplication::UnicodeUTF8));
        nameBar->setText(QApplication::translate("MissionBar", "Mission", 0, QApplication::UnicodeUTF8));
        sizeBar->setText(QApplication::translate("MissionBar", "0kb", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MissionBar: public Ui_MissionBar {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MISSIONBAR_H
