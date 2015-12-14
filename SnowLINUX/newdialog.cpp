#include"newdialog.h"
#include "missionbar.h"
#include "ui_mainwindow.h"
#include "global.h"
#include <QCheckBox>

NewDialog::NewDialog(MainWindow*parent):QDialog(parent),ui(new Ui::NewDialog){

    ui->setupUi(this);
    this->m_parent=parent;
    QObject::connect(ui->beginButton,SIGNAL(clicked(bool)),parent,SLOT(slotNewMissionBar()));
    QObject::connect(ui->beginButton,SIGNAL(clicked(bool)),this,SLOT(close()));
    QObject::connect(ui->cancelButton,SIGNAL(clicked(bool)),this,SLOT(close()));
    QObject::connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),ui->spinBox,SLOT(setValue(int)));
    QObject::connect(ui->spinBox,SIGNAL(valueChanged(int)),ui->horizontalSlider,SLOT(setValue(int)));
    QObject::connect(ui->pathButton,SIGNAL(clicked(bool)),parent,SLOT(slotSetNextMissionDir()));
    QObject::connect(ui->pathButton,SIGNAL(clicked(bool)),this,SLOT(slotChangePath()));
    //Check if the dir is correct before
    QObject::connect(ui->pathEdit,SIGNAL(textEdited(QString)),this,SLOT(slotChangePathS(QString)));
    QObject::connect(ui->urlEdit,SIGNAL(textEdited(QString)),this,SLOT(slotChangeURL(QString)));
    QObject::connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(slotChangeTN(int)));
    QObject::connect(ui->spinBox,SIGNAL(valueChanged(int)),this,SLOT(slotChangeTN(int)));


    this->ui->horizontalSlider->setRange(1,30);
    this->ui->spinBox->setRange(1,30);
    this->ui->spinBox->setValue(5);

    this->ui->pathEdit->setText(QString("/home/kimmin"));
    g_PathString=QString("/home/kimmin");
}


NewDialog::~NewDialog(){
    delete ui;
}

void NewDialog::slotChangePath(){
    pthread_mutex_lock(&pathMutex);
    this->ui->pathEdit->setText(g_PathString);
}

void NewDialog::slotChangePathS(QString s){
    //this->ui->pathEdit->setText(s);
    g_PathString=s;
}

void NewDialog::slotChangeURL(QString s){
    g_URLString=s;
}

void NewDialog::slotChangeTN(int tn){
    g_ThreadNum=tn;
}


void NewDialog::changePath(QString s){
    this->ui->pathEdit->setText(s);
}

