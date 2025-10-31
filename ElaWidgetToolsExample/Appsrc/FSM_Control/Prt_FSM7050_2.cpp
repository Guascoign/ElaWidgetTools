/********************************************************************************
    * 文件名称 ：Prt_FSM7050_2.cpp
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2025-06-06
    * 功     能：通信协议 界面
*********************************************************************************/
#include "Prt_FSM7050_2.h"

Prt_FSM7050_2::Prt_FSM7050_2(QWidget *parent)
    : ProtocolBaseWidget(parent)
{
    // 左侧上中下区域
    ElaScrollPageArea* leftTopArea = new ElaScrollPageArea(this);    // 上：信息显示
    ElaScrollPageArea* leftMidArea = new ElaScrollPageArea(this);    // 中：模式选择
    ElaScrollPageArea* leftBotArea = new ElaScrollPageArea(this);    // 下：参数设置
    ElaScrollPageArea* rightArea = new ElaScrollPageArea(this);      // 右：操作历史

    // 调整区域尺寸，与Prt_TEST一致
    leftTopArea->setFixedHeight(300);      // 上
    leftTopArea->setFixedWidth(300);
    leftMidArea->setFixedHeight(110);      // 中
    leftMidArea->setFixedWidth(300);
    leftBotArea->setFixedHeight(125);      // 下
    leftBotArea->setFixedWidth(300);
    rightArea->setFixedHeight(550);        // 右
    // rightArea->setFixedWidth(300);      // 可选，若需固定宽度

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
    mainLayout->addLayout(leftLayout, 3);      // 左右比例3:3，与Prt_TEST一致
    mainLayout->addWidget(rightArea, 3);
    setLayout(mainLayout);

    // ===== 上：信息显示区 =====
    QGridLayout* infoGrid = new QGridLayout(leftTopArea);
    infoGrid->setContentsMargins(4, 4, 4, 4);
    infoGrid->setSpacing(2);

    // 统一字体设置
    QFont labelFont;
    labelFont.setPointSize(15); // 统一字号，可根据实际界面调整
    labelFont.setBold(true);

    QFont valueFont;
    valueFont.setPointSize(15);

    ElaText* Text1 = new ElaText("相对位置", leftTopArea);
    Text1->setFont(labelFont);
    ElaText* Text2 = new ElaText("零点位置", leftTopArea);
    Text2->setFont(labelFont);

    ElaText* x1Label = new ElaText("X1:", leftTopArea);
    ElaText* y1Label = new ElaText("Y1:", leftTopArea);
    ElaText* x2Label = new ElaText("X2:", leftTopArea);
    ElaText* y2Label = new ElaText("Y2:", leftTopArea);
    ElaText* x1ZeroLabel = new ElaText("X1:", leftTopArea);
    ElaText* y1ZeroLabel = new ElaText("Y1:", leftTopArea);
    ElaText* x2ZeroLabel = new ElaText("X2:", leftTopArea);
    ElaText* y2ZeroLabel = new ElaText("Y2:", leftTopArea);

    // 设置标签字体
    x1Label->setFont(labelFont);
    y1Label->setFont(labelFont);
    x2Label->setFont(labelFont);
    y2Label->setFont(labelFont);
    x1ZeroLabel->setFont(labelFont);
    y1ZeroLabel->setFont(labelFont);
    x2ZeroLabel->setFont(labelFont);
    y2ZeroLabel->setFont(labelFont);

    X1_Info = new ElaText("N/A", leftTopArea);
    Y1_Info = new ElaText("N/A", leftTopArea);
    X2_Info = new ElaText("N/A", leftTopArea);
    Y2_Info = new ElaText("N/A", leftTopArea);    
    X1_Zero = new ElaText("N/A", leftTopArea);
    Y1_Zero = new ElaText("N/A", leftTopArea);
    X2_Zero = new ElaText("N/A", leftTopArea);
    Y2_Zero = new ElaText("N/A", leftTopArea);

    // 设置数值字体
    X1_Info->setFont(valueFont);
    Y1_Info->setFont(valueFont);
    X2_Info->setFont(valueFont);
    Y2_Info->setFont(valueFont);
    X1_Zero->setFont(valueFont);
    Y1_Zero->setFont(valueFont);
    X2_Zero->setFont(valueFont);
    Y2_Zero->setFont(valueFont);

    infoGrid->addWidget(Text1, 0, 0);
    infoGrid->addWidget(x1Label, 1, 0);
    infoGrid->addWidget(X1_Info, 1, 1);
    infoGrid->addWidget(y1Label, 1, 2);
    infoGrid->addWidget(Y1_Info, 1, 3);
    infoGrid->addWidget(x2Label, 2, 0);
    infoGrid->addWidget(X2_Info, 2, 1);
    infoGrid->addWidget(y2Label, 2, 2);
    infoGrid->addWidget(Y2_Info, 2, 3);
    infoGrid->addWidget(Text2, 3, 0);
    infoGrid->addWidget(x1ZeroLabel, 4, 0);
    infoGrid->addWidget(X1_Zero, 4, 1);
    infoGrid->addWidget(y1ZeroLabel, 4, 2);
    infoGrid->addWidget(Y1_Zero, 4, 3);
    infoGrid->addWidget(x2ZeroLabel, 5, 0);
    infoGrid->addWidget(X2_Zero, 5, 1);
    infoGrid->addWidget(y2ZeroLabel, 5, 2);
    infoGrid->addWidget(Y2_Zero, 5, 3);


    // ===== 中：模式选择区 =====
    // 在模式选择区上方添加接收帧数和校验错误帧信息
    QVBoxLayout* midLayout = new QVBoxLayout(leftMidArea);
    midLayout->setContentsMargins(4, 4, 4, 4);
    midLayout->setSpacing(2);

    // 信息行
    QHBoxLayout* infoRow = new QHBoxLayout();
    RecvCountLabel = new ElaText("接收帧数: 0", leftMidArea);
    RecvCountLabel->setFont(labelFont);
    CheckErrLabel = new ElaText("校验错误帧: 0", leftMidArea);
    CheckErrLabel->setFont(labelFont);
    infoRow->addSpacing(20);
    infoRow->addWidget(RecvCountLabel);
    infoRow->addSpacing(20);
    infoRow->addWidget(CheckErrLabel);
    infoRow->addStretch();
    midLayout->addLayout(infoRow);

    // 原有模式选择行
    QHBoxLayout* modeLayout = new QHBoxLayout();
    modeLayout->setContentsMargins(8, 8, 8, 8);
    modeLayout->setSpacing(10);
    ElaText* modeLabel = new ElaText("模式选择:", leftMidArea);
    modeLabel->setFont(labelFont);
    Mode_Select = new ElaComboBox(leftMidArea);
    Mode_Select->setFont(valueFont);
    Mode_Select->addItems({"00位置模式", "10零位设置", "11零位恢复", "12读取零位"});
    Mode_Select->setFixedWidth(180);
    modeLayout->addWidget(modeLabel);
    modeLayout->addWidget(Mode_Select);
    modeLayout->addStretch();
    midLayout->addLayout(modeLayout);

    // 记录当前模式
    connect(Mode_Select, QOverload<int>::of(&ElaComboBox::currentIndexChanged), this, &Prt_FSM7050_2::onModeChanged);

    // ===== 下：参数设置区 =====
    QGridLayout* paramGrid = new QGridLayout(leftBotArea);
    paramGrid->setContentsMargins(8, 8, 8, 8);
    paramGrid->setSpacing(6);
    ElaText* x1SetLabel = new ElaText("X1:", leftBotArea);
    ElaText* y1SetLabel = new ElaText("Y1:", leftBotArea);
    ElaText* x2SetLabel = new ElaText("X2:", leftBotArea);
    ElaText* y2SetLabel = new ElaText("Y2:", leftBotArea);
    x1SetLabel->setFont(labelFont);
    y1SetLabel->setFont(labelFont);
    x2SetLabel->setFont(labelFont);
    y2SetLabel->setFont(labelFont);
    X1_Edit = new ElaLineEdit(leftBotArea);
    Y1_Edit = new ElaLineEdit(leftBotArea);
    X2_Edit = new ElaLineEdit(leftBotArea);
    Y2_Edit = new ElaLineEdit(leftBotArea);
    X1_Edit->setFont(valueFont);
    Y1_Edit->setFont(valueFont);
    X2_Edit->setFont(valueFont);
    Y2_Edit->setFont(valueFont);
    X1_Edit->setPlaceholderText("X1位置");
    Y1_Edit->setPlaceholderText("Y1位置");
    X2_Edit->setPlaceholderText("X2位置");
    Y2_Edit->setPlaceholderText("Y2位置");
    //默认lineEdit参数
    X1_Edit->setText("0");
    Y1_Edit->setText("0");
    X2_Edit->setText("0");
    Y2_Edit->setText("0");
    SendParamBtn = new ElaPushButton("发送位置", leftBotArea);
    //SendParamBtn->setFont(labelFont);
    paramGrid->addWidget(x1SetLabel, 0, 0);
    paramGrid->addWidget(X1_Edit, 0, 1);
    paramGrid->addWidget(y1SetLabel, 0, 2);
    paramGrid->addWidget(Y1_Edit, 0, 3);
    paramGrid->addWidget(x2SetLabel, 1, 0);
    paramGrid->addWidget(X2_Edit, 1, 1);
    paramGrid->addWidget(y2SetLabel, 1, 2);
    paramGrid->addWidget(Y2_Edit, 1, 3);
    paramGrid->addWidget(SendParamBtn, 2, 0, 1, 4);

    connect(SendParamBtn, &ElaPushButton::clicked, this, &Prt_FSM7050_2::onSendParamClicked);

    // ===== 右：操作历史 =====
    QVBoxLayout* rightLayout = new QVBoxLayout(rightArea);
    rightLayout->setContentsMargins(8, 8, 8, 8);
    rightLayout->setSpacing(6);
    ElaText* logTitle = new ElaText("操作历史", rightArea);
    logTitle->setFont(labelFont);
    logList = new ElaListView(rightArea);
    logModel = new T_LogModel(this);            // 日志模型
    logModel->setDisplayMode(Default);           // 可选：设置显示模式
    logList->setModel(logModel);                // 绑定模型
    rightLayout->addWidget(logTitle);
    rightLayout->addWidget(logList);
    logModel->appendLogList(QString("日志记录已开启"));
    // 字体美化
    QFont infoFont; infoFont.setPointSize(13);
    X1_Info->setFont(infoFont); Y1_Info->setFont(infoFont);
    X2_Info->setFont(infoFont); Y2_Info->setFont(infoFont);

    // 信号槽可后续补充
    // connect(SendParamBtn, &ElaPushButton::clicked, this, &Prt_FSM7050_2::onSendParamClicked);
}

