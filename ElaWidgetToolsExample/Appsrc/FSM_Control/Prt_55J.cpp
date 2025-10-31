/********************************************************************************
    * 文件名称 ：Prt_55J.cpp
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2025-06-06
    * 功     能：通信协议 界面
*********************************************************************************/
#include "Prt_55J.h"
#include <algorithm>
#include <limits>

Prt_55J::Prt_55J(QWidget *parent)
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

    ElaText* Text1 = new ElaText("X", leftTopArea);
    Text1->setFont(labelFont);
    ElaText* Text2 = new ElaText("Y", leftTopArea);
    Text2->setFont(labelFont);

    ElaText* x1fedb = new ElaText("反馈OPD:", leftTopArea);
    ElaText* x1TAG = new ElaText("期望:", leftTopArea);
    ElaText* x1ErrLabel = new ElaText("误差:", leftTopArea);
    ElaText* x1CtrlLabel = new ElaText("控制:", leftTopArea);
    ElaText* y1fedb = new ElaText("反馈:", leftTopArea);
    ElaText* y1TAG = new ElaText("期望:", leftTopArea);
    ElaText* y1ErrLabel = new ElaText("误差:", leftTopArea);
    ElaText* y1CtrlLabel = new ElaText("控制:", leftTopArea);

    // 设置标签字体
    x1fedb->setFont(labelFont);
    x1TAG->setFont(labelFont);
    x1ErrLabel->setFont(labelFont);
    x1CtrlLabel->setFont(labelFont);
    y1fedb->setFont(labelFont);
    y1TAG->setFont(labelFont);
    y1ErrLabel->setFont(labelFont);
    y1CtrlLabel->setFont(labelFont);

    X1_FB = new ElaText("N/A", leftTopArea);
    X1_TAG = new ElaText("N/A", leftTopArea);
    X1_ERR = new ElaText("N/A", leftTopArea);
    X1_CTL = new ElaText("N/A", leftTopArea);
    Y1_FB = new ElaText("N/A", leftTopArea);
    Y1_TAG = new ElaText("N/A", leftTopArea);
    Y1_ERR = new ElaText("N/A", leftTopArea);
    Y1_CTL = new ElaText("N/A", leftTopArea);

    // 设置数值字体
    X1_FB->setFont(valueFont);
    X1_TAG->setFont(valueFont);
    X1_ERR->setFont(valueFont);
    X1_CTL->setFont(valueFont);
    Y1_FB->setFont(valueFont);
    Y1_TAG->setFont(valueFont);
    Y1_ERR->setFont(valueFont);
    Y1_CTL->setFont(valueFont);

    infoGrid->addWidget(Text1, 0, 0);
    infoGrid->addWidget(x1fedb, 1, 0);
    infoGrid->addWidget(X1_FB, 1, 1);
    infoGrid->addWidget(x1TAG, 1, 2);
    infoGrid->addWidget(X1_TAG, 1, 3);
    infoGrid->addWidget(x1ErrLabel, 2, 0);
    infoGrid->addWidget(X1_ERR, 2, 1);
    infoGrid->addWidget(x1CtrlLabel, 2, 2);
    infoGrid->addWidget(X1_CTL, 2, 3);
    infoGrid->addWidget(Text2, 3, 0);
    infoGrid->addWidget(y1fedb, 4, 0);
    infoGrid->addWidget(Y1_FB, 4, 1);
    infoGrid->addWidget(y1TAG, 4, 2);
    infoGrid->addWidget(Y1_TAG, 4, 3);
    infoGrid->addWidget(y1ErrLabel, 5, 0);
    infoGrid->addWidget(Y1_ERR, 5, 1);
    infoGrid->addWidget(y1CtrlLabel, 5, 2);
    infoGrid->addWidget(Y1_CTL, 5, 3);


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
    Mode_Select->addItems({"位置设置"});
    Mode_Select->setFixedWidth(180);
    modeLayout->addWidget(modeLabel);
    modeLayout->addWidget(Mode_Select);
    modeLayout->addStretch();
    midLayout->addLayout(modeLayout);

    // 记录当前模式
    connect(Mode_Select, QOverload<int>::of(&ElaComboBox::currentIndexChanged), this, &Prt_55J::onModeChanged);

    // ===== 下：参数设置区 =====
    QGridLayout* paramGrid = new QGridLayout(leftBotArea);
    paramGrid->setContentsMargins(8, 8, 8, 8);
    paramGrid->setSpacing(6);
    ElaText* x1SetLabel = new ElaText("X1:", leftBotArea);
    ElaText* y1SetLabel = new ElaText("Y1:", leftBotArea);
    x1SetLabel->setFont(labelFont);
    y1SetLabel->setFont(labelFont);
    X1_Edit = new ElaLineEdit(leftBotArea);
    Y1_Edit = new ElaLineEdit(leftBotArea);
    X1_Edit->setFont(valueFont);
    Y1_Edit->setFont(valueFont);
    X1_Edit->setPlaceholderText("X位置");
    Y1_Edit->setPlaceholderText("Y位置");
    
    // 添加输入限幅 -32767 到 32767
    QIntValidator* validator = new QIntValidator(-32767, 32767, this);
    X1_Edit->setValidator(validator);
    Y1_Edit->setValidator(validator);
    
    //默认lineEdit参数
    X1_Edit->setText("0");
    Y1_Edit->setText("0");
    SendParamBtn = new ElaPushButton("发送位置", leftBotArea);
    //SendParamBtn->setFont(labelFont);
    paramGrid->addWidget(x1SetLabel, 0, 0);
    paramGrid->addWidget(X1_Edit, 0, 1);
    paramGrid->addWidget(y1SetLabel, 0, 2);
    paramGrid->addWidget(Y1_Edit, 0, 3);
    paramGrid->addWidget(SendParamBtn, 2, 0, 1, 4);

    connect(SendParamBtn, &ElaPushButton::clicked, this, &Prt_55J::onSendParamClicked);

    // 创建UI更新定时器 - 优化更新频率以提高性能和流畅度
    uiUpdateTimer = new QTimer(this);
    uiUpdateTimer->setInterval(50); // 50ms = 20Hz，平衡性能和响应性
    connect(uiUpdateTimer, &QTimer::timeout, this, &Prt_55J::updateUI);
    uiUpdateTimer->start();
    
    // 初始化数据结构
    latestValidData.hasNewData = false;
    latestValidData.X1_FB = 0;
    latestValidData.X1_TAG = 0;
    latestValidData.X1_ERR = 0;
    latestValidData.X1_CTL = 0;
    latestValidData.mode = 0;
    
    // 初始化图表数据
    timeCounter = 0.0;

    // ===== 右：数据曲线图表 =====
    QVBoxLayout* rightLayout = new QVBoxLayout(rightArea);
    rightLayout->setContentsMargins(8, 8, 8, 8);
    rightLayout->setSpacing(6);
    
    ElaText* chartTitle = new ElaText("数据曲线", rightArea);
    chartTitle->setFont(labelFont);
    
    // 创建图表的水平布局容器
    QHBoxLayout* chartsLayout = new QHBoxLayout();
    
    // 创建X轴图表
    X_customPlot = new QCustomPlot(rightArea);
    X_customPlot->setMinimumSize(200, 250);
    
    // 创建Y轴图表
    Y_customPlot = new QCustomPlot(rightArea);
    Y_customPlot->setMinimumSize(200, 250);
    
    // 配置X轴图表 - 添加四条曲线
    X_customPlot->addGraph(); // X反馈曲线
    X_customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255), 2));
    X_customPlot->graph(0)->setName("X反馈");
    
    X_customPlot->addGraph(); // X目标曲线
    X_customPlot->graph(1)->setPen(QPen(QColor(255, 110, 40), 2));
    X_customPlot->graph(1)->setName("X目标");
    
    X_customPlot->addGraph(); // X控制
    X_customPlot->graph(2)->setPen(QPen(QColor(0, 180, 0), 2));
    X_customPlot->graph(2)->setName("X控制");

    X_customPlot->addGraph(); // X误差
    X_customPlot->graph(3)->setPen(QPen(QColor(180, 0, 180), 2));
    X_customPlot->graph(3)->setName("X误差");
    
    // 配置Y轴图表 - 添加四条曲线
    Y_customPlot->addGraph(); // Y反馈曲线
    Y_customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255), 2));
    Y_customPlot->graph(0)->setName("Y反馈");
    
    Y_customPlot->addGraph(); // Y目标曲线
    Y_customPlot->graph(1)->setPen(QPen(QColor(255, 110, 40), 2));
    Y_customPlot->graph(1)->setName("Y目标");
    
    Y_customPlot->addGraph(); // Y控制
    Y_customPlot->graph(2)->setPen(QPen(QColor(0, 180, 0), 2));
    Y_customPlot->graph(2)->setName("Y控制");

    Y_customPlot->addGraph(); // Y误差
    Y_customPlot->graph(3)->setPen(QPen(QColor(180, 0, 180), 2));
    Y_customPlot->graph(3)->setName("Y误差");
    
    // 配置X轴图表属性
    X_customPlot->xAxis->setLabel("时间 (秒)");
    X_customPlot->yAxis->setLabel("X轴数值");
    X_customPlot->xAxis->setRange(0, 3); // 显示最近3秒数据
    X_customPlot->yAxis->setRange(-35000, 35000);
    X_customPlot->xAxis->grid()->setVisible(true);
    X_customPlot->yAxis->grid()->setVisible(true);
    X_customPlot->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
    X_customPlot->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
    X_customPlot->legend->setVisible(true);
    X_customPlot->legend->setFont(QFont("微软雅黑", 8));
    X_customPlot->legend->setRowSpacing(-3);
    X_customPlot->legend->setBrush(QBrush(QColor(255, 255, 255, 200)));
    X_customPlot->legend->setBorderPen(QPen(QColor(0, 0, 0, 0)));
    X_customPlot->setBackground(QBrush(QColor(245, 245, 245)));
    X_customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    
    // 配置Y轴图表属性
    Y_customPlot->xAxis->setLabel("时间 (秒)");
    Y_customPlot->yAxis->setLabel("Y轴数值");
    Y_customPlot->xAxis->setRange(0, 3); // 显示最近3秒数据
    Y_customPlot->yAxis->setRange(-35000, 35000);
    Y_customPlot->xAxis->grid()->setVisible(true);
    Y_customPlot->yAxis->grid()->setVisible(true);
    Y_customPlot->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
    Y_customPlot->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
    Y_customPlot->legend->setVisible(true);
    Y_customPlot->legend->setFont(QFont("微软雅黑", 8));
    Y_customPlot->legend->setRowSpacing(-3);
    Y_customPlot->legend->setBrush(QBrush(QColor(255, 255, 255, 200)));
    Y_customPlot->legend->setBorderPen(QPen(QColor(0, 0, 0, 0)));
    Y_customPlot->setBackground(QBrush(QColor(245, 245, 245)));
    Y_customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    
    // 添加初始数据点，确保图表能正常显示
    timeData.append(0);
    X_FB_L.append(0);
    X_TAG_L.append(0);
    Y_FB_L.append(0);
    Y_TAG_L.append(0);
    X_customPlot->graph(0)->setData(timeData, X_FB_L);
    X_customPlot->graph(1)->setData(timeData, X_TAG_L);
    Y_customPlot->graph(0)->setData(timeData, Y_FB_L);
    Y_customPlot->graph(1)->setData(timeData, Y_TAG_L);
    X_customPlot->replot();
    Y_customPlot->replot();
    
    // 将两个图表添加到水平布局
    chartsLayout->addWidget(X_customPlot);
    chartsLayout->addWidget(Y_customPlot);
    
    // 创建操作历史列表（保留在图表下方）
    ElaText* logTitle = new ElaText("操作历史", rightArea);
    logTitle->setFont(labelFont);
    logList = new ElaListView(rightArea);
    logModel = new T_LogModel(this);            // 日志模型
    logModel->setDisplayMode(Default);           // 可选：设置显示模式
    logList->setModel(logModel);                // 绑定模型
    logList->setMaximumHeight(150);             // 限制日志列表高度
    
    rightLayout->addWidget(chartTitle);
    rightLayout->addLayout(chartsLayout, 1);      // 图表布局占主要空间
    rightLayout->addWidget(logTitle);
    rightLayout->addWidget(logList);
    logModel->appendLogList(QString("日志记录已开启"));
    
    // 字体美化
    QFont infoFont; infoFont.setPointSize(13);
    X1_FB->setFont(infoFont); X1_TAG->setFont(infoFont);
    X1_ERR->setFont(infoFont); X1_CTL->setFont(infoFont);

    // 信号槽可后续补充
    // connect(SendParamBtn, &ElaPushButton::clicked, this, &Prt_55J::onSendParamClicked);
}

