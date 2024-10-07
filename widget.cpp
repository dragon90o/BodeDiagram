#include "widget.h"
#include "ui_widget.h"
#include "magnitudeplot.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);


    magnitudePlot = new MagnitudePlot(ui->customPlotMagnitude, this);
}

Widget::~Widget()
{
    delete ui;  // Elimina la UI
}

