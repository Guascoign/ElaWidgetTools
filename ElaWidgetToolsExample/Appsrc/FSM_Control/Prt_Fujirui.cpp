/********************************************************************************
    * 文件名称 ：Prt_Fujirui.cpp
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2025-06-06
    * 功     能：通信协议 界面
*********************************************************************************/
#include "Prt_Fujirui.h"
#include <algorithm>
#include <limits>

Prt_Fujirui::Prt_Fujirui(QWidget *parent)
    : ProtocolBaseWidget(parent)
{
    // 左侧上中下区域
    ElaScrollPageArea* leftTopArea = new ElaScrollPageArea(this);    // 上：信息显示
    ElaScrollPageArea* leftMidArea = new ElaScrollPageArea(this);    // 中：模式选择
    ElaScrollPageArea* leftBotArea = new ElaScrollPageArea(this);    // 下：参数设置
    ElaScrollPageArea* rightArea = new ElaScrollPageArea(this);      // 右：操作历史

    // 调整区域尺寸，左边框宽度两倍
    leftTopArea->setFixedHeight(100);      // 上
    leftTopArea->setFixedWidth(400);       // 宽度两倍
    leftMidArea->setFixedHeight(310);      // 中
    leftMidArea->setFixedWidth(400);       // 宽度两倍
    leftBotArea->setFixedHeight(125);      // 下
    leftBotArea->setFixedWidth(400);       // 宽度两倍
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

    ElaText* Text1 = new ElaText("反馈", leftTopArea);
    Text1->setFont(labelFont);

    ElaText* xLabel = new ElaText("摆镜位置反馈:", leftTopArea);
    ElaText* statusLabel = new ElaText("摆镜状态:", leftTopArea);

    // 设置标签字体
    xLabel->setFont(labelFont);
    statusLabel->setFont(labelFont);

    X_Info = new ElaText("N/A", leftTopArea);
    MirrorStatusInfo = new ElaText("未知", leftTopArea);

    // 设置数值字体
    X_Info->setFont(valueFont);
    MirrorStatusInfo->setFont(valueFont);

    infoGrid->addWidget(Text1, 0, 0, 1, 2);  // 标题跨两列
    infoGrid->addWidget(xLabel, 1, 0);
    infoGrid->addWidget(X_Info, 1, 1);
    infoGrid->addWidget(statusLabel, 2, 0);
    infoGrid->addWidget(MirrorStatusInfo, 2, 1);


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

    // 控制选择区 - 使用三列网格布局
    QGridLayout* controlGrid = new QGridLayout();
    controlGrid->setContentsMargins(8, 8, 8, 8);
    controlGrid->setSpacing(10);
    
    // 标签
    ElaText* modeLabel = new ElaText("模式指令:", leftMidArea);
    ElaText* syncLabel = new ElaText("同步:", leftMidArea);
    ElaText* freqLabel = new ElaText("频率:", leftMidArea);
    modeLabel->setFont(labelFont);
    syncLabel->setFont(labelFont);
    freqLabel->setFont(labelFont);
    
    // ComboBox控件
    Mode_Select = new ElaComboBox(leftMidArea);
    Mode_Select->setFont(valueFont);
    Mode_Select->addItems({"扫描","位置",});
    Mode_Select->setFixedWidth(120);
    
    sync_mode_Select = new ElaComboBox(leftMidArea);
    scan_freq_Select = new ElaComboBox(leftMidArea);
    sync_mode_Select->setFont(valueFont);
    scan_freq_Select->setFont(valueFont);
    sync_mode_Select->addItems({"外同步", "内同步"});
    scan_freq_Select->addItems({"低频", "中频", "高频"});
    sync_mode_Select->setCurrentIndex(0); // 默认选择外同步
    scan_freq_Select->setCurrentIndex(0); // 默认选择低频
    sync_mode_Select->setFixedWidth(120);
    scan_freq_Select->setFixedWidth(120);
    
    // 布局：三列显示
    controlGrid->addWidget(modeLabel, 0, 0);
    controlGrid->addWidget(Mode_Select, 1, 0);
    controlGrid->addWidget(syncLabel, 0, 1);
    controlGrid->addWidget(sync_mode_Select, 1, 1);
    controlGrid->addWidget(freqLabel, 0, 2);
    controlGrid->addWidget(scan_freq_Select, 1, 2);
    
    midLayout->addLayout(controlGrid);

    // 记录当前模式
    connect(Mode_Select, QOverload<int>::of(&ElaComboBox::currentIndexChanged), this, &Prt_Fujirui::onModeChanged);

    // ===== 下：参数设置区 =====
    QGridLayout* paramGrid = new QGridLayout(leftBotArea);
    paramGrid->setContentsMargins(8, 8, 8, 8);
    paramGrid->setSpacing(6);
    
    // 第一行：控制参数 (±800°/s)
    ElaText* ctrlLabel = new ElaText("控制(°/s):", leftBotArea);
    ctrlLabel->setFont(labelFont);
    ctrl_Edit = new ElaLineEdit(leftBotArea);
    ctrl_Edit->setFont(valueFont);
    ctrl_Edit->setPlaceholderText("±800°/s");
    ctrl_Edit->setFixedWidth(100);  // 减小宽度
    ctrlValueLabel = new ElaText("0", leftBotArea);
    ctrlValueLabel->setFont(valueFont);
    ctrlValueLabel->setStyleSheet("QLabel { color: #009900; font-weight: bold; }");
    ctrlHexLabel = new ElaText("0x0000", leftBotArea);
    ctrlHexLabel->setFont(valueFont);
    ctrlHexLabel->setStyleSheet("QLabel { color: #FF6600; font-weight: bold; }");
    
    // 第二行：匀速段角位置调节 (±1.5°)
    ElaText* speedLabel = new ElaText("角位置(°):", leftBotArea);
    speedLabel->setFont(labelFont);
    speedpos_Edit = new ElaLineEdit(leftBotArea);
    speedpos_Edit->setFont(valueFont);
    speedpos_Edit->setPlaceholderText("±1.5°");
    speedpos_Edit->setFixedWidth(100);  // 减小宽度
    speedValueLabel = new ElaText("32767", leftBotArea);
    speedValueLabel->setFont(valueFont);
    speedValueLabel->setStyleSheet("QLabel { color: #009900; font-weight: bold; }");
    speedHexLabel = new ElaText("0x7FFF", leftBotArea);
    speedHexLabel->setFont(valueFont);
    speedHexLabel->setStyleSheet("QLabel { color: #FF6600; font-weight: bold; }");
    
    // 度数输入限制器
    QDoubleValidator* speedValidator = new QDoubleValidator(-800.0, 800.0, 3, this);
    QDoubleValidator* degreeValidator1_5 = new QDoubleValidator(-1.5, 1.5, 3, this);
    ctrl_Edit->setValidator(speedValidator);
    speedpos_Edit->setValidator(degreeValidator1_5);
    
    //默认lineEdit参数
    ctrl_Edit->setText("0");
    speedpos_Edit->setText("0");
    SendParamBtn = new ElaPushButton("发送参数", leftBotArea);
    
    // 布局
    paramGrid->addWidget(ctrlLabel, 0, 0);
    paramGrid->addWidget(ctrl_Edit, 0, 1);
    paramGrid->addWidget(ctrlValueLabel, 0, 2);
    paramGrid->addWidget(ctrlHexLabel, 0, 3);
    
    paramGrid->addWidget(speedLabel, 1, 0);
    paramGrid->addWidget(speedpos_Edit, 1, 1);
    paramGrid->addWidget(speedValueLabel, 1, 2);
    paramGrid->addWidget(speedHexLabel, 1, 3);
    
    paramGrid->addWidget(SendParamBtn, 2, 0, 1, 4);

    connect(SendParamBtn, &ElaPushButton::clicked, this, &Prt_Fujirui::onSendParamClicked);

    // 连接度数转换实时更新信号槽
    connect(ctrl_Edit, &ElaLineEdit::textChanged, this, &Prt_Fujirui::updateCtrlValue);
    connect(speedpos_Edit, &ElaLineEdit::textChanged, this, &Prt_Fujirui::updateSpeedValue);

    // 创建UI更新定时器 - 优化更新频率以提高性能和流畅度
    uiUpdateTimer = new QTimer(this);
    uiUpdateTimer->setInterval(50); // 50ms = 20Hz，平衡性能和响应性
    connect(uiUpdateTimer, &QTimer::timeout, this, &Prt_Fujirui::updateUI);
    uiUpdateTimer->start();
    
    // 初始化数据结构
    latestValidData.hasNewData = false;
    latestValidData.posfb = 0;
    latestValidData.mirrorStatus = 0x00;  // 初始化为正常状态
    
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
    customPlot->graph(0)->setName("摆镜位置");
    
    
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
    customPlot->graph(0)->setData(timeData, x1Data);
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
    X_Info->setFont(infoFont);

    // 信号槽可后续补充
    // connect(SendParamBtn, &ElaPushButton::clicked, this, &Prt_Fujirui::onSendParamClicked);
}