Prt_FSM7050_2::~Prt_FSM7050_2()
{
}

void Prt_FSM7050_2::SerialConnected(bool connected)
{
    // 连接状态处理
    if (connected) {
        // 连接成功时的逻辑
    } else {
        // 断开连接时的逻辑
    }
}

void Prt_FSM7050_2::Creat_data(uint8_t *data, uint8_t mode, int16_t X1, int16_t Y1 , int16_t X2, int16_t Y2)
{
    // 生成协议数据的逻辑
    data[0]  = 0xA2;                // 帧头
    data[1]  = 0x38;                // 帧头
    data[2]  = 0x10;                // 帧长度
    data[3]  = 0x8A;                // 指令类型
    data[4]  = mode;                // 模式
    data[5]  = (X1 >> 8) & 0xFF;    // X1高字节
    data[6]  = X1 & 0xFF;           // X1低字节
    data[7]  = (Y1 >> 8) & 0xFF;    // Y1高字节
    data[8]  = Y1 & 0xFF;           // Y1低字节
    data[9]  = (X2 >> 8) & 0xFF;    // X2高字节
    data[10] = X2 & 0xFF;           // X2低字节
    data[11] = (Y2 >> 8) & 0xFF;    // Y2高字节
    data[12] = Y2 & 0xFF;           // Y2低字节
    data[13] = 0x00;                //备用
    // 校验和 data[2] ~ data[13] 相加低8位
    uint8_t checksum = 0;
    for (int i = 2; i < 14; ++i) {
        checksum += data[i];
    }
    data[14] = checksum;            // 校验和
    data[15] = 0xED;                // 帧尾
}

