#ifndef WAVETESTTHREAD_H
#define WAVETESTTHREAD_H

#include <QThread>
#include <QVector>
#include <QMutex>
#include <QQueue>

struct WaveTestData {
    double timestamp;
    double x;
    double y;
};

class WaveTestThread : public QThread
{
    Q_OBJECT
public:
    explicit WaveTestThread(QObject *parent = nullptr);
    ~WaveTestThread();

    void addData(double timestamp, double x, double y);
    void stopTest();
    bool isRunning;
signals:
    void peakToPeakUpdated(double x_value, double y_value , double x_mrad, double y_mrad);
    void overshootUpdated(double x_ratio, double y_ratio);

protected:
    void run() override;

private:
    QMutex mutex;
    QQueue<WaveTestData> dataQueue;
    const int windowSize = 150; // 5秒数据 (假设30Hz采样率)

    void calculatePeakToPeak();
};

#endif // WAVETESTTHREAD_H
