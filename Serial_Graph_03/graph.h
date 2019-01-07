#ifndef GRAPH_H
#define GRAPH_H

#include <QObject>
#include <QVector>
#include <QtCharts/QAbstractSeries>
#include <QtCharts/QAbstractAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QXYSeries>
#include <QtCharts/QLineSeries>
#include "serial.h"
#include "backend.h"

QT_CHARTS_USE_NAMESPACE

class graph : public QObject
{
    Q_OBJECT
public:
    explicit graph();
    void connections();
    void deInit();
    void managePoints();
    void setGraphsNames(QList<QString>);
    void setIsSerialOpened(bool);
    void setCanMode(bool);
    void restartSequence();

public slots:
    void printCoord(QAbstractSeries *, int);
    void setGeneralYRange();
    void setSpecificYRange();
    void getAxisValues(QAbstractAxis *, int, int, int);

public:
    QString portOpened;

    QXYSeries *mSeries;
    QValueAxis *valueAxis;

    int totalGraphs;
    int maxPoints = 400;
    int maxIndexes[3];
    int minIndexes[3];
    int yMaxIndexes[1000];
    int yMinIndexes[1000];
};

#endif // GRAPH_H
