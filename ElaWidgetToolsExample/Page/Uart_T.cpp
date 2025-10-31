#include "Uart_T.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedLayout>
#include <QLabel>
#include <QGroupBox>
#include <QTextCursor>
#include "ElaComboBox.h"
#include "ElaPushButton.h"
#include "ElaToggleButton.h"
#include "ElaPlainTextEdit.h"
#include "ElaSpinBox.h"
#include "ElaToolButton.h"
#include "ElaApplication.h"
#include "ElaWindow.h"
#include "Serial_thread.h"
#include "ElaMessageBar.h"
#include "../mainwindow.h"

//控件
#include "ElaText.h"
Uart_T::Uart_T(QWidget* parent)
    : T_BasePage(parent)
{
    ElaWindow* window = dynamic_cast<ElaWindow*>(parent);

    // 主体水平布局
    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("Serial");
    setTitleVisible(false);// 隐藏标题栏
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(8, 8, 8, 8);
    mainLayout->setSpacing(14);

    // 左侧参数设置区
    QWidget* leftWidget = new QWidget(centralWidget);
    leftWidget->setObjectName("uartLeftPanel");
    QVBoxLayout* leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->setSpacing(5);

    // 选择串口
    ElaText* portLabel = new ElaText("串口", leftWidget);
    portLabel->setTextPixelSize(14);
    portCombo = new ElaComboBox(leftWidget);
    connect(portCombo, &ElaComboBox::clicked, this, &Uart_T::on_portCombo_clicked);
    QHBoxLayout* portLayout = new QHBoxLayout();
    portCombo->setFixedSize(180, 30);
    portLayout->addWidget(portLabel);
    portLayout->addStretch();
    leftLayout->addLayout(portLayout);
    leftLayout->addWidget(portCombo);

    // 波特率
    ElaText* baudLabel = new ElaText("波特率", leftWidget);
    baudLabel->setTextPixelSize(14);
    baudCombo = new ElaComboBox(leftWidget);
    baudCombo->addItems({"9600", "19200", "38400", "57600", "115200", "230400", "460800", "921600"});
    baudCombo->setFixedSize(180, 30);
    leftLayout->addWidget(baudLabel);
    leftLayout->addWidget(baudCombo);

    // 数据位
    ElaText* dataBitsLabel = new ElaText("数据位", leftWidget);
    dataBitsLabel->setTextPixelSize(14);
    dataBitsCombo = new ElaComboBox(leftWidget);
    dataBitsCombo->setFixedSize(180, 30);
    dataBitsCombo->addItems({"8", "7", "6", "5"});
    leftLayout->addWidget(dataBitsLabel);
    leftLayout->addWidget(dataBitsCombo);

    // 校验位
    ElaText* parityLabel = new ElaText("校验位", leftWidget);
    parityLabel->setTextPixelSize(14);
    parityCombo = new ElaComboBox(leftWidget);
    parityCombo->setFixedSize(180, 30);
    parityCombo->addItems({"None", "Even", "Odd"});
    leftLayout->addWidget(parityLabel);
    leftLayout->addWidget(parityCombo);

    // 停止位
    ElaText* stopBitsLabel = new ElaText("停止位", leftWidget);
    stopBitsLabel->setTextPixelSize(14);
    stopBitsCombo = new ElaComboBox(leftWidget);
    stopBitsCombo->setFixedSize(180, 30);
    stopBitsCombo->addItems({"1", "1.5", "2"});
    leftLayout->addWidget(stopBitsLabel);
    leftLayout->addWidget(stopBitsCombo);

    // 流控
    ElaText* flowControlLabel = new ElaText("流控", leftWidget);
    flowControlLabel->setTextPixelSize(14);
    flowControlCombo = new ElaComboBox(leftWidget);
    flowControlCombo->setFixedSize(180, 30);
    flowControlCombo->addItems({"None", "RTS/CTS", "XON/XOFF"});
    leftLayout->addWidget(flowControlLabel);
    leftLayout->addWidget(flowControlCombo);
    //间距
    leftLayout->addSpacing(10);
    // 连接按钮
    connectBtn = new ElaToggleButton("连接", leftWidget);
    connectBtn->setFixedSize(170, 35);
    connectBtn->setIsToggled(false);
    connectBtn->setText("连接");
    connect(connectBtn, &ElaToggleButton::toggled, this, &Uart_T::on_connectBtn_toggled);

    //设置comboBox默认选择
    portCombo->setCurrentIndex(0);
    baudCombo->setCurrentIndex(4); // 默认115200
    dataBitsCombo->setCurrentIndex(0); // 默认8位
    parityCombo->setCurrentIndex(0); // 默认无校验
    stopBitsCombo->setCurrentIndex(0); // 默认1位停止
    flowControlCombo->setCurrentIndex(0); // 默认无流控

    // 用水平布局让按钮向右移动
    QHBoxLayout* connectBtnLayout = new QHBoxLayout();
    connectBtnLayout->addSpacing(5); // 右移5像素
    connectBtnLayout->addWidget(connectBtn);  // 按钮在右
    connectBtnLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->addLayout(connectBtnLayout);

    leftLayout->addStretch();



    // 底部按钮区
    QHBoxLayout* bottomBtnLayout = new QHBoxLayout();
    ElaToolButton* docBtn = new ElaToolButton(leftWidget);
    docBtn->setElaIcon(ElaIconType::Book);
    docBtn->setToolTip("帮助文档");

    docBtn->setFixedSize(28, 28);
    bottomBtnLayout->addStretch();
    bottomBtnLayout->addWidget(docBtn);
    leftLayout->addLayout(bottomBtnLayout);
    leftWidget->setFixedWidth(190);// 设置左侧宽度
    mainLayout->addWidget(leftWidget , 2);

    // 右侧收发显示区
    QWidget* rightWidget = new QWidget(centralWidget);
    QVBoxLayout* rightLayout = new QVBoxLayout(rightWidget);
    rightLayout->setSpacing(10);

    // 顶部大文本框
    recvEdit = new ElaPlainTextEdit(rightWidget);
    recvEdit->setPlaceholderText("数据接收区...");
    rightLayout->addWidget(recvEdit, 5);

    // 中部收发模式与统计
    QHBoxLayout* statusLayout = new QHBoxLayout();
    recvModeBtn = new ElaToggleButton("接收:HEX", rightWidget);
    sendModeBtn = new ElaToggleButton("发送:HEX", rightWidget);
    recvModeBtn->setFixedSize(80, 36);
    sendModeBtn->setFixedSize(80, 36);
    rxLabel = new ElaText("Rx: 0 Bytes", rightWidget);
    txLabel = new ElaText("Tx: 0 Bytes", rightWidget);
    rxLabel->setTextPixelSize(15);
    txLabel->setTextPixelSize(15);
    ElaToggleButton* autoScrollBtn = new ElaToggleButton("滚动:关闭", rightWidget);
    autoScrollBtn->setFixedSize(80, 36);
    //ElaToggleButton点击后切换文本
    connect(autoScrollBtn, &ElaToggleButton::toggled, [autoScrollBtn](bool checked) {
        if (checked) {
            autoScrollBtn->setText("滚动:开启");
        } else {
            autoScrollBtn->setText("滚动:关闭");
        }
    });
    connect(recvModeBtn, &ElaToggleButton::toggled, [this](bool checked) {
        recvHexMode = !checked;
        if (checked) {
            recvModeBtn->setText("接收:ASCII");
        } else {
            recvModeBtn->setText("接收:HEX");
        }
    });
    connect(sendModeBtn, &ElaToggleButton::toggled, [this](bool checked) {
        sendHexMode = !checked;
        if (checked) {
            sendModeBtn->setText("发送:ASCII");
            sendEdit->setPlaceholderText("输入要发送的ASCII文本...");
        } else {
            sendModeBtn->setText("发送:HEX");
            sendEdit->setPlaceholderText("输入要发送的HEX数据...\n格式: FF 00 01 或 FF0001");
        }
    });
    ElaPushButton* clearBtn = new ElaPushButton("清屏", rightWidget);
    clearBtn->setFixedSize(80, 36);
    statusLayout->addWidget(recvModeBtn);
    statusLayout->addWidget(sendModeBtn);
    statusLayout->addWidget(rxLabel);
    statusLayout->addWidget(txLabel);
    statusLayout->addStretch();
    statusLayout->addWidget(autoScrollBtn);
    statusLayout->addWidget(clearBtn);
    rightLayout->addLayout(statusLayout);

    // 清屏按钮点击事件
    connect(clearBtn, &QPushButton::clicked, this, [this]() {
        recvEdit->clear();
        dataRxNumber = 0;
        dataTxNumber = 0;
        rxLabel->setText("Rx: 0 Bytes");
        txLabel->setText("Tx: 0 Bytes");
        ElaMessageBar::information(ElaMessageBarType::TopRight, "完成", "接收已清空", 2000);
    });

    // 底部发送区（右下角悬浮发送按钮）
    QWidget* sendArea = new QWidget(rightWidget);
    QVBoxLayout* sendVLayout = new QVBoxLayout(sendArea);
    sendVLayout->setContentsMargins(0, 0, 0, 12);
    sendVLayout->setSpacing(0);

    // 叠加层：编辑框和右下角按钮
    QWidget* sendStack = new QWidget(sendArea);
    sendStack->setAttribute(Qt::WA_StyledBackground); // 防止透明
    QGridLayout* stackGrid = new QGridLayout(sendStack);
    stackGrid->setContentsMargins(0, 0, 0, 0);
    stackGrid->setSpacing(0);

    sendEdit = new ElaPlainTextEdit(sendStack);
    sendEdit->setPlaceholderText("输入要发送的HEX数据...\n格式: FF 00 01 或 FF0001");
    stackGrid->addWidget(sendEdit, 0, 0);

    // 右下角按钮层
    QWidget* btnFloatWidget = new QWidget(sendStack);
    btnFloatWidget->setAttribute(Qt::WA_TranslucentBackground); // 允许背景透明
    QHBoxLayout* btnFloatLayout = new QHBoxLayout(btnFloatWidget);
    btnFloatLayout->addStretch();
    ElaToggleButton* autoSendBtn = new ElaToggleButton("自动发送", btnFloatWidget);
    ElaPushButton* sendBtn = new ElaPushButton("发送", btnFloatWidget);
    sendBtn->setFixedSize(80, 36);
    autoSendBtn->setFixedSize(80, 36);
    btnFloatLayout->addWidget(autoSendBtn, 0, Qt::AlignRight | Qt::AlignBottom);
    btnFloatLayout->addWidget(sendBtn, 0, Qt::AlignRight | Qt::AlignBottom);
    btnFloatLayout->setContentsMargins(0, 0, 10, 8); // 右下角边距
    btnFloatLayout->setSpacing(4);
    btnFloatLayout->setAlignment(Qt::AlignRight | Qt::AlignBottom);
    btnFloatWidget->setLayout(btnFloatLayout);

    // 让按钮浮在右下角
    stackGrid->addWidget(btnFloatWidget, 0, 0, Qt::AlignRight | Qt::AlignBottom);

    // 发送按钮点击事件
    connect(sendBtn, &QPushButton::clicked, this, [this]() {
        QString text = sendEdit->toPlainText();
        if (!text.isEmpty()) {
            QByteArray data;
            QString displayText;
            
            if (sendHexMode) {
                // HEX模式：将输入的hex字符串转换为字节数组
                data = hexStringToByteArray(text);
                displayText = text; // 显示原始hex输入
            } else {
                // ASCII模式：直接转换为UTF-8字节数组
                data = text.toUtf8();
                displayText = text;
            }
            
            if (!data.isEmpty()) {
                Send_Data(data);
                // 文本同步到接收区，前缀->(发送)
                recvEdit->appendPlainText("-> " + displayText);
                
                // 检查文本框内容大小，超过2K时清空前1K防止卡顿
                checkAndTrimTextBuffer();
            }
        }
    });

    sendVLayout->addWidget(sendStack);

    rightLayout->addWidget(sendArea, 1);//5:1 发送:接收比例

    mainLayout->addWidget(rightWidget, 4);// 6:2 左右比例

    addCentralWidget(centralWidget, true, true, 0);

    //获取可用串口列表
    Update_SerialPort();
}

