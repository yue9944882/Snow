#ifndef GLOBAL_T_H
#define GLOBAL_T_H

//Global Type Struct Defination

#include <netinet/in.h>
#include <pthread.h>

struct MissionInfo{
    int m_iThreadNum;
    long m_lConsumeTime;
    long m_lDoneBytes;
    long m_lSpeedBytes;
    long m_lTotalBytes;
    int m_iCompIndex;
    int m_iMissionIndex;
    struct ThreadInfo*m_stThreadTable;
    pthread_mutex_t mutex;
    pthread_mutex_t pauseMutex;
    pthread_t m_missionTID;
    bool m_bRunning;
	//LOG INFO
	char m_szURL[1024];
	char m_szPath[1024];
	char m_szFile[1024];
};

struct ThreadInfo{
    struct sockaddr_in sin;
    long lBeginPos;
    long lEndPos;
    long lCurrentPos;
    MissionInfo*parentMission;
    //Compatible Member
    char*msg;
    pthread_t tid;
};


struct URLinfo{
    //// info analysis from url pattern
    char szHostname[1024];
    char szURLname[1024];
    char szFilename[1024];
    char szIPv4addr[16];
    int iPort;
    //// info analysis from head reply
    long lContentLen;
};

struct MissionArg{
    char szPath[1024];
    char szUrl[1024];
    char szFile[1024];
    int iThreadNum;
    int iMissionIndex;
};


struct pthreadArg{
    ThreadInfo*ti;
    char*szURLname;
    char*szHostname;
    char*szFilename;
    long*llContentLen;
    long*mllContentLen;
    long*mllPosition;
    long*mllSpeedLen;
    int index;//Thread Index
    int midx;//Mission Index

};

struct ThreadLog{
	long lBegin;
	long lEnd;
	long lPos;
};




#define HEAD_GRAM "HEAD %s HTTP/1.1\r\nHost: %s\r\nUser-Agent: %sConnection: close\r\n\r\n"
#define GET_GRAM "GET %s HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s\r\nRange: bytes=%lld-\r\nConnection: keep-alive\r\n\r\n"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<netinet/in.h>
#include<signal.h>
#include<resolv.h>
#include<netdb.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/nameser.h>
#include<arpa/inet.h>
#include<vector>
#include<QString>


#include<netinet/in.h>
#include<resolv.h>
#include<netdb.h>
#include<errno.h>
#include<fcntl.h>
#include<pthread.h>
#include<time.h>
#include<sys/types.h>

#include<QFileDialog>
#include<vector>


#endif // GLOBAL_T_H

