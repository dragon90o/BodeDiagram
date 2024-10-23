#include "magnitudeplot.h"
#include "serialport.h"
#include <QDebug>
#include <QColor>
#include <QPen>
#include <QBrush>

// Constructor de la clase MagnitudePlot
MagnitudePlot::MagnitudePlot(QCustomPlot *plotWidget,SerialPort *serialPort, QObject *parent)
    : QObject(parent), customPlotMagnitude(plotWidget), serialPort(serialPort), outputValue(0),plotMode(PlotMode::Dynamic) // Inicializa outputValue
{

    setupPlot();  // Llama el método para configurar el gráfico
}

// Destructor de la clase MagnitudePlot
MagnitudePlot::~MagnitudePlot()
{
    // Opcional: Aquí puedes cerrar el puerto serial si es necesario
    // serial->close();
}
void MagnitudePlot::HandleUserInput(bool isCalculatePressed) {
    if (isCalculatePressed) {
        plotMode = PlotMode::Static; // Cambia a modo estático
    } else {
        //nada es por defecto dinamico

    // Aquí podrías agregar lógica para actualizar la gráfica si es necesario
    qDebug() << "Plot mode changed to:" << (plotMode == PlotMode::Static ? "Static" : "Dynamic");
}}
QSerialPort* MagnitudePlot::getSerial() {
    return serialPort->getSerial(); // Devuelve el puntero al puerto serial
}
// Configura el gráfico de magnitud
void MagnitudePlot::setupPlot()
{
    // Añade un gráfico al widget de QCustomPlot
    customPlotMagnitude->addGraph();
    customPlotMagnitude->graph(0)->setName("Magnitud");
    // Etiquetas de ejes
    customPlotMagnitude->xAxis->setLabel("Frecuencia");
    customPlotMagnitude->yAxis->setLabel("Magnitud (dB)");

    // Configura el rango inicial del gráfico
    customPlotMagnitude->xAxis->setRange(0, 10000);  // Cambia esto según tus requerimientos
    customPlotMagnitude->yAxis->setRange(-100, 1000);

    // Establece el número de ticks en el eje x
    customPlotMagnitude->xAxis->ticker()->setTickCount(5);
    // Establece el tipo de escala del eje x como logarítmica
    customPlotMagnitude->xAxis->setScaleType(QCPAxis::stLogarithmic);

    // Configura el QPen con color y grosor
    QPen pen(Qt::blue);
    pen.setWidth(2); // Establece el grosor de la línea
    customPlotMagnitude->graph(0)->setPen(pen); // Aplica el pen al gráfico


}

// Lee los datos y actualiza el gráfico
void MagnitudePlot::updatePlot(double inputValue)
{
    // Verifica que inputValue sea mayor que cero
    if (inputValue <= 0) {
        qDebug() << "Error: inputValue es <= 0, no se puede calcular log10.";
        return;  // Salta si inputValue es menor o igual a cero
    }

    // Verifica que outputValue sea mayor que cero
    if (outputValue <= 0) {
        qDebug() << "Error: outputValue es <= 0, no se puede calcular log10.";
        return;  // Salta si outputValue es menor o igual a cero
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

// Método para establecer el valor de outputValue
void MagnitudePlot::setOutputAmplitude(double value)
{
    outputValue = value; // Establece el valor de outputValue
}