Uart_T::~Uart_T()
{
}

void Uart_T::on_portCombo_clicked()
{
    //获取可用串口列表
    Update_SerialPort();
}

void Uart_T::Update_SerialPort()
{
 QStringList serialNamePort;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        QString portInfo = QString("%1: %2").arg(info.portName()).arg(info.description());
        serialNamePort << portInfo;
    }
    portCombo->clear();
    portCombo->addItems(serialNamePort);

}

void Uart_T::on_connectBtn_toggled(bool checked)
{
    if (checked) {
        qint32 state = 0;
        QString portName = portCombo->currentText().split(":").first();
        currentPortName = portCombo->currentText(); // 保存当前端口名
        int baudRate = baudCombo->currentText().toInt();
        
        // 创建串口配置结构体
        SerialConfig config;
        config.portName = portName;
        config.baudRate = baudRate;
        config.dataBits = getDataBits();
        config.parity = getParity();
        config.stopBits = getStopBits();
        config.flowControl = getFlowControl();
        
        // 创建ComSerialPort对象，使用配置参数
        if (!serial) serial = new ComSerialPort(config, &state, nullptr);
        
        if (state) {
            connectBtn->setText("断开");
            serialOpened = true;
            // 连接信号槽
            connect(this, &Uart_T::SendData, serial, &ComSerialPort::SendSerialData, Qt::AutoConnection);
            connect(serial, &ComSerialPort::UpdateData, this, &Uart_T::Recv_Data, Qt::AutoConnection);
            // 禁用参数设置
            portCombo->setEnabled(false);
            baudCombo->setEnabled(false);
            dataBitsCombo->setEnabled(false);
            parityCombo->setEnabled(false);
            stopBitsCombo->setEnabled(false);
            flowControlCombo->setEnabled(false);
            
            // 显示详细的连接信息
            QString configInfo = QString("端口: %1, 波特率: %2, 数据位: %3, 校验: %4, 停止位: %5, 流控: %6")
                                 .arg(portName)
                                 .arg(baudRate)
                                 .arg(dataBitsCombo->currentText())
                                 .arg(parityCombo->currentText())
                                 .arg(stopBitsCombo->currentText())
                                 .arg(flowControlCombo->currentText());
            
            ElaMessageBar::success(ElaMessageBarType::TopRight, "已连接", configInfo, 3000);
            emit Com_Opened(currentPortName);

        } else {
            ElaMessageBar::error(ElaMessageBarType::TopRight, "连接失败", currentPortName, 2000);
            connectBtn->setText("连接");
            //released
            connectBtn->setIsToggled(false);
            
        }
    } else {
        if (serial) {
            serial->CloseSerial();
            serial->deleteLater();
            serial = nullptr;
            ElaMessageBar::success(ElaMessageBarType::TopRight, "已断开", currentPortName, 2000);
            emit Com_Closed();
        }
        serialOpened = false;
        connectBtn->setText("连接");
        portCombo->setEnabled(true);
        baudCombo->setEnabled(true);
        dataBitsCombo->setEnabled(true);
        parityCombo->setEnabled(true);
        stopBitsCombo->setEnabled(true);
        flowControlCombo->setEnabled(true);
        dataRxNumber = 0;
        dataTxNumber = 0;
        rxLabel->setText("Rx: 0 Bytes");
        txLabel->setText("Tx: 0 Bytes");
    }
}

