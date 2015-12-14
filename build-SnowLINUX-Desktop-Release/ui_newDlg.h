/********************************************************************************
** Form generated from reading UI file 'newDlg.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWDLG_H
#define UI_NEWDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_NewDialog
{
public:
    QPushButton *beginButton;
    QPushButton *cancelButton;
    QFrame *frame;
    QSpinBox *spinBox;
    QSlider *horizontalSlider;
    QLabel *staticLabel;
    QLineEdit *urlEdit;
    QLineEdit *pathEdit;
    QLabel *label;
    QLabel *label_2;
    QPushButton *pathButton;

    void setupUi(QDialog *NewDialog)
    {
        if (NewDialog->objectName().isEmpty())
            NewDialog->setObjectName(QString::fromUtf8("NewDialog"));
        NewDialog->resize(400, 300);
        beginButton = new QPushButton(NewDialog);
        beginButton->setObjectName(QString::fromUtf8("beginButton"));
        beginButton->setGeometry(QRect(46, 250, 121, 31));
        cancelButton = new QPushButton(NewDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setGeometry(QRect(240, 250, 111, 31));
        frame = new QFrame(NewDialog);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(30, 139, 341, 91));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        spinBox = new QSpinBox(frame);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setGeometry(QRect(30, 40, 60, 33));
        horizontalSlider = new QSlider(frame);
        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setGeometry(QRect(130, 50, 181, 19));
        horizontalSlider->setOrientation(Qt::Horizontal);
        staticLabel = new QLabel(frame);
        staticLabel->setObjectName(QString::fromUtf8("staticLabel"));
        staticLabel->setGeometry(QRect(30, 10, 151, 21));
        urlEdit = new QLineEdit(NewDialog);
        urlEdit->setObjectName(QString::fromUtf8("urlEdit"));
        urlEdit->setGeometry(QRect(100, 30, 261, 33));
        pathEdit = new QLineEdit(NewDialog);
        pathEdit->setObjectName(QString::fromUtf8("pathEdit"));
        pathEdit->setGeometry(QRect(100, 80, 221, 33));
        label = new QLabel(NewDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 30, 81, 31));
        label_2 = new QLabel(NewDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 90, 81, 21));
        pathButton = new QPushButton(NewDialog);
        pathButton->setObjectName(QString::fromUtf8("pathButton"));
        pathButton->setGeometry(QRect(330, 80, 41, 31));

        retranslateUi(NewDialog);

        QMetaObject::connectSlotsByName(NewDialog);
    } // setupUi

    void retranslateUi(QDialog *NewDialog)
    {
        NewDialog->setWindowTitle(QApplication::translate("NewDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        beginButton->setText(QApplication::translate("NewDialog", "Begin Download", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("NewDialog", "Cancel", 0, QApplication::UnicodeUTF8));
        staticLabel->setText(QApplication::translate("NewDialog", "Number of Threads", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("NewDialog", "Target URL", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("NewDialog", "Target Path", 0, QApplication::UnicodeUTF8));
        pathButton->setText(QApplication::translate("NewDialog", "...", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class NewDialog: public Ui_NewDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWDLG_H
