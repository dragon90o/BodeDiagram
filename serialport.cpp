#include "serialport.h"
#include <QDebug>
#include <QtSerialPort/QSerialPort>

SerialPort::SerialPort(QObject *parent) : QObject(parent), serial(new QSerialPort(this)), outputValue(0) {
    setupSerial();
}

void SerialPort::setupSerial() {
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    connect(serial, &QSerialPort::readyRead, this, &SerialPort::readSerial);
}

void SerialPort::closeSerial() {
    if (serial->isOpen()) {
        serial->close();
    }
}


// Lee los datos del puerto serial y actualiza el gráfico
void SerialPort::readSerial() {
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

        // Aquí llamamos a un método para actualizar el gráfico de magnitud
        updateMagnitudeGraph(magnitude);

        // Lógica para calcular y actualizar la fase (puedes agregar tus cálculos de fase aquí)
        double phase = calculatePhase(inputValue, outputValue); // Implementa tu método para calcular la fase
        updatePhaseGraph(phase); // Método que actualiza el gráfico de fase
        emit dataReady();
    }
}
bool SerialPort::canReadLine() {
    return serial && serial->canReadLine();  // Verifica que el puntero serial no sea nulo y que pueda leer líneas
}
void SerialPort::updateMagnitudeGraph(double magnitude) {
    // Agrega el valor al gráfico de magnitud
    int dataCount = customPlotMagnitude->graph(0)->dataCount();
    customPlotMagnitude->graph(0)->addData(dataCount, magnitude);

    // Ajusta el rango si los datos exceden el rango actual del gráfico
    customPlotMagnitude->graph(0)->rescaleAxes();
    customPlotMagnitude->replot();  // Redibuja el gráfico
}

void SerialPort::updatePhaseGraph(double phase) {
    // Agrega el valor al gráfico de fase
    int dataCount = customPlotPhase->graph(0)->dataCount();
    customPlotPhase->graph(0)->addData(dataCount, phase);

    // Ajusta el rango si los datos exceden el rango actual del gráfico
    customPlotPhase->graph(0)->rescaleAxes();
    customPlotPhase->replot();  // Redibuja el gráfico
}

double SerialPort::calculatePhase(double inputValue, double outputValue) {
    // Lógica para calcular la fase
    // Reemplaza este ejemplo con tus propios cálculos
    double phase = 0; // Ajusta según tu lógica
    qDebug()<< "el output value es: "<< outputValue;
    return phase;
}

// Método para establecer el valor de outputValue
void SerialPort::setOutputAmplitude(double value) {
    outputValue = value; // Establece el valor de outputValue
}

QSerialPort* SerialPort::getSerial() {
    return serial;
}
