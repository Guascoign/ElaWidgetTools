#ifndef LINECHART_T_H
#define LINECHART_T_H

#include "T_BasePage.h"
#include "QCustomplot/qcustomplot.h"
#include <QTimer>
#include "ElaToggleButton.h"

class LineChart_T : public T_BasePage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit LineChart_T(QWidget* parent = nullptr);
    ~LineChart_T();

private slots:
    void updatePlot();
    void onToggleButtonClicked(bool checked);

private:
    QCustomPlot* customPlot;
    QTimer* dataTimer;
    double timeCounter;
    QVector<double> xData, yData;
    const int maxDataPoints = 1000;
};

#endif // LINECHART_T_H