Prt_Fujirui::~Prt_Fujirui()
{
    // 停止定时器
    if (uiUpdateTimer) {
        uiUpdateTimer->stop();
    }
}

void Prt_Fujirui::SerialConnected(bool connected)
{
    // 连接状态处理
    if (connected) {
        // 连接成功时的逻辑
    } else {
        // 断开连接时的逻辑
    }
}

void Prt_Fujirui::Creat_data(uint8_t *data, uint8_t mode, uint8_t sync_mode , uint8_t scan_freq , int16_t ctrl , int16_t speedpos)
{
    data[0] = 0x55; // 帧头55
    data[1] = 0xAA; // 帧头AA
    data[2] = mode; // 模式指令
    data[3] = sync_mode; // 内同步外同步设置
    data[4] = scan_freq; // 摆扫频率参数设置00->35Hz 01->50HZ 
    data[5] = (ctrl >> 8) & 0xFF; // 控制参数高字节
    data[6] = ctrl & 0xFF; // 控制参数低字节
    data[7] = (speedpos >> 8) & 0xFF; // 匀速段角位置调节
    data[8] = speedpos & 0xFF; // 匀速段角位置调节低字节

    // 校验 data[2]~data[8]与0xFF异或和
    uint8_t checksum = 0xff;
    for (int i = 2; i < 9; ++i) {
        checksum ^= data[i];
    }
    data[9] = checksum; // 校验和
    data[10] = 0xCC; // 帧尾CC
}

