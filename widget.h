#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "qcustomplot/qcustomplot.h"
#include "magnitudeplot.h"
#include "QLineEdit"
#include "QPushButton"

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

private:
    MagnitudePlot *magnitudePlot;
    Ui::Widget *ui;
    QCustomPlot *customPlotMagnitude; // grafico de magnitud
    double outputValue = 0.0;
    void startPushButtonClicked();


};
#endif // WIDGET_H
