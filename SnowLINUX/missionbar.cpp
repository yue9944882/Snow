#include "missionbar.h"
#include <sstream>
#include <string.h>
#include <stdio.h>
#include <QString>
#include "global.h"
MissionBar::MissionBar(QWidget*parent,int cindx):QWidget(parent){

    std::string mName;
    std::ostringstream oss(mName);
    oss<<"Mission "<<cindx;
    if(MissionBar::objectName().isEmpty()){
        MissionBar::setObjectName(QString::fromUtf8("MissionBar"));
    }

    this->compIndex=cindx;

    MissionBar::resize(540,35);
    MissionBar::setWindowModality(Qt::NonModal);

    progressBar=new QProgressBar(this);
    progressBar->setObjectName(QString::fromUtf8("progressBar"));
    progressBar->setGeometry(160,10,211,23);
    progressBar->setValue(0);

    //Statistics : SPEED !
    staticLabel=new QLabel(this);
    staticLabel->setGeometry(380,10,91,21);
    staticLabel->setText(tr("0KB/s"));

    timeBar=new QLabel(this);
    timeBar->setObjectName(QString::fromUtf8("timeBar"));
    timeBar->setGeometry(470,10,66,21);
    timeBar->setText(tr("00:00:00"));

    nameBar=new QLabel(this);
    nameBar->setObjectName(QString::fromUtf8("nameBar"));
    nameBar->setText(oss.str().c_str());
    nameBar->setGeometry(10,10,71,21);

    sizeBar=new QLabel(this);
    sizeBar->setObjectName(QString::fromUtf8("sizeBar"));
    sizeBar->setGeometry(80,10,56,21);
    sizeBar->setText(tr("0KB"));
    sizeBar->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    timer=new QTimer(this);
    timer->start(1000);

    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(slotUpdateProgress()));


//    pthread_mutex_lock(&tableMutex);
//    int midx=-1;
//    int sel=-1;
//    for(int i=0;i<g_vecMissionTable.size();i++){
//        if(((MissionInfo*)g_vecMissionTable[i])->m_iCompIndex==this->compIndex){
//            midx=((MissionInfo*)g_vecMissionTable[i])->m_iMissionIndex;
//            sel=i;
//        }
//    }
//    if(sel==-1||midx==-1){
//        return;
//    }

//    pthread_mutex_unlock(&tableMutex);
}





MissionBar::~MissionBar(){

    delete progressBar;
    delete timeBar;
    delete nameBar;
    delete sizeBar;

}

void MissionBar::adjustPosition(){
    this->move(20,0+45*this->compIndex);
}


void MissionBar::setProgressBar(double value){
    this->progressBar->setValue((int)(value*100));
}

void MissionBar::slotUpdateProgress(){
    pthread_mutex_lock(&tableMutex);
    int midx=-1;
    int sel=-1;
    for(int i=0;i<g_vecMissionTable.size();i++){
        if(((MissionInfo*)g_vecMissionTable[i])->m_iCompIndex==this->compIndex){
            midx=((MissionInfo*)g_vecMissionTable[i])->m_iMissionIndex;
            sel=i;
        }
    }
    if(sel==-1||midx==-1){
        return;
    }
    pthread_mutex_lock(&finishMutex);
    bool bTmp=((MissionInfo*)g_vecMissionTable[midx])->m_bRunning;
    pthread_mutex_unlock(&finishMutex);

    if(bTmp==false){
        pthread_mutex_lock(&timeMutex);
        ((MissionInfo*)g_vecMissionTable[midx])->m_lConsumeTime++;
        pthread_mutex_unlock(&timeMutex);
    }
    pthread_mutex_lock(&(((MissionInfo*)g_vecMissionTable[midx])->mutex));
    if(bTmp==false){
        double db=(double)(((MissionInfo*)g_vecMissionTable[midx])->m_lDoneBytes);
        double tb=(double)(((MissionInfo*)g_vecMissionTable[midx])->m_lTotalBytes);
        double pq=db/tb;
        this->progressBar->setValue((int)(pq*100));
    }
    char tmpsz[64]={};
    char tmpsp[64]={};
    long ltmp=(((MissionInfo*)g_vecMissionTable[midx])->m_lTotalBytes);
    if(ltmp<1000){
        sprintf(tmpsz,"%ldBytes",ltmp);
    }else if(ltmp<1000*1000){
        sprintf(tmpsz,"%ldKB",ltmp/(1000));
    }else{
        sprintf(tmpsz,"%ldMB",ltmp/(1000*1000));
    }
    this->sizeBar->setText(QString(tmpsz));

    pthread_mutex_lock(&timeMutex);
    long ldtmp=(((MissionInfo*)g_vecMissionTable[midx])->m_lSpeedBytes);
    fprintf(stderr,"SpeedByte :%ld <>  %ld\n",(((MissionInfo*)g_vecMissionTable[midx])->m_lSpeedBytes),(((MissionInfo*)g_vecMissionTable[midx])->m_lDoneBytes));
    long ltime=((MissionInfo*)g_vecMissionTable[midx])->m_lConsumeTime;
    this->timeBar->setText(setTimeFormat(ltime));
    sprintf(tmpsp,"%ldKB/s",(ldtmp/1000)/ltime);
    this->staticLabel->setText(QString(tmpsp));
    pthread_mutex_unlock(&timeMutex);
    pthread_mutex_unlock(&(((MissionInfo*)g_vecMissionTable[midx])->mutex));
    //pthread_mutex_unlock(&outputMutex);
    pthread_mutex_unlock(&tableMutex);
}

//void MissionBar::timerEvent(QTimerEvent *te){
//    pthread_mutex_lock(&tableMutex);
//    for(int i;g_vecMissionTable.size();i++){
//        pthread_mutex_lock(&outputMutex);
//        int cidx=((MissionInfo*)g_vecMissionTable[i])->m_iCompIndex;
//        double pq=(double)(((MissionInfo*)g_vecMissionTable[i])->m_lDoneBytes)/(double)(((MissionInfo*)g_vecMissionTable[i])->m_lTotalBytes);
//        ((MissionBar*)compMissionBarTable[cidx])->setProgressBar(pq);
//        pthread_mutex_unlock(&outputMutex);
//    }
//    pthread_mutex_unlock(&tableMutex);
//}

QString MissionBar::setTimeFormat(long lSec){
    char tmp[32]={};
    long h=lSec/3600;
    long hr=lSec%3600;
    long m=hr/60;
    long s=hr%60;
    sprintf(tmp,"%02ld:%02ld:%02ld",h,m,s);
    return QString(tmp);
}