void Prt_Fujirui::Proctol_Analysis(const QByteArray &data)
{
    recvCount++;
    if (data.size() < 7) return;  // 新协议长度为7字节

    const uint8_t *buf = reinterpret_cast<const uint8_t*>(data.constData());
    // 检查帧头：0x55AA
    if (buf[0] != 0x55 || buf[1] != 0xAA) {
        checkErrCount++;
        return;
    }
    // 检查帧尾：0xCC
    if (buf[6] != 0xCC) {
        checkErrCount++;
        return;
    }
    
    // 校验：不包括信息头和帧尾的其它字节与0xFF的异或和
    uint8_t checksum = 0xFF;  // 从0xFF开始
    for (int i = 2; i <= 4; ++i) {  // 字节2(状态)和字节3-4(位置)参与校验
        checksum ^= buf[i];
    }
    if (checksum != buf[5]) {
        checkErrCount++;
        return;
    }

    uint8_t mirrorStatus = buf[2];  // 摆镜状态
    int16_t mirrorPos = (buf[3] << 8) | buf[4];  // 摆镜位置反馈：16位补码，高字节先发
    
    // 更新数据
    {
        QMutexLocker locker(&dataMutex);
        latestValidData.posfb = mirrorPos;
        latestValidData.mirrorStatus = mirrorStatus;
        latestValidData.hasNewData = true;

        timeCounter += 0.033;
        timeData.append(timeCounter);
        // 将原始数据转换为度数：16位补码，满量程对应±10°
        double posInDegrees = mirrorPos * 10.0 / 32768.0;  // 16位补码转换为±10°
        x1Data.append(posInDegrees);

        while (timeData.size() > maxDataPoints) {
            timeData.removeFirst();
            x1Data.removeFirst();
        }
    }
}

