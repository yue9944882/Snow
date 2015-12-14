
#include "global_t.h"
#include <list>
#include <vector>
#include <pthread.h>
#include <signal.h>
#include <QString>
#include "missionbar.h"
#include "missioncheck.h"

int g_iMissionNum=0;
int g_iNextMissionIndex=0;
std::vector<MissionInfo*> g_vecMissionTable;

pthread_mutex_t outputMutex;
pthread_mutex_t tableMutex;
pthread_mutex_t timeMutex;
pthread_mutex_t finishMutex;
pthread_mutex_t pathMutex;

//Communication between NEWDIALOG & MAINWINDOW
QString g_PathString;
QString g_URLString;
int g_ThreadNum;

sigset_t signal_set;
pthread_t hthread; //Helper Thread

///Test Val

std::vector<MissionBar*> compMissionBarTable;
std::vector<MissionCheck*> compMissionCheckTable;



