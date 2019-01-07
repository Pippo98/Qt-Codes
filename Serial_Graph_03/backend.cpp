#include "backend.h"
#include "serial.h"
#include "graph.h"

static serial s;
static graph  g;

static QVector<int> requestedGraphs;

backend::backend(QObject *parent) : QObject(parent)
{
}

void backend::comboChanged(QString active_member, int index, int identifier){
    switch (identifier) {
    case 0:
        s.serialPortIndex = index;
        s.serialPortSelected = active_member;
        break;
    }
}

void backend::buttonClicked(int id, int value){
    switch (id) {
    case 0:
        if(value == 0){
            s.init();
            s.connection();
            g.connections();
            emit portStateChanged(1);
        }
        else{
            s.deInit();
            g.deInit();
            emit portStateChanged(0);
        }
        break;
    default:
        break;

    }
}

void backend::logSwitchChanged(int value){
    s.setLogState(value);
}

void backend::switchChanged(int id, int value){

    while(requestedGraphs.count() <= id){
        requestedGraphs.append(1);
    }
    requestedGraphs[id] = value;
    s.setGraphsRequested(requestedGraphs);

    switch(id){
    case 0:
        break;
    case 1:
        break;
    case 2:
        break;
    }
}

QVector<int> backend::getRequestedGraphs(){
    return requestedGraphs;
}
