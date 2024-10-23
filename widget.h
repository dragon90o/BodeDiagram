#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "qcustomplot/qcustomplot.h"
#include "magnitudeplot.h"
#include "QLineEdit"
#include "QPushButton"
#include "exprtk/exprtk.hpp"
#include "phaseplot.h"
#include "serialport.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
    double getDoubleFromLineEdit(QLineEdit *lineEdit);

private slots:

    void onStartButtonClicked();



private:
    SerialPort *serialPort;
    MagnitudePlot *magnitudePlot;
    PhasePlot *phasePlot;
    Ui::Widget *ui;
    QCustomPlot *customPlotMagnitude; // grafico de magnitud
    QCustomPlot *customPlotPhase; //grafico de phase
    double outputValue = 0.0;
    double frequencyMinV, frequencyMaxV, kV, wcV, tV, dV;
    void startPushButtonClicked();
    void calculateButtonClicked();



};
#endif // WIDGET_H
