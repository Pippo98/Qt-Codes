#include "graph.h"
#include "serial.h"
#include "backend.h"

#include <QDir>
#include <QFile>
#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

static QFile *fil = new QFile();
static QTimer *tim1 = new QTimer();

static graph g;
static backend b;
static serial *s = new serial;

/*----------------------------------------------------------------------------------------------*/
//THESE VARIABLES COULD BE MODIFIED

//default graph mode
static bool CAN_MODE = true;

//Directory of the log file
static QString dirName = "/home/filippo/Scrivania/logFolder";

//graphs names
//has to be in the same order of how are appended the variables in canArr in parseCan() function
static QList<QString> Frontal   = {"enc1", "enc2", "aX", "aY", "aZ", "gX", "gY", "gZ", "steer"};
static QList<QString> Pedals    = {"apps1", "apps2", "brk"};
static QList<QString> Ecu       = {"cmd1", "cmd2"};
static QList<QString> Inverter  = {"temp1", "temp2"};
static QList<QString> Lv        = {"volts", "amps"};
static QList<QString> Hv        = {"volts", "amps", "temp"};

static QStringList              switchNames = {"Frontal", "Pedals", "ECU", "Inverter", "Lv", "Hv"};
static QVector<int>             switchesSelections;
static QList<QStringList>       secondarySwitchNames = {Frontal, Pedals, Ecu, Inverter, Lv, Hv};
static QVector<QVector<int>>    secondarySwitchSelections;
static QVector<int>             graphSelection;
/*----------------------------------------------------------------------------------------------*/

static bool logState = false;

static bool isPortOpened;
static double average = 0;
static int iterations = 0;
static bool doneCalibration = false;

static QVector<double> canArr;
static QVector<double> dataArray;

static QList<QString> graphsNames;
static QList<QString> grph = {Frontal + Pedals + Ecu + Inverter + Lv + Hv};

static QVector<QPair<int, QVector<int>>> switchesState;

static QVector<QVector<int>> secondarySwitches;
/*----------------------------------------------------------------------------------------------*/
//Structures

struct frontalStc{
    double encoder1;
    double encoder2;
    double ax;
    double ay;
    double az;
    double gx;
    double gy;
    double gz;
    double steer;
};

struct pedalsStc{
    double apps1;
    double apps2;
    double brk;
};

struct ecuStc{

};

struct inverterStc{

};

struct lvAccumulatorStc{

};

struct hvAccumulatorStc{

};

static struct frontalStc       frontal;
static struct pedalsStc        pedals;
static struct ecuStc           ecu;
static struct inverterStc      inverter;
static struct lvAccumulatorStc lvAccumulator;
static struct hvAccumulatorStc hvAccumulator;

/*----------------------------------------------------------------------------------------------*/

serial::serial(QObject * parent) : QObject (parent)
{
}
serial::~serial(){
    if(fil->isOpen())
    fil->close();
}

//main function to manage the program flow
void serial::manageFunctions(){
    while(serialPort->bytesAvailable() > 50){
        serialData = serialPort->readLine();
        if(logState)
        fil->write(serialData);
    }

    if(!doneCalibration && !CAN_MODE){
        detectGraphs();
    }
    if(doneCalibration || CAN_MODE){
        parseData();

        if(CAN_MODE){
            parseCan();
            g.managePoints(canArr);
        }
        else{
            g.managePoints(dataArray);
        }
    }
}

