#include "global_t.h"
#include <stddef.h>
#include <stdio.h>
#include "global.h"

void*partDownload(void*arg){

    return NULL;
}


void init_log(const char*fullpathname);

long preConnect(char*url,URLinfo*u,int midx){

    int i,j;
    ////URL parsing

    if(strlen(url)<7||strncmp(url,"http://",7)!=0){
        fprintf(stderr,"Http Support Only( Not Https )\n");
        exit(-1);
    }

    u->iPort=80;

    char*s=url+7;

    for(i=0;*s!='/'&&i<1024;s++,i++){
        u->szHostname[i]=*s;
    }u->szHostname[i]='\0';

    for(i=0;*s!='\0'&&i<1024;s++,i++){
        u->szURLname[i]=*s;
    }u->szURLname[i]='\0';

    --s;

    char tmpname[1024]={};
    for(i=0;*s!='/'&&i<1024;i++,s--){
        tmpname[i]=*s;
    }tmpname[i]='\0';
    int tmplen=strlen(tmpname);

    for(j=0,i=tmplen-1;i>=0;i--,j++){
        u->szFilename[j]=tmpname[i];
    }u->szFilename[j]='\0';

    char*sendBuf=(char*)malloc(4096*sizeof(char));
    char*recvBuf=(char*)malloc(4096*sizeof(char));

    struct hostent*host=gethostbyname(u->szHostname);

    if(host==NULL){
        fprintf(stderr,"Resolve Hostname Failure!\n");
        exit(-1);
    }

    strncpy(u->szIPv4addr,inet_ntoa(*(struct in_addr*)host->h_addr),16);

    struct sockaddr_in*sock=(struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));

    bzero(sock,sizeof(struct sockaddr_in));
    sock->sin_family=AF_INET;
    sock->sin_addr.s_addr=inet_addr(u->szIPv4addr);
    sock->sin_port=htons(u->iPort);

    int sockdesc=socket(AF_INET,SOCK_STREAM,0);
    if(sockdesc==-1){
        fprintf(stderr,"Socket Creation Failure!\n");
        exit(-1);
    }
    if(connect(sockdesc,(const struct sockaddr*)sock,sizeof(struct sockaddr_in))==-1){
        fprintf(stderr,"Socket Connection Failure!\n");
        exit(-1);
    }

    sprintf(sendBuf,HEAD_GRAM,u->szURLname,u->szHostname,"Snow");

    if(send(sockdesc,sendBuf,strlen(sendBuf),0)==-1){
        fprintf(stderr,"Header Sending Failure!\n");
        exit(-1);
    }

    int dr=0;

    if(dr=recv(sockdesc,recvBuf,4096,0)==-1){
        fprintf(stderr,"Header Recving Failure!\n");
        exit(-1);
    }

    char*tok=strtok(recvBuf,"\r\n");
    if(strstr(tok,"HTTP/1.1 200")!=NULL){
        while(tok=strtok(NULL,"\r\n")){
            if(strncasecmp(tok,"Content-Length:",15)==0){
                char*tmp=(tok+15);
                u->lContentLen=atoll(tmp);
            }
        }
    }else{
        fprintf(stderr,"Connection Rejected Failure!\n");
        exit(-1);
    }

    close(sockdesc);

    free(sendBuf);
    free(recvBuf);

    pthread_mutex_lock(&outputMutex);
    ((MissionInfo*)(g_vecMissionTable[midx]))->m_lTotalBytes=u->lContentLen;
    //strncpy(((MissionInfo*)(g_vecMissionTable[midx]))->m_szPath,
	pthread_mutex_unlock(&outputMutex);

    return u->lContentLen;
}





