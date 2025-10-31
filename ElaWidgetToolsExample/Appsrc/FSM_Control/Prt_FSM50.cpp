/********************************************************************************
    * 文件名称 ：Prt_FSM50.cpp
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2025-06-06
    * 功     能：通信协议 界面
*********************************************************************************/
#include "Prt_FSM50.h"
#include <algorithm>
#include <limits>

Prt_FSM50::Prt_FSM50(QWidget *parent)
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
    ElaText* x1ZeroLabel = new ElaText("X1:", leftTopArea);
    ElaText* y1ZeroLabel = new ElaText("Y1:", leftTopArea);

    // 设置标签字体
    x1Label->setFont(labelFont);
    y1Label->setFont(labelFont);
    x1ZeroLabel->setFont(labelFont);
    y1ZeroLabel->setFont(labelFont);

    X1_Info = new ElaText("N/A", leftTopArea);
    Y1_Info = new ElaText("N/A", leftTopArea);
    X1_Zero = new ElaText("N/A", leftTopArea);
    Y1_Zero = new ElaText("N/A", leftTopArea);

    // 设置数值字体
    X1_Info->setFont(valueFont);
    Y1_Info->setFont(valueFont);
    X1_Zero->setFont(valueFont);
    Y1_Zero->setFont(valueFont);

    infoGrid->addWidget(Text1, 0, 0);
    infoGrid->addWidget(x1Label, 1, 0);
    infoGrid->addWidget(X1_Info, 1, 1);
    infoGrid->addWidget(y1Label, 1, 2);
    infoGrid->addWidget(Y1_Info, 1, 3);
    infoGrid->addWidget(Text2, 3, 0);
    infoGrid->addWidget(x1ZeroLabel, 4, 0);
    infoGrid->addWidget(X1_Zero, 4, 1);
    infoGrid->addWidget(y1ZeroLabel, 4, 2);
    infoGrid->addWidget(Y1_Zero, 4, 3);


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
    Mode_Select->addItems({"10相对位置设置"});
    Mode_Select->setFixedWidth(180);
    modeLayout->addWidget(modeLabel);
    modeLayout->addWidget(Mode_Select);
    modeLayout->addStretch();
    midLayout->addLayout(modeLayout);

    // 记录当前模式
    connect(Mode_Select, QOverload<int>::of(&ElaComboBox::currentIndexChanged), this, &Prt_FSM50::onModeChanged);

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
    X1_Edit->setPlaceholderText("X1位置");
    Y1_Edit->setPlaceholderText("Y1位置");
    
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

    connect(SendParamBtn, &ElaPushButton::clicked, this, &Prt_FSM50::onSendParamClicked);

    // 创建UI更新定时器 - 优化更新频率以提高性能和流畅度
    uiUpdateTimer = new QTimer(this);
    uiUpdateTimer->setInterval(50); // 50ms = 20Hz，平衡性能和响应性
    connect(uiUpdateTimer, &QTimer::timeout, this, &Prt_FSM50::updateUI);
    uiUpdateTimer->start();
    
    // 初始化数据结构
    latestValidData.hasNewData = false;
    latestValidData.X1 = 0;
    latestValidData.Y1 = 0;
    latestValidData.X1_Zero = 0;
    latestValidData.Y1_Zero = 0;
    latestValidData.mode = 0;
    
    // 初始化图表数据
    timeCounter = 0.0;

    // ===== 右：数据曲线图表 =====
    QVBoxLayout* rightLayout = new QVBoxLayout(rightArea);
    rightLayout->setContentsMargins(8, 8, 8, 8);
    rightLayout->setSpacing(6);
    
    ElaText* chartTitle = new ElaText("数据曲线", rightArea);
    chartTitle->setFont(labelFont);
    
    // 创建QCustomPlot图表
    customPlot = new QCustomPlot(rightArea);
    customPlot->setMinimumSize(400, 300);
    
    // 配置图表 - 添加两条曲线
    customPlot->addGraph(); // X1曲线
    customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255), 2));
    customPlot->graph(0)->setName("X1位置");
    
    customPlot->addGraph(); // Y1曲线
    customPlot->graph(1)->setPen(QPen(QColor(255, 110, 40), 2));
    customPlot->graph(1)->setName("Y1位置");
    
    // 设置坐标轴标签
    customPlot->xAxis->setLabel("时间 (秒)");
    customPlot->yAxis->setLabel("位置值");
    
    // 设置坐标轴范围
    customPlot->xAxis->setRange(0, 10); // 显示最近10秒数据
    customPlot->yAxis->setRange(-35000, 35000); // 根据数据范围调整
    
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
    
    // 添加初始数据点，确保图表能正常显示
    timeData.append(0);
    x1Data.append(0);
    y1Data.append(0);
    customPlot->graph(0)->setData(timeData, x1Data);
    customPlot->graph(1)->setData(timeData, y1Data);
    customPlot->replot();
    
    // 创建操作历史列表（保留在图表下方）
    ElaText* logTitle = new ElaText("操作历史", rightArea);
    logTitle->setFont(labelFont);
    logList = new ElaListView(rightArea);
    logModel = new T_LogModel(this);            // 日志模型
    logModel->setDisplayMode(Default);           // 可选：设置显示模式
    logList->setModel(logModel);                // 绑定模型
    logList->setMaximumHeight(150);             // 限制日志列表高度
    
    rightLayout->addWidget(chartTitle);
    rightLayout->addWidget(customPlot, 1);      // 图表占主要空间
    rightLayout->addWidget(logTitle);
    rightLayout->addWidget(logList);
    logModel->appendLogList(QString("日志记录已开启"));
    
    // 字体美化
    QFont infoFont; infoFont.setPointSize(13);
    X1_Info->setFont(infoFont); Y1_Info->setFont(infoFont);
    X1_Zero->setFont(infoFont); Y1_Zero->setFont(infoFont);

    // 信号槽可后续补充
    // connect(SendParamBtn, &ElaPushButton::clicked, this, &Prt_FSM50::onSendParamClicked);
}