void Prt_Fujirui::SerialDataReceived(QByteArray data)
{
    // 高频数据优化：改进数据缓冲处理，提高1ms高频数据的稳定性
    static QByteArray staticBuffer;  // 使用静态缓冲区避免频繁的互斥锁操作
    
    // 将新数据追加到静态缓冲区
    staticBuffer.append(data);
    
    // 限制缓冲区大小，防止内存无限增长
    if (staticBuffer.size() > 1024) {
        // 保留最后512字节，丢弃过旧数据
        staticBuffer = staticBuffer.right(512);
    }
    
    // 处理缓冲区中的完整数据包
    while (staticBuffer.size() >= 7) {  // 新协议长度为7字节
        // 查找帧头 0x55AA
        int frameStart = -1;
        for (int i = 0; i <= staticBuffer.size() - 2; ++i) {
            if (static_cast<uint8_t>(staticBuffer[i]) == 0x55 && 
                static_cast<uint8_t>(staticBuffer[i+1]) == 0xAA) {
                frameStart = i;
                break;
            }
        }
        
        if (frameStart == -1) {
            // 没有找到有效帧头，保留最后6个字节（可能包含不完整的帧头）
            if (staticBuffer.size() > 6) {
                staticBuffer = staticBuffer.right(6);
            }
            break;
        }
        
        // 如果帧头不在开始位置，移除无效数据
        if (frameStart > 0) {
            staticBuffer.remove(0, frameStart);
        }
        
        // 检查是否有完整的数据包
        if (staticBuffer.size() >= 7) {
            // 验证帧尾是否正确
            if (static_cast<uint8_t>(staticBuffer[6]) == 0xCC) {
                QByteArray packet = staticBuffer.left(7);
                staticBuffer.remove(0, 7);
                
                // 解析这个数据包（在独立线程中处理，避免阻塞）
                Proctol_Analysis(packet);
            } else {
                // 帧尾不正确，跳过第一个字节继续查找
                staticBuffer.remove(0, 1);
            }
        } else {
            break;
        }
    }
}

void Prt_Fujirui::COM_Send_data(uint8_t *data, uint8_t len)
{
    // 发送数据的逻辑
    QByteArray sendData(reinterpret_cast<const char*>(data), len);
    emit Proctol_out(sendData); // 发射信号
}

// 添加槽函数实现
void Prt_Fujirui::onModeChanged(int index)
{

}


// 32767 -> 0V uint16


// 1.5° -> 1.5V
void Prt_Fujirui::onSendParamClicked()
{
    // 根据当前模式执行不同操作
            uint8_t mode = Mode_Select->currentIndex()+ 1; // 1->扫描模式, 2->位置模式
            uint8_t sync_mode = sync_mode_Select->currentIndex(); // 0->外同步, 1->内同步
            uint8_t scan_freq = scan_freq_Select->currentIndex() + 1; // 1->低频, 2->中频, 3->高频
            int16_t ctrl = speedToValue(ctrl_Edit->text().toDouble());
            int16_t speedpos = degreesToValue1_5(speedpos_Edit->text().toDouble());
            
            uint8_t data[11];
            Creat_data(data, mode, sync_mode, scan_freq, ctrl, speedpos);
            COM_Send_data(data, sizeof(data));
            
            //添加操作记录
            if (logModel) {
                QString modeText =  mode == 1 ? "扫描模式" : "位置模式";
                QString syncText = sync_mode == 0 ? "外同步" : "内同步";
                QString freqText = QStringList{"低频", "中频", "高频"}[scan_freq - 1];
                logModel->appendLogList(QString("扫描模式发送: %1, %2, 控制=%3, 角位置=%4")
                    .arg(modeText).arg(syncText).arg(freqText).arg(ctrl).arg(speedpos));
            }
    
}

