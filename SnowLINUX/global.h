#ifndef GLOBAL_H
#define GLOBAL_H

#include"pthread.h"
#include <netinet/in.h>
#include <vector>
#include "global_t.h"
#include <signal.h>
#include "missionbar.h"
#include "missioncheck.h"

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

extern QString g_PathString;
extern QString g_URLString;
extern int g_ThreadNum;

extern std::vector<MissionBar*> compMissionBarTable;
extern std::vector<MissionCheck*> compMissionCheckTable;

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

