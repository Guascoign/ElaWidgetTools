#ifndef UART_T_H
#define UART_T_H

#include "T_BasePage.h"

#include <QWidget>
#include <QQueue>
#include <QThread>
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "Serial_thread.h"

class ElaComboBox;
class ElaPushButton;
class ElaToggleButton;
class ElaPlainTextEdit;
class ElaSpinBox;
class ElaText;
class ElaToolButton;
class ElaMessageBar;
class Uart_T : public T_BasePage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit Uart_T(QWidget* parent = nullptr);
    ~Uart_T();
    void Send_Data(const QByteArray data); //发送数据

private:
    ElaComboBox* portCombo{nullptr};
    ElaComboBox* baudCombo{nullptr};
    ElaComboBox* dataBitsCombo{nullptr};
    ElaComboBox* parityCombo{nullptr};
    ElaComboBox* stopBitsCombo{nullptr};
    ElaComboBox* flowControlCombo{nullptr};
    ElaToggleButton* connectBtn{nullptr};
    ElaPlainTextEdit* recvEdit{nullptr};
    ElaPlainTextEdit* sendEdit{nullptr};
    ElaText* rxLabel{nullptr};
    ElaText* txLabel{nullptr};

    ComSerialPort* serial = nullptr;
    bool serialOpened = false;
    qint64 dataRxNumber = 0;
    qint64 dataTxNumber = 0;
    QString currentPortName; // 当前打开的串口名称
    bool recvHexMode = true; // 接收显示模式：true=HEX, false=ASCII
    bool sendHexMode = true; // 发送显示模式：true=HEX, false=ASCII
    ElaToggleButton* recvModeBtn = nullptr; // 接收模式按钮
    ElaToggleButton* sendModeBtn = nullptr; // 发送模式按钮

private slots:
    void on_portCombo_clicked();
    void Update_SerialPort();
    void on_connectBtn_toggled(bool checked);
    void updateStatus();
    void Recv_Data(QByteArray data); //接收数据

private:
    void checkAndTrimTextBuffer(); // 检查并修剪文本缓冲区大小
    QByteArray hexStringToByteArray(const QString &hexString); // HEX字符串转字节数组
    
    // 串口参数转换辅助方法
    QSerialPort::DataBits getDataBits() const;
    QSerialPort::Parity getParity() const;
    QSerialPort::StopBits getStopBits() const;
    QSerialPort::FlowControl getFlowControl() const;

signals:
    void Com_Opened(const QString &portName); //串口打开信号
    void Com_Closed(); //串口关闭信号
    void Com_Error(const QString &errorMessage); //串口错误信号
    void SendData(QByteArray data);//发送出数据
    void RecvData(QByteArray data);//接收数据

};

#endif // UART_T_H
