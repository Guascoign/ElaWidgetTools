/********************************************************************************
    * 文件名称 ：Prt_FSM7050_UART1.cpp
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2025-06-06
    * 功     能：通信协议 界面
*********************************************************************************/
#include "Prt_FSM7050_UART1.h"
#include <QTimer>

Prt_FSM7050_UART1::Prt_FSM7050_UART1(QWidget *parent)
    : ProtocolBaseWidget(parent)
{
    // 左侧上中下区域
    ElaScrollPageArea* leftTopArea = new ElaScrollPageArea(this);    // 上：信息显示
    ElaScrollPageArea* leftMidArea = new ElaScrollPageArea(this);    // 中：模式选择
    ElaScrollPageArea* leftBotArea = new ElaScrollPageArea(this);    // 下：参数设置
    ElaScrollPageArea* rightArea = new ElaScrollPageArea(this);      // 右：操作历史

    leftTopArea->setFixedHeight(180);
    leftTopArea->setFixedWidth(300);
    leftMidArea->setFixedHeight(80);
    leftMidArea->setFixedWidth(300);
    leftBotArea->setFixedHeight(110);
    leftBotArea->setFixedWidth(300);
    rightArea->setFixedHeight(380);

    // 左侧垂直布局
    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(5);
    leftLayout->addWidget(leftTopArea);
    leftLayout->addWidget(leftMidArea);
    leftLayout->addWidget(leftBotArea);

    // 整体左右布局
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(5);
    mainLayout->addLayout(leftLayout, 3);
    mainLayout->addWidget(rightArea, 3);
    setLayout(mainLayout);

    // ===== 上：信息显示区 =====
    QGridLayout* infoGrid = new QGridLayout(leftTopArea);
    infoGrid->setContentsMargins(4, 4, 4, 4);
    infoGrid->setSpacing(2);

    QFont labelFont; labelFont.setPointSize(15); labelFont.setBold(true);
    QFont valueFont; valueFont.setPointSize(15);

    // 新增：接收帧数和校验错误帧数
    QHBoxLayout* infoRow = new QHBoxLayout();
    RecvCountLabel = new ElaText("接收帧数: 0", leftTopArea);
    RecvCountLabel->setFont(labelFont);
    CheckErrLabel = new ElaText("校验错误帧: 0", leftTopArea);
    CheckErrLabel->setFont(labelFont);
    infoRow->addSpacing(10);
    infoRow->addWidget(RecvCountLabel);
    infoRow->addSpacing(20);
    infoRow->addWidget(CheckErrLabel);
    infoRow->addStretch();
    infoGrid->addLayout(infoRow, 0, 0, 1, 4);

    ElaText* Text1 = new ElaText("相对位置", leftTopArea); Text1->setFont(labelFont);
    ElaText* Text2 = new ElaText("零点位置", leftTopArea); Text2->setFont(labelFont);

    ElaText* x1Label = new ElaText("X1:", leftTopArea); x1Label->setFont(labelFont);
    ElaText* y1Label = new ElaText("Y1:", leftTopArea); y1Label->setFont(labelFont);
    ElaText* x1ZeroLabel = new ElaText("X1:", leftTopArea); x1ZeroLabel->setFont(labelFont);
    ElaText* y1ZeroLabel = new ElaText("Y1:", leftTopArea); y1ZeroLabel->setFont(labelFont);

    X1_Info = new ElaText("N/A", leftTopArea); X1_Info->setFont(valueFont);
    Y1_Info = new ElaText("N/A", leftTopArea); Y1_Info->setFont(valueFont);
    X1_Zero = new ElaText("N/A", leftTopArea); X1_Zero->setFont(valueFont);
    Y1_Zero = new ElaText("N/A", leftTopArea); Y1_Zero->setFont(valueFont);

    infoGrid->addWidget(Text1, 1, 0);
    infoGrid->addWidget(x1Label, 2, 0);
    infoGrid->addWidget(X1_Info, 2, 1);
    infoGrid->addWidget(y1Label, 2, 2);
    infoGrid->addWidget(Y1_Info, 2, 3);
    infoGrid->addWidget(Text2, 3, 0);
    infoGrid->addWidget(x1ZeroLabel, 4, 0);
    infoGrid->addWidget(X1_Zero, 4, 1);
    infoGrid->addWidget(y1ZeroLabel, 4, 2);
    infoGrid->addWidget(Y1_Zero, 4, 3);

    // ===== 中：模式选择区 =====
    QHBoxLayout* modeLayout = new QHBoxLayout(leftMidArea);
    modeLayout->setContentsMargins(8, 8, 8, 8);
    modeLayout->setSpacing(10);
    ElaText* modeLabel = new ElaText("模式选择:", leftMidArea); modeLabel->setFont(labelFont);
    Mode_Select = new ElaComboBox(leftMidArea); Mode_Select->setFont(valueFont);
    Mode_Select->addItems({"00位置模式", "01跟踪模式", "10零位设置(保留)", "12读取零位(保留)"});
    Mode_Select->setFixedWidth(180);
    modeLayout->addWidget(modeLabel);
    modeLayout->addWidget(Mode_Select);
    modeLayout->addStretch();
    connect(Mode_Select, QOverload<int>::of(&ElaComboBox::currentIndexChanged), this, &Prt_FSM7050_UART1::onModeChanged);

    // ===== 下：参数设置区 =====
    QGridLayout* paramGrid = new QGridLayout(leftBotArea);
    paramGrid->setContentsMargins(8, 8, 8, 8);
    paramGrid->setSpacing(6);
    ElaText* x1SetLabel = new ElaText("X1:", leftBotArea); x1SetLabel->setFont(labelFont);
    ElaText* y1SetLabel = new ElaText("Y1:", leftBotArea); y1SetLabel->setFont(labelFont);
    X1_Edit = new ElaLineEdit(leftBotArea); X1_Edit->setFont(valueFont); X1_Edit->setPlaceholderText("X1位置"); X1_Edit->setText("0");
    Y1_Edit = new ElaLineEdit(leftBotArea); Y1_Edit->setFont(valueFont); Y1_Edit->setPlaceholderText("Y1位置"); Y1_Edit->setText("0");
    SendParamBtn = new ElaPushButton("发送位置", leftBotArea);

    paramGrid->addWidget(x1SetLabel, 0, 0);
    paramGrid->addWidget(X1_Edit, 0, 1);
    paramGrid->addWidget(y1SetLabel, 0, 2);
    paramGrid->addWidget(Y1_Edit, 0, 3);
    paramGrid->addWidget(SendParamBtn, 1, 0, 1, 2);

    connect(SendParamBtn, &ElaPushButton::clicked, this, &Prt_FSM7050_UART1::onSendParamClicked);

    // ===== 右：操作历史 =====
    QVBoxLayout* rightLayout = new QVBoxLayout(rightArea);
    rightLayout->setContentsMargins(8, 8, 8, 8);
    rightLayout->setSpacing(6);
    logList = new ElaListView(rightArea);
    logModel = new T_LogModel(this);
    logModel->setDisplayMode(Default);
    logList->setModel(logModel);
    rightLayout->addWidget(logList);
    logModel->appendLogList(QString("日志记录已开启"));
}

