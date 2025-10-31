/********************************************************************************
    * 文件名称 ：Prt_FSM7050_UART2.cpp
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2025-06-13
    * 功     能：通信协议 界面
*********************************************************************************/
#include "Prt_FSM7050_UART2.h"
#include <QTimer>

Prt_FSM7050_UART2::Prt_FSM7050_UART2(QWidget *parent)
    : ProtocolBaseWidget(parent)
{
    // 左侧上中下区域合并为上下
    ElaScrollPageArea* leftBotArea = new ElaScrollPageArea(this);    // 下：参数设置
    ElaScrollPageArea* rightArea = new ElaScrollPageArea(this);      // 右：操作历史

    // 区域尺寸
    leftBotArea->setFixedHeight(550); // 合并后高度
    leftBotArea->setFixedWidth(300);
    rightArea->setFixedHeight(550);

    // 左侧垂直布局
    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(5);
    leftLayout->addWidget(leftBotArea);

    // 整体左右布局
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(5);
    mainLayout->addLayout(leftLayout, 3);
    mainLayout->addWidget(rightArea, 3);
    setLayout(mainLayout);


    // ===== 下：参数设置区（合并，单列显示）=====
    QVBoxLayout* paramLayout = new QVBoxLayout(leftBotArea);
    paramLayout->setContentsMargins(8, 8, 8, 8);
    paramLayout->setSpacing(8);

    QStringList paramNames = {
        "4.设备类型标识", 
        "5.设备ID", 
        "8.图像灰度", 
        "9.图像波门框灰度",
        "10.图像梯度H", 
        "11.图像梯度L", 
        "12.图像波门梯度H", 
        "13.图像波门梯度L", 
        "14.脱靶有效",
        "15-16.中心脱靶X", 
        "17-18.中心脱靶Y",
    };

    QFont labelFont;
    labelFont.setPointSize(15);
    labelFont.setBold(true);
    QFont sendFont;
    sendFont.setPointSize(12);
    sendFont.setBold(true);
    QFont editFont;
    editFont.setPointSize(15);

    int labelWidth = 180;
    for (int i = 0; i < paramNames.size(); ++i) {
        QHBoxLayout* row = new QHBoxLayout();
        paramLabels[i] = new ElaText(paramNames[i], leftBotArea);
        paramLabels[i]->setFont(labelFont);
        paramLabels[i]->setFixedWidth(labelWidth);
        paramEdits[i] = new ElaLineEdit(leftBotArea);
        paramEdits[i]->setFont(editFont);
        paramEdits[i]->setPlaceholderText("HEX");
        paramEdits[i]->setFixedWidth(90);
        paramEdits[i]->setAlignment(Qt::AlignLeft);
        row->addWidget(paramLabels[i]);
        row->addWidget(paramEdits[i]);
        paramLayout->addLayout(row);
    }
    
    paramEdits[0]->setText("0x5A"); // 设备类型标识
    paramEdits[1]->setText("0x11"); // 设备ID
    paramEdits[2]->setText("0x00"); // 图像灰度
    paramEdits[2]->setPlaceholderText("00~FF"); // 图像灰度范围
    paramEdits[3]->setText("0x00"); // 图像波门框灰度
    paramEdits[3]->setPlaceholderText("00~FF"); // 图像波门框灰度范围
    paramEdits[4]->setText("0x00"); // 图像梯度H
    paramEdits[4]->setPlaceholderText("00~FF"); // 图像梯度H范围
    paramEdits[5]->setText("0x00"); // 图像梯度L
    paramEdits[5]->setPlaceholderText("00~FF"); // 图像梯度L范围
    paramEdits[6]->setText("0x00"); // 图像波门梯度H
    paramEdits[6]->setPlaceholderText("00~FF"); // 图像波门梯度H
    paramEdits[7]->setText("0x00"); // 图像波门梯度L
    paramEdits[7]->setPlaceholderText("00~FF"); // 图像波门梯度L
    paramEdits[8]->setText("0x00"); // 脱靶有效
    paramEdits[8]->setPlaceholderText("0/1"); // 脱靶有效范围
    paramEdits[9]->setText("0"); // 中心脱靶X
    paramEdits[9]->setPlaceholderText("0~5375"); // 中心脱靶X范围
    paramEdits[10]->setText("0"); // 中心脱靶X
    paramEdits[10]->setPlaceholderText("0~5375"); // 中心脱靶Y范围

    // 发送按钮
    SendParamBtn = new ElaPushButton("发送协议", leftBotArea);
    SendParamBtn->setFont(sendFont);
    paramLayout->addWidget(SendParamBtn);

    // 循环发送按钮
    LoopSendBtn = new ElaToggleButton("循环发送", leftBotArea);
    LoopSendBtn->setFont(sendFont);
    paramLayout->addWidget(LoopSendBtn);

    loopSendTimer = new QTimer(this);
    loopSendTimer->setInterval(2); // 2.5ms约等于400Hz，实际2ms更快，建议2~3ms之间
    connect(loopSendTimer, &QTimer::timeout, this, [this]() {
        uint8_t data[20];
        Creat_data(data);
        COM_Send_data(data, sizeof(data));
        // 不记录发送历史
    });

    connect(LoopSendBtn, &ElaToggleButton::toggled, this, [this](bool checked) {
        if (checked) {
            loopSendTimer->start();
        } else {
            loopSendTimer->stop();
        }
    });

    connect(SendParamBtn, &ElaPushButton::clicked, this, &Prt_FSM7050_UART2::onSendParamClicked);

    // ===== 右：操作历史 =====
    QVBoxLayout* rightLayout = new QVBoxLayout(rightArea);
    rightLayout->setContentsMargins(8, 8, 8, 8);
    rightLayout->setSpacing(6);
    ElaText* logTitle = new ElaText("操作历史", rightArea);
    QFont logFont;
    logFont.setPointSize(16);
    logFont.setBold(true);
    logTitle->setFont(logFont);
    logList = new ElaListView(rightArea);
    logModel = new T_LogModel(this);
    logModel->setDisplayMode(Default);
    logList->setModel(logModel);
    rightLayout->addWidget(logTitle);
    rightLayout->addWidget(logList);
    logModel->appendLogList(QString("日志记录已开启"));
}

