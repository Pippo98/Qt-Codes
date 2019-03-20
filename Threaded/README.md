## Detailed Description

### Slot - Signals

In this program I used slot functions to call them from the qml, is the easyest way to communicate between cpp and qml.

Q_INVOKABLE could be placed before any function declaration to make it visible and invokable from the qml.

Only Q_INVOKABLE and Slots functions are visible from qml.

Signals can be declared both in qml and cpp, when a signal is emitted is visible everywhere.

A cpp signal could be emitted only from the class that declasred it.

A signal is a function that can not be implemented, in fact it is only declared and "called" (using emit).

A signal could accept arguments that can be catched from the slot attached to it.

In qml, the only way to acces to the argument carried from the signal is to use the same variable name as in the declaration in the .h file.


### Thread

In this version I used QThread to limitate lags.

I threaded two functions: 

    *ex manageFunctions in serial file
    *ex updateSeries in graph file

To use threading you must include QThread, this class contatins "run" function that can be overrided and completed with the blocking code that need to be placed in a thread.

Then calling the "start" function, that appears when QThread is included, the thread is started.

QMutex class helps you to avoid crashes that could occur in multithreading.

These crashes occurs like when two threads are trying to acces at the same time at he same memory address.

Declare a QMutex instance (for example mute) to call mute.lock() and mute.unlock().

mute.lock() need to be called at the top of run() function and mute.unlock() at the bottom.

See my run() functions as examples.


### Chart

This program manages the graphs from the QtChart library, I implemented GraphChart.qml file.

In the qml file I'm declaring all the LineSeries that then I'm going to update, in this example I used 20 LineSeries, one X axis and 20 Y axis.


Every LineSeries need to be attached to one X axis and one Y axis, I used one common X axis and different Y axis to resize the graph.

To update the points from the cpp, I needed to share the pointer of the LineSeries to the cpp file.

The way to use qml pointers in cpp is to use QAbstractSeries and QAbstractAxis (the declaration is in the graph.cpp file).

In the first versions of this program every time a series needed to be updated, a function in cpp file was called from qml passing the pointer of the series.

Then in the actual version once the declaration of all the LineSeries is completed, qml calls a cpp function to pass all the pointers of the series.

In cpp a QVector variable stores all the pointers of all the series and all the axes, so until the end of the program The pointers are available in cpp.





