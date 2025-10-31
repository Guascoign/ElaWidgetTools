#include "WaveTestThread.h"
#include <algorithm>
#include <QDebug>

WaveTestThread::WaveTestThread(QObject *parent)
    : QThread(parent), isRunning(false), windowSize(100)  // 设置默认窗口大小为100个数据点
{
}

WaveTestThread::~WaveTestThread()
{
    stopTest();
    wait();
}

void WaveTestThread::addData(double timestamp, double x, double y)
{
    QMutexLocker locker(&mutex);
    dataQueue.enqueue({timestamp, x, y});
    
    // 保持队列大小在窗口范围内
    while (dataQueue.size() > windowSize) {
        dataQueue.dequeue();
    }
    
    //qDebug() << "[WaveTest] 添加数据点:" << "时间戳:" << timestamp << "X:" << x << "Y:" << y << "队列大小:" << dataQueue.size();
}

void WaveTestThread::stopTest()
{
    QMutexLocker locker(&mutex);
    isRunning = false;
}

void WaveTestThread::run()
{
    isRunning = true;
    while (isRunning) {
        calculatePeakToPeak();
        msleep(100); // 每100ms计算一次
    }
}

void WaveTestThread::calculatePeakToPeak()
{
    QVector<WaveTestData> data;
    {
        QMutexLocker locker(&mutex);
        if (dataQueue.isEmpty()) {
            qDebug() << "[WaveTest] 数据队列为空，跳过计算";
            return;
        }
        
        // 复制数据用于计算
        for (const auto &d : dataQueue) {
            data.append(d);
        }
        //qDebug() << "[WaveTest] 开始计算峰峰值，数据点数量:" << data.size();
    }

    if (data.size() < 2) return;

    // 找到X和Y的最大最小值
    double x_min = std::numeric_limits<double>::max();
    double x_max = std::numeric_limits<double>::lowest();
    double y_min = std::numeric_limits<double>::max();
    double y_max = std::numeric_limits<double>::lowest();

    for (const auto &d : data) {
        x_min = std::min(x_min, d.x);
        x_max = std::max(x_max, d.x);
        y_min = std::min(y_min, d.y);
        y_max = std::max(y_max, d.y);
    }

    // 计算峰峰值
    double x_ptp = x_max - x_min;
    double y_ptp = y_max - y_min;

    //qDebug() << "[WaveTest] 峰峰值计算结果:";
    //qDebug() << "  X - 最小值:" << x_min << "最大值:" << x_max << "峰峰值:" << x_ptp;
    //qDebug() << "  Y - 最小值:" << y_min << "最大值:" << y_max << "峰峰值:" << y_ptp;
    //计算幅值(mrad)

    double x_mrad = x_ptp / 1252; // 1mrad对应1252码值
    double y_mrad = y_ptp / 1252; // 1mrad对应1252码值

    // 发送结果
    emit peakToPeakUpdated(x_ptp, y_ptp , x_mrad , y_mrad); // 转换为mrad
}