void Prt_FSM7050_2::Proctol_Analysis(const QByteArray &data)
{
    recvCount++;
    if (RecvCountLabel) RecvCountLabel->setText(QString("接收帧数: %1").arg(recvCount));
    // 协议解析逻辑
    if (data.size() < 16) {
        return;
    }
    const uint8_t *buf = reinterpret_cast<const uint8_t*>(data.constData());
    if (buf[0] != 0xA2 || buf[1] != 0x38 || buf[15] != 0xED) {
        checkErrCount++;
        if (CheckErrLabel) CheckErrLabel->setText(QString("校验错误帧: %1").arg(checkErrCount));
        return;
    }
    // 检查校验和
    uint8_t checksum = 0;
    for (int i = 2; i < 14; ++i) {
        checksum += buf[i];
    }
    if (checksum != buf[14]) {
        // 校验和不匹配，忽略
        checkErrCount++;
        if (CheckErrLabel) CheckErrLabel->setText(QString("校验错误帧: %1").arg(checkErrCount));
        return;
    }
    // 解析数据
    uint8_t mode = buf[4];
    int16_t X1 = (buf[5] << 8) | buf[6];
    int16_t Y1 = (buf[7] << 8) | buf[8];
    int16_t X2 = (buf[9] << 8) | buf[10];
    int16_t Y2 = (buf[11] << 8) | buf[12];
    switch (mode) {
    case 0x00: // 位置模式
        X1_Info->setText(QString::number(X1));
        Y1_Info->setText(QString::number(Y1));
        X2_Info->setText(QString::number(X2));
        Y2_Info->setText(QString::number(Y2));
        break;
    case 0x10: // 零位设置
        // X1_Zero->setText(QString::number(X1 / 100.0, 'f', 2));
        // Y1_Zero->setText(QString::number(Y1 / 100.0, 'f', 2));
        // X2_Zero->setText(QString::number(X2 / 100.0, 'f', 2));
        // Y2_Zero->setText(QString::number(Y2 / 100.0, 'f', 2));
        break;
    case 0x11: // 零位恢复
        // 这里可以添加零位恢复的逻辑
        // X1_Zero->setText(QString::number(X1 / 100.0, 'f', 2));
        // Y1_Zero->setText(QString::number(Y1 / 100.0, 'f', 2));
        // X2_Zero->setText(QString::number(X2 / 100.0, 'f', 2));
        // Y2_Zero->setText(QString::number(Y2 / 100.0, 'f', 2));
        break;
    case 0x12: // 读取零位
        // 这里可以添加读取零位的逻辑
        X1_Zero->setText(QString::number(X1));
        Y1_Zero->setText(QString::number(Y1));
        X2_Zero->setText(QString::number(X2));
        Y2_Zero->setText(QString::number(Y2));
        break;
    default:
        // 未知模式，忽略
        return;
    }

}

