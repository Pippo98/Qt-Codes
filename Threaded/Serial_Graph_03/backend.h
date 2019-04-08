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

signals:
    void portStateChanged(int state);

public slots:
    void logSwitchChanged(int);
    void buttonClicked(int, int);
    void sliderChanged(int, int);
    void comboChanged(QString, int, int);
    void switchChanged(int, int);
};

#endif // BACKEND_H