void Uart_T::updateStatus()
{
      rxLabel->setText(QString("Rx: %1 Bytes").arg(dataRxNumber));
      txLabel->setText(QString("Tx: %1 Bytes").arg(dataTxNumber));
}

void Uart_T::Recv_Data(QByteArray data)
{
    dataRxNumber += data.size(); // 累加接收字节数
    QString rxText;
    if (dataRxNumber < 1024) {
        rxText = QString("Rx: %1 Bytes").arg(dataRxNumber);
    } else if (dataRxNumber < 1024 * 1024) {
        rxText = QString("Rx: %1 KB").arg(QString::number(dataRxNumber / 1024.0, 'f', 2));
    } else {
        rxText = QString("Rx: %1 MB").arg(QString::number(dataRxNumber / 1024.0 / 1024.0, 'f', 2));
    }
    rxLabel->setText(rxText);
    
    // 根据接收模式显示数据
    QString displayText;
    if (recvHexMode) {
        // HEX模式：显示十六进制
        QStringList hexList;
        for (int i = 0; i < data.size(); ++i) {
            hexList << QString("%1").arg((unsigned char)data[i], 2, 16, QChar('0')).toUpper();
        }
        displayText = hexList.join(" ");
    } else {
        // ASCII模式：显示文本
        displayText = QString::fromUtf8(data);
    }
    
    // 显示接收数据，前缀<-(接收)
    recvEdit->appendPlainText("<- " + displayText);
    
    // 检查文本框内容大小，超过2K时清空前1K防止卡顿
    checkAndTrimTextBuffer();
    
    emit RecvData(data); // 继续发射给外部
}



