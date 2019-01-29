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















