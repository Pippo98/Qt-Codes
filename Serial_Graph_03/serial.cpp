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

const auto ser = QSerialPortInfo::availablePorts();
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
static QList<QString> Ecu       = {};
static QList<QString> Inverter  = {};
static QList<QString> Lv        = {};
static QList<QString> Hv        = {};
/*----------------------------------------------------------------------------------------------*/

static bool logState = false;

static bool isPortOpened;
static double average = 0;
static int iterations = 0;
static bool doneCalibration = false;

static QString text;

static QVector<double> canArr;
static QVector<double> dataArray;

static QList<QString> graphsNames;

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

//principal function to manage the functuons flow
void serial::manageFunctions(){

    serialData = serialPort->readLine();
    while(serialPort->bytesAvailable() > 100){
        serialPort->readLine();
    }
    //serialData = serialPort->readAll();
    //serialPort->readAll();

    if(!doneCalibration && !CAN_MODE){
        detectGraphs();
    }
    if(doneCalibration || CAN_MODE){
        parseData();

        if(CAN_MODE){
        parseCan();
        }

        g.managePoints();

        if(logState)
        fil->write(serialData);
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


    //FRONTAL
    if(requestedGraphs.at(0) == 1){
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
    if(requestedGraphs.at(1) == 1){
        canArr.append(pedals.apps1);
        canArr.append(pedals.apps2);
        canArr.append(pedals.brk);
    }
    //ECU
    if(requestedGraphs.at(2) == 1){

    }
    //INVERTER
    if(requestedGraphs.at(3) == 1){

    }
    //LOW VOLTAGE
    if(requestedGraphs.at(4) == 1){

    }
    //HIGH VOLTAGE
    if(requestedGraphs.at(6) == 1){

    }
}

//function to get all the numbers in the buffer received
void serial::parseData(){

    QStringList numberArr;
    QByteArray number;
    QChar byte;

    for(int i = 0; i < serialData.count(); i++){
        byte = QChar(serialData[i]);
        if(byte != '\t' && byte != '\n'){  //these are the separator from the different numbers in the received buffer
            number.append(byte);
        }
        else{
            numberArr.append(number);
            number.clear();
        }
    }

    if(numberArr.length() == g.totalGraphs || CAN_MODE){            //if the numbers found in the received string are te same number ad the average found then do the cast fo float
        for(int i = 0; i < numberArr.length(); i++){
            if(dataArray.length() < numberArr.length()){
                dataArray.append(numberArr[i].toDouble());
            }
            else{
                dataArray.replace(i, numberArr[i].toDouble());
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

    for(int j = 0; j < serialData.length(); j++){
        if(serialData[j] == '\t' || serialData[j] == '\n'){
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

    port_list.append("      ");

    for(const QSerialPortInfo &ser_ : ser)    {
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
    if(requestedGraphs.at(0) == 1)  graphsNames.append(Frontal);
    if(requestedGraphs.at(1) == 1)  graphsNames.append(Pedals);
    if(requestedGraphs.at(2) == 1)  graphsNames.append(Ecu);
    g.setGraphsNames(graphsNames);
}

QString serial::print_data(){
    return text;
}

bool serial::isSerialOpened(){
    return isPortOpened;
}

//INIT-DEINIT Functions

void serial::restartSequence(){
    dataArray.clear();
    canArr.clear();
}

//connection needed
void serial::connection(){
    //connection to read the buffer
    connect(serialPort, &QSerialPort::readyRead, s, [=]{
        manageFunctions();
    });

    tim1->setInterval(10);
    tim1->start();
    connect(tim1, &QTimer::timeout, this, [=]{
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

QString serial::getText(){
    return text;
}

bool serial::getCanMode(){
    return CAN_MODE;
}

void serial::setGraphsRequested(QVector<int> reqGrph){
    this->requestedGraphs = reqGrph;
    updateGraphsNames();
}

void serial::setCanMode(int value){
    CAN_MODE = value;
    g.setCanMode(CAN_MODE);
    restartSequence();
    g.restartSequence();
}

void serial::setPrint_data(QString var){
    Q_UNUSED(var)
}

void serial::setText(){
    text.append(serialData);

    if(text.size() > 500){
        text.remove(0, serialData.count());
    }
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
        fil->setFileName("log" +
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