void Uart_T::Send_Data(QByteArray data)
{
    if (serial && serialOpened) {
        serial->SendSerialData(data);
        dataTxNumber += data.size(); // 累加发送字节数
        QString txText;
        if (dataTxNumber < 1024) {
            txText = QString("Tx: %1 Bytes").arg(dataTxNumber);
        } else if (dataTxNumber < 1024 * 1024) {
            txText = QString("Tx: %1 KB").arg(QString::number(dataTxNumber / 1024.0, 'f', 2));
        } else {
            txText = QString("Tx: %1 MB").arg(QString::number(dataTxNumber / 1024.0 / 1024.0, 'f', 2));
        }
        txLabel->setText(txText);
    } else {
        qDebug() << "串口未打开，无法发送数据！";
    }
}

void Uart_T::checkAndTrimTextBuffer()
{
    QString currentText = recvEdit->toPlainText();
    if (currentText.size() > 10 * 1024) { // 超过10K字符
        // 保留后面的内容，删除前5K字符
        int removeSize = 5 * 1024;
        if (currentText.size() > removeSize) {
            QString newText = currentText.mid(removeSize);
            recvEdit->setPlainText(newText);
            // 移动光标到末尾
            QTextCursor cursor = recvEdit->textCursor();
            cursor.movePosition(QTextCursor::End);
            recvEdit->setTextCursor(cursor);
        }
    }
}

    /*********************************************************/
    // 连接串口打开信号
    // connect(p_serialManager, &SerialManager::Com_Opened, this, [=](const QString &portName){
    //     qDebug() << "Serial port opened:" << portName;
    //     //连接fsm发送数据信号->串口发送数据->串口线程发送
    //     connect(p_fsmControl, &FSM_Control::sendSerialData, p_serialManager, &SerialManager::Send_Data);
    //     //连接串口接收数据UpdateData->fsm接收数据
    //     connect(p_serialManager, &SerialManager::RecvData, p_fsmControl, &FSM_Control::onSerialDataReceived);
        
    // });
    // 连接串口关闭信号
    // connect(p_serialManager, &SerialManager::Com_Closed, this, [=](){
    //     qDebug() << "Serial port closed";
    //     // 断开连接
    //     disconnect(p_fsmControl, &FSM_Control::sendSerialData, p_serialManager, &SerialManager::Send_Data);
    //     disconnect(p_serialManager, &SerialManager::RecvData, p_fsmControl, &FSM_Control::onSerialDataReceived);
    // });
    /*********************************************************/