// 添加UI更新函数 - 定期批量更新界面，提高高频数据处理性能
void Prt_Fujirui::updateUI()
{
    // 更新接收统计信息
    if (RecvCountLabel) RecvCountLabel->setText(QString("接收帧数: %1").arg(recvCount));
    if (CheckErrLabel) CheckErrLabel->setText(QString("校验错误帧: %1").arg(checkErrCount));
    
    // 更新数据显示
    QMutexLocker locker(&dataMutex);
    if (latestValidData.hasNewData) {
        // 更新摆镜位置反馈，显示度数格式
        if (X_Info) {
            double posInDegrees = latestValidData.posfb * 10.0 / 32768.0;  // 转换为±10°
            X_Info->setText(QString::number(posInDegrees, 'f', 2) + "°");
        }
        
        // 更新摆镜状态，设置颜色
        if (MirrorStatusInfo) {
            if (latestValidData.mirrorStatus == 0x00) {
                MirrorStatusInfo->setText("正常");
                MirrorStatusInfo->setStyleSheet("QLabel { color: #00AA00; font-weight: bold; }");  // 绿色字体
            } else {
                MirrorStatusInfo->setText("故障");
                MirrorStatusInfo->setStyleSheet("QLabel { color: #FF0000; font-weight: bold; }");  // 红色字体
            }
        }
        
        // 更新图表显示 - 优化渲染性能
        if (customPlot && !timeData.isEmpty()) {
            // 使用快速更新方式，减少重绘开销
            customPlot->graph(0)->setData(timeData, x1Data);
            
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
                if (x1Data.size() > 0) {
                    // 计算最近数据的范围，而不是全部数据
                    int recentDataSize = qMin(100, timeData.size()); // 最近100个点
                    int startIdx = qMax(0, timeData.size() - recentDataSize);
                    
                    double minVal = std::numeric_limits<double>::max();
                    double maxVal = std::numeric_limits<double>::lowest();
                    
                    for (int i = startIdx; i < x1Data.size(); ++i) {
                        minVal = qMin(minVal, x1Data[i]);
                        maxVal = qMax(maxVal, x1Data[i]);
                    }
                    
                    if (minVal != std::numeric_limits<double>::max()) {
                        double range = maxVal - minVal;
                        double margin = qMax(50.0, range * 0.1); // 至少1000的边距
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

void Prt_Fujirui::processBufferedData()
{
    // 预留函数，用于处理更复杂的数据缓冲逻辑
    // 如果需要处理粘包或分包情况，可以在这里实现
}

// 速度转码值函数：±400~800°/s，1LSB=0.05°/s
// 0→0, -800→49536, -400→57356, 400→8000, 800→16000
int16_t Prt_Fujirui::speedToValue(double speed)
{
    // 限制范围 [-1000, 1000]
    speed = qBound(-1000.0, speed, 1000.0);
    return static_cast<int16_t>(speed * 20.0);
    
}

// 度数转码值函数：±5°对应16bit完整范围 -5° -> 0, 0° -> 32768, +5° -> 65535
int16_t Prt_Fujirui::degreesToValue1_5(double degrees)
{
    // 限制范围 [-5.0, 5.0]
    degrees = qBound(-5.0, degrees, 5.0);
    // 线性映射：-5° -> 0, 0° -> 32768, +5° -> 65535
    // 映射公式：32768 + (degrees * 32768 / 5)
    double mapped = 32768.0 + (degrees * 32768.0 / 5.0);
    return static_cast<int16_t>(mapped);
}

// 控制参数实时更新槽函数
void Prt_Fujirui::updateCtrlValue()
{
    bool ok;
    double speed = ctrl_Edit->text().toDouble(&ok);
    if (ok) {
        uint16_t value = speedToValue(speed);
        ctrlValueLabel->setText(QString::number(value));
        ctrlHexLabel->setText(QString("0x%1").arg(value, 4, 16, QChar('0')).toUpper());
    } else {
        ctrlValueLabel->setText("0");
        ctrlHexLabel->setText("0x0000");
    }
}

// 角位置参数实时更新槽函数
void Prt_Fujirui::updateSpeedValue()
{
    bool ok;
    double degrees = speedpos_Edit->text().toDouble(&ok);
    if (ok) {
        uint16_t value = degreesToValue1_5(degrees);
        speedValueLabel->setText(QString::number(value));
        speedHexLabel->setText(QString("0x%1").arg(value, 4, 16, QChar('0')).toUpper());
    } else {
        speedValueLabel->setText("32768");
        speedHexLabel->setText("0x8000");
    }
}
