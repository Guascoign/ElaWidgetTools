#include "LineChart_T.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTimer>
#include <QtMath>
#include "ElaApplication.h"
#include "ElaWindow.h"
//控件
#include "ElaText.h"
#include "QCustomplot/qcustomplot.h"
LineChart_T::LineChart_T(QWidget* parent)
    : T_BasePage(parent), timeCounter(0.0)
{
    ElaWindow* window = dynamic_cast<ElaWindow*>(parent);

    // 预览窗口标题
    setWindowTitle("LineChart");
    // 顶部元素
    createCustomWidget("实时正弦波图表");

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("LineChart");
    QVBoxLayout* centerLayout = new QVBoxLayout(centralWidget);
    centerLayout->setContentsMargins(0, 0, 0, 0);
    addCentralWidget(centralWidget, true, true, 0);

    // 创建QCustomPlot图表
    customPlot = new QCustomPlot(this);
    customPlot->setMinimumSize(800, 400);
    
    // 配置图表
    customPlot->addGraph();
    customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255), 2));
    customPlot->graph(0)->setName("正弦波");
    
    // 设置坐标轴标签
    customPlot->xAxis->setLabel("时间 (秒)");
    customPlot->yAxis->setLabel("幅度");
    
    // 设置坐标轴范围
    customPlot->xAxis->setRange(0, 10);
    customPlot->yAxis->setRange(-1.5, 1.5);
    
    // 设置网格
    customPlot->xAxis->grid()->setVisible(true);
    customPlot->yAxis->grid()->setVisible(true);
    customPlot->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
    customPlot->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
    
    // 启用图例
    customPlot->legend->setVisible(true);
    customPlot->legend->setFont(QFont("微软雅黑", 9));
    customPlot->legend->setRowSpacing(-3);
    customPlot->legend->setBrush(QBrush(QColor(255, 255, 255, 200)));
    customPlot->legend->setBorderPen(QPen(QColor(0, 0, 0, 0)));
    
    // 设置背景色
    customPlot->setBackground(QBrush(QColor(245, 245, 245)));
    
    // 启用交互
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    // 创建定时器
    dataTimer = new QTimer(this);
    connect(dataTimer, &QTimer::timeout, this, &LineChart_T::updatePlot);
    //dataTimer->start(50); // 每50毫秒更新一次，20 FPS

    // 数据统计文本
    ElaText* dataText = new ElaText(QString("实时正弦波数据可视化"), this);
    ElaText* dataText1 = new ElaText(QString("频率: 1Hz, 采样率: 20Hz"), this);
    
    // 设置文本样式
    dataText->setTextPixelSize(15);
    dataText1->setTextPixelSize(13);
    
    // 创建刷新控制按钮
    ElaToggleButton* refreshToggle = new ElaToggleButton(this);
    refreshToggle->setText("开始刷新");
    refreshToggle->setIsToggled(true);  // 默认开启状态
    connect(refreshToggle, &ElaToggleButton::toggled, this, &LineChart_T::onToggleButtonClicked);
    
    // 初始状态下启动定时器
   // dataTimer->start(50);
    
    // 添加到布局
    QHBoxLayout* controlLayout = new QHBoxLayout();
    controlLayout->addWidget(refreshToggle);
    controlLayout->addStretch();
    
    centerLayout->addWidget(dataText);
    centerLayout->addWidget(dataText1);
    centerLayout->addLayout(controlLayout);
    centerLayout->addWidget(customPlot);
    centerLayout->addStretch();
}

LineChart_T::~LineChart_T()
{
    if (dataTimer) {
        dataTimer->stop();
        delete dataTimer;
    }
}

void LineChart_T::onToggleButtonClicked(bool checked)
{
    if (checked) {
        dataTimer->start(50);  // 开始刷新
    } else {
        dataTimer->stop();     // 停止刷新
    }
}

void LineChart_T::updatePlot()
{
    // 生成正弦波数据
    double currentTime = timeCounter;
    double amplitude = qSin(currentTime * 2 * M_PI); // 1Hz频率的正弦波
    
    // 添加新数据点
    xData.append(currentTime);
    yData.append(amplitude);
    
    // 限制数据点数量，保持图表性能
    if (xData.size() > maxDataPoints) {
        xData.removeFirst();
        yData.removeFirst();
    }
    
    // 更新图表数据
    customPlot->graph(0)->setData(xData, yData);
    
    // 动态调整x轴范围，始终显示最后10秒的数据
    if (currentTime > 10.0) {
        customPlot->xAxis->setRange(currentTime - 10.0, currentTime);
    }
    
    // 重绘图表
    customPlot->replot();
    
    // 增加时间计数器
    timeCounter += 0.05; // 对应50ms的时间间隔
}
