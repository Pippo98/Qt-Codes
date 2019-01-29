#ifndef SERIAL_H
#define SERIAL_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QVector>

#include "backend.h"

class serial : public QObject
{
    Q_OBJECT
public:
    explicit serial(QObject *parent = nullptr);
    ~serial();

    QVector<double> getPointsData();

    void deInit();
    void parseCan();
    void parseData();
    void connection();
    void detectGraphs();
    void setLogState(bool);
    void updateGraphsNames();
    void restartSequence();

    bool init();
    bool isSerialOpened();

    void setSwitchesState(QVector<int>);

signals:
    void dataChanged(QString data);
    void portStateChanged(int state);

public slots:
    QStringList detectPort();
    QStringList getSwitchNames();
    QString portInfo(QString port);
    QStringList getSecondarySwitchNames(int);
    QVector<int> getSecondarySwitchesSelections(int);
    void setSecondarySwitchesSelections(int, int, int);
    void setPrimarySwitches(int, int);


    bool getCanMode();
    void setCanMode(int);
    void manageFunctions();
    void setGraphsRequested(QVector<int>);

public:
    int baudRateSelected;
    int serialPortIndex;

    QVector<int> requestedGraphs;

    QString serialPortSelected;
    QSerialPort * serialPort;
    QStringList serialPorts;
    QByteArray serialData;
};

#endif // SERIAL_H
