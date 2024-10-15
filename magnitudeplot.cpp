#include "magnitudeplot.h"
#include <QDebug>
#include <QColor>
#include <QPen>
#include <QBrush>
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
    closeSerial(); //destruir la clase cuando cierre el puerto
}

// Configura el gráfico de magnitud
void MagnitudePlot::setupPlot()
{
    customPlotMagnitude->addGraph();  // Añade un gráfico al widget QCustomPlot
    customPlotMagnitude->graph(0)->setName("Magnitud");
    customPlotMagnitude->xAxis->setLabel("Frecuencia");
    customPlotMagnitude->yAxis->setLabel("Magnitud (dB)");

    // Configura el rango inicial del gráfico

    // Configura el rango del gráfico
    customPlotMagnitude->xAxis->setRange(0, 10000);  // Cambia esto según tus requerimientos
    customPlotMagnitude->yAxis->setRange(-100, 1000);

    // Establece el número de ticks
    customPlotMagnitude->xAxis->ticker()->setTickCount(5);
    customPlotMagnitude->xAxis->setScaleType(QCPAxis::stLogarithmic);

    // Configura el QPen con color y grosor
    QPen pen(Qt::blue);
    pen.setWidth(2); // Establece el grosor de la línea
    customPlotMagnitude->graph(0)->setPen(pen); // Aplica el pen al gráfico

    // Establece el fondo del gráfico
    customPlotMagnitude->setBackground(QBrush(QColor(0,255,0,128))); // Cambia el fondo a verde
}



// Configura el puerto serial
void MagnitudePlot::setupSerial()
{
    // Configura el puerto serial
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    // Conexión del puerto serial con el método readSerial
    connect(serial, &QSerialPort::readyRead, this, &MagnitudePlot::readSerial);
}

void MagnitudePlot::closeSerial(){
    if (serial->isOpen()) {
        serial->close();
    }
}

// Lee los datos del puerto serial y actualiza el gráfico
void MagnitudePlot::readSerial()
{
    while (serial->canReadLine()) {
        QByteArray data = serial->readLine().trimmed();  // Lee una línea de datos y elimina espacios en blanco
        qDebug() << "Datos recibidos:" << data; // Mensaje para depurar

        bool ok;
        double inputValue = data.toDouble(&ok);  // Convierte los datos en un número

        if (!ok) {
            qDebug() << "Error: No se pudo convertir inputValue a un número válido.";
            continue;  // Salta esta iteración si el valor no es un número válido
        }

        // Verifica que inputValue sea mayor que cero
        if (inputValue <= 0) {
            qDebug() << "Error: inputValue es <= 0, no se puede calcular log10.";
            continue;  // Salta si inputValue es menor o igual a cero
        }

        // Verifica que outputValue sea mayor que cero (lo ingresas desde tu UI)
        if (outputValue <= 0) {
            qDebug() << "Error: outputValue es = 0, no se puede calcular log10.";
            continue;  // Salta si outputValue es menor o igual a cero
        }

        // Calcula la magnitud
        double ratio = inputValue / outputValue;
        double magnitude = 20 * log10(ratio);
        qDebug() << "Input Value:" << inputValue << "Output Value:" << outputValue << "Magnitude:" << magnitude;

        // Agrega el valor al gráfico
        int dataCount = customPlotMagnitude->graph(0)->dataCount();
        customPlotMagnitude->graph(0)->addData(dataCount, magnitude);

        // Ajusta el rango si los datos exceden el rango actual del gráfico
        customPlotMagnitude->graph(0)->rescaleAxes();
        customPlotMagnitude->replot();  // Redibuja el gráfico
    }
}


QSerialPort* MagnitudePlot::getSerial(){
    return serial;
}

// Método para establecer el valor de outputValue
void MagnitudePlot::setOutputAmplitude(double value)
{
    outputValue = value; // Establece el valor de outputValue
}