void*partget(void*arg){

    struct pthreadArg*parg=(struct pthreadArg*)arg;
    struct ThreadInfo*ti=parg->ti;
    int midx=parg->midx;
    int tidx=parg->index;
    int dw=0,dr=0;
    long _llBeginPos=ti->lBeginPos;
    long _llEndPos=ti->lEndPos;
    long _llCurrentPos=_llBeginPos;
    pthread_mutex_lock(&(((MissionInfo*)g_vecMissionTable[midx])->mutex));
    ti->lCurrentPos=ti->lBeginPos;
    pthread_mutex_unlock(&(((MissionInfo*)g_vecMissionTable[midx])->mutex));
    char*sendBuf=(char*)malloc(sizeof(char)*4096);
    char*recvBuf=(char*)malloc(sizeof(char)*4096);
    int sockdesc=socket(AF_INET,SOCK_STREAM,0);
    if(sockdesc==-1){
        fprintf(stderr,"Socket Creation Failure!\n");
        exit(-1);
    }

    if((connect(sockdesc,(const struct sockaddr*)(&ti->sin),sizeof(struct sockaddr_in)))==-1){
        fprintf(stderr,"Socket Connection Failure!\n");
        exit(-1);
    }
    bzero(sendBuf,4096);
    sprintf(sendBuf,GET_GRAM,parg->szURLname,parg->szHostname,"Snow",ti->lBeginPos);
    if((send(sockdesc,sendBuf,strlen(sendBuf),0))==-1){
        fprintf(stderr,"Header Sending Failure!\n");
        exit(-1);
    }


    if((dr=recv(sockdesc,recvBuf,4096,0))==-1){
        fprintf(stderr,"Header Recving Failure!\n");
        exit(-1);
    }

    ///////////

//    pthread_mutex_lock(&(((MissionInfo*)g_vecMissionTable[midx])->mutex));

//    ((MissionInfo*)(g_vecMissionTable[midx]))->m_lDoneBytes+=dr;

//    pthread_mutex_unlock(&(((MissionInfo*)g_vecMissionTable[midx])->mutex));



    char*skiphead=recvBuf;

    long headlength=0;

    while(1){
        if(*skiphead=='\n'&&*(skiphead-1)=='\r'&&*(skiphead-2)=='\n'&&*(skiphead-3)=='\r'){
            skiphead++;
            headlength++;
            break;
        }
        skiphead++;
        headlength++;
    }
//    pthread_mutex_lock(&(((MissionInfo*)g_vecMissionTable[midx])->mutex));
    _llCurrentPos=_llBeginPos;
//    pthread_mutex_unlock(&(((MissionInfo*)g_vecMissionTable[midx])->mutex));

    int file=open(parg->szFilename,O_CREAT|O_RDWR,S_IRWXU);

    if(file==-1){
        fprintf(stderr,"File Open Failure!\n");
        fprintf(stderr,"%s",parg->szFilename);
        exit(-1);
    }

    if(dr-headlength>_llEndPos){
        dw=pwrite(file,skiphead,_llEndPos-headlength,_llCurrentPos);
        pthread_mutex_lock(&(((MissionInfo*)g_vecMissionTable[midx])->mutex));

        ((MissionInfo*)(g_vecMissionTable[midx]))->m_lDoneBytes+=dw;
        ((MissionInfo*)(g_vecMissionTable[midx]))->m_lSpeedBytes=((MissionInfo*)(g_vecMissionTable[midx]))->m_lDoneBytes;
        //ti->lCurrentPos=_llCurrentPos;
        ti->lCurrentPos+=dw;

        pthread_mutex_unlock(&(((MissionInfo*)g_vecMissionTable[midx])->mutex));

    }else{
        dw=pwrite(file,skiphead,dr-headlength,_llCurrentPos);
        pthread_mutex_lock(&(((MissionInfo*)g_vecMissionTable[midx])->mutex));

        ((MissionInfo*)(g_vecMissionTable[midx]))->m_lDoneBytes+=dw;
        ((MissionInfo*)(g_vecMissionTable[midx]))->m_lSpeedBytes=((MissionInfo*)(g_vecMissionTable[midx]))->m_lDoneBytes;
        //ti->lCurrentPos=_llCurrentPos;
        ti->lCurrentPos+=dw;

        pthread_mutex_unlock(&(((MissionInfo*)g_vecMissionTable[midx])->mutex));
    }
    _llCurrentPos+=dw;

    while(_llCurrentPos<_llEndPos){

        //pthread_mutex_lock(&(((MissionInfo*)g_vecMissionTable[midx])->pauseMutex));

        dr=recv(sockdesc,recvBuf,4096,0);
////////

//        pthread_mutex_lock(&(((MissionInfo*)g_vecMissionTable[midx])->mutex));

//        ((MissionInfo*)(g_vecMissionTable[midx]))->m_lDoneBytes+=dr;

//        pthread_mutex_unlock(&(((MissionInfo*)g_vecMissionTable[midx])->mutex));

//////////
        if(dr+_llCurrentPos>*(parg->llContentLen)){
            dw=pwrite(file,recvBuf,*(parg->llContentLen)-headlength-_llCurrentPos,_llCurrentPos);
            pthread_mutex_lock(&(((MissionInfo*)g_vecMissionTable[midx])->mutex));

            ((MissionInfo*)(g_vecMissionTable[midx]))->m_lDoneBytes+=dw;
            ((MissionInfo*)(g_vecMissionTable[midx]))->m_lSpeedBytes=((MissionInfo*)(g_vecMissionTable[midx]))->m_lDoneBytes;
            //ti->lCurrentPos=_llCurrentPos;
            ti->lCurrentPos+=dw;

            pthread_mutex_unlock(&(((MissionInfo*)g_vecMissionTable[midx])->mutex));

            _llCurrentPos+=dw;
            break;
        }else{
            dw=pwrite(file,recvBuf,dr,_llCurrentPos);
            pthread_mutex_lock(&(((MissionInfo*)g_vecMissionTable[midx])->mutex));

            ((MissionInfo*)(g_vecMissionTable[midx]))->m_lDoneBytes+=dw;
            ((MissionInfo*)(g_vecMissionTable[midx]))->m_lSpeedBytes=((MissionInfo*)(g_vecMissionTable[midx]))->m_lDoneBytes;
            //ti->lCurrentPos=_llCurrentPos;
            ti->lCurrentPos+=dw;

            pthread_mutex_unlock(&(((MissionInfo*)g_vecMissionTable[midx])->mutex));
            _llCurrentPos+=dw;
        }

        //pthread_mutex_unlock(&(((MissionInfo*)g_vecMissionTable[midx])->pauseMutex));

    }

    close(file);
    free(sendBuf);
    free(recvBuf);

//    pthread_mutex_lock(&outputMutex);

//    ((MissionInfo*)(g_vecMissionTable[midx]))->m_lDoneBytes=;

//    pthread_mutex_unlock(&outputMutex);

}