Prt_55J::~Prt_55J()
{
    // 停止定时器
    if (uiUpdateTimer) {
        uiUpdateTimer->stop();
    }
}

void Prt_55J::SerialConnected(bool connected)
{
    // 连接状态处理
    if (connected) {
        // 连接成功时的逻辑
    } else {
        // 断开连接时的逻辑
    }
}

void Prt_55J::Creat_data(uint8_t *data, uint8_t mode, int16_t X1, int16_t Y1 )
{
    data[0] = 0xAB; // 帧头
    data[1] = mode; // 模式
    data[2] = X1 >> 8;
    data[3] = X1 & 0xFF;
    data[4] = Y1 >> 8;
    data[5] = Y1 & 0xFF;
    // 校验
    uint8_t checksum = 0;
    for (int i = 0; i <= 5; ++i) {
        checksum += data[i];
    }
    data[6] = checksum;
    data[7] = 0xCC; // 帧尾
}

void Prt_55J::Proctol_Analysis(const QByteArray &data)
{
    recvCount++;
    if (data.size() < 20) return; // 修改为20字节

    const uint8_t *buf = reinterpret_cast<const uint8_t*>(data.constData());
    if (buf[0] != 0xAB || buf[19] != 0xCC) { // 修改帧尾位置为第19字节
        checkErrCount++;
        return;
    }
    // 校验 - 根据20字节协议
    uint8_t checksum = 0;
    for (int i = 0; i < 18; ++i) { // 校验第0-17字节
        checksum += buf[i];
    }
    if (static_cast<uint8_t>(checksum) != buf[18]) { // 校验位在第18字节
        checkErrCount++;
        return;
    }

    uint8_t mode = buf[1];
    int16_t X_FB = (buf[2] << 8) | buf[3];     //x轴反馈位置
    int16_t X_TAG = (buf[4] << 8) | buf[5];    //x轴目标位置
    int16_t X_DActrl  = (buf[6] << 8) | buf[7]; //x轴DA控制
    int16_t X_ERROR  = (buf[8] << 8) | buf[9]; //x轴误差
    int16_t Y_FB = (buf[10] << 8) | buf[11];    //y轴反馈位置
    int16_t Y_TAG = (buf[12] << 8) | buf[13];   //y轴目标位置
    int16_t Y_DActrl  = (buf[14] << 8) | buf[15]; //y轴DA控制
    int16_t Y_ERROR  = (buf[16] << 8) | buf[17]; //y轴误差

    // 更新数据
    {
        QMutexLocker locker(&dataMutex);
        latestValidData.X1_FB = X_FB;
        latestValidData.X1_TAG = X_TAG;
        latestValidData.X1_ERR = X_ERROR;  // X_ERROR填入"误差"显示
        latestValidData.X1_CTL = X_DActrl;  // X_DActrl填入"控制"显示
        latestValidData.Y1_FB = Y_FB;
        latestValidData.Y1_TAG = Y_TAG;
        latestValidData.Y1_ERR = Y_ERROR;  // Y_ERROR填入"误差"显示
        latestValidData.Y1_CTL = Y_DActrl;  // Y_DActrl
        latestValidData.mode = mode;
        latestValidData.hasNewData = true;

        timeCounter += 0.033;
        timeData.append(timeCounter);
        X_FB_L.append(X_FB);
        X_TAG_L.append(X_TAG);
        X_CTL_L.append(X_DActrl);  // X控制值
        X_ERR_L.append(X_ERROR);   // X误差值
        Y_FB_L.append(Y_FB);
        Y_TAG_L.append(Y_TAG);
        Y_CTL_L.append(Y_DActrl);  // Y控制值
        Y_ERR_L.append(Y_ERROR);   // Y误差值
        

        while (timeData.size() > maxDataPoints) {
            timeData.removeFirst();
            X_FB_L.removeFirst();
            X_TAG_L.removeFirst();
            X_CTL_L.removeFirst();
            X_ERR_L.removeFirst();
            Y_FB_L.removeFirst();
            Y_TAG_L.removeFirst();
            Y_CTL_L.removeFirst();
            Y_ERR_L.removeFirst();
        }
    }
}

