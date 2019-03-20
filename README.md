# Qt-Codes

# Serial Graph Plotter

## Description
This is an application able to plot multiple graphs

The way it gets the data is via serial port

The device connected will send numbers representing the data to be plot

The numbers must be separated by '\t' and the the line must end with '\r\n'

The lines must have the same quantity of numbers


The application has two modality: CAN mode and normal mode

### CAN mode
CAN mode need to receive a CAN message including the id and preceded by the timestamp of the message

In the code can be managed the ids of the messages to plot the data carried in the payload

The function that could be modified is parseCan in serial.cpp file

### Normal mode

Normal mode autodetects how many numbers there are in one message, that must not change

This means that if in the first message there are 5 numbers, the applications search only for 5 numbers

### Logging
Activating the 'Log enable' switch tll the data received in the serial port can be saved in a file
The path of the directory must be changed
There variable in serial.cpp file named dir contains the path

### Others informations
In the Chart Page there is a slider that changes the number of points present in one line


The axis rescaling is managed with a timer in the Graph Chart file

The qml calls 'setAxis()' that is a function in the graph class

In the qml could be changed the argument of the setAxis function to modify the way the rescale is done

In graph.cpp setAxis function is commented to explain how the arguments works



