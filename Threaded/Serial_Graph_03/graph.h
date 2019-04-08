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

class graph : public QThread
{
    Q_OBJECT
public:
    explicit graph(QObject *parent = nullptr);
    ~graph() override;

    void deInit();
    void connections();
    void setCanMode(bool);
    void setMaxPoints(int);
    void restartSequence();
    void setIsSerialOpened(bool);
    void managePoints(QVector<double>);
    void setGraphsNames(QList<QString>);

    void run() override;

public slots:
    void setAxis(int, int);
    void updateSeries();
    void setGeneralYRange();
    void setSpecificYRange();
    void getAxisValues(QAbstractAxis *, int, int, int);
    void setSeriesArray(QAbstractSeries *, QAbstractAxis *, QAbstractAxis *);
    void setFrameRate(int);

signals:
    void graphChanged();

public:
    QString portOpened;

    QXYSeries *mSeries;
    QValueAxis *valueAxis;

    int totalGraphs;
};

#endif // GRAPH_H