Prt_FSM7050_UART1::~Prt_FSM7050_UART1()
{
}

void Prt_FSM7050_UART1::SerialConnected(bool connected)
{
    // 连接状态处理
    if (connected) {
        // 连接成功时的逻辑
    } else {
        // 断开连接时的逻辑
    }
}

void Prt_FSM7050_UART1::Creat_data(uint8_t *data , uint8_t mode, int16_t X1, int16_t Y1)
{
    data[0] = 0xA2; // 协议起始字节
    data[1] = 0x38; // 协议版本
    data[2] = 0x0C; // 协议类型
    data[3] = 0x89; // 保留字节
    data[4] = mode; // 模式选择
    data[5] = (X1 >> 8) & 0xFF; // X位置H
    data[6] = X1 & 0xFF; // X位置L
    data[7] = (Y1 >> 8) & 0xFF; // Y位置H
    data[8] = Y1 & 0xFF; // Y位置L
    data[9] = 0x00; // 备用
    uint8_t checksum = 0;
    for (int i = 2; i < 10; i++) {
        checksum += data[i]; // 计算校验和
    }
    data[10] = checksum; // 校验和
    data[11] = 0xED; // 结束字节
}

void Prt_FSM7050_UART1::Proctol_Analysis(const QByteArray &data)
{
    recvCount++;
    if (RecvCountLabel) RecvCountLabel->setText(QString("接收帧数: %1").arg(recvCount));
    if (data.size() < 12) {
        return;
    }
    const uint8_t *buf = reinterpret_cast<const uint8_t*>(data.constData());
    if (buf[0] != 0xA2 || buf[1] != 0x38 || buf[11] != 0xED) {
        checkErrCount++;
        if (CheckErrLabel) CheckErrLabel->setText(QString("校验错误帧: %1").arg(checkErrCount));
        return;
    }
    //检查校验和
    uint8_t checksum = 0;
    for (int i = 2; i < 10; ++i) {
        checksum += buf[i];
    }
    if (checksum != buf[10]) {
        // 校验和不匹配，忽略
        checkErrCount++;
        if (CheckErrLabel) CheckErrLabel->setText(QString("校验错误帧: %1").arg(checkErrCount));
        return;
    }   
    // 解析数据
    uint8_t mode = buf[4];
    int16_t X1 = (buf[5] << 8) | buf[6];
    int16_t Y1 = (buf[7] << 8) | buf[8];
    int16_t FSM_Status = buf[9]; // FSM自检状态
    //数据处理
    switch (mode){  // 根据模式选择处理逻辑
    case 0: // 00位置模式
        X1_Info->setText(QString::number(X1));
        Y1_Info->setText(QString::number(Y1));
        break;
    case 1: // 01跟踪模式
        X1_Info->setText(QString::number(X1));
        Y1_Info->setText(QString::number(Y1));
        break;
    case 2: // 10零位设置(保留)
        X1_Zero->setText(QString::number(X1));
        Y1_Zero->setText(QString::number(Y1));
        break;  
    case 3: // 12读取零位(保留)
        // 读取零位时，直接显示缓存的零点位置
        X1_Zero->setText(QString::number(Zero_cache[0])); // 更新X1零点显示
        Y1_Zero->setText(QString::number(Zero_cache[1])); // 更新Y1零点显示
        break;
    default:
        return; // 未知模式，忽略
    }
}