//differentiate the data from different id
void serial::parseCan(){
    canArr.clear();
    if(dataArray.count() >= 8){
        //PEADLS
        if(int(dataArray[1]) == 176){
            if(int(dataArray[2]) == 1){
                pedals.apps1 = dataArray[3];
                pedals.apps2 = dataArray[4];
            }
            if(int(dataArray[2]) == 2){
                pedals.brk = dataArray[3];
            }
        }
        //FRONTAL
        if(int(dataArray[1]) == 192){
            if(int(dataArray[2]) == 2){
                frontal.steer = dataArray[3];
            }
        }
        if(int(dataArray[1]) == 208){
            if(int(dataArray[2]) == 6){
                frontal.encoder1 = dataArray[3];
            }
            if(int(dataArray[2]) == 7){
                frontal.encoder2 = dataArray[3];
            }
        }
    }

    if(switchesSelections.count() > 2){
        //FRONTAL
        if(switchesSelections.at(0) == 1){
            canArr.append(frontal.encoder1);
            canArr.append(frontal.encoder2);
            canArr.append(frontal.ax);
            canArr.append(frontal.ay);
            canArr.append(frontal.az);
            canArr.append(frontal.gx);
            canArr.append(frontal.gy);
            canArr.append(frontal.gz);
            canArr.append(frontal.steer);
        }
        //PEDALS
        if(switchesSelections.at(1) == 1){
            canArr.append(pedals.apps1);
            canArr.append(pedals.apps2);
            canArr.append(pedals.brk);
        }
        //ECU
        if(switchesSelections.at(2)== 1){

        }/*
        //INVERTER
        if(switchesSelections.at(3) == 1){

        }
        //LOW VOLTAGE
        if(switchesSelections.at(4) == 1){

        }
        //HIGH VOLTAGE
        if(switchesSelections.at(5) == 1){

        }*/
        QVector<double> buff;
        for(int i = 0; i < graphSelection.count(); i++){
            if(graphSelection.at(i) == 1 && i < canArr.count())
            buff.append(canArr.at(i));
        }
        canArr = buff;
    }
}

//function to get all the numbers in the buffer received
void serial::parseData(){

    QStringList numberArr;
    QByteArray number;
    QList<QByteArray> dataSplitted;

    dataSplitted = serialData.split('\t');

    if(dataSplitted.count() == g.totalGraphs || CAN_MODE){            //if the numbers found in the received string are te same number ad the average found then do the cast fo float
        for(int i = 0; i < dataSplitted.count(); i++){
            if(dataArray.length() < dataSplitted.count()){
                dataArray.append(dataSplitted[i].toDouble());
            }
            else{
                dataArray.replace(i, dataSplitted[i].toDouble());
            }
        }
    }
    else{
        qDebug() << "wrong";
    }
}

//initialization
//sometimes the read buffer iss not correct
//do an average of the numbers found in the firsts strings
//this average is the number of the graphs that can be visible in the chart
void serial::detectGraphs(){
    int counter = 0;

    for(int i = 0; i < serialData.length(); i++){
        if(serialData[i] == '\t' || serialData[i] == '\n'){
            counter ++;
        }
    }
    average += counter;
    if(iterations == 19){
        average /= 20;
        average = round(average);
        g.totalGraphs = int(average);
        doneCalibration = true;
    }
    iterations ++;
}

//function to detect the available ports
//this is called fron the qml to set the combo box
QStringList serial::detectPort(){
    QStringList port_list;
    const auto serial = QSerialPortInfo::availablePorts();

    port_list.append("      ");

    for(const QSerialPortInfo &ser_ : serial){
        port_list.append(ser_.portName());
    }

    serialPorts = port_list;

    return port_list;
}

//gets the info of the port given as argument
QString serial::portInfo(QString port){
    QString info;
    const auto serialPortInfos = QSerialPortInfo::availablePorts();
    for(int i = 0; i < serialPortInfos.count(); i++) {
        if(serialPortInfos.at(i).portName() == port){
            const QSerialPortInfo &serialPortInfo = serialPortInfos.at(i);
            info.append(serialPortInfo.description());
            info.append("\r\n");
            info.append(serialPortInfo.manufacturer());
        }
    }
    return info;
}

void serial::updateGraphsNames(){
    graphsNames.clear();
    if(switchesSelections.count() < switchNames.count())
        return;
    for(int i = 0; i < switchesSelections.count(); i++){
        for(int j = 0; j < secondarySwitchSelections.at(i).count(); j ++){
            if(switchesSelections.at(i) == 1 && secondarySwitchSelections.at(i).at(j) == 1){
                graphsNames.append(secondarySwitchNames.at(i).at(j));
            }
        }
    }
    g.setGraphsNames(graphsNames);
}

bool serial::isSerialOpened(){
    return isPortOpened;
}

//INIT-DEINIT Functions

void serial::restartSequence(){
    iterations = 0;
    dataArray.clear();
    canArr.clear();
}