void Prt_55J::SerialDataReceived(QByteArray data)
{
    // 高频数据优化：添加数据缓冲处理，避免粘包问题
    QMutexLocker locker(&dataMutex);
    dataBuffer.append(data);
    
    // 处理缓冲区中的完整数据包
    while (dataBuffer.size() >= 20) { // 修改为20字节
        // 查找帧头
        int frameStart = -1;
        for (int i = 0; i <= dataBuffer.size() - 3; ++i) {
            if (static_cast<uint8_t>(dataBuffer[i]) == 0xAB) {
                frameStart = i;
                break;
            }
        }
        
        if (frameStart == -1) {
            // 没有找到有效帧头，清空缓冲区
            dataBuffer.clear();
            break;
        }
        
        // 如果帧头不在开始位置，移除无效数据
        if (frameStart > 0) {
            dataBuffer.remove(0, frameStart);
        }
        
        // 检查是否有完整的数据包
        if (dataBuffer.size() >= 20) { // 修改为20字节
            QByteArray packet = dataBuffer.left(20); // 修改为20字节
            dataBuffer.remove(0, 20); // 修改为20字节
            
            // 解析这个数据包（不使用互斥锁，因为已经在锁定状态）
            locker.unlock();
            Proctol_Analysis(packet);
            locker.relock();
        } else {
            break;
        }
    }
}

