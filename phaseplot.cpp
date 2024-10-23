#include "phaseplot.h"
#include "serialport.h"
#include "qcustomplot.h"
#include <QDebug>
#include <QColor>
#include <QPen>
#include <QBrush>

// Constructor
PhasePlot::PhasePlot(QCustomPlot *plotWidget, SerialPort *serialPort, QObject *parent)
    : QObject(parent), customPlotPhase(plotWidget), serialPort(serialPort), outputValue(0), plotMode(PlotMode::Dynamic)

    {

        // Configuración del gráfico
        setupPlot();
        // Conectar la señal readyRead de SerialPort al slot readSerial
        connect(serialPort, &SerialPort::dataReady, this, &PhasePlot::readSerial);

    }



// Destructor
PhasePlot::~PhasePlot()
{

}
void PhasePlot::HandleUserInput(bool isCalculatePressed) {
    if (isCalculatePressed) {
        plotMode = PlotMode::Static; // Cambia a modo estático
    } else {
        //nada es por defecto dinamico
    }

    // Aquí podrías agregar lógica para actualizar la gráfica si es necesario
    qDebug() << "Plot mode changed to:" << (plotMode == PlotMode::Static ? "Static" : "Dynamic");
}
// Configura el gráfico de magnitud
void PhasePlot::setupPlot()
{
    customPlotPhase->addGraph();  // Añade un gráfico al widget QCustomPlot
    customPlotPhase->graph(0)->setName("Phase");
    customPlotPhase->xAxis->setLabel("Frecuencia (Hz)");
    customPlotPhase->yAxis->setLabel("Phase (°)");

     // Configura el rango del gráfico
    customPlotPhase->xAxis->setRange(1, 10000); // Cambia según el rango que esperas
    customPlotPhase->yAxis->setRange(-180, 0); // Rango del eje Y (fase de -180 grados a 0 grados)

    // Establece el número de ticks en el eje x
    customPlotPhase->xAxis->ticker()->setTickCount(5);
    // Establece el tipo de escala del eje x como logarítmica
    customPlotPhase->xAxis->setScaleType(QCPAxis::stLogarithmic);

    // Configura el QPen con color y grosor
    QPen pen(Qt::blue);
    pen.setWidth(2); // Establece el grosor de la línea
    customPlotPhase->graph(0)->setPen(pen); // Aplica el pen al gráfico


    customPlotPhase->replot();  // Redibuja el gráfico

}


// Lee los datos del puerto serial y actualiza el gráfico
void PhasePlot::readSerial()

{
    while (serialPort->getSerial()->canReadLine()) {
        QByteArray data = serialPort->getSerial()->readLine().trimmed();
     // Lee una línea de datos y elimina espacios en blanco
        qDebug() << "Datos recibidos:" << data; // Mensaje para depurar

        bool ok;
        double inputValue = data.toDouble(&ok);  // Convierte los datos en un número

        if (!ok) {
            qDebug() << "Error: No se pudo convertir inputValue a un número válido.";
            continue;  // Salta esta iteración si el valor no es un número válido
        }

        // Verifica que inputValue y outputValue sean mayores que cero
        if (inputValue <= 0 || outputValue <= 0) {
            qDebug() << "Error: inputValue o outputValue es <= 0, no se puede calcular log10.";
            continue;  // Salta si alguno de los valores es menor o igual a cero
        }

        // Calcula la magnitud
        double ratio = inputValue / outputValue;
        double phase = 20 * log10(ratio); // Si estás calculando fase, ajusta esta fórmula
        qDebug() << "Input Value:" << inputValue << "Output Value:" << outputValue << "Phase:" << phase;

        // Agrega el valor al gráfico
        int dataCount = customPlotPhase->graph(0)->dataCount();
        customPlotPhase->graph(0)->addData(dataCount, phase);

        // Ajusta el rango si los datos exceden el rango actual del gráfico
        customPlotPhase->graph(0)->rescaleAxes();
        customPlotPhase->replot();  // Redibuja el gráfico
    }
}

// Método para establecer el valor de outputValue
void PhasePlot::setOutputAmplitude(double value)
{
    outputValue = value; // Establece el valor de outputValue
}