Prt_FSM50::~Prt_FSM50()
{
    // 停止定时器
    if (uiUpdateTimer) {
        uiUpdateTimer->stop();
    }
}

void Prt_FSM50::SerialConnected(bool connected)
{
    // 连接状态处理
    if (connected) {
        // 连接成功时的逻辑
    } else {
        // 断开连接时的逻辑
    }
}

void Prt_FSM50::Creat_data(uint8_t *data, uint8_t mode, int16_t X1, int16_t Y1 )
{
    // 生成协议数据的逻辑
    data[0] = 0x55; // 帧头
    data[1] = 0xAA; // 帧类型
    data[2] = mode; // 模式指令
    data[3] = X1 >> 8; // X1高字节
    data[4] = X1 & 0xFF; // X1低字节
    data[5] = Y1 >> 8; // Y1高字节
    data[6] = Y1 & 0xFF; // Y1低字节
    //不包括信息头和字尾的其它字节异后再与与0xFF异或
    uint8_t checksum = 0;
    for (int i = 2; i < 7; ++i) {
        checksum ^= data[i];
    }
    data[7] = ~checksum; // 校验和
    data[8] = 0xCC; // 帧尾

}

void Prt_FSM50::Proctol_Analysis(const QByteArray &data)
{
    // 使用原子操作更新计数器，避免频繁的UI更新
    recvCount++;
    
    // 协议解析逻辑 - 修改为9字节协议
    if (data.size() < 9) {
        return;
    }
    
    const uint8_t *buf = reinterpret_cast<const uint8_t*>(data.constData());
    if (buf[0] != 0x55 || buf[1] != 0xAA || buf[8] != 0xCC) {
        checkErrCount++;
        return;
    }
    
    // 检查校验和 - 只在数据有误时记录错误日志
    uint8_t checksum = 0;
    for (int i = 2; i < 7; ++i) {
        checksum ^= buf[i];
    }
    
    if (static_cast<uint8_t>(~checksum) != buf[7]) {
        checkErrCount++;
        // 错误日志记录限流 - 避免过多日志影响性能
        static int errorLogCount = 0;
        if (++errorLogCount <= 10 || errorLogCount % 100 == 0) {
            if (logModel) {
                QString hexData = data.toHex(' ');
                logModel->appendLogList(QString("校验错误数据[%1]: %2").arg(errorLogCount).arg(hexData));
                logModel->appendLogList(QString("接收校验和: %1, 计算校验和: %2")
                    .arg(buf[7], 2, 16, QChar('0'))
                    .arg(static_cast<uint8_t>(~checksum), 2, 16, QChar('0')));
            }
        }
        return;
    }
    
    // 解析数据 - 只更新内存中的数据，不直接更新UI
    uint8_t mode = buf[2];
    int16_t X1 = (buf[3] << 8) | buf[4];
    int16_t Y1 = (buf[5] << 8) | buf[6];
    
    // 验证模式范围并更新数据
    if (mode == 0x10) {
        // 使用互斥锁保护共享数据
        QMutexLocker locker(&dataMutex);
        latestValidData.X1 = X1;
        latestValidData.Y1 = Y1;
        latestValidData.mode = mode;
        latestValidData.hasNewData = true;
        
        // 更新图表数据 - 优化为滑动窗口方式
        timeCounter += 0.033; // 假设大约30Hz的数据更新频率
        timeData.append(timeCounter);
        x1Data.append(X1);
        y1Data.append(Y1);
        
        // 使用滑动窗口，保持固定数量的数据点以确保流畅度
        while (timeData.size() > maxDataPoints) {
            timeData.removeFirst();
            x1Data.removeFirst();
            y1Data.removeFirst();
        }
    }
}