void*rpartget(void*arg){

    struct pthreadArg*parg=(struct pthreadArg*)arg;
    struct ThreadInfo*ti=parg->ti;
    int midx=parg->midx;
    int tidx=parg->index;
    int dw=0,dr=0;
    long _llBeginPos=ti->lBeginPos;
    long _llEndPos=ti->lEndPos;
    long _llCurrentPos=ti->lCurrentPos;
    long _llSpeedSub=((MissionInfo*)(g_vecMissionTable[midx]))->m_lDoneBytes;
    //pthread_mutex_lock(&(((MissionInfo*)g_vecMissionTable[midx])->mutex));
    //ti->lCurrentPos=ti->lBeginPos;
    //pthread_mutex_unlock(&(((MissionInfo*)g_vecMissionTable[midx])->mutex));

    char*sendBuf=(char*)malloc(sizeof(char)*4096);
    char*recvBuf=(char*)malloc(sizeof(char)*4096);
    int sockdesc=socket(AF_INET,SOCK_STREAM,0);
    if(sockdesc==-1){
        fprintf(stderr,"Socket Creation Failure!\n");
        exit(-1);
    }

    if((connect(sockdesc,(const struct sockaddr*)(&ti->sin),sizeof(struct sockaddr_in)))==-1){
        fprintf(stderr,"Socket Connection Failure!\n");
        exit(-1);
    }
    bzero(sendBuf,4096);
    sprintf(sendBuf,GET_GRAM,parg->szURLname,parg->szHostname,"Snow",ti->lBeginPos);
    if((send(sockdesc,sendBuf,strlen(sendBuf),0))==-1){
        fprintf(stderr,"Header Sending Failure!\n");
        exit(-1);
    }


    if((dr=recv(sockdesc,recvBuf,4096,0))==-1){
        fprintf(stderr,"Header Recving Failure!\n");
        exit(-1);
    }

    ///////////

//    pthread_mutex_lock(&(((MissionInfo*)g_vecMissionTable[midx])->mutex));

//    ((MissionInfo*)(g_vecMissionTable[midx]))->m_lDoneBytes+=dr;

//    pthread_mutex_unlock(&(((MissionInfo*)g_vecMissionTable[midx])->mutex));



    char*skiphead=recvBuf;

    long headlength=0;

    while(1){
        if(*skiphead=='\n'&&*(skiphead-1)=='\r'&&*(skiphead-2)=='\n'&&*(skiphead-3)=='\r'){
            skiphead++;
            headlength++;
            break;
        }
        skiphead++;
        headlength++;
    }
//    pthread_mutex_lock(&(((MissionInfo*)g_vecMissionTable[midx])->mutex));
    _llCurrentPos=_llBeginPos;
//    pthread_mutex_unlock(&(((MissionInfo*)g_vecMissionTable[midx])->mutex));

    int file=open(parg->szFilename,O_CREAT|O_RDWR,S_IRWXU);

    if(file==-1){
        fprintf(stderr,"File Open Failure!\n");
        fprintf(stderr,"%s",parg->szFilename);
        exit(-1);
    }

    if(dr-headlength>_llEndPos){
        dw=pwrite(file,skiphead,_llEndPos-headlength,_llCurrentPos);
        pthread_mutex_lock(&(((MissionInfo*)g_vecMissionTable[midx])->mutex));

        ((MissionInfo*)(g_vecMissionTable[midx]))->m_lDoneBytes+=dw;
        ((MissionInfo*)(g_vecMissionTable[midx]))->m_lSpeedBytes=((MissionInfo*)(g_vecMissionTable[midx]))->m_lDoneBytes-_llSpeedSub;
        //ti->lCurrentPos=_llCurrentPos;
        ti->lCurrentPos+=dw;

        pthread_mutex_unlock(&(((MissionInfo*)g_vecMissionTable[midx])->mutex));

    }else{
        dw=pwrite(file,skiphead,dr-headlength,_llCurrentPos);
        pthread_mutex_lock(&(((MissionInfo*)g_vecMissionTable[midx])->mutex));
        long ld=((MissionInfo*)(g_vecMissionTable[midx]))->m_lDoneBytes;
        ((MissionInfo*)(g_vecMissionTable[midx]))->m_lDoneBytes+=dw;
        ((MissionInfo*)(g_vecMissionTable[midx]))->m_lSpeedBytes=((MissionInfo*)(g_vecMissionTable[midx]))->m_lDoneBytes-_llSpeedSub;
        //ti->lCurrentPos=_llCurrentPos;
        ti->lCurrentPos+=dw;

        pthread_mutex_unlock(&(((MissionInfo*)g_vecMissionTable[midx])->mutex));
    }
    _llCurrentPos+=dw;
    long ld=((MissionInfo*)(g_vecMissionTable[midx]))->m_lDoneBytes;

    while(_llCurrentPos<_llEndPos){

        //pthread_mutex_lock(&(((MissionInfo*)g_vecMissionTable[midx])->pauseMutex));

        dr=recv(sockdesc,recvBuf,4096,0);
////////

//        pthread_mutex_lock(&(((MissionInfo*)g_vecMissionTable[midx])->mutex));

//        ((MissionInfo*)(g_vecMissionTable[midx]))->m_lDoneBytes+=dr;

//        pthread_mutex_unlock(&(((MissionInfo*)g_vecMissionTable[midx])->mutex));

//////////
        if(dr+_llCurrentPos>*(parg->llContentLen)){
            dw=pwrite(file,recvBuf,*(parg->llContentLen)-headlength-_llCurrentPos,_llCurrentPos);
            pthread_mutex_lock(&(((MissionInfo*)g_vecMissionTable[midx])->mutex));

            ((MissionInfo*)(g_vecMissionTable[midx]))->m_lDoneBytes+=dw;
            ((MissionInfo*)(g_vecMissionTable[midx]))->m_lSpeedBytes=((MissionInfo*)(g_vecMissionTable[midx]))->m_lDoneBytes-_llSpeedSub;
            //ti->lCurrentPos=_llCurrentPos;
            ti->lCurrentPos+=dw;

            pthread_mutex_unlock(&(((MissionInfo*)g_vecMissionTable[midx])->mutex));

            _llCurrentPos+=dw;
            break;
        }else{
            dw=pwrite(file,recvBuf,dr,_llCurrentPos);
            pthread_mutex_lock(&(((MissionInfo*)g_vecMissionTable[midx])->mutex));

            ((MissionInfo*)(g_vecMissionTable[midx]))->m_lDoneBytes+=dw;
            ((MissionInfo*)(g_vecMissionTable[midx]))->m_lSpeedBytes=((MissionInfo*)(g_vecMissionTable[midx]))->m_lDoneBytes-_llSpeedSub;
            //ti->lCurrentPos=_llCurrentPos;
            ti->lCurrentPos+=dw;

            pthread_mutex_unlock(&(((MissionInfo*)g_vecMissionTable[midx])->mutex));
            _llCurrentPos+=dw;
        }

        //pthread_mutex_unlock(&(((MissionInfo*)g_vecMissionTable[midx])->pauseMutex));

    }

    close(file);
    free(sendBuf);
    free(recvBuf);

//    pthread_mutex_lock(&outputMutex);

//    ((MissionInfo*)(g_vecMissionTable[midx]))->m_lDoneBytes=;

//    pthread_mutex_unlock(&outputMutex);

}