//connection needed
void serial::connection(){
    //connection to read the buffer
    connect(serialPort, &QSerialPort::readyRead, s, [=]{
        manageFunctions();
    });
}

//init function for the serial port
bool serial::init(){

    bool result = false;
    serialPort = new QSerialPort();

    serialPort->setPortName(serialPortSelected);
    serialPort->setBaudRate(250000);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);
    serialPort->setParity(QSerialPort::NoParity);

    if(serialPort->open(QSerialPort::ReadOnly)){
        result = true;
    }
    else{
        qDebug() << "Cannot Open Serial Port";
        result = false;
    }
    isPortOpened = result;
    g.setIsSerialOpened(isPortOpened);

    return result;
}

//disconnect everything and closes the serial port
void serial::deInit(){
    QObject::disconnect(serialPort, &QSerialPort::readyRead, nullptr, nullptr);
    QObject::disconnect(tim1, &QTimer::timeout, nullptr, nullptr);

    isPortOpened = false;
    doneCalibration = false;
    iterations = 0;
    average = 0;

    g.setIsSerialOpened(isPortOpened);

    serialPort->close();
    emit portStateChanged(0);

    qDebug() << "Serial Port Closed";
}

//SET-GET Functions

QVector<double> serial::getPointsData(){
    if(CAN_MODE == true){
        return canArr;
    }
    else{
        return dataArray;
    }
}

bool serial::getCanMode(){
    return CAN_MODE;
}

void serial::setGraphsRequested(QVector<int> reqGrph){
    this->requestedGraphs = reqGrph;
}

void serial::setCanMode(int value){
    CAN_MODE = value;
    g.setCanMode(CAN_MODE);
    restartSequence();
    g.restartSequence();
}

QStringList serial::getSwitchNames(){
    return switchNames;
}

QStringList serial::getSecondarySwitchNames(int index){
    return secondarySwitchNames.at(index);
}

void serial::setSwitchesState(QVector<int> state){
    switchesSelections = state;
}

void serial::setPrimarySwitches(int id, int value){
    while(switchesSelections.count() < switchNames.count()){
        switchesSelections.append(0);
    }
    switchesSelections[id] = value;
    g.setSecondarySwitchesSelections(graphSelection);
    updateGraphsNames();
}

QVector<int> serial::getSecondarySwitchesSelections(int index){
    if(secondarySwitchSelections.count() <= 0){
        for(int i = 0; i < secondarySwitchNames.count(); i ++){
            QVector<int> buff;
            for(int j = 0; j < secondarySwitchNames.at(i).count(); j++){
                buff.append(0);
            }
            secondarySwitchSelections.append(buff);
        }
    }
    graphSelection.clear();
    for(int i = 0; i < switchesSelections.count(); i++){
        if(switchesSelections.at(i) == 1)
        graphSelection.append(secondarySwitchSelections.at(i));
    }
    g.setSecondarySwitchesSelections(graphSelection);
    return secondarySwitchSelections.at(index);
}
void serial::setSecondarySwitchesSelections(int primaryId, int secondaryId, int value){
    secondarySwitchSelections[primaryId][secondaryId] = value;
    graphSelection.clear();
    for(int i = 0; i < switchesSelections.count(); i++){
        if(switchesSelections.at(i) == 1)
        graphSelection.append(secondarySwitchSelections.at(i));
    }
    g.setSecondarySwitchesSelections(graphSelection);
    updateGraphsNames();
}

//enables and disables the real time log
void serial::setLogState(bool state){
    logState = state;
    if(logState){
        QByteArray intestation;

        intestation.append("Eagle Trento Racing Team\r\n");
        intestation.append("Real Time Log File\r\n");
        intestation.append(QDateTime::currentDateTime().date().toString());
        intestation.append("\t");
        intestation.append(QDateTime::currentDateTime().time().toString());
        intestation.append("\r\n");

        QDir dir;
        dir.setCurrent(dirName);
        fil->setFileName("log " +
                         QDateTime::currentDateTime().date().toString()
                         + " " +
                         QDateTime::currentDateTime().time().toString()
                         );
        fil->open(QFile::ReadWrite);
        fil->write(intestation);
    }
    else{
        fil->close();
    }
}