void Prt_FSM50::SerialDataReceived(QByteArray data)
{
    // 高频数据优化：添加数据缓冲处理，避免粘包问题
    QMutexLocker locker(&dataMutex);
    dataBuffer.append(data);
    
    // 处理缓冲区中的完整数据包
    while (dataBuffer.size() >= 9) {
        // 查找帧头
        int frameStart = -1;
        for (int i = 0; i <= dataBuffer.size() - 3; ++i) {
            if (static_cast<uint8_t>(dataBuffer[i]) == 0x55 && 
                static_cast<uint8_t>(dataBuffer[i + 1]) == 0xAA) {
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
        if (dataBuffer.size() >= 9) {
            QByteArray packet = dataBuffer.left(9);
            dataBuffer.remove(0, 9);
            
            // 解析这个数据包（不使用互斥锁，因为已经在锁定状态）
            locker.unlock();
            Proctol_Analysis(packet);
            locker.relock();
        } else {
            break;
        }
    }
}

void Prt_FSM50::COM_Send_data(uint8_t *data, uint8_t len)
{
    // 发送数据的逻辑
    QByteArray sendData(reinterpret_cast<const char*>(data), len);
    emit Proctol_out(sendData); // 发射信号
}

// 添加槽函数实现
void Prt_FSM50::onModeChanged(int index)
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
        SendParamBtn->setText("设置零位");
        X1_Edit->setEnabled(true);
        Y1_Edit->setEnabled(true);
        X1_Edit->setPlaceholderText("X1零位");
        Y1_Edit->setPlaceholderText("Y1零位");
        // 恢复零位参数
        X1_Edit->setText(QString::number(Zero_cache[0]));
        Y1_Edit->setText(QString::number(Zero_cache[1]));
        break;
    default:
        break;
    }
}

