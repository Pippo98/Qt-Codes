#include "serial.h"
#include "graph.h"

#include <QDebug>
#include <QObject>
#include <QtCharts/QXYSeries>
#include <QtCharts/QLineSeries>
#include <QtSerialPort>
#include <QTimer>
#include <QtCore>
#include <QMutex>

QT_CHARTS_USE_NAMESPACE

Q_DECLARE_METATYPE(QAbstractSeries *)
Q_DECLARE_METATYPE(QAbstractAxis *)

static QTimer *tim1 = new QTimer();
static QTime  *Time = new QTime();

static QVector<QXYSeries *> castedSeriesArray;
static QVector<QPair<QAbstractAxis *, QAbstractAxis *>> axisArray;

static double x;

static QList<QString> graphsNames;
static QList<QVector<QPointF>> pointsList;
static QVector<double> dataArray;
static QVector<int> secondaryGaphSelection;

static int maxPoints;

static bool CAN_MODE = true;
static bool isSerialOpened;

static int maxIndexes[2];
static int minIndexes[2];
static int yMaxIndexes[1000];
static int yMinIndexes[1000];

static bool update;
static bool threadActive = true;

static int frameRate = 20;

static serial s;
static backend b;
static QMutex mute;

graph::graph(QObject * parent) : QThread (parent)
{
    qRegisterMetaType<QAbstractSeries*>();
    qRegisterMetaType<QAbstractAxis*>();
}

//Deconstructor
graph::~graph(){
    threadActive = false;
    this->exit();
}

//adds the received point to the points list
void graph::managePoints(QVector<double> data){
    dataArray = data;

    while(pointsList.count() < 20){
        QVector<QPointF> points;
        pointsList.append(points);
    }

    for(int i = 0; i < dataArray.count(); i++){
        pointsList[i].append(QPointF(x, dataArray[i]));
        while(pointsList.at(i).count() > maxPoints){
            pointsList[i].remove(0);
        }
    }

    update = true;
}

//appends to the array all the pointers to the line series of the qml chart
void graph::setSeriesArray(QAbstractSeries * ser, QAbstractAxis * axisX, QAbstractAxis * axisY){
    if(ser)
    castedSeriesArray.append(dynamic_cast<QXYSeries *>(ser));
    if(axisX && axisY)
    axisArray.append(qMakePair(axisX, axisY));
}

//access to the array of pointers to the line series of the qml chart
//updates the points and changes the names
void graph::updateSeries(){
    if(isSerialOpened && dataArray.count() > 0){
        for(int i = 0; i < castedSeriesArray.count(); i++){
            if(i < pointsList.count() && i < dataArray.count()){
                castedSeriesArray.at(i)->replace(pointsList.at(i));
                if(!castedSeriesArray.at(i)->isVisible()){
                    castedSeriesArray.at(i)->setVisible(true);
                }
                if(CAN_MODE){
                    if(i < graphsNames.count() && graphsNames.at(i) != castedSeriesArray.at(i)->name()){
                        castedSeriesArray.at(i)->setName(graphsNames.at(i));
                    }
                }
            }
            else{
                if(castedSeriesArray.at(i)->isVisible()){
                    castedSeriesArray.at(i)->setVisible(false);
                }
                else{
                    break;
                }
            }
        }
    }
}

