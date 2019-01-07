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
    //explicit serial();
    ~serial();

    //Q_PROPERTY(QString getText READ getText WRITE setText)
    Q_PROPERTY(QString print_data READ print_data WRITE setPrint_data NOTIFY print_dataChanged)

    QString print_data();
    QVector<double> getPointsData();

    void deInit();
    void setText();
    void parseCan();
    void parseData();
    void connection();
    void detectGraphs();
    void setLogState(bool);
    void setPrint_data(QString);
    void updateGraphsNames();
    void restartSequence();

    bool init();
    bool isSerialOpened();

signals:
    void print_dataChanged();
    void dataChanged(QString data);
    void graphsChanged(int number);
    void portStateChanged(int state);

public slots:
    QString getText();
    QStringList detectPort();
    QString portInfo(QString port);

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