void Prt_FSM50::onSendParamClicked()
{
    // 根据当前模式执行不同操作
    switch(currentMode) {
    case 0: // 10零位设置
        // 读取输入框参数并设置零位
        {
            int16_t X1_Zero = X1_Edit->text().toInt();
            int16_t Y1_Zero = Y1_Edit->text().toInt();
            uint8_t data[9];
            Creat_data(data, 0x10, X1_Zero, Y1_Zero);
            COM_Send_data(data, sizeof(data));
            //ElaMessageBar::information(ElaMessageBarType::TopRight, "零位设置", "已发送！", 1500);
            //添加操作记录
            if (logModel) {
                logModel->appendLogList(QString("设置零位: X1=%1, Y1=%2").arg(X1_Zero).arg(Y1_Zero));
                // 不再清除图表数据，保持数据连续性
            }
        }
        break;
    default:
        break;
    }
}

// 添加UI更新函数 - 定期批量更新界面，提高高频数据处理性能
void Prt_FSM50::updateUI()
{
    // 更新接收统计信息
    if (RecvCountLabel) RecvCountLabel->setText(QString("接收帧数: %1").arg(recvCount));
    if (CheckErrLabel) CheckErrLabel->setText(QString("校验错误帧: %1").arg(checkErrCount));
    
    // 更新数据显示
    QMutexLocker locker(&dataMutex);
    if (latestValidData.hasNewData) {
        if (X1_Info) X1_Info->setText(QString::number(latestValidData.X1));
        if (Y1_Info) Y1_Info->setText(QString::number(latestValidData.Y1));
        if (X1_Zero) X1_Zero->setText(QString::number(latestValidData.X1_Zero));
        if (Y1_Zero) Y1_Zero->setText(QString::number(latestValidData.Y1_Zero));
        
        // 更新图表显示 - 优化渲染性能
        if (customPlot && !timeData.isEmpty()) {
            // 使用快速更新方式，减少重绘开销
            customPlot->graph(0)->setData(timeData, x1Data);
            customPlot->graph(1)->setData(timeData, y1Data);
            
            // 自动调整X轴范围，显示最近的数据
            if (timeData.size() > 1) {
                double maxTime = timeData.last();
                double minTime = timeData.first();
                double timeSpan = maxTime - minTime;
                if (timeSpan > 0) {
                    // 显示固定10秒时间窗口，保持数据流畅滚动
                    double displaySpan = qMin(timeSpan, 10.0);
                    customPlot->xAxis->setRange(maxTime - displaySpan, maxTime);
                }
            }
            
            // 动态调整Y轴范围，但降低更新频率避免抖动
            static int yRangeUpdateCounter = 0;
            if (++yRangeUpdateCounter % 10 == 0) { // 每10次更新一次Y轴范围
                if (x1Data.size() > 0 && y1Data.size() > 0) {
                    // 计算最近数据的范围，而不是全部数据
                    int recentDataSize = qMin(100, timeData.size()); // 最近100个点
                    int startIdx = qMax(0, timeData.size() - recentDataSize);
                    
                    double minVal = std::numeric_limits<double>::max();
                    double maxVal = std::numeric_limits<double>::lowest();
                    
                    for (int i = startIdx; i < x1Data.size(); ++i) {
                        minVal = qMin(minVal, qMin(x1Data[i], y1Data[i]));
                        maxVal = qMax(maxVal, qMax(x1Data[i], y1Data[i]));
                    }
                    
                    if (minVal != std::numeric_limits<double>::max()) {
                        double range = maxVal - minVal;
                        double margin = qMax(1000.0, range * 0.1); // 至少1000的边距
                        customPlot->yAxis->setRange(minVal - margin, maxVal + margin);
                    }
                }
            }
            
            // 使用快速重绘模式
            customPlot->replot(QCustomPlot::rpQueuedReplot);
        }
        
        latestValidData.hasNewData = false;
    }
}

void Prt_FSM50::processBufferedData()
{
    // 预留函数，用于处理更复杂的数据缓冲逻辑
    // 如果需要处理粘包或分包情况，可以在这里实现
}
