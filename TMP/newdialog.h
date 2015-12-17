#ifndef NEWDIALOG_H
#define NEWDIALOG_H

#include <QWidget>
#include <QString>
#include "ui_newDlg.h"
#include "mainwindow.h"


class NewDialog:public QDialog{

    Q_OBJECT

public slots:

    void slotChangePath();
    void slotChangeURL(QString);
    void slotChangePathS(QString);
    void slotChangeTN(int);//For Thread Num

public:

    NewDialog(MainWindow*);
    ~NewDialog();

    void changePath(QString);

private:
    Ui::NewDialog*ui;
    MainWindow*m_parent;
};



#endif // NEWDIALOG_H