void Prt_FSM7050_2::SerialDataReceived(QByteArray data)
{
    Proctol_Analysis(data);
}

void Prt_FSM7050_2::COM_Send_data(uint8_t *data, uint8_t len)
{
    // 发送数据的逻辑
    QByteArray sendData(reinterpret_cast<const char*>(data), len);
    emit Proctol_out(sendData); // 发射信号
}

// 添加槽函数实现
void Prt_FSM7050_2::onModeChanged(int index)
{
    // 保存上一个模式的参数
    switch(currentMode) {
    case 0: // 位置模式
        pos_cache[0] = X1_Edit->text().toInt();
        pos_cache[1] = Y1_Edit->text().toInt();
        pos_cache[2] = X2_Edit->text().toInt();
        pos_cache[3] = Y2_Edit->text().toInt();
        break;
    case 1: // 零位设置
        Zero_cache[0] = X1_Edit->text().toInt();
        Zero_cache[1] = Y1_Edit->text().toInt();
        Zero_cache[2] = X2_Edit->text().toInt();
        Zero_cache[3] = Y2_Edit->text().toInt();
        break;
    default:
        break;
    }

    // 更新当前模式
    currentMode = index;

    // 恢复新模式的参数
    switch(index) {
    case 0:
        SendParamBtn->setText("发送位置");
        X1_Edit->setEnabled(true);
        Y1_Edit->setEnabled(true);
        X2_Edit->setEnabled(true);
        Y2_Edit->setEnabled(true);
        X1_Edit->setPlaceholderText("X1位置");
        Y1_Edit->setPlaceholderText("Y1位置");
        X2_Edit->setPlaceholderText("X2位置");
        Y2_Edit->setPlaceholderText("Y2位置");
        // 恢复位置参数
        X1_Edit->setText(QString::number(pos_cache[0]));
        Y1_Edit->setText(QString::number(pos_cache[1]));
        X2_Edit->setText(QString::number(pos_cache[2]));
        Y2_Edit->setText(QString::number(pos_cache[3]));
        break;
    case 1:
        SendParamBtn->setText("设置零位");
        X1_Edit->setEnabled(true);
        Y1_Edit->setEnabled(true);
        X2_Edit->setEnabled(true);
        Y2_Edit->setEnabled(true);
        X1_Edit->setPlaceholderText("X1零位");
        Y1_Edit->setPlaceholderText("Y1零位");
        X2_Edit->setPlaceholderText("X2零位");
        Y2_Edit->setPlaceholderText("Y2零位");
        // 恢复零位参数
        X1_Edit->setText(QString::number(Zero_cache[0]));
        Y1_Edit->setText(QString::number(Zero_cache[1]));
        X2_Edit->setText(QString::number(Zero_cache[2]));
        Y2_Edit->setText(QString::number(Zero_cache[3]));
        break;
    case 2:
        SendParamBtn->setText("重置零位");
        X1_Edit->setEnabled(false);
        Y1_Edit->setEnabled(false);
        X2_Edit->setEnabled(false);
        Y2_Edit->setEnabled(false);
        break;
    case 3:
        SendParamBtn->setText("读取零位");
        X1_Edit->setEnabled(false);
        Y1_Edit->setEnabled(false);
        X2_Edit->setEnabled(false);
        Y2_Edit->setEnabled(false);
        break;
    default:
        break;
    }
}