void Prt_FSM7050_UART1::SerialDataReceived(QByteArray data)
{
    Proctol_Analysis(data);
}

void Prt_FSM7050_UART1::COM_Send_data(uint8_t *data, uint8_t len)
{
    // 发送数据的逻辑
    QByteArray sendData(reinterpret_cast<const char*>(data), len);
    emit Proctol_out(sendData); // 发射信号
}

// 添加槽函数实现
void Prt_FSM7050_UART1::onModeChanged(int index)
{
     // 保存上一个模式的参数
    switch(currentMode) {
    case 0: // 位置模式
        break;
    case 1: // 零位设置
        break;
    default:
        break;
    }
    // 更新当前模式
    currentMode = index;
    // 恢复新模式的参数
    switch(index) {
    case 0:
        break;
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    default:
        break;
    }
}

void Prt_FSM7050_UART1::onSendParamClicked()
{
    uint8_t data[12] = {0}; // 协议数据缓冲区
    switch(currentMode) {
        case 0: // 位置模式
            Creat_data(data, 0x00, X1_Edit->text().toUInt(nullptr), Y1_Edit->text().toUInt(nullptr));
            break;
        case 1: // 跟踪模式
            Creat_data(data, 0x01, X1_Edit->text().toUInt(nullptr), Y1_Edit->text().toUInt(nullptr));
            break;
        case 2: // 零位设置
            Creat_data(data, 0x10, X1_Edit->text().toUInt(nullptr), Y1_Edit->text().toUInt(nullptr));
            break;
        case 3: // 读取零位
            Creat_data(data, 0x12, X1_Edit->text().toUInt(nullptr), Y1_Edit->text().toUInt(nullptr));
            break;
        default:
            return; // 未知模式，忽略
    }
    COM_Send_data(data, sizeof(data)); // 发送数据
    logModel->appendLogList(QString("发送数据: %1").arg(QByteArray(reinterpret_cast<const char*>(data), sizeof(data)).toHex().toUpper()));
}

