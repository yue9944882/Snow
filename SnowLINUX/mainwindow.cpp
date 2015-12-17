#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "newdialog.h"
#include "global.h"
#include "global_f.h"
#include "missionbar.h"
#include <QCheckBox>
#include <QFont>
#include <QGraphicsScene>
#include <QMatrix>
#include <QPainter>
#include <iterator>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Adjust postion
    this->move(400,100);
    this->setWindowTitle(tr("Snow"));
    //ToolButtonIcon
    ui->newButton->setIcon(QIcon(std::string(g_homeDIR+std::string("/new.png")).c_str()));
    ui->newButton->setStatusTip(tr("Start a new mission!"));
    ui->contButton->setIcon(QIcon(std::string(g_homeDIR+std::string("/continue.png")).c_str()));
    ui->contButton->setStatusTip(tr("Continue a already paused mission!"));
    ui->pauseButton->setIcon(QIcon(std::string(g_homeDIR+std::string("/pause.png")).c_str()));
    ui->pauseButton->setStatusTip(tr("Pause a running mission!"));
    ui->deleteButton->setIcon(QIcon(std::string(g_homeDIR+std::string("/delete.png")).c_str()));
    ui->deleteButton->setStatusTip(tr("Delete a mission and its disk storage!"));
    ui->restButton->setIcon(QIcon(std::string(g_homeDIR+std::string("/restart.png")).c_str()));
    ui->restButton->setStatusTip(tr("Restart a mission!"));

    //Connections
    QObject::connect(ui->newButton,SIGNAL(clicked()),this,SLOT(slotNewMission()));
    QObject::connect(ui->deleteButton,SIGNAL(clicked()),this,SLOT(slotDelMission()));
    QObject::connect(ui->pauseButton,SIGNAL(clicked()),this,SLOT(slotPauseMission()));
    QObject::connect(ui->contButton,SIGNAL(clicked()),this,SLOT(slotContMission()));
    QObject::connect(ui->restButton,SIGNAL(clicked()),this,SLOT(slotRestMission()));

    QFont font=this->ui->labelPath->font();
    font.setPointSize(10);
    this->ui->labelPath->setWordWrap(true);
    this->ui->labelPath->setFont(font);
    this->ui->labelPath->setAlignment(Qt::AlignTop);

    this->ui->newButton->setEnabled(true);
    this->ui->contButton->setEnabled(false);
    this->ui->pauseButton->setEnabled(false);
    this->ui->restButton->setEnabled(false);
    this->ui->deleteButton->setEnabled(false);

    //Temp String ENV
    g_bPlotFull=false;
    g_tkPosition=1;
    g_lastRate=0;
    g_maxRate=0;
    g_Scene=new QGraphicsScene;
    QMatrix mat;
    mat=this->ui->graphicsView->matrix();
    mat.scale(1,0.05);
    this->ui->graphicsView->setMatrix(mat);

    timer=new QTimer;
    timer->start(1000);
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(slotPlotWave()));
    this->setAttribute(Qt::WA_PaintOutsidePaintEvent);
    //this->setAttribute(Qt::WA_P);
    //m_szPath=QString("/home/kimmin");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotNewMission(){
    NewDialog*dlg=new NewDialog(this);
    //this->m_queryDlg=dlg;
    dlg->show();
}

