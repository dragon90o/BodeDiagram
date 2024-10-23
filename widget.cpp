#include "widget.h"
#include "ui_widget.h"
#include "magnitudeplot.h"
#include "phaseplot.h"
#include "QMessageBox"
#include "exprtk/exprtk.hpp"
#include "complex"
#include "cmath"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    connect(ui->startPushButton, &QPushButton::clicked, this, &Widget::onStartButtonClicked);
    connect(ui->calculateButton, &QPushButton::clicked, this, &Widget::calculateButtonClicked);
    serialPort = new SerialPort(this);

    magnitudePlot = new MagnitudePlot(ui->customPlotMagnitude, serialPort, this);
    phasePlot = new PhasePlot(ui->customPlotPhase, serialPort, this);

}
Widget::~Widget()
{
    delete ui;  // Elimina la UI
}

void Widget::onStartButtonClicked()
{
    qDebug() << "onStartButtonClicked() se llamó";  // Depuración
    startPushButtonClicked();
    calculateButtonClicked();
}

void Widget::startPushButtonClicked()
{
    QString inputValueStr = ui->usbOutputValue->text();
    bool ok;
    double inputValue = inputValueStr.toDouble(&ok); // Solo números

    // Verificar si la conversión fue exitosa
    if (!ok) {
        QMessageBox::warning(this, "Invalid input", "Try just numbers, no symbols.");
        return;
    }
    qDebug() << "El valor ingresado es: " << inputValue;
    outputValue = inputValue;  // Asigna el valor de salida
    magnitudePlot->setOutputAmplitude(outputValue);

    QString inputPortStr = ui->usbPort->text();
    if (serialPort){
    serialPort->closeSerial();  // Cierra el puerto antes de abrir uno nuevo
    }
    magnitudePlot->getSerial()->setPortName(inputPortStr);  // Configura el puerto

    // Intenta abrir el puerto serial
    if (!magnitudePlot->getSerial()->open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Connect port", "Not able to read port");
        return; // Detiene la ejecución
    } else {
        qDebug() << "Puerto abierto exitosamente:" << inputPortStr;
    }
}

void Widget::calculateButtonClicked()
{
    qDebug() << "calculateButtonClicked() se llamó";  // Mensaje de depuración
    phasePlot->HandleUserInput(true);  // Cambia a modo estático al presionar calcular

    // Obtener la función de transferencia ingresada por el usuario
    QString transferFValueStr = ui->transferFValue->text();
    std::string transferFValueStd = transferFValueStr.toStdString();

    // Definir el objeto exprtk para evaluar la expresión
    exprtk::expression<double> expr;
    exprtk::parser<double> parser;
    exprtk::symbol_table<double> symbol_table;

    // Variables para la expresión
    double w = 0.0, wcV = 0.0, kV = 0.0, pi = 3.14159265358979323846, two= 2.0 ;

    // Registrar variables en la tabla de símbolos
    symbol_table.add_variable("w", w);
    symbol_table.add_variable("wc", wcV);
    symbol_table.add_variable("k", kV);
    symbol_table.add_constant("pi", 3.14159265358979323846);
    symbol_table.add_constant("two", 2.0);
    symbol_table.add_constants();  // Agregar constantes matemáticas

    // Asociar la tabla de símbolos con la expresión
    expr.register_symbol_table(symbol_table);

    // Procesar los valores de entrada desde los QLineEdits
    double frequencyMinV, frequencyMaxV, tV, dV;
    if (!(frequencyMinV = getDoubleFromLineEdit(ui->frequencyMinValue))) return;  // Mínima frecuencia
    if (!(frequencyMaxV = getDoubleFromLineEdit(ui->frequencyMaxValue))) return;  // Máxima frecuencia
    if (!(kV = getDoubleFromLineEdit(ui->kValue))) return;  // Ganancia k
    if (!(wcV = getDoubleFromLineEdit(ui->wcValue))) return;  // Frecuencia de corte wc
    if (!(tV = getDoubleFromLineEdit(ui->tValue))) return;  // Otro parámetro, t
    if (!(dV = getDoubleFromLineEdit(ui->dampingValue))) return;  // Factor de amortiguamiento d

    // Crear vectores para almacenar los valores de frecuencia y magnitud
    QVector<double> freqValues, magnitudeValues;
    const int numSamples = 100;  // Número de muestras
    std::vector<double> wValues;  // Vector para almacenar valores de w

    // Calcular w para cada frecuencia en el rango [frequencyMinV, frequencyMaxV]
    for (int i = 0; i <= numSamples; ++i) {
        double frequency = frequencyMinV + i * (frequencyMaxV - frequencyMinV) / numSamples;  // Calcular frecuencia
        w = two * pi * frequency;  // Calcular w
        wValues.push_back(w);  // Agregar valor de w

        // Cargar y evaluar la función de transferencia ingresada por el usuario
        std::string expr_str = transferFValueStd;
        if (!parser.compile(expr_str, expr)) {  // Intentar parsear
            QMessageBox::warning(this, "Error", "Error in expression syntax");
            return;
        }
        expr.value();  // Evaluar la función de transferencia para el valor actual de w

        // Calcular magnitud y fase de la función de transferencia
        double H_value = expr.value();  // Evaluar
        double magnitude = 20 * log10(std::abs(H_value));  // Calcular magnitud
        double phase = std::arg(H_value) * (180.0 / M_PI);  // Calcular fase

        // Almacenar los valores calculados
        freqValues.push_back(frequency);  // Almacenar la frecuencia actual
        magnitudeValues.push_back(magnitude);  // Almacenar la magnitud correspondiente

        // Imprimir valores de depuración
        qDebug() << "Frecuencia:" << frequency << " Magnitud:" << magnitude << " Fase:" << phase;
    }

    // Graficar los resultados de magnitud
    if (ui->customPlotMagnitude->graphCount() == 0)
        ui->customPlotMagnitude->addGraph();  // Agregar gráfico si no hay ninguno

    ui->customPlotMagnitude->graph(0)->setData(freqValues, magnitudeValues);  // Asignar datos
    ui->customPlotMagnitude->replot();  // Dibujar gráfico
    phasePlot->HandleUserInput(true);
}

// Función para obtener un valor de tipo double de un QLineEdit
double Widget::getDoubleFromLineEdit(QLineEdit *lineEdit)
{
    // Verificar si el puntero lineEdit no es nulo
    if (lineEdit) {
        bool ok;  // Variable para indicar si la conversión fue exitosa
        double value = lineEdit->text().toDouble(&ok);  // Intentar convertir el texto

        // Verificar si la conversión fue exitosa
        if (!ok) {
            QMessageBox::warning(this, "Invalid input", "Please enter a valid number.");
            return 0.0; // Retorna 0 si la conversión falla
        }
        return value;  // Retornar el valor convertido
    }
    return 0.0;  // Retornar 0 si lineEdit es nulo
}