int multiDownload(char*path,URLinfo*u,int num,int midx){

    MissionInfo*m=(MissionInfo*)(g_vecMissionTable[midx]);
    m->m_iThreadNum=num;
    m->m_stThreadTable=(ThreadInfo*)malloc(num*sizeof(ThreadInfo));

    ThreadInfo*tis=m->m_stThreadTable;
    pthreadArg*parg=(pthreadArg*)malloc(num*sizeof(pthreadArg));

    strncpy(u->szFilename,std::string(std::string(path)+std::string(u->szFilename)).c_str(),1024);
    
	strncpy(((MissionInfo*)(g_vecMissionTable[midx]))->m_szPath,u->szFilename,1024);
	strcpy(((MissionInfo*)(g_vecMissionTable[midx]))->m_szFile,u->szFilename);

    init_log(std::string(u->szFilename).c_str());

    //tis=(struct ThreadInfo*)malloc(sizeof(struct ThreadInfo)*num);
    parg=(struct pthreadArg*)malloc(sizeof(struct pthreadArg)*num);

    int i;
    for(i=0;i<num;i++){
        tis[i].lBeginPos=i*(u->lContentLen/num);
        tis[i].lEndPos=(i+1)*(u->lContentLen/num);
        if(i==num-1)tis[i].lEndPos=u->lContentLen;
    }

    long dr=0,dw=0;

    char*sendBuf=(char*)malloc(sizeof(char)*4096);
    char*recvBuf=(char*)malloc(sizeof(char)*4096);

    struct hostent*host=gethostbyname(u->szHostname);
    if(host==NULL){
        fprintf(stderr,"Resolve Hostname Failure!\n");
        exit(-1);
    }

    strncpy(u->szIPv4addr,inet_ntoa(*(struct in_addr*)host->h_addr),16);
    struct sockaddr_in*sock=(struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));
    bzero(sock,sizeof(struct sockaddr_in));
    sock->sin_family=AF_INET;
    sock->sin_addr.s_addr=inet_addr(u->szIPv4addr);
    sock->sin_port=htons(u->iPort);

    time_t t1,t2;

    time(&t1);

    for(i=0;i<num;i++){
        tis[i].sin=*sock;
        tis[i].msg=(char*)malloc(sizeof(char)*4096);
        sprintf(tis[i].msg,GET_GRAM,u->szURLname,u->szHostname,"Snow",tis[i].lBeginPos);
        parg[i].llContentLen=&(u->lContentLen);
        parg[i].szFilename=u->szFilename;
        parg[i].szHostname=u->szHostname;
        parg[i].szURLname=u->szURLname;
        parg[i].ti=&(tis[i]);
        parg[i].mllContentLen=&(m->m_lTotalBytes);
        parg[i].mllPosition=&(m->m_lDoneBytes);
        parg[i].mllSpeedLen=&(m->m_lSpeedBytes);
        parg[i].midx=midx;
        parg[i].index=i;
        pthread_create(&(tis[i].tid),NULL,partget,&(parg[i]));
    }

    int iSuccess=0;

    for(i=0;i<num;i++){
        pthread_join(tis[i].tid,NULL);
        iSuccess++;
    }

    time(&t2);

    if(iSuccess==num)printf("Download Success!Using Time:%d sec\n",(t2-t1));
    else printf("Download Failure!\n");

    free(sendBuf);
    free(recvBuf);
    //free(tis);
    free(parg);


    pthread_mutex_lock(&(((MissionInfo*)(g_vecMissionTable[midx]))->mutex));
    ((MissionInfo*)(g_vecMissionTable[midx]))->m_lDoneBytes=((MissionInfo*)(g_vecMissionTable[midx]))->m_lTotalBytes;
    pthread_mutex_unlock(&(((MissionInfo*)(g_vecMissionTable[midx]))->mutex));

    return iSuccess;

}








