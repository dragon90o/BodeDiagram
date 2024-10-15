#include "widget.h"
#include "ui_widget.h"
#include "magnitudeplot.h"
#include "QMessageBox"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    connect(ui->startPushButton, &QPushButton::clicked, this, &Widget::startPushButtonClicked);
    magnitudePlot = new MagnitudePlot(ui->customPlotMagnitude, this);
}

Widget::~Widget()
{
    delete ui;  // Elimina la UI
}


void Widget::startPushButtonClicked()
{

    QString inputValueStr = ui->usbOutputValue->text();
    bool ok;
    double inputValue = inputValueStr.toDouble(&ok); // just numbers

    //si fueron solo numeros es exitoso
    if (!ok) {
        QMessageBox::warning(this,"invalid input","try just numbers no symbols.");
        return;
    }
    qDebug() << "El valor ingresado es: " << inputValue;
    //asigna el valor de salida de el texto outputvalue en usbport
    outputValue = inputValue;

    magnitudePlot->setOutputAmplitude(outputValue);


    QString inputPortStr = ui->usbPort->text();
    //cierra el puerto antes de abrir uno nuevo
    magnitudePlot->closeSerial();

    //configura el puerto inicial con el nombre ingresado
    magnitudePlot->getSerial()->setPortName(inputPortStr);

    //intento de abrir en puerto serial
    if (!magnitudePlot->getSerial()->open(QIODevice::ReadOnly)){
        QMessageBox::warning(this,"Connect port","Not able to read port");
        return; //Deten la ejecucion

    } else {
        qDebug() << "Puerto abierto exitosamente:" << inputPortStr;
    }

}

