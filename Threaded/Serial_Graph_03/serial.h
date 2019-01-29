#ifndef SERIAL_H
#define SERIAL_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QVector>

#include <QThread>

#include "backend.h"

class serial : public QThread
{
    Q_OBJECT
public:
    explicit serial(QObject *parent = nullptr);
    ~serial() override;

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


    void run() override;

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
};

#endif // SERIAL_H
