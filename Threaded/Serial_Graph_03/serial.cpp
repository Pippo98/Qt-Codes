#include "graph.h"
#include "serial.h"
#include "backend.h"

#include <QDir>
#include <QFile>
#include <QtCore>
#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

#include <QMessageBox>

static QFile *fil = new QFile();
static QTimer *tim1 = new QTimer();
static QTimer *commandTimer = new QTimer();
static QMutex mute;

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
static QList<QString> Inverter  = {"motor temp", "IGBT temp", "batt voltage", "voltage output", "dc bus voltage", "power", "current", "torque", "speed"};
static QList<QString> Lv        = {"volt", "avgTemp", "maxTemp"};
static QList<QString> Hv        = {};

static QStringList              switchNames = {"Frontal", "Pedals", "ECU", "Inverter", "Lv"};
static QVector<int>             switchesSelections;
static QList<QStringList>       secondarySwitchNames = {Frontal, Pedals, Ecu, Inverter, Lv};
static QVector<QVector<int>>    secondarySwitchSelections;
static QVector<int>             graphSelection;
/*----------------------------------------------------------------------------------------------*/

static QSerialPort *serialPort = new QSerialPort();

static QByteArray serialData;
static QList<QByteArray> serialDataQueue;

static bool logState = false;

//text area  -> page 3 -> terminal
//text field -> page 3 -> send prompt
//info area  -> page 1 -> messages per sec
static QObject * textArea;
static QObject * textField;
static QObject * infoTextArea;

//page 3 variables
static QList<QByteArray> terminalText;
static QList<int> terminalIds;

static int messagesCounter = 0;
static int parsedCounter = 0;

static QWaitCondition newData;
static bool isPortOpened;
static bool doneCalibration;
static bool threadActive = true;
static double average = 0;
static int iterations = 0;

static QVector<double> canArr;
static QVector<double> dataArray;

static QList<QString> graphsNames;
static QList<QString> grph = {Frontal + Pedals + Ecu + Inverter + Lv + Hv};

static QVector<QPair<int, QVector<int>>> switchesState;

static QVector<QVector<int>> secondarySwitches;

static QElapsedTimer timer;
static qint64 nanoSec;
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

    double motorTemp;
    double IGBTTemp;
    double battVoltage;
    double voltageOutput;
    double dcBusVoltge;
    double power;
    double current;
    double torque;
    double speed;

};