void Prt_55J::COM_Send_data(uint8_t *data, uint8_t len)
{
    // 发送数据的逻辑
    QByteArray sendData(reinterpret_cast<const char*>(data), len);
    emit Proctol_out(sendData); // 发射信号
}

// 添加槽函数实现
void Prt_55J::onModeChanged(int index)
{
    // 保存上一个模式的参数
    switch(currentMode) {
    case 1: // 零位设置
        Zero_cache[0] = X1_Edit->text().toInt();
        Zero_cache[1] = Y1_Edit->text().toInt();
        break;
    default:
        break;
    }

    // 更新当前模式
    currentMode = index;

    // 恢复新模式的参数
    switch(index) {
    case 1:
        SendParamBtn->setText("移动");
        X1_Edit->setEnabled(true);
        Y1_Edit->setEnabled(true);
        X1_Edit->setPlaceholderText("X位置");
        Y1_Edit->setPlaceholderText("NA");
        // 恢复零位参数
        X1_Edit->setText(QString::number(Zero_cache[0]));
        Y1_Edit->setText(QString::number(Zero_cache[1]));
        break;
    default:
        break;
    }
}

void Prt_55J::onSendParamClicked()
{
    // 根据当前模式执行不同操作
    switch(currentMode) {
    case 0: // 10零位设置
        // 读取输入框参数并设置零位
        {
            int16_t X1_ERR = X1_Edit->text().toInt();
            int16_t X1_CTL = Y1_Edit->text().toInt();
            uint8_t data[12];
            Creat_data(data, 0x00, X1_ERR, X1_CTL);
            COM_Send_data(data, sizeof(data));
            //ElaMessageBar::information(ElaMessageBarType::TopRight, "零位设置", "已发送！", 1500);
            //添加操作记录
            if (logModel) {
                logModel->appendLogList(QString("设置位置: X=%1, Y=%2").arg(X1_ERR).arg(X1_CTL));
                // 不再清除图表数据，保持数据连续性
            }
        }
        break;
    default:
        break;
    }
}

