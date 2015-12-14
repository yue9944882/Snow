#include"mainwindow.h"
#include"newdialog.h"
#include "global.h"
#include "global_f.h"
#include <pthread.h>

#include<QApplication>


//// Four Things U must need to know before starting a routine of downloading
//// 1. URL 2.PATH 3.MISSION_INDEX 4.THREAD_NUM


int main(int argc, char *argv[])
{

    //// Global Initialization
    //INIT : Waiter thread & globals
    int wait_ret;

    sigemptyset(&signal_set);
    sigaddset(&signal_set,SIGINT);
    sigaddset(&signal_set,SIGALRM);

    pthread_sigmask(SIG_BLOCK,&signal_set,NULL);

    if((wait_ret=pthread_create(&hthread,NULL,signal_waiter,NULL))!=0){
        fprintf(stderr,"MAIN :cannot create WAITER thread!\nExiting....\n");
        exit(-1);
    }

    //INIT : Global mutex
    pthread_mutex_init(&outputMutex,NULL);
    pthread_mutex_init(&tableMutex,NULL);
    pthread_mutex_init(&finishMutex,NULL);
    pthread_mutex_init(&timeMutex,NULL);
    pthread_mutex_init(&pathMutex,NULL);

    pthread_mutex_lock(&pathMutex);

    //Enable Windows
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();



//    sigemptyset(&signal_set);
//    sigaddset(&signal_set, SIGINT);
//    sigaddset(&signal_set, SIGALRM);

    // Block out all signals
//    pthread_sigmask(SIG_BLOCK, &signal_set, NULL);




//    g_vecMissionTable.push_back(new MissionInfo);

//    g_iMissionNum=1;
//    MissionArg marg;
//    marg.iMissionIndex=0;
//    marg.iThreadNum=5;
//    strcpy(marg.szUrl,"http://www.us.apache.org/dist/hadoop/common/hadoop-2.7.1/hadoop-2.7.1-src.tar.gz");
//    //marg.szUrl="http://files.cnblogs.com/files/guguli/100003309726139.gif";
//    strcpy(marg.szPath,"/home/kimmin/Downloads/");
//    //marg.szPath="/home/kimmin/Downloads/";
//    pthread_t pt;
//    void*lp;

//    pthread_create(&pt,NULL,begin_mission,(void*)(&marg));

////    for(int i;i<1000;i++){
////        fprintf(stderr,"Progress %ld / %ld",((MissionInfo*)g_vecMissionTable[0])->m_lDoneBytes,((MissionInfo*)g_vecMissionTable[0])->m_lTotalBytes);
////        sleep(1);
////    }

//    pthread_join(pt,&lp);

//    fprintf(stderr,"%ld\n",*(long*)lp);

}
