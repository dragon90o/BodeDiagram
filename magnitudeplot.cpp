#include "magnitudeplot.h"
#include <QDebug>

// Constructor
MagnitudePlot::MagnitudePlot(QCustomPlot *plotWidget, QObject *parent)
    : QObject(parent), customPlotMagnitude(plotWidget), serial(new QSerialPort(this))
{
    setupPlot();  // Configura el gráfico
    setupSerial();  // Configura el puerto serial
}

// Destructor
MagnitudePlot::~MagnitudePlot()
{
    if (serial->isOpen()) {
        serial->close();  // Cierra el puerto serial si está abierto
    }
}

// Configura el gráfico de magnitud
void MagnitudePlot::setupPlot()
{
    customPlotMagnitude->addGraph();  // Añade un gráfico al widget QCustomPlot
    customPlotMagnitude->graph(0)->setName("Magnitud");
    customPlotMagnitude->xAxis->setLabel("Frecuencia");
    customPlotMagnitude->yAxis->setLabel("Magnitud");

    // Configura el rango inicial del gráfico
    customPlotMagnitude->xAxis->setRange(0, 100);   // Cambia esto según tus requerimientos
    customPlotMagnitude->yAxis->setRange(0, 100);
}

// Configura el puerto serial
void MagnitudePlot::setupSerial()
{
    // Configura el puerto serial
    serial->setPortName("COM9");  // Cambia esto según el puerto adecuado
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    // Conexión del puerto serial con el método readSerial
    connect(serial, &QSerialPort::readyRead, this, &MagnitudePlot::readSerial);

    // Intento de abrir el puerto serial
    if (!serial->open(QIODevice::ReadOnly)) {
        qDebug() << "Error: No se pudo abrir el puerto serial";
    }
}

// Lee los datos del puerto serial y actualiza el gráfico
void MagnitudePlot::readSerial()
{
    while (serial->canReadLine()) {
        QByteArray data = serial->readLine().trimmed();  // Lee una línea de datos y elimina espacios en blanco

        bool ok;
        double value = data.toDouble(&ok);  // Convierte los datos en un número

        if (ok) {
            // Agrega el valor al gráfico
            int dataCount = customPlotMagnitude->graph(0)->dataCount();
            customPlotMagnitude->graph(0)->addData(dataCount, value);

            // Ajusta el rango si los datos exceden el rango actual del gráfico
            customPlotMagnitude->graph(0)->rescaleAxes();
            customPlotMagnitude->replot();  // Redibuja el gráfico
        } else {
            qDebug() << "Error: No se pudieron convertir los datos del puerto serial";
        }
    }
}
