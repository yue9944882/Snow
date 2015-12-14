
#include "missioncheck.h"

MissionCheck::MissionCheck(QWidget*parent,int cidx):QCheckBox(parent){
    this->compIndex=cidx;
}

void MissionCheck::adjustPosition(){
    this->move(2,3+45*this->compIndex);
}


MissionCheck::~MissionCheck(){

}