void*begin_mission(void*arg){

    MissionArg*marg=(MissionArg*)arg;
    long*l=new long;
    int*i=new int;
    pthread_mutex_lock(&finishMutex);
    ((MissionInfo*)g_vecMissionTable[marg->iMissionIndex])->m_bRunning=false;
    pthread_mutex_unlock(&finishMutex);

    URLinfo*u=new URLinfo;
    *l=preConnect(marg->szUrl,u,marg->iMissionIndex);
    *i=multiDownload(marg->szPath,u,marg->iThreadNum,marg->iMissionIndex);
    pthread_mutex_lock(&finishMutex);
    ((MissionInfo*)g_vecMissionTable[marg->iMissionIndex])->m_bRunning=true;
    pthread_mutex_unlock(&finishMutex);
    return i;
}

///////////////////
///  SIGNAL HANDLING SECTION
///////////////////

void sigalrm_handler(void);

void*signal_waiter(void*){

    int signal;
    pthread_sigmask(SIG_UNBLOCK,&signal_set,NULL);

//    while(1){
//        pthread_mutex_lock(&tableMutex);
//        for(int i=0;i<g_vecMissionTable.size();i++){
//            int cidx=((MissionInfo*)g_vecMissionTable[i])->m_iCompIndex;
//            int midx=((MissionInfo*)g_vecMissionTable[i])->m_iMissionIndex;
//            pthread_mutex_lock(&outputMutex);
//            double pq=(double)(((MissionInfo*)g_vecMissionTable[i])->m_lDoneBytes)/(double)(((MissionInfo*)g_vecMissionTable[i])->m_lTotalBytes);
//            ((MissionBar*)compMissionBarTable[cidx])->setProgressBar(pq);
//            pthread_mutex_unlock(&outputMutex);
//        }
//        pthread_mutex_unlock(&tableMutex);
//        sleep(1);
//    }


//    while(1){
//        sigwait(&signal_set,&signal);
//        switch(signal){
//            case SIGINT:
//                ;
//                break;
//            case SIGALRM:
//                sigalrm_handler();
//                break;

//        }
//    }

}

