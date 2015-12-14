#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "newdialog.h"
#include "global.h"
#include "global_f.h"
#include "missionbar.h"
#include <QCheckBox>
#include <iterator>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //Adjust postion
    this->move(400,100);

    //ToolButtonIcon
    ui->newButton->setIcon(QIcon("/home/kimmin/Github/Snow/Snow/new.png"));
    ui->newButton->setStatusTip(tr("Start a new mission!"));
    ui->contButton->setIcon(QIcon("/home/kimmin/Github/Snow/Snow/continue.png"));
    ui->contButton->setStatusTip(tr("Continue a already paused mission!"));
    ui->pauseButton->setIcon(QIcon("/home/kimmin/Github/Snow/Snow/pause.png"));
    ui->pauseButton->setStatusTip(tr("Pause a running mission!"));
    ui->deleteButton->setIcon(QIcon("/home/kimmin/Github/Snow/Snow/delete.png"));
    ui->deleteButton->setStatusTip(tr("Delete a mission and its disk storage!"));
    ui->restButton->setIcon(QIcon("/home/kimmin/Github/Snow/Snow/restart.png"));
    ui->restButton->setStatusTip(tr("Restart a mission!"));

    //Connections
    QObject::connect(ui->newButton,SIGNAL(clicked()),this,SLOT(slotNewMission()));

    //Temp String ENV
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
    pthread_mutex_init(&(mission->mutex),NULL);
    MissionBar*mbar=new MissionBar(ui->scrollAreaWidgetContents,g_iMissionNum);
    MissionCheck*cbox=new MissionCheck(ui->scrollAreaWidgetContents,g_iMissionNum);
    cbox->setGeometry(2,3+45*g_iMissionNum,20,35);
    mbar->setGeometry(20,0+45*g_iMissionNum,540,35);

    pthread_mutex_lock(&tableMutex);
    compMissionBarTable.push_back(mbar);
    compMissionCheckTable.push_back(cbox);
    pthread_mutex_unlock(&tableMutex);

    mbar->show();
    cbox->show();

    g_iMissionNum++;
    g_iNextMissionIndex++;
    pthread_mutex_lock(&tableMutex);
    g_vecMissionTable.push_back(mission);
    pthread_mutex_unlock(&tableMutex);


    MissionArg*marg=new MissionArg;
    marg->iMissionIndex=0;
    marg->iThreadNum=g_ThreadNum;

    strcpy(marg->szUrl,g_URLString.toStdString().c_str());
    //marg.szUrl="http://files.cnblogs.com/files/guguli/100003309726139.gif";
    QString tmpStr=g_PathString.append('/');
    strcpy(marg->szPath,tmpStr.toStdString().c_str());
    //marg.szPath="/home/kimmin/Downloads/";

    pthread_t pt;
    pthread_create(&pt,NULL,begin_mission,(void*)(marg));

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

    //pthread_join(pt,NULL);
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
