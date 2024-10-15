#ifndef MAGNITUDEPLOT_H
#define MAGNITUDEPLOT_H

#include <QWidget>
#include <QObject>
#include <QSerialPort>
#include "qcustomplot/qcustomplot.h"

class MagnitudePlot : public QObject
{
    Q_OBJECT
public:
    //contructor
    MagnitudePlot(QCustomPlot *plotWidget, QObject *parent = nullptr);//QCustomPlot es el widget donde se dibuja la grafica
    ~MagnitudePlot();
    void setOutputAmplitude(double value);
    void setupPlot();   //metodos
    void setupSerial();  //metodos
    void closeSerial(); //metodo para cerrar el puerto serial
    QSerialPort* getSerial();//metodo para obtener el puntero al puerto serial

private slots: //funcion slot disenada para ser llamada en respuesta a una senal de un objeto
    void readSerial(); // Metodo para leer los datos de arduino  y actualizar la grafica
private:
    QCustomPlot *customPlotMagnitude;  // Puntero al widget QCustomPlot.  esto quiere decir que la clase MagnitudePlot tiene un miembro de tipo
    //o etiqueta QCustomPlot y se llama customPlotMagnitude
    QSerialPort *serial;
    double outputValue;
};

#endif // MAGNITUDEPLOT_H