void sigint_handler(void){

}


void sigalrm_handler(void){

//    for(int i=0;i<g_iMissionNum;i++){
//        fprintf(stderr,"Progress:%ld / %ld\n",((MissionInfo*)g_vecMissionTable[i])->m_lDoneBytes,((MissionInfo*)g_vecMissionTable[i])->m_lTotalBytes);
//    }
//    /// DO SOME UPDATING!
//    pthread_mutex_lock(&tableMutex);
//    for(int i;g_vecMissionTable.size();i++){
//        pthread_mutex_lock(&outputMutex);
//        int cidx=((MissionInfo*)g_vecMissionTable[i])->m_iCompIndex;
//        double pq=(double)(((MissionInfo*)g_vecMissionTable[i])->m_lDoneBytes)/(double)(((MissionInfo*)g_vecMissionTable[i])->m_lTotalBytes);
//        ((MissionBar*)compMissionBarTable[cidx])->setProgressBar(pq);
//        pthread_mutex_unlock(&outputMutex);
//    }
//    pthread_mutex_unlock(&tableMutex);
//    alarm(1);
}

void init_log(const char*fullpathname){
	std::string logpath(std::string(fullpathname)+std::string(".log"));

	FILE*file=fopen(logpath.c_str(),"w+");
	fprintf(file,"SNOWLOG\n");
	fclose(file);
}


