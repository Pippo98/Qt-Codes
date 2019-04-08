#include "backend.h"
#include "serial.h"
#include "graph.h"

#include <QQuickView>

static serial s;
static graph  g;

static QVector<QPair<int, QVector<int>>> requestedGraphs;
static QVector<QPair<int, QVector<int>>> switchSelections;

//-----------
static QVector<int> primaryGraphSelection;
static QVector<QVector<bool>> secondaryGraphsSelection;
static QVector<QPair<QString, int>> secondarySwitch;

backend::backend(QObject *parent) : QObject(parent)
{
}
void backend::sliderChanged(int id, int value){
    if(id == 0){
        g.setMaxPoints(value);
    }
}

void backend::comboChanged(QString active_member, int index, int identifier){
    switch (identifier) {
    case 0:
        s.serialPortIndex = index;
        s.serialPortSelected = active_member;
        break;
    case 1:
        s.serialPortBaudrate = active_member.toInt();
        break;
    }
}

void backend::buttonClicked(int id, int value){
    switch (id) {
    case 0:
        if(value == 0){
            emit portStateChanged(s.init());
            s.connection();
            g.connections();
        }
        else{
            emit portStateChanged(s.deInit());
            g.deInit();
        }
        break;
    case 2:
        s.displayHelp();
        break;
    default:
        break;

    }
}

void backend::switchChanged(int id, int value){
    switch(id){
    case 0:
        s.setLogState(value);
        break;
    case 1:
        s.setCanMode(value);
        s.restartSequence();
        break;
    }
}

void backend::logSwitchChanged(int value){
}















