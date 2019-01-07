#include "serial.h"
#include "graph.h"

#include <QDebug>
#include <QObject>
#include <QtCharts/QXYSeries>
#include <QtCharts/QLineSeries>
#include <QtSerialPort>
#include <QTimer>

QT_CHARTS_USE_NAMESPACE

Q_DECLARE_METATYPE(QAbstractSeries *)
Q_DECLARE_METATYPE(QAbstractAxis *)

static QTimer *tim1 = new QTimer();
static QTime  *Time = new QTime();

static double x;

static QList<QString> graphsNames;
static QList<QVector<QPointF>> pointsList;
static QVector<double> dataArray;

static bool CAN_MODE = true;
static bool isSerialOpened;

static serial s;
static backend b;

graph::graph(){
    qRegisterMetaType<QAbstractSeries*>();
    qRegisterMetaType<QAbstractAxis*>();
}

//adds the received point to the points list
void graph::managePoints(){
    dataArray = s.getPointsData();

    while(pointsList.count() < 20){
        QVector<QPointF> points;
        pointsList.append(points);
    }

    for(int i = 0; i < dataArray.count(); i++){
        pointsList[i].append(QPointF(x, dataArray[i]));
        if(pointsList.at(i).count() > 1000){
            pointsList[i].remove(0);
        }
    }
}

//this function updates the values of the line series in the qml
//series is an abstract value that points to the qml line series
//using mSeries is possible to assign c++ values to qml series
void graph::printCoord(QAbstractSeries * series, int index){
    if(series){
        mSeries = static_cast<QXYSeries *>(series);
        if(isSerialOpened && dataArray.count() > 0){
            if(index < pointsList.count() && index < dataArray.count()){
                mSeries->replace(pointsList.at(index));
                mSeries->setVisible(true);
                qDebug() << (x*1000 - Time->elapsed());
                if(CAN_MODE){
                    if(index < graphsNames.count()){
                        mSeries->setName(graphsNames.at(index));
                    }
                }
            }
            else{
                mSeries->setVisible(false);
            }
        }
    }
}

//this function detects the indexes of the max and min values on the graph
//detects one max and one min for every existing graph
//the range calculated is relative
void graph::setSpecificYRange(){
    for(int i = 0; i < pointsList.count(); i ++){
        double max = -100000000;
        double min = 100000000;
        for (int j = 0; j < pointsList.at(i).count(); j ++){
            if(pointsList.at(i).at(j).y() > max){
                max = pointsList.at(i).at(j).y();
                yMaxIndexes[i] = j;
            }
            if(pointsList.at(i).at(j).y() < min){
                min = pointsList.at(i).at(j).y();
                yMinIndexes[i] = j;
            }
        }
    }
}

//this function detects the index of the max and min values on the graps
//there is only one max and one min for all the graphs
//the range calculated is absolute
void graph::setGeneralYRange(){
    double max = -100000000;
    double min = 100000000;
    QVector<QPointF> points;
    for(int i = 0; i < pointsList.count(); i++){
        for(int j = 0; j < pointsList.at(i).count(); j ++){
            points = pointsList.at(i);
            if(points.at(j).y() > max){
                max = points.at(j).y();
                maxIndexes[0] = i;
                maxIndexes[1] = j;
            }
            if(points.at(j).y() < min){
                min = points.at(j).y();
                minIndexes[0] = i;
                minIndexes[1] = j;
            }
        }
    }
}

//sets x and y axis.
//index is the index of the graph attached to this axis.
//x_y defines if will be set the x or the y axis.
//single or total defines id the y scaling will be done from the max anx min of the single graph
//attached to that axis or one single range for all the graphs.
void graph::getAxisValues(QAbstractAxis * axis, int index, int x_y, int single_total){
    if(index < pointsList.count() && index < dataArray.count() && s.isSerialOpened() == true){
        //different range for the graphs in the chart
        if(single_total == 0){
            if(x_y == 0){                                                           //X axis
                axis->setMin(pointsList.at(index).at(0).x());
                axis->setMax(pointsList.at(index).at(pointsList.at(index).count()-1).x());
            }
            if(x_y == 1){                                                           //Y axis
                double max;
                double min;

                min = pointsList.at(index).at(yMinIndexes[index]).y();
                max = pointsList.at(index).at(yMaxIndexes[index]).y();

                axis->setMin(min * 1.1);
                axis->setMax(max * 1.1);
            }
        }
        //same range for all the grahs in the chart
        if(single_total == 1){
            if(x_y == 0){                                                           //X axis
                axis->setMin(pointsList.at(0).at(0).x());
                axis->setMax(pointsList.at(0).at(pointsList.at(0).count()-1).x());
            }
            if(x_y == 1){                                                           //Y axis
                axis->setMin(pointsList.at(minIndexes[0]).at(minIndexes[1]).y());
                axis->setMax(pointsList.at(maxIndexes[0]).at(maxIndexes[1]).y());
            }
        }
    }
}

//INIT-DEINIT Functions
void graph::deInit(){
    disconnect(tim1, &QTimer::timeout, nullptr, nullptr);
}

void graph::restartSequence(){
    pointsList.clear();
    Time->restart();
}

//sets the x value basing on the time (10 ms)
void graph::connections(){
    tim1->setInterval(10);
    tim1->start();
    QObject::connect(tim1, &QTimer::timeout, [=]{
        x += 0.01;
    });
}

//SET-GET Functions

void  graph::setIsSerialOpened(bool value){
    isSerialOpened = value;
}

void graph::setCanMode(bool value){
    CAN_MODE = value;
}

void graph::setGraphsNames(QList<QString> names){
    graphsNames = names;
}