void write_log(char*fullpathname,MissionInfo*minfo){

	std::string logpath(std::string(fullpathname)+std::string(".log"));
    init_log(fullpathname);

	FILE*file=fopen(logpath.c_str(),"a+");
	int tn=minfo->m_iThreadNum;
	char filename[1024]={};
	char urlname[1024]={};

	strncpy(filename,minfo->m_szPath,1024);
	strncpy(urlname,minfo->m_szURL,1024);

	struct ThreadInfo*tinfo=minfo->m_stThreadTable;
	
	fprintf(file,"%s\n",filename);
    fprintf(file,"%s\n",urlname);
    fprintf(file,"%d\n",minfo->m_iThreadNum);
	pthread_mutex_lock(&(minfo->mutex));
    fprintf(file,"LENGTH:%ld,%ld\n",minfo->m_lTotalBytes,minfo->m_lDoneBytes);
	pthread_mutex_unlock(&(minfo->mutex));
	for(int i=0;i<tn;i++){
    	pthread_mutex_lock(&(minfo->mutex));
        fprintf(file,"%ld,%ld,%ld\n",tinfo[i].lBeginPos,tinfo[i].lEndPos,tinfo[i].lCurrentPos);
		pthread_mutex_unlock(&(minfo->mutex));
	}
	fprintf(file,"SNOWLOGEND");
	fclose(file);
}



bool validate_log(const char*fullpathname);

void read_log(const char*fullpathname,MissionInfo*minfo){

	//FULLPATHNAME Already contained XX.log
	FILE*file=fopen(fullpathname,"r");
    MissionInfo*mission=minfo;
    ThreadInfo*tis=(ThreadInfo*)malloc(sizeof(ThreadInfo)*(mission->m_iThreadNum));
	mission->m_stThreadTable=tis;

    bool bVal=validate_log(fullpathname);
	if(!bVal)return;
	char tmp[1024]={};
	fgets(tmp,1024,file);//Validate Head
	bzero(tmp,1024);
	
	fgets(tmp,1024,file);//
	sscanf(tmp,"%s",mission->m_szPath);
	bzero(tmp,1024);
	
	fgets(tmp,1024,file);//
	sscanf(tmp,"%s",mission->m_szURL);
	bzero(tmp,1024);
	
	fgets(tmp,1024,file);//
	sscanf(tmp,"%d",&(mission->m_iThreadNum));
	bzero(tmp,1024);
	
	fgets(tmp,1024,file);//
    sscanf(tmp,"LENGTH:%ld,%ld",&(mission->m_lTotalBytes),&(mission->m_lDoneBytes));
	bzero(tmp,1024);

	for(int i=0;i<(mission->m_iThreadNum);i++){
		fgets(tmp,1024,file);
        //pthread_mutex_lock(&(mission->mutex));
        ///Debug Temp Value
        long l1,l2,l3;
        //sscanf(tmp,"%ld,%ld,%ld",&(tis[i].lBeginPos),&(tis[i].lEndPos),&(tis[i].lCurrentPos));
        sscanf(tmp,"%ld,%ld,%ld",&l1,&l2,&l3);
        tis[i].lBeginPos=l1;
        tis[i].lEndPos=l2;
        tis[i].lCurrentPos=l3;
        tis[i].parentMission=mission;
		bzero(tmp,1024);
	}

	fclose(file);
}

bool validate_log(const char*fullpathname){
	 FILE*file=fopen(fullpathname,"r");
	 bool start=false,end=false;
	 char tmp[1024]={};
	 fgets(tmp,1024,file);
     if(strncmp(tmp,"SNOWLOG",7)==NULL){
		start=true;
	 }else{
	 	return false;	
	 }
	 bzero(tmp,1024);
	 while(NULL!=fgets(tmp,1024,file)){
        if(strncmp(tmp,"SNOWLOGEND",10)){
			end=true;
		}
	 }
	if(start&&end)return true;
	else return false;

	fclose(file);
}