// 添加UI更新函数 - 定期批量更新界面，提高高频数据处理性能
void Prt_55J::updateUI()
{
    // 更新接收统计信息
    if (RecvCountLabel) RecvCountLabel->setText(QString("接收帧数: %1").arg(recvCount));
    if (CheckErrLabel) CheckErrLabel->setText(QString("校验错误帧: %1").arg(checkErrCount));
    
    // 更新数据显示
    QMutexLocker locker(&dataMutex);
    if (latestValidData.hasNewData) {
        if (X1_FB) X1_FB->setText(QString::number(latestValidData.X1_FB));
        if (X1_TAG) X1_TAG->setText(QString::number(latestValidData.X1_TAG));
        if (X1_ERR) X1_ERR->setText(QString::number(latestValidData.X1_ERR)); // 显示X_ERROR(误差)
        if (X1_CTL) X1_CTL->setText(QString::number(latestValidData.X1_CTL)); // 显示X_DActrl(控制)
        if (Y1_FB) Y1_FB->setText(QString::number(latestValidData.Y1_FB));
        if (Y1_TAG) Y1_TAG->setText(QString::number(latestValidData.Y1_TAG));
        if (Y1_ERR) Y1_ERR->setText(QString::number(latestValidData.Y1_ERR)); // 显示Y_ERROR(误差)
        if (Y1_CTL) Y1_CTL->setText(QString::number(latestValidData.Y1_CTL)); // 显示Y_DActrl(控制)
        
        // 更新图表显示 - 优化渲染性能
        if (X_customPlot && Y_customPlot && !timeData.isEmpty()) {
            // 使用快速更新方式，减少重绘开销
            X_customPlot->graph(0)->setData(timeData, X_FB_L);
            X_customPlot->graph(1)->setData(timeData, X_TAG_L);
            X_customPlot->graph(2)->setData(timeData, X_CTL_L);
            X_customPlot->graph(3)->setData(timeData, X_ERR_L);
            
            Y_customPlot->graph(0)->setData(timeData, Y_FB_L);
            Y_customPlot->graph(1)->setData(timeData, Y_TAG_L);
            Y_customPlot->graph(2)->setData(timeData, Y_CTL_L);
            Y_customPlot->graph(3)->setData(timeData, Y_ERR_L);
            
            // 自动调整X轴范围，显示最近的数据
            if (timeData.size() > 1) {
                double maxTime = timeData.last();
                double minTime = timeData.first();
                double timeSpan = maxTime - minTime;
                if (timeSpan > 0) {
                    // 显示固定3秒时间窗口，保持数据流畅滚动
                    double displaySpan = qMin(timeSpan, 3.0);
                    X_customPlot->xAxis->setRange(maxTime - displaySpan, maxTime);
                    Y_customPlot->xAxis->setRange(maxTime - displaySpan, maxTime);
                }
            }
            
            // 动态调整Y轴范围，但降低更新频率避免抖动
            static int yRangeUpdateCounter = 0;
            if (++yRangeUpdateCounter % 10 == 0) { // 每10次更新一次Y轴范围
                // 更新X轴图表的Y轴范围
                if (X_FB_L.size() > 0 && X_TAG_L.size() > 0) {
                    int recentDataSize = qMin(30, timeData.size()); // 最近30个点（约1秒数据）
                    int startIdx = qMax(0, timeData.size() - recentDataSize);
                    
                    double minVal = std::numeric_limits<double>::max();
                    double maxVal = std::numeric_limits<double>::lowest();
                    
                    for (int i = startIdx; i < X_FB_L.size(); ++i) {
                        minVal = qMin(minVal, qMin(X_FB_L[i], X_TAG_L[i]));
                        maxVal = qMax(maxVal, qMax(X_FB_L[i], X_TAG_L[i]));
                    }
                    
                    if (minVal != std::numeric_limits<double>::max()) {
                        double range = maxVal - minVal;
                        double margin = qMax(1000.0, range * 0.1);
                        X_customPlot->yAxis->setRange(minVal - margin, maxVal + margin);
                    }
                }
                
                // 更新Y轴图表的Y轴范围
                if (Y_FB_L.size() > 0 && Y_TAG_L.size() > 0) {
                    int recentDataSize = qMin(30, timeData.size()); // 最近30个点（约1秒数据）
                    int startIdx = qMax(0, timeData.size() - recentDataSize);
                    
                    double minVal = std::numeric_limits<double>::max();
                    double maxVal = std::numeric_limits<double>::lowest();
                    
                    for (int i = startIdx; i < Y_FB_L.size(); ++i) {
                        minVal = qMin(minVal, qMin(Y_FB_L[i], Y_TAG_L[i]));
                        maxVal = qMax(maxVal, qMax(Y_FB_L[i], Y_TAG_L[i]));
                    }
                    
                    if (minVal != std::numeric_limits<double>::max()) {
                        double range = maxVal - minVal;
                        double margin = qMax(1000.0, range * 0.1);
                        Y_customPlot->yAxis->setRange(minVal - margin, maxVal + margin);
                    }
                }
            }
            
            // 使用快速重绘模式
            X_customPlot->replot(QCustomPlot::rpQueuedReplot);
            Y_customPlot->replot(QCustomPlot::rpQueuedReplot);
        }
        
        latestValidData.hasNewData = false;
    }
}

void Prt_55J::processBufferedData()
{
    // 预留函数，用于处理更复杂的数据缓冲逻辑
    // 如果需要处理粘包或分包情况，可以在这里实现
}
