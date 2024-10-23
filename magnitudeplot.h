#ifndef MAGNITUDEPLOT_H
#define MAGNITUDEPLOT_H

#include <QWidget>
#include <QObject>
#include <QSerialPort>
#include "qcustomplot/qcustomplot.h"
#include "serialport.h"

class MagnitudePlot : public QObject
{
    Q_OBJECT

public:
    // Constructor
    explicit MagnitudePlot(QCustomPlot *plotWidget,SerialPort *serialPort, QObject *parent = nullptr); // QCustomPlot es el widget donde se dibuja la gráfica
    ~MagnitudePlot();

    void setOutputAmplitude(double value);   // Método para establecer el valor de amplitud de salida
    QSerialPort* getSerial();

public slots:
    void HandleUserInput(bool isCalculatePressed);

private:
    void setupPlot();   // Método para la configuración del gráfico
    void updatePlot(double inputValue); // Método para actualizar el gráfico con nuevos datos
    SerialPort *serialPort;
    QCustomPlot *customPlotMagnitude;  // Puntero al widget QCustomPlot
    double outputValue;                 // Valor de amplitud de salida
    enum class PlotMode {
        Static,
        Dynamic
    };
    PlotMode plotMode;

};

#endif // MAGNITUDEPLOT_H

