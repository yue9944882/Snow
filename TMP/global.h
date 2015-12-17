#ifndef GLOBAL_H
#define GLOBAL_H

#include"pthread.h"
#include <netinet/in.h>
#include <vector>
#include "global_t.h"
#include <signal.h>
#include <string>
#include <QGraphicsScene>
#include "missionbar.h"
#include "missioncheck.h"


extern std::string g_homeDIR;

extern pthread_t hthread;
extern int g_iNextMissionIndex;
extern int g_iMissionNum;
extern std::vector<MissionInfo*> g_vecMissionTable;
extern sigset_t signal_set;

extern pthread_mutex_t outputMutex;
extern pthread_mutex_t tableMutex;
extern pthread_mutex_t finishMutex;
extern pthread_mutex_t timeMutex;
extern pthread_mutex_t pathMutex;
extern pthread_mutex_t clickMutex;

extern QString g_PathString;
extern QString g_URLString;
extern int g_ThreadNum;

extern std::vector<MissionBar*> compMissionBarTable;
extern std::vector<MissionCheck*> compMissionCheckTable;
extern std::vector<bool> compMissionSelectTable;
extern std::vector<bool> compPauseSelectTable;


extern bool g_bPlotFull;
extern int g_tkPosition;

extern QGraphicsScene*g_Scene;
extern int g_lastRate;
extern int g_maxRate;
extern int g_clickIndex;
/*
struct pthreadArg{
    ThreadInfo*ti;
    char*szURLname;
    char*szHostname;
    char*szFilename;
    long*llContentLen;
    long*mllContentLen;
    int*mllPosition;
    int index;

};
*/




#endif // GLOBAL_H

