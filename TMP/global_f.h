#ifndef GLOBAL_F_H
#define GLOBAL_F_H

#include "global_t.h"

//Global Functions Defination


extern long preConnect(char*url,URLinfo*u,int midx);

extern void*partget(void*arg);

extern int multiDownload(char*path,URLinfo*u,int num,int midx);

extern void*begin_mission(void*arg);

///////////////////
///  SIGNAL HANDLING SECTION
///////////////////

extern void sigalrm_handler(void);

extern void*signal_waiter(void*);

extern void sigint_handler(void);

extern void init_log(char*fullpathname);

extern void write_log(char*fullpathname,MissionInfo*minfo);

extern void read_log(const char*fullpathname,MissionInfo*minfo);

extern void validate_log(const char*fullpathname);

extern void*resumeDownload(void*);

#endif // GLOBAL_F_H
