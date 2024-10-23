#ifndef PHASEPLOT_H
#define PHASEPLOT_H

#include <QWidget>
#include <QObject>
#include <QSerialPort>
#include "qcustomplot/qcustomplot.h"
#include "serialport.h"

class PhasePlot : public QObject
{
    Q_OBJECT
public:
    // Constructor
    PhasePlot(QCustomPlot *plotWidget, SerialPort *serialPort, QObject *parent = nullptr);

    // Destructor
    ~PhasePlot();

    // Método para establecer el valor de outputValue
    void setOutputAmplitude(double value);

    // Método para configurar el gráfico
    void setupPlot();
    void HandleUserInput(bool isCalculatePressed);
private slots:
    // Método para leer los datos de Arduino y actualizar la gráfica
    void readSerial();



private:
    QCustomPlot *customPlotPhase;  // Puntero al widget QCustomPlot.
    SerialPort *serialPort;        // Puntero a la clase SerialPort.
    double outputValue;            // Valor de salida.
    enum class PlotMode {
        Static,
        Dynamic
    };
    PlotMode plotMode;
};

#endif // PHASEPLOT_H