void*resumeDownload(void*arg){
	// the parameter is assigned by 'read_log' function

    MissionInfo*mission=(MissionInfo*)arg;

    URLinfo*u=new URLinfo;

    preConnect(mission->m_szURL,u,mission->m_iMissionIndex);

    MissionInfo*m=mission;

    pthread_mutex_lock(&timeMutex);
    m->m_lConsumeTime=10;
    pthread_mutex_unlock(&timeMutex);

    pthread_mutex_lock(&finishMutex);
    (m)->m_bRunning=false;
    pthread_mutex_unlock(&finishMutex);

    pthread_mutex_init(&(m->mutex),NULL);


    read_log(std::string(std::string(mission->m_szPath)+std::string(".log")).c_str(),m);


    //m->m_iThreadNum=num;
    //m->m_stThreadTable=(ThreadInfo*)malloc((m->m_iThreadNum)*sizeof(ThreadInfo));

    ThreadInfo*tis=m->m_stThreadTable;
    pthreadArg*parg=(pthreadArg*)malloc((m->m_iThreadNum)*sizeof(pthreadArg));

//    strncpy(u->szFilename,std::string(std::string(path)+std::string(u->szFilename)).c_str(),1024);

//    strncpy(((MissionInfo*)(g_vecMissionTable[midx]))->m_szPath,u->szFilename,1024);
//    strcpy(((MissionInfo*)(g_vecMissionTable[midx]))->m_szFile,u->szFilename);

    init_log(std::string(u->szFilename).c_str());

    //tis=(struct ThreadInfo*)malloc(sizeof(struct ThreadInfo)*num);
    parg=(struct pthreadArg*)malloc(sizeof(struct pthreadArg)*(m->m_iThreadNum));

    int i;

    long ltmptotal=0;
    for(i=0;i<m->m_iThreadNum;i++){
        pthread_mutex_lock(&(m->mutex));
        ltmptotal+=(m->m_stThreadTable[i].lCurrentPos-m->m_stThreadTable[i].lBeginPos);
        pthread_mutex_unlock(&(m->mutex));
    }

    pthread_mutex_lock(&(m->mutex));
    m->m_lDoneBytes=ltmptotal;
    m->m_lSpeedBytes=m->m_lDoneBytes;
    pthread_mutex_unlock(&(m->mutex));

    long dr=0,dw=0;

    char*sendBuf=(char*)malloc(sizeof(char)*4096);
    char*recvBuf=(char*)malloc(sizeof(char)*4096);

    struct hostent*host=gethostbyname(u->szHostname);
    if(host==NULL){
        fprintf(stderr,"Resolve Hostname Failure!\n");
        exit(-1);
    }

    strncpy(u->szIPv4addr,inet_ntoa(*(struct in_addr*)host->h_addr),16);
    struct sockaddr_in*sock=(struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));
    bzero(sock,sizeof(struct sockaddr_in));
    sock->sin_family=AF_INET;
    sock->sin_addr.s_addr=inet_addr(u->szIPv4addr);
    sock->sin_port=htons(u->iPort);

    time_t t1,t2;

    time(&t1);

    strncpy(u->szFilename,m->m_szPath,1024);

    for(i=0;i<(m->m_iThreadNum);i++){
        tis[i].sin=*sock;
        tis[i].msg=(char*)malloc(sizeof(char)*4096);
        sprintf(tis[i].msg,GET_GRAM,u->szURLname,u->szHostname,"Snow",tis[i].lBeginPos);
        parg[i].llContentLen=&(u->lContentLen);
        parg[i].szFilename=u->szFilename;
        parg[i].szHostname=u->szHostname;
        parg[i].szURLname=u->szURLname;
        parg[i].ti=&(tis[i]);
        parg[i].mllContentLen=&(m->m_lTotalBytes);
        parg[i].mllPosition=&(m->m_lDoneBytes);
        parg[i].mllSpeedLen=&(m->m_lSpeedBytes);
        parg[i].midx=m->m_iMissionIndex;
        parg[i].index=i;
        pthread_create(&(tis[i].tid),NULL,rpartget,&(parg[i]));
    }

    int iSuccess=0;

    for(i=0;i<(m->m_iThreadNum);i++){
        pthread_join(tis[i].tid,NULL);
        iSuccess++;
    }

    time(&t2);

    if(iSuccess==(m->m_iThreadNum))printf("Download Success!Using Time:%d sec\n",(t2-t1));
    else printf("Download Failure!\n");

    free(sendBuf);
    free(recvBuf);
    //free(tis);
    free(parg);


    pthread_mutex_lock(&(m->mutex));
    m->m_lDoneBytes=m->m_lTotalBytes;
    pthread_mutex_unlock(&(m->mutex));

}



