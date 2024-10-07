#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "qcustomplot/qcustomplot.h"
#include "magnitudeplot.h"

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




};
#endif // WIDGET_H