Prt_FSM7050_UART2::~Prt_FSM7050_UART2()
{
}

void Prt_FSM7050_UART2::SerialConnected(bool connected)
{
    // 连接状态处理
    if (connected) {
        // 连接成功时的逻辑
    } else {
        // 断开连接时的逻辑
    }
}


void Prt_FSM7050_UART2::SerialDataReceived(QByteArray data)
{
    
}

void Prt_FSM7050_UART2::Creat_data(uint8_t *data)
{
    // 协议头
    data[0] = 0x67;                // 帧头
    data[1] = 0x7E;                // 数据长度
    data[2] = 0x14;                // 数据长度（20字节）
    // 设备类型标识
    data[3] = paramEdits[0]->text().toUInt(nullptr, 16); // 设备类型
    // 设备ID
    data[4] = paramEdits[1]->text().toUInt(nullptr, 16); // 设备ID

    // 计数高、低字节（0~999循环）
    farme_count = (farme_count + 1) % 1000;
    data[5] = (farme_count >> 8) & 0xFF; // 高字节
    data[6] = farme_count & 0xFF;        // 低字节

    // 图像灰度
    data[7] = paramEdits[2]->text().toUInt(nullptr, 16); // 图像灰度
    // 图像波门灰度
    data[8] = paramEdits[3]->text().toUInt(nullptr, 16); // 波门灰度
    // 图像梯度高
    data[9] = paramEdits[4]->text().toUInt(nullptr, 16); // 梯度高
    // 图像梯度低
    data[10] = paramEdits[5]->text().toUInt(nullptr, 16); // 梯度低
    // 图像波门梯度高
    data[11] = paramEdits[6]->text().toUInt(nullptr, 16); // 波门梯度高
    // 图像波门梯度低
    data[12] = paramEdits[7]->text().toUInt(nullptr, 16); // 波门梯度低
    // 脱靶有效
    data[13] = paramEdits[8]->text().toUInt(nullptr, 16); // 脱靶有效

    // 中心脱靶X（十进制输入，拆高低字节）
    int xVal = paramEdits[9]->text().toInt(); // 十进制
    data[14] = (xVal >> 8) & 0xFF; // X高
    data[15] = xVal & 0xFF;        // X低

    // 中心脱靶Y（十进制输入，拆高低字节）
    int yVal = paramEdits[10]->text().toInt(); // 十进制
    data[16] = (yVal >> 8) & 0xFF; // Y高
    data[17] = yVal & 0xFF;        // Y低

    // 校验和
    uint8_t checksum = 0;
    for (int i = 0; i < 18; ++i) { // 0~17字节累加
        checksum += data[i];
    }
    data[18] = checksum;

    // 帧尾
    data[19] = 0x77;
}


void Prt_FSM7050_UART2::COM_Send_data(uint8_t *data, uint8_t len)
{
    // 发送数据的逻辑
    QByteArray sendData(reinterpret_cast<const char*>(data), len);
    emit Proctol_out(sendData); // 发射信号
}


void Prt_FSM7050_UART2::onSendParamClicked()
{
    uint8_t data[20];
    // 生成协议数据
    Creat_data(data);
    COM_Send_data(data, sizeof(data));
    //添加操作记录
    if (logList) logModel->appendLogList(QString("发送协议数据: %1").arg(QByteArray(reinterpret_cast<const char*>(data), sizeof(data)).toHex().toUpper()));
}