QByteArray Uart_T::hexStringToByteArray(const QString &hexString)
{
    QByteArray result;
    QString cleanHex = hexString.simplified().remove(' ').remove('\n').remove('\r').remove('\t');
    
    // 确保是偶数长度
    if (cleanHex.length() % 2 != 0) {
        cleanHex.prepend('0');
    }
    
    for (int i = 0; i < cleanHex.length(); i += 2) {
        bool ok;
        QString hexByte = cleanHex.mid(i, 2);
        uchar byte = hexByte.toUInt(&ok, 16);
        if (ok) {
            result.append(byte);
        } else {
            // 如果转换失败，跳过这个字节
            qDebug() << "Invalid hex byte:" << hexByte;
        }
    }
    return result;
}

// 串口参数转换辅助方法实现
QSerialPort::DataBits Uart_T::getDataBits() const
{
    int dataBits = dataBitsCombo->currentText().toInt();
    switch (dataBits) {
        case 5: return QSerialPort::Data5;
        case 6: return QSerialPort::Data6;
        case 7: return QSerialPort::Data7;
        case 8: 
        default: return QSerialPort::Data8;
    }
}

QSerialPort::Parity Uart_T::getParity() const
{
    QString parity = parityCombo->currentText();
    if (parity == "Even") {
        return QSerialPort::EvenParity;
    } else if (parity == "Odd") {
        return QSerialPort::OddParity;
    } else {
        return QSerialPort::NoParity; // "None"
    }
}

QSerialPort::StopBits Uart_T::getStopBits() const
{
    QString stopBits = stopBitsCombo->currentText();
    if (stopBits == "1.5") {
        return QSerialPort::OneAndHalfStop;
    } else if (stopBits == "2") {
        return QSerialPort::TwoStop;
    } else {
        return QSerialPort::OneStop; // "1"
    }
}

QSerialPort::FlowControl Uart_T::getFlowControl() const
{
    QString flowControl = flowControlCombo->currentText();
    if (flowControl == "RTS/CTS") {
        return QSerialPort::HardwareControl;
    } else if (flowControl == "XON/XOFF") {
        return QSerialPort::SoftwareControl;
    } else {
        return QSerialPort::NoFlowControl; // "None"
    }
}