void MainWindow::slotSetNextMissionDir(){

    QString dir = QFileDialog::getExistingDirectory(0, "Open Directory", "/home/kimmin", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    g_PathString=dir;
    pthread_mutex_unlock(&pathMutex);
    //this->m_szPath=dir;
    //this->m_queryDlg->changePath(dir);

}

void MainWindow::slotNewMissionBar(){


    MissionInfo*mission=new MissionInfo;
    mission->m_iCompIndex=g_iMissionNum;
    int midx=g_iNextMissionIndex;
    mission->m_iMissionIndex=g_iNextMissionIndex;
    mission->m_bRunning=false;
    mission->m_lDoneBytes=0;
    mission->m_lTotalBytes=1;
    mission->m_lConsumeTime=0;
    mission->m_lSpeedBytes=0;


    pthread_mutex_init(&(mission->mutex),NULL);
    pthread_mutex_init(&(mission->pauseMutex),NULL);

    MissionBar*mbar=new MissionBar(ui->scrollAreaWidgetContents,compMissionBarTable.size());//COMP INDEX
    MissionCheck*cbox=new MissionCheck(ui->scrollAreaWidgetContents,compMissionBarTable.size());//COMP INDEX
    QObject::connect(cbox,SIGNAL(clicked(bool)),cbox,SLOT(slotChangeGlobalIndex()));
    QObject::connect(cbox,SIGNAL(clicked(bool)),this,SLOT(slotUpdateSelectTable()));

    cbox->adjustPosition();
    mbar->adjustPosition();

    pthread_mutex_lock(&tableMutex);
    compMissionBarTable.push_back(mbar);
    compMissionCheckTable.push_back(cbox);
    compMissionSelectTable.push_back(false);
    compPauseSelectTable.push_back(false);
    pthread_mutex_unlock(&tableMutex);

    mbar->show();
    cbox->show();

    g_iMissionNum++;
    g_iNextMissionIndex++;
    pthread_mutex_lock(&tableMutex);
    g_vecMissionTable.push_back(mission);
    pthread_mutex_unlock(&tableMutex);


    MissionArg*marg=new MissionArg;
    marg->iMissionIndex=midx;
    marg->iThreadNum=g_ThreadNum;
    mission->m_stThreadTable=(ThreadInfo*)malloc(marg->iThreadNum*sizeof(ThreadInfo));

    strcpy(marg->szUrl,g_URLString.toStdString().c_str());
    strncpy(mission->m_szURL,g_URLString.toStdString().c_str(),1024);
	//marg.szUrl="http://files.cnblogs.com/files/guguli/100003309726139.gif";
    QString tmpStr=g_PathString.append('/');
    strcpy(marg->szPath,tmpStr.toStdString().c_str());
    //marg.szPath="/home/kimmin/Downloads/";

    pthread_create(&(mission->m_missionTID),NULL,begin_mission,(void*)(marg));

//    while(pthread_tryjoin_np(pt,NULL)!=0){
//        pthread_mutex_lock(&tableMutex);
//        int cidx=((MissionInfo*)g_vecMissionTable[midx])->m_iCompIndex;
//        pthread_mutex_lock(&outputMutex);
//        double pq=(double)(((MissionInfo*)g_vecMissionTable[midx])->m_lDoneBytes)/(double)(((MissionInfo*)g_vecMissionTable[midx])->m_lTotalBytes);
//        pthread_mutex_unlock(&outputMutex);
//        ((MissionBar*)compMissionBarTable[cidx])->setProgressBar(pq);
//        pthread_mutex_unlock(&tableMutex);
//        sleep(1);
//    }

//  pthread_join(pt,NULL);

}

void MainWindow::slotUpdateSelectTable(){
    //
    pthread_mutex_lock(&clickMutex);
    bool bs;

    if(((MissionCheck*)compMissionCheckTable[g_clickIndex])->isChecked()){
        bs=true;
        compMissionSelectTable[g_clickIndex]=true;
        ((MissionCheck*)compMissionCheckTable[g_clickIndex])->setChecked(true);
        this->ui->deleteButton->setEnabled(true);
        this->ui->restButton->setEnabled(true);
    }else{
        bs=false;
        compMissionSelectTable[g_clickIndex]=false;
        ((MissionCheck*)compMissionCheckTable[g_clickIndex])->setChecked(false);
        this->ui->deleteButton->setEnabled(false);
        this->ui->restButton->setEnabled(false);
    }

    for(int i=0;i<compMissionSelectTable.size();i++){
        if(i!=g_clickIndex){
            compMissionSelectTable[i]=false;
            ((MissionCheck*)compMissionCheckTable[i])->setChecked(false);
        }
    }
    this->ui->newButton->setEnabled(true);
    //this->ui->deleteButton->setEnabled(true);
    //this->ui->restButton->setEnabled(true);
    if(bs){
        if(compPauseSelectTable[g_clickIndex]==false){
            this->ui->pauseButton->setEnabled(true);
            this->ui->contButton->setEnabled(false);
        }else{
            this->ui->contButton->setEnabled(true);
            this->ui->pauseButton->setEnabled(false);
        }

    }

    int midx=-1;
    int sel=-1;
    for(int j=0;j<g_vecMissionTable.size();j++){
        if(((MissionInfo*)g_vecMissionTable[j])->m_iCompIndex==g_clickIndex){
            midx=((MissionInfo*)g_vecMissionTable[j])->m_iMissionIndex;
            sel=j;
        }
    }
    if(sel==-1||midx==-1){
        return;
    }

    char tmp[1024]={};

    sprintf(tmp,"Thread Number :%d",((MissionInfo*)g_vecMissionTable[midx])->m_iThreadNum);
    ui->labelTN->setText(QString(tmp));
    bzero(tmp,1024);

    sprintf(tmp,"Thread ID : \n%ld\nMission ID :\n%d",((MissionInfo*)g_vecMissionTable[midx])->m_missionTID,((MissionInfo*)g_vecMissionTable[midx])->m_iMissionIndex);
    ui->labelPath->setText(QString(tmp));
    ui->labelPath->adjustSize();
    bzero(tmp,1024);

//    sprintf(tmp,"URL :\n%s",((MissionInfo*)g_vecMissionTable[midx])->m_szURL);
//    ui->labelPath->setText(QString(tmp));

//    bool tmp=false;
//    for(int i=0;i<compMissionCheckTable.size();i++){
//        if(((MissionCheck*)compMissionCheckTable[i])->isChecked()){
//            compMissionSelectTable[i]=true;
//            tmp=true;
//        }else{
//            compMissionSelectTable[i]=false;
//        }
//    }
//    if(tmp==false){
//        this->ui->newButton->setEnabled(true);
//        this->ui->contButton->setEnabled(false);
//        if(compPauseSelectTable[i]==true)this->ui->pauseButton->setEnabled(false);
//        else()
//        this->ui->restButton->setEnabled(false);
//        this->ui->deleteButton->setEnabled(false);
//    }else{
//        this->ui->newButton->setEnabled(true);
//        this->ui->contButton->setEnabled(false);
//        this->ui->pauseButton->setEnabled(true);
//        this->ui->restButton->setEnabled(true);
//        this->ui->deleteButton->setEnabled(true);
//    }
}


void MainWindow::slotDelMission(){
    for(int i=0;i<compMissionSelectTable.size();i++){
        if(compMissionSelectTable[i]==true){
            //Do Deletion
            int midx=-1;
            int sel=-1;
            for(int j=0;j<g_vecMissionTable.size();j++){
                if(((MissionInfo*)g_vecMissionTable[j])->m_iCompIndex==i){
                    midx=((MissionInfo*)g_vecMissionTable[j])->m_iMissionIndex;
                    sel=j;
                }
            }
            if(sel==-1||midx==-1){
                return;
            }
            //Thread Operating

            //Comp Operating
            MissionBar*tmpBar=compMissionBarTable[i];
            MissionCheck*tmpCheck=compMissionCheckTable[i];

            for(int k=i;k<compMissionBarTable.size();k++){
                if(k!=compMissionBarTable.size()-1){
                    compMissionBarTable[k]=compMissionBarTable[k+1];
                    ((MissionBar*)compMissionBarTable[k])->compIndex=k;
                    compMissionCheckTable[k]=compMissionCheckTable[k+1];
                    ((MissionCheck*)compMissionCheckTable[k])->compIndex=k;
                    compMissionSelectTable[k]=compMissionSelectTable[k+1];
                    compPauseSelectTable[k]=compPauseSelectTable[k+1];
                }else{
                    compMissionBarTable.pop_back();
                    compMissionCheckTable.pop_back();
                    compMissionSelectTable.pop_back();
                    compPauseSelectTable.pop_back();
                }
            }
            tmpBar->close();
            tmpCheck->close();
            ThreadInfo*tis=((MissionInfo*)g_vecMissionTable[midx])->m_stThreadTable;


            pthread_mutex_lock(&finishMutex);
            ((MissionInfo*)g_vecMissionTable[midx])->m_bRunning=true;

            pthread_mutex_unlock(&finishMutex);



            delete tmpBar;
            delete tmpCheck;

            ((MissionInfo*)g_vecMissionTable[midx])->m_iCompIndex=-1;
            g_iMissionNum--;

            pthread_mutex_lock(&tableMutex);
            for(int n=midx;n<g_vecMissionTable.size();n++){
                ((MissionInfo*)g_vecMissionTable[n-0])->m_iCompIndex--;
            }

            pthread_mutex_unlock(&tableMutex);

        }
    }


    for(int k=0;k<compMissionBarTable.size();k++){
        ((MissionBar*)compMissionBarTable[k])->adjustPosition();
        ((MissionCheck*)compMissionCheckTable[k])->adjustPosition();
    }

}

void MainWindow::slotRestMission(){
    for(int i=0;i<compMissionSelectTable.size();i++){
        if(compMissionSelectTable[i]==true){
            //Do Deletion
            int midx=-1;
            int sel=-1;
            for(int j=0;j<g_vecMissionTable.size();j++){
                if(((MissionInfo*)g_vecMissionTable[j])->m_iCompIndex==i){
                    midx=((MissionInfo*)g_vecMissionTable[j])->m_iMissionIndex;
                    sel=j;
                }
            }
            if(sel==-1||midx==-1){
                return;
            }
            //Thread Operating

            ThreadInfo*tis=((MissionInfo*)g_vecMissionTable[midx])->m_stThreadTable;

            for(int m=0;m<((MissionInfo*)g_vecMissionTable[midx])->m_iThreadNum;m++){
                pthread_cancel(tis[m].tid);
            }

            MissionInfo*mission=g_vecMissionTable[midx];
            mission->m_bRunning=false;
            mission->m_lDoneBytes=0;
            mission->m_lTotalBytes=1;
            mission->m_lConsumeTime=0;
            mission->m_lSpeedBytes=0;
            MissionArg*marg=new MissionArg;
            marg->iMissionIndex=midx;
            marg->iThreadNum=g_ThreadNum;
            mission->m_stThreadTable=(ThreadInfo*)malloc(marg->iThreadNum*sizeof(ThreadInfo));

            strcpy(marg->szUrl,g_URLString.toStdString().c_str());
            strncpy(mission->m_szURL,g_URLString.toStdString().c_str(),1024);
            //marg.szUrl="http://files.cnblogs.com/files/guguli/100003309726139.gif";
            QString tmpStr=g_PathString.append('/');
            strcpy(marg->szPath,tmpStr.toStdString().c_str());
            //marg.szPath="/home/kimmin/Downloads/";

            pthread_create(&(mission->m_missionTID),NULL,begin_mission,(void*)(marg));



//            pthread_mutex_lock(&finishMutex);
//            ((MissionInfo*)g_vecMissionTable[midx])->m_bRunning=true;
//            pthread_mutex_unlock(&finishMutex);

        }
    }
}


void MainWindow::slotPauseMission(){
    for(int i=0;i<compMissionSelectTable.size();i++){
        if(compMissionSelectTable[i]==true){
            //Do Deletion
            int midx=-1;
            int sel=-1;
            for(int j=0;j<g_vecMissionTable.size();j++){
                if(((MissionInfo*)g_vecMissionTable[j])->m_iCompIndex==i){
                    midx=((MissionInfo*)g_vecMissionTable[j])->m_iMissionIndex;
                    sel=j;
                }
            }
            if(sel==-1||midx==-1){
                return;
            }http://www.eu.apache.org/dist/hadoop/common/hadoop-2.7.1/hadoop-2.7.1-src.tar.gz
            //Thread Operating

            int tn=(((MissionInfo*)g_vecMissionTable[midx])->m_iThreadNum);
            ThreadInfo*tinfo=(((MissionInfo*)g_vecMissionTable[midx])->m_stThreadTable);
            for(int i=0;i<tn;i++){
                pthread_cancel(tinfo[i].tid);
            }

            //Comp Operating

            //pthread_mutex_lock(&((MissionInfo*)g_vecMissionTable[midx])->pauseMutex);
            //write_log((std::string(std::string(((MissionInfo*)g_vecMissionTable[midx])->m_szFile)+std::string(".log"))).c_str(),g_vecMissionTable[midx]);
            write_log(((MissionInfo*)g_vecMissionTable[midx])->m_szFile,g_vecMissionTable[midx]);
            pthread_mutex_lock(&finishMutex);
            ((MissionInfo*)g_vecMissionTable[midx])->m_bRunning=true;
            pthread_mutex_unlock(&finishMutex);
            compPauseSelectTable[i]=true;
            ((MissionCheck*)compMissionCheckTable[i])->click();
            ((MissionCheck*)compMissionCheckTable[i])->click();

//            pthread_mutex_lock(&finishMutex);
//            ((MissionInfo*)g_vecMissionTable[midx])->m_bRunning=true;
//            pthread_mutex_unlock(&finishMutex);

        }
    }
}

void MainWindow::slotContMission(){
    for(int i=0;i<compMissionSelectTable.size();i++){
        if(compMissionSelectTable[i]==true){
            //Do Deletion
            int midx=-1;
            int sel=-1;
            for(int j=0;j<g_vecMissionTable.size();j++){
                if(((MissionInfo*)g_vecMissionTable[j])->m_iCompIndex==i){
                    midx=((MissionInfo*)g_vecMissionTable[j])->m_iMissionIndex;
                    sel=j;
                }
            }
            if(sel==-1||midx==-1){
                return;
            }

            //Thread Operating

            //Comp Operating

            //pthread_mutex_unlock(&((MissionInfo*)g_vecMissionTable[midx])->pauseMutex);

            pthread_t pt;
            pthread_create(&pt,NULL,resumeDownload,g_vecMissionTable[midx]);
            //resumeDownload(g_vecMissionTable[midx]);
            compPauseSelectTable[i]=false;
            ((MissionCheck*)compMissionCheckTable[i])->click();
            ((MissionCheck*)compMissionCheckTable[i])->click();

//            pthread_mutex_lock(&finishMutex);
//            ((MissionInfo*)g_vecMissionTable[midx])->m_bRunning=false;
//            pthread_mutex_unlock(&finishMutex);
        }
    }
}


void MainWindow::paintEvent(QPaintEvent *pe){
//    QPainter painter(this);//10,90,171,131
//    painter.drawLine(10,110,180,220);//Width 170 Height 110
//    update();
//    delete g_Scene;
    this->ui->graphicsView->show();

}


void MainWindow::slotPlotWave(){
    long tdb=0;
    bool bExist=false;
    long tdt=1;
    for(int i=0;i<g_vecMissionTable.size();i++){
        pthread_mutex_lock(&finishMutex);
        bool btmp=((MissionInfo*)g_vecMissionTable[i])->m_bRunning;
        pthread_mutex_unlock(&finishMutex);
        if(btmp==false){
            bExist=true;
            pthread_mutex_lock(&outputMutex);
            tdb+=((MissionInfo*)g_vecMissionTable[i])->m_lSpeedBytes;
            pthread_mutex_unlock(&outputMutex);
            pthread_mutex_lock(&timeMutex);
            tdt+=((((MissionInfo*)g_vecMissionTable[i])->m_lConsumeTime)+1);
            pthread_mutex_unlock(&timeMutex);
        }
    }

    int rate=tdb/tdt;

    if(bExist){
        g_Scene->addLine((qreal)g_tkPosition,-(((qreal)g_lastRate/1000)/100)*90,(qreal)g_tkPosition+2,-(((qreal)rate/1000)/100)*90);
        char tmp[128]={};
        sprintf(tmp,"%9dKB/s",rate/1000);
        this->ui->speedLabel->setText(tmp);
    }

//    QMatrix mat;

    g_maxRate=g_maxRate>rate?g_maxRate:rate;

    this->ui->graphicsView->setScene(g_Scene);

//    mat=this->ui->graphicsView->matrix();
//    mat.scale(1,1/((((qreal)rate)/1000)/100));
//    this->ui->graphicsView->setMatrix(mat);

    this->ui->graphicsView->show();

    update();

    g_lastRate=rate;
    g_tkPosition+=2;

    update();
}


//void MainWindow::slotNewMissionThread(){
//        MissionArg marg;
//        marg.iMissionIndex=0;
//        marg.iThreadNum=5;
//        strcpy(marg.szUrl,"http://www.us.apache.org/dist/hadoop/common/hadoop-2.7.1/hadoop-2.7.1-src.tar.gz");
//        //marg.szUrl="http://files.cnblogs.com/files/guguli/100003309726139.gif";
//        strcpy(marg.szPath,"/home/kimmin/Downloads/");
//        //marg.szPath="/home/kimmin/Downloads/";
//        pthread_t pt;
//        pthread_create(&pt,NULL,begin_mission,(void*)(&marg));
//        for(int i;i<10;i++){
//            pthread_mutex_lock(&outputMutex);
//            fprintf(stderr,"Progress %ld / %ld",((MissionInfo*)g_vecMissionTable[0])->m_lDoneBytes,((MissionInfo*)g_vecMissionTable[0])->m_lTotalBytes);
//            pthread_mutex_unlock(&outputMutex);
//            sleep(1);
//        }
//        pthread_join(pt,&lp);
//        fprintf(stderr,"%ld\n",*(long*)lp);
//}
