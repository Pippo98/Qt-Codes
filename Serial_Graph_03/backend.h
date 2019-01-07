#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QDebug>
#include <QVector>

class backend : public QObject
{
    Q_OBJECT
public:
    explicit backend(QObject *parent = nullptr);

    QVector<int> getRequestedGraphs();

signals:
    void portStateChanged(int state);

public slots:
    void buttonClicked(int, int);
    void comboChanged(QString, int, int);
    void switchChanged(int, int);
    void logSwitchChanged(int);
};

#endif // BACKEND_H