struct lvAccumulatorStc{
    double volt;
    double avgTemp;
    double maxTemp;
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

serial::serial(QObject * parent) : QThread (parent)
{
}
serial::~serial(){
    threadActive = false;
    this->exit();
    if(fil->isOpen())
    fil->close();
}

//thread function
//main function that manage the flow of the program
//mute.lock() is a function needed for QThread to avoid crashes
void serial::run(){
    while(threadActive){
        if(mute.try_lock() && newData.wait(&mute)){
            qDebug() << timer.nsecsElapsed();
            if(!doneCalibration && !CAN_MODE){
                detectGraphs();
                mute.unlock();
            }
            if(doneCalibration || CAN_MODE){
                parseData();
                mute.unlock();

                if(CAN_MODE){
                    QElapsedTimer el1;
                    el1.start();

                    parseCan();

                    //qDebug() << el1.nsecsElapsed()/1000;
                    //qDebug() << serialDataQueue.length();
                    //g.managePoints(canArr);
                    canSniffer();
                }
                else{
                    //qDebug() << dataArray;
                    g.managePoints(dataArray);
                }
            }
        }
        else{
            //qDebug() << "lock failed -> serial -> run()";
        }
    }
}

//main function to manage the program flow
void serial::manageFunctions(){
    while(serialPort->bytesAvailable() > 50){
        mute.lock();
        serialData = serialPort->readLine();
        serialDataQueue.append(serialData);
        mute.unlock();
        timer.start();
        newData.wakeAll();
        if(logState)
        fil->write(serialData);
        messagesCounter ++;
    }
}

void serial::canSniffer(){
    int flag = 0;
    if(terminalIds.count() > 0 && terminalText.count() > 0 && dataArray.count() > 8){
        for(int i = 0; i < terminalIds.count(); i++){
            if(terminalIds.at(i) == int(dataArray.at(1))){
                terminalText[i] = serialData;
                flag = 1;
                break;
            }
        }
    }
    if(!flag){
        if(dataArray.count() > 8){
            terminalIds.append(int(dataArray.at(1)));
            terminalText.append(serialData);
        }
    }
}

//differentiate the data from different id
void serial::parseCan(){
    parsedCounter ++;

    canArr.clear();

    //Check if the CAN message is correct
    if(dataArray.count() >= 8 && switchesSelections.count() > 0){
        //FRONTAL
        if(switchesSelections.at(0)){
            if(int(dataArray[1]) == 192){
                if(int(dataArray[2]) == 2){
                    frontal.steer = dataArray[4];
                }
                else if(int(dataArray[2]) == 4){
                    frontal.gx = dataArray[3] * 256;
                    frontal.gx += dataArray[4];
                    frontal.gy = dataArray[5] * 256;
                    frontal.gy += dataArray[6];
                    frontal.gz = dataArray[7] * 256;
                    frontal.gz += dataArray[8];
                }
                else if(int(dataArray[2]) == 5){
                    frontal.ax = dataArray[3] * 256;
                    frontal.ax += dataArray[4];
                    frontal.ay = dataArray[5] * 256;
                    frontal.ay += dataArray[6];
                    frontal.az = dataArray[7] * 256;
                    frontal.az += dataArray[8];
                }
            }
            else if(int(dataArray[1]) == 208){
                if(int(dataArray[2]) == 6){
                    frontal.encoder1 = dataArray[3] * 256;
                    frontal.encoder1 += dataArray[4];
                }
                else if(int(dataArray[2]) == 7){
                    frontal.encoder2 = dataArray[3];
                }
            }

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
        //PEADLS
        else if(switchesSelections.at(1)){
            if(int(dataArray[1]) == 176){
                if(int(dataArray[2]) == 1){
                    pedals.apps1 = dataArray[3];
                    pedals.apps2 = dataArray[4];
                }
                else if(int(dataArray[2]) == 2){
                    pedals.brk = dataArray[3];
                }
            }

            canArr.append(pedals.apps1);
            canArr.append(pedals.apps2);
            canArr.append(pedals.brk);

        }
        //LVACCUMULATOR
        else if(switchesSelections.at(4)){
            if(int(dataArray[1]) == 255){
                lvAccumulator.volt = dataArray[2] / 10;
                lvAccumulator.avgTemp = dataArray[4];
                lvAccumulator.maxTemp = dataArray[5];
            }

            canArr.append(lvAccumulator.volt);
            canArr.append(lvAccumulator.avgTemp);
            canArr.append(lvAccumulator.maxTemp);

        }
    }

    if(switchesSelections.count() > 0){
        if(canArr.size() > 0 && graphSelection.size() > 0){
            QVector<double> buff;
            for(int i = 0; i < graphSelection.size(); i++){
                if(i < canArr.count()){
                    if(graphSelection.at(i))
                    buff.append(canArr.at(i));
                }
                else{
                    break;
                }
            }
            canArr = buff;
        }
    }
}

//function to get all the numbers in the buffer received
void serial::parseData(){

    if(serialDataQueue.length() == 0)
    return;
    QList<QByteArray> dataSplitted = serialDataQueue[0].split('\t');
    serialDataQueue.removeAt(0);
    //QList<QByteArray> dataSplitted = serialData.split('\t');

    //if the numbers found in the received string are te same number ad the average found then do the cast fo float
    if(dataSplitted.count() == g.totalGraphs || CAN_MODE){
        for(int i = 0; i < dataSplitted.count(); i++){
            if(dataArray.length() < dataSplitted.count()){
                dataArray.append(dataSplitted[i].toDouble());
            }
            else{
                dataArray[i] = dataSplitted[i].toDouble();
            }
        }
    }
    else{
        qDebug() << "Detect Graphs Number Failed";
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
    qDebug() << average;
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

//function to set the names of the single graphs
//it is based on which switch is selected
void serial::updateGraphsNames(){
    if(switchesSelections.count() < switchNames.count()){
        return;
    }
    graphsNames.clear();
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

//--------------------------------------INIT-DEINIT Functions--------------------------------------//

void serial::restartSequence(){
    iterations = 0;
    dataArray.clear();
    canArr.clear();
}

//connection needed
void serial::connection(){
    //connection to read the buffer
    connect(serialPort, SIGNAL(readyRead()), this, SLOT(manageFunctions()));
}

//init function for the serial port
bool serial::init(){

    bool result = false;

    serialPort->setPortName(serialPortSelected);
    serialPort->setBaudRate(serialPortBaudrate);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);
    serialPort->setParity(QSerialPort::NoParity);

    if(serialPort->open(QSerialPort::ReadWrite)){
        tim1->setInterval(1000);
        tim1->start();
        QObject::connect(tim1, SIGNAL(timeout()), SLOT(displayPerformanceInfo()));
        result = true;
    }
    else{
        result = false;
        QMessageBox msgBox;
        msgBox.setText("Port NOT opened, check port name and baudrate");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
    }
    isPortOpened = result;

    g.setIsSerialOpened(isPortOpened);

    terminalIds.clear();
    terminalText.clear();

    return result;
}

//disconnect everything and closes the serial port
bool serial::deInit(){

    bool result = false;

    QObject::disconnect(serialPort, &QSerialPort::readyRead, nullptr, nullptr);
    QObject::disconnect(tim1, &QTimer::timeout, nullptr, nullptr);

    isPortOpened = false;
    doneCalibration = false;
    iterations = 0;
    average = 0;
    dataArray.clear();
    canArr.clear();

    g.setIsSerialOpened(isPortOpened);

    serialPort->close();

    return result;
}

void serial::displayHelp(){
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setText("Select port name, baudrate, click the Closed button to open the port\n"
                   "log button enables the log of the received data, to select the directory check dirName varialbe in serial.cpp file\n"
                   "CAN mode if you want to plot the CAN data, else you can plot raw data: same numbers of data in all the messages, numbers separated by \\t and the line end with \\r\\n");
    msgBox.exec();
}

int lenBFF = 0;

//set the text in the qml files with the given object
void serial::displayPerformanceInfo(){
    QString txt = "messages per second: " + QString::number(messagesCounter * 1000/tim1->interval())
                + "\r\n"
                + "lost per second: " + QString::number(abs(messagesCounter - parsedCounter));
    qDebug() << serialDataQueue.length() - lenBFF;
    lenBFF = serialDataQueue.length();
    messagesCounter = 0;
    parsedCounter = 0;
    infoTextArea->setProperty("displayableText", txt);
    if(terminalText.count() > 0){
        QByteArray buff = terminalText.join("");
        textArea->setProperty("displaybleText", buff);
    }
}

//--------------------------------------SET-GET Functions--------------------------------------//

void serial::initTextArea(QObject * obj){
    textArea = obj;
}

void serial::initTextField(QObject * obj){
    textField = obj;
}

void serial::initInfoTextArea(QObject * obj){
    infoTextArea = obj;
}

void serial::sendCommand(QString data, QString _delay){

    bool castSucceed;

    QList<QString> bytes = data.split(" ");
    int delay = _delay.toInt(&castSucceed);

    if(CAN_MODE){
        if(data.length() == 0){
            textField->setProperty("propColor", "red");
            return;
        }

        bool success;
        normalizeCommand(bytes, &success);

    }

    const QByteArray &buff = bytes.join(" ").toUtf8();

    if(castSucceed && delay != 0 && serialPort->isWritable() && serialPort->isOpen()){
        sendDelayedCommand(buff, delay);
    }
    else{
        if(serialPort->isWritable() && serialPort->isOpen()){
            textField->setProperty("propColor", "green");
            serialPort->write(buff);
        }
        else{
            textField->setProperty("propColor", "red");
        }
    }
}

void serial::sendDelayedCommand(const QByteArray buffer, int delay){

    QObject::disconnect(commandTimer, &QTimer::timeout, nullptr, nullptr);
    commandTimer->setInterval(delay);
    commandTimer->start();
    connect(commandTimer, &QTimer::timeout, [=](){
        serialPort->write(buffer);
        qDebug() << "sent" << buffer;
    });
}

QList<QString> serial::normalizeCommand(QList<QString> bytes, bool * success){
    *success = true;

    while(bytes.last() == " " || bytes.last() == "" || bytes.length() > 9){
        bytes.removeAt(bytes.length()-1);
    }

    while(bytes.length() < 9){
        bytes.append("000");
    }

    for(int i = 0; i < bytes.length(); i++){
        while(bytes.at(i).length() < 3){
            bytes[i].prepend("0");
        }

        bool isNumber = false;
        bytes.at(i).toInt(&isNumber);

        if(!isNumber){
            textField->setProperty("propColor", "red");
            *success = false;
            break;
        }
        if(bytes.at(i).toInt() > 255){
            bytes[i] = "255";
        }
    }

    bytes.append("-");

    return bytes;
}

bool serial::getCanMode(){
    return CAN_MODE;
}

void serial::setCanMode(int value){
    CAN_MODE = value;
    g.setCanMode(CAN_MODE);
    restartSequence();
    g.restartSequence();
}

//returns the primary switches names
QStringList serial::getSwitchNames(){
    return switchNames;
}

//returns the names of the secondary switch of the index requested
QStringList serial::getSecondarySwitchNames(int index){
    return secondarySwitchNames.at(index);
}

//slot called from qml to update the vector containing the primary switches selections
void serial::setPrimarySwitches(int id, int value){
    while(switchesSelections.count() < switchNames.count()){
        switchesSelections.append(0);
    }
    switchesSelections[id] = value;
    updateGraphsNames();
}

//slot called from qml to get the vactor containing the switches selections
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
    return secondarySwitchSelections.at(index);
}

//slot called from qml to update the vector contatining the switche selections
void serial::setSecondarySwitchesSelections(int primaryId, int secondaryId, int value){
    if(primaryId < secondarySwitchSelections.length() && secondaryId < secondarySwitchSelections.at(primaryId).length()){
        secondarySwitchSelections[primaryId][secondaryId] = value;
    }
    else{
        qDebug() << "wrong indexes";
    }
    graphSelection.clear();
    for(int i = 0; i < switchesSelections.count(); i++){
        if(switchesSelections.at(i) == 1)
        graphSelection.append(secondarySwitchSelections.at(i));
    }
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










