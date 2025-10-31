/********************************************************************************
    * 文件名称 Serial_thread.h
    * 版     本：V1.0
    * 编写日期 ：2025-02-18
    * 功     能：串口收发线程
*********************************************************************************
V1.0 2025-02-18 First release @ZM
*********************************************************************************/
#ifndef SERIAL_THREAD_H
#define SERIAL_THREAD_H

#include <QThread>
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QQueue>

// 串口配置结构体
struct SerialConfig {
    QString portName;
    qint32 baudRate;
    QSerialPort::DataBits dataBits;
    QSerialPort::Parity parity;
    QSerialPort::StopBits stopBits;
    QSerialPort::FlowControl flowControl;
};

class ComSerialPort : public QObject
{
    Q_OBJECT
public:
    explicit ComSerialPort(const SerialConfig& config, qint32* state, QObject *parent = nullptr);
    explicit ComSerialPort(QString PortName,qint32 BaudRate,qint32* state,QObject *parent = nullptr); // 保持兼容性
    ~ComSerialPort();
    void CloseSerial();
    void handleSerialError(QSerialPort::SerialPortError error);

signals:
    void UpdateData(QByteArray data);//上传串口接收到的数据
    void UpdateError(const QString &errorMessage);//上传串口错误
public slots:
    void RcvData();
    void SendSerialData(QByteArray data);

private:
    bool InitSerial(const SerialConfig& config);
    bool InitSerial(QString portname,qint32 BaudRate); // 保持兼容性
    QSerialPort *Serial;
    QThread* thread;
    QQueue<QByteArray> ringBuffer;
};


#endif // SERIAL_THREAD_H