void Prt_FSM7050_2::onSendParamClicked()
{
    // 根据当前模式执行不同操作
    switch(currentMode) {
    case 0: // 00位置模式
        // 读取输入框参数并发送位置
        {
            int16_t X1 = X1_Edit->text().toInt();
            int16_t Y1 = Y1_Edit->text().toInt();
            int16_t X2 = X2_Edit->text().toInt();
            int16_t Y2 = Y2_Edit->text().toInt();
            uint8_t data[16];
            Creat_data(data, 0x00, X1, Y1, X2, Y2);
            COM_Send_data(data, sizeof(data));
            //添加操作记录
            if (logList) logModel->appendLogList(QString("发送位置: X1=%1, Y1=%2, X2=%3, Y2=%4").arg(X1).arg(Y1).arg(X2).arg(Y2));
        }
        break;
    case 1: // 10零位设置
        // 读取输入框参数并设置零位
        {
            int16_t X1_Zero = X1_Edit->text().toInt();
            int16_t Y1_Zero = Y1_Edit->text().toInt();
            int16_t X2_Zero = X2_Edit->text().toInt();
            int16_t Y2_Zero = Y2_Edit->text().toInt();
            uint8_t data[16];
            Creat_data(data, 0x10, X1_Zero, Y1_Zero, X2_Zero, Y2_Zero);
            COM_Send_data(data, sizeof(data));
            ElaMessageBar::information(ElaMessageBarType::TopRight, "零位设置", "已发送！", 1500);
            //添加操作记录
            if (logList) logModel->appendLogList(QString("设置零位: X1=%1, Y1=%2, X2=%3, Y2=%4").arg(X1_Zero).arg(Y1_Zero).arg(X2_Zero).arg(Y2_Zero));
        }
        break;
    case 2: // 11零位恢复
        // 发送零位恢复命令
        {
            uint8_t data[16];
            Creat_data(data, 0x11, 0, 0, 0, 0); // 零位恢复不需要具体位置参数
            COM_Send_data(data, sizeof(data));
            ElaMessageBar::information(ElaMessageBarType::TopRight, "零位恢复", "已发送！", 1500);
            //添加操作记录
            if (logList) logModel->appendLogList("零位恢复");
        }
        break;
    case 3: // 12读取零位
        // 发送读取零位命令
        {
            uint8_t data[16];
            Creat_data(data, 0x12, 0, 0, 0, 0); // 读取零位不需要具体位置参数
            COM_Send_data(data, sizeof(data));
            //添加操作记录
            if (logList) logModel->appendLogList("读取零位");
        }
        break;
    default:
        break;
    }
}

