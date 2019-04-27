import QtQuick 2.0
import QtQuick.Controls 2.2
import QtCharts 2.0
import QtQuick.Layouts 1.3

ChartView {
    id: chartView

    antialiasing: true
    theme: ChartView.ChartThemeQt
    backgroundRoundness: 10

    property bool graphRequested: false
    property int resizeInterval: 100
    property bool openGL: true

    Component.onCompleted: {
        for(var i = 0; i < 20; i++){
            graph.setSeriesArray(chartView.series(i), chartView.axisX(series(i)), chartView.axisY(series(i)))
        }
    }

    Timer{
        id: resizeTimer
        interval: resizeInterval
        running: true
        repeat: true
        onTriggered: {
            graph.setAxis(1, 0)
        }
        onIntervalChanged: {
            stop()
            running = true
        }
    }

    ValueAxis {
        id: axisY1
        min: 0
        max: 80
        visible: true
        tickCount: 7
        labelsAngle: -45
    }
    ValueAxis {
        id: axisY2
        min: 0
        max: 80
        visible: true
        gridVisible: false
        labelsAngle: -45
        tickCount: 7
    }
    ValueAxis {
        id: axisY3
        min: 0
        max: 80
        visible: true
        gridVisible: false
        labelsAngle: -45
        tickCount: 7
    }
    ValueAxis {
        id: axisY4
        min: 0
        max: 80
        visible: false
    }
    ValueAxis {
        id: axisY5
        min: 0
        max: 80
        visible: false
    }
    ValueAxis {
        id: axisY6
        min: 0
        max: 80
        visible: false
    }
    ValueAxis {
        id: axisY7
        min: 0
        max: 80
        visible: false
    }
    ValueAxis {
        id: axisY8
        min: 0
        max: 80
        visible: false
    }
    ValueAxis {
        id: axisY9
        min: 0
        max: 80
        visible: false
    }
    ValueAxis {
        id: axisY10
        min: 0
        max: 80
        visible: false
    }

    ValueAxis {
        id: axisY11
        min: 0
        max: 80
        visible: false
    }
    ValueAxis {
        id: axisY12
        min: 0
        max: 80
        visible: false
    }
    ValueAxis {
        id: axisY13
        min: 0
        max: 80
        visible: false
    }
    ValueAxis {
        id: axisY14
        min: 0
        max: 80
        visible: false
    }
    ValueAxis {
        id: axisY15
        min: 0
        max: 80
        visible: false
    }
    ValueAxis {
        id: axisY16
        min: 0
        max: 80
        visible: false
    }
    ValueAxis {
        id: axisY17
        min: 0
        max: 80
        visible: false
    }
    ValueAxis {
        id: axisY18
        min: 0
        max: 80
        visible: false
    }
    ValueAxis {
        id: axisY19
        min: 0
        max: 80
        visible: false
    }
    ValueAxis {
        id: axisY20
        min: 0
        max: 80
        visible: false
    }

    ValueAxis {
        id: axisX
        min: 0
        max: 1000
        gridVisible: false
    }

    LineSeries {
        id: lineSeries1
        name: "signal 1"
        color: "red"
        axisX: axisX
        axisY: axisY1
        visible: false
        useOpenGL: chartView.openGL
    }

    LineSeries {
        id: lineSeries2
        name: "signal 2"
        color: "green"
        axisX: axisX
        axisY: axisY2
        visible: false
        useOpenGL: chartView.openGL
    }
    LineSeries {
        id: lineSeries3
        name: "signal 3"
        color: "blue"
        axisX: axisX
        axisY: axisY3
        visible: false
        useOpenGL: chartView.openGL
    }
    LineSeries {
        id: lineSeries4
        name: "signal 4"
        color: "black"
        axisX: axisX
        axisY: axisY4
        visible: false
        useOpenGL: chartView.openGL
    }
    LineSeries {
        id: lineSeries5
        name: "signal 5"
        color: "black"
        axisX: axisX
        axisY: axisY5
        visible: false
        useOpenGL: chartView.openGL
    }
    LineSeries {
        id: lineSeries6
        name: "signal 6"
        color: "black"
        axisX: axisX
        axisY: axisY6
        visible: false
        useOpenGL: chartView.openGL
    }
    LineSeries {
        id: lineSeries7
        name: "signal 7"
        color: "black"
        axisX: axisX
        axisY: axisY7
        visible: false
        useOpenGL: chartView.openGL
    }
    LineSeries {
        id: lineSeries8
        name: "signal 8"
        color: "black"
        axisX: axisX
        axisY: axisY8
        visible: false
        useOpenGL: chartView.openGL
    }
    LineSeries {
        id: lineSeries9
        name: "signal 9"
        color: "black"
        axisX: axisX
        axisY: axisY9
        visible: false
        useOpenGL: chartView.openGL
    }
    LineSeries {
        id: lineSeries10
        name: "signal 10"
        color: "black"
        axisX: axisX
        axisY: axisY10
        visible: false
        useOpenGL: chartView.openGL
    }

    LineSeries {
        id: lineSeries11
        name: "signal 10"
        color: "black"
        axisX: axisX
        axisY: axisY11
        visible: false
        useOpenGL: chartView.openGL
    }
    LineSeries {
        id: lineSeries12
        name: "signal 10"
        color: "black"
        axisX: axisX
        axisY: axisY12
        visible: false
        useOpenGL: chartView.openGL
    }
    LineSeries {
        id: lineSeries13
        name: "signal 10"
        color: "black"
        axisX: axisX
        axisY: axisY13
        visible: false
        useOpenGL: chartView.openGL
    }
    LineSeries {
        id: lineSeries14
        name: "signal 10"
        color: "black"
        axisX: axisX
        axisY: axisY14
        visible: false
        useOpenGL: chartView.openGL
    }
    LineSeries {
        id: lineSeries15
        name: "signal 10"
        color: "black"
        axisX: axisX
        axisY: axisY15
        visible: false
        useOpenGL: chartView.openGL
    }
    LineSeries {
        id: lineSeries16
        name: "signal 10"
        color: "black"
        axisX: axisX
        axisY: axisY16
        visible: false
        useOpenGL: chartView.openGL
    }
    LineSeries {
        id: lineSeries17
        name: "signal 10"
        color: "black"
        axisX: axisX
        axisY: axisY17
        visible: false
        useOpenGL: chartView.openGL
    }
    LineSeries {
        id: lineSeries18
        name: "signal 10"
        color: "black"
        axisX: axisX
        axisY: axisY18
        visible: false
        useOpenGL: chartView.openGL
    }
    LineSeries {
        id: lineSeries19
        name: "signal 10"
        color: "black"
        axisX: axisX
        axisY: axisY19
        visible: false
        useOpenGL: chartView.openGL
    }
    LineSeries {
        id: lineSeries20
        name: "signal 10"
        color: "black"
        axisX: axisX
        axisY: axisY20
        visible: false
        useOpenGL: chartView.openGL
    }
}
