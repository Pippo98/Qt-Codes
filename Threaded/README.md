### Detailed Description

## Slot - Signals

In this program I used slot functions to call them from the qml, is the easyest way to communicate between cpp and qml.
Q_INVOKABLE could be placed before any function declaration to make it visible from the qml.

Signals can be declared both in qml and cpp, when a signal is emitted is visible everywhere.
A cpp signal could be emitted only from the class that declasred it.
A signal is a function that can not be implemented, in fact it is only declared and "called" (using emit).
A signal could accept arguments that can be catched from the slot attached to it
In qml, the only way to acces to the argument carried from the signal is to use the same variable name as in the declaration in the .h file.