//thread function
//only if new data are available update the line series in the qml chart using the pointers
void graph::run(){
    while (threadActive) {
        mute.lock();
        if(isSerialOpened && dataArray.count() > 0 && update){
            for(int i = 0; i < castedSeriesArray.count(); i++){
                if(i < pointsList.count() && i < dataArray.count()){
                    castedSeriesArray.at(i)->replace(pointsList.at(i));
                    if(!castedSeriesArray.at(i)->isVisible()){
                        castedSeriesArray.at(i)->setVisible(true);
                    }
                    if(CAN_MODE){
                        if(i < graphsNames.count() && graphsNames.at(i) != castedSeriesArray.at(i)->name()){
                            castedSeriesArray.at(i)->setName(graphsNames.at(i));
                        }
                    }
                    setAxis(0, 0);
                }
                else{
                    if(castedSeriesArray.at(i)->isVisible()){
                        castedSeriesArray.at(i)->setVisible(false);
                    }
                    else{
                        break;
                    }
                }
            }
            update = false;
        }
        mute.unlock();
        this->msleep(u_int64_t(frameRate));
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

//this function accesses to the list of pair pointers of the x and y axis of all the graphs of the qml chart
//then simply sets the max and min values basing on the argument given
//x_y is to know if is requested to refresh the x or y axis (x -> x_y = 0||||| y -> x_y = 1)
//single_total is needed to know if the range will be common to all the graphs in the chart or
//if every signle graph will have his own range (common range -> single_total = 1 ||||| different ranges -> single_total = 0)
void graph::setAxis(int x_y, int single_total){

    if(x_y == 1){
        setGeneralYRange();
        setSpecificYRange();
    }

    for(int i = 0; i < axisArray.count(); i++){
        if(i < dataArray.count() && i < pointsList.count() && pointsList.at(i).count() != 0){
            if(single_total == 0){
                if(x_y == 0){                                                           //X axis
                    axisArray.at(i).first->setMin(pointsList.at(i).first().x());
                    axisArray.at(i).first->setMax(pointsList.at(i).last().x());
                }
                if(x_y == 1){                                                           //Y axis
                    double max;
                    double min;

                    min = pointsList.at(i).at(yMinIndexes[i]).y();
                    max = pointsList.at(i).at(yMaxIndexes[i]).y();

                    axisArray.at(i).second->setMin(min * 1.1);
                    axisArray.at(i).second->setMax(max * 1.1);
                }
            }
            //same range for all the grahs in the chart
            if(single_total == 1){
                if(x_y == 0){                                                           //X axis
                    axisArray.at(i).first->setMin(pointsList.at(0).at(0).x());
                    axisArray.at(i).first->setMax(pointsList.at(0).at(pointsList.at(0).count()-1).x());
                }
                if(x_y == 1){                                                           //Y axis
                    axisArray.at(i).second->setMin(pointsList.at(minIndexes[0]).at(minIndexes[1]).y());
                    axisArray.at(i).second->setMax(pointsList.at(maxIndexes[0]).at(maxIndexes[1]).y());
                }
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
    if(index < pointsList.count() && index < dataArray.count() && isSerialOpened){
        //different range for the graphs in the chart
        if(single_total == 0){
            if(x_y == 0){                                                           //X axis
                axis->setMin(pointsList.at(index).at(0).x());
                axis->setMax(pointsList.at(index).at(pointsList.at(index).count()-1).x());
            }
            if(x_y == 1){                                                           //Y axis
                double max;
                double min;

                max = pointsList.at(index).at(yMaxIndexes[index]).y();
                min = pointsList.at(index).at(yMinIndexes[index]).y();

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

//--------------------------------------INIT-DEINIT Functions--------------------------------------//
void graph::deInit(){
    disconnect(tim1, &QTimer::timeout, nullptr, nullptr);
}

void graph::restartSequence(){
    for(int i = 0; i < pointsList.count(); i++){
        pointsList[i].clear();
    }
    Time->restart();
}

//sets the x value basing on the time (10 ms)
void graph::connections(){
    x = 0;
    tim1->setInterval(10);
    tim1->start();
    QObject::connect(tim1, &QTimer::timeout, [=]{
        x += 0.01;
    });
}

//--------------------------------------SET-GET Functions--------------------------------------//

void graph::setSecondarySwitchesSelections(QVector<int> value){
    secondaryGaphSelection = value;
}

void graph::setSecondaryGraphSelection(QVector<int> values){
    secondaryGaphSelection = values;
}

void  graph::setIsSerialOpened(bool value){
    isSerialOpened = value;
}

void graph::setCanMode(bool value){
    CAN_MODE = value;
}

void graph::setGraphsNames(QList<QString> names){
    graphsNames = names;
}

void graph::setMaxPoints(int maxP){
    maxPoints = maxP;
}

void graph::setFrameRate(int value){
    frameRate = value;
}



