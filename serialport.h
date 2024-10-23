#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>
#include <QSerialPort>
#include "qcustomplot/qcustomplot.h"

class SerialPort : public QObject {
    Q_OBJECT

public:
    explicit SerialPort(QObject *parent = nullptr);

    void setupSerial();  // Configura el puerto serial

    void setOutputAmplitude(double value);  // Establece el valor de outputValue
    QSerialPort* getSerial();  // Obtiene el objeto del puerto serial
    bool canReadLine();

signals:
    void dataReady();
public slots:
    void readSerial();  // Lee los datos del puerto serial
    void closeSerial();
private:
    QSerialPort *serial;  // Objeto del puerto serial
    double outputValue;  // Valor de salida
    QCustomPlot *customPlotMagnitude;  // Gráfico de magnitud
    QCustomPlot *customPlotPhase;  // Gráfico de fase


    void updateMagnitudeGraph(double magnitude);  // Actualiza el gráfico de magnitud
    void updatePhaseGraph(double phase);  // Actualiza el gráfico de fase
    double calculatePhase(double inputValue, double outputValue);  // Calcula la fase
};

#endif // SERIALPORT_H
