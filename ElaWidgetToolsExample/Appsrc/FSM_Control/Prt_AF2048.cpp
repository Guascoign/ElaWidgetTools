/********************************************************************************
    * 文件名称 ：Prt_AF2048.cpp
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2025-06-06
    * 功     能：通信协议 界面
*********************************************************************************/
#include "Prt_AF2048.h"
#include <algorithm>
#include <limits>

Prt_AF2048::Prt_AF2048(QWidget *parent)
    : ProtocolBaseWidget(parent)
{
    // 左侧上中下区域
    ElaScrollPageArea* leftTopArea = new ElaScrollPageArea(this);    // 上：信息显示
    ElaScrollPageArea* leftMidArea = new ElaScrollPageArea(this);    // 中：模式选择
    ElaScrollPageArea* leftBotArea = new ElaScrollPageArea(this);    // 下：参数设置
    ElaScrollPageArea* wavetestArea = new ElaScrollPageArea(this);    // 下：参数设置
    ElaScrollPageArea* rightArea = new ElaScrollPageArea(this);      // 右：操作历史

    // 调整区域尺寸，与Prt_TEST一致
    leftTopArea->setFixedHeight(100);      // 上
    leftTopArea->setFixedWidth(300);
    leftMidArea->setFixedHeight(110);      // 中
    leftMidArea->setFixedWidth(300);
    leftBotArea->setFixedHeight(135);      // 下
    leftBotArea->setFixedWidth(300);
    wavetestArea->setFixedHeight(180);      // 下
    wavetestArea->setFixedWidth(300);
    rightArea->setFixedHeight(550);        // 右
    // rightArea->setFixedWidth(300);      // 可选，若需固定宽度

    // 左侧垂直布局

    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(5);
    leftLayout->addWidget(leftTopArea);
    leftLayout->addWidget(leftMidArea);
    leftLayout->addWidget(leftBotArea);
    leftLayout->addWidget(wavetestArea);
    leftLayout->addStretch(); // 保持底部对齐

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
    labelFont.setPointSize(14); // 统一字号，可根据实际界面调整
    labelFont.setBold(true);

    QFont labelFont1;
    labelFont1.setPointSize(12); // 统一字号，可根据实际界面调整
    labelFont1.setBold(true);

    QFont valueFont;
    valueFont.setPointSize(15);

    QFont valueFont1;
    valueFont1.setPointSize(12);


    ElaText* Text1 = new ElaText("位置", leftTopArea);
    Text1->setFont(labelFont);

    ElaText* x1Label = new ElaText("X1:", leftTopArea);
    ElaText* y1Label = new ElaText("Y1:", leftTopArea);

    // 设置标签字体
    x1Label->setFont(labelFont);
    y1Label->setFont(labelFont);


    X1_Info = new ElaText("N/A", leftTopArea);
    Y1_Info = new ElaText("N/A", leftTopArea);

    // 设置数值字体
    X1_Info->setFont(valueFont);
    Y1_Info->setFont(valueFont);

    infoGrid->addWidget(Text1, 0, 0);
    infoGrid->addWidget(x1Label, 1, 0);
    infoGrid->addWidget(X1_Info, 1, 1);
    infoGrid->addWidget(y1Label, 1, 2);
    infoGrid->addWidget(Y1_Info, 1, 3);


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
    Mode_Select->addItems({
        "模拟闭环",
        "模拟开环",
        "数字闭环",
        "数字开环",
    });
    Mode_Select->setCurrentIndex(0); // 默认选择模拟闭环
    Mode_Select->setFixedWidth(180);
    modeLayout->addWidget(modeLabel);
    modeLayout->addWidget(Mode_Select);
    modeLayout->addStretch();
    midLayout->addLayout(modeLayout);

    // 记录当前模式
    connect(Mode_Select, QOverload<int>::of(&ElaComboBox::currentIndexChanged), this, &Prt_AF2048::onModeChanged);

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
    
    // 添加循环发送控件
    LoopSendBtn = new ElaToggleButton("循环发送", leftBotArea);
    //LoopSendBtn->setFont(labelFont);
    
    ElaText* freqLabel = new ElaText("频率(Hz):", leftBotArea);
    freqLabel->setFont(labelFont1);
    FreqEdit = new ElaLineEdit(leftBotArea);
    FreqEdit->setFont(valueFont);
    FreqEdit->setPlaceholderText("1-1000");
    FreqEdit->setText("25");  // 默认10Hz
    FreqEdit->setFixedWidth(80);
    
    // 添加频率限制
    QIntValidator* freqValidator = new QIntValidator(1, 1000, this);
    FreqEdit->setValidator(freqValidator);
    
    paramGrid->addWidget(x1SetLabel, 0, 0);
    paramGrid->addWidget(X1_Edit, 0, 1);
    paramGrid->addWidget(y1SetLabel, 0, 2);
    paramGrid->addWidget(Y1_Edit, 0, 3);
    paramGrid->addWidget(SendParamBtn, 1, 0, 1, 4);
    paramGrid->addWidget(LoopSendBtn, 2, 0, 1, 2);
    paramGrid->addWidget(freqLabel, 2, 2);
    paramGrid->addWidget(FreqEdit, 2, 3);


    // ===== wavetestArea：波形测试区域 =====
    QGridLayout* waveTestGrid = new QGridLayout(wavetestArea);
    waveTestGrid->setContentsMargins(8, 8, 8, 8);
    waveTestGrid->setSpacing(6);

    // 峰值测试标题
    ElaText* peakTestTitle = new ElaText("峰值测试", wavetestArea);
    peakTestTitle->setFont(labelFont1);

    // 开启测试按钮
    startTestBtn = new ElaToggleButton(wavetestArea);
    startTestBtn->setText("开始测试");
    startTestBtn->setIsToggled(false);  // 默认关闭状态
    
    // 测试模式选择
    ElaText* testModeLabel = new ElaText("测试模式:", wavetestArea);
    testModeLabel->setFont(labelFont1);
    testModeSelect = new ElaComboBox(wavetestArea);
    //超调比例 ≤10%合格  ＞10%不合格
    testModeSelect->addItems({
        "电/20/0.5",    // 电调镜测试 频率20Hz  幅值0.5mrad 码值±626 电压±0.191V
        "电/10/1.0",    // 电调镜测试 频率10Hz  幅值1.0mrad 码值±1252 电压±0.382V
        "电/5/1.5",     // 电调镜测试 频率5Hz   幅值1.5mrad 码值±1878 电压±0.573V
        "电/1/3.0",     // 电调镜测试 频率1Hz   幅值3.0mrad 码值±3756 电压±1.147V
        "快/200/0.5",   // 快反镜测试 频率200Hz 幅值0.5mrad 码值±626 电压±0.191V
        "快/100/1.0",   // 快反镜测试 频率100Hz 幅值1.0mrad 码值±1252 电压±0.382V
        "快/50/1.5",    // 快反镜测试 频率50Hz  幅值1.5mrad 码值±1878 电压±0.573V
        "快/20/3.0"     // 快反镜测试 频率20Hz  幅值3.0mrad 码值±3756 电压±1.147V
    });
    testModeSelect->setCurrentIndex(0);
    testModeSelect->setFixedWidth(180);

    // 峰峰值数据显示
    QHBoxLayout* peakLayout = new QHBoxLayout();
    ElaText* peakToPeakLabel = new ElaText("峰峰值:", wavetestArea);
    peakToPeakLabel->setFont(labelFont1);
    peakToPeakValue_X = new ElaText("N/A", wavetestArea);
    peakToPeakValue_X->setFont(valueFont1);
    peakToPeakValue_Y = new ElaText("N/A", wavetestArea);
    peakToPeakValue_Y->setFont(valueFont1);
    peakLayout->addWidget(peakToPeakLabel);
    peakLayout->addWidget(peakToPeakValue_X);
    peakLayout->addWidget(peakToPeakValue_Y);
    peakLayout->addStretch();

    //峰峰值 度数数据显示 mard
    // 1度 = π/180 弧度 ≈ 0.01745 弧度
    QHBoxLayout* degreeLayout = new QHBoxLayout();
    ElaText* degreeLabel = new ElaText("峰峰值(度):", wavetestArea);
    degreeLabel->setFont(labelFont1);
    degreeValue_X = new ElaText("N/A", wavetestArea);
    degreeValue_X->setFont(valueFont1);
    degreeValue_Y = new ElaText("N/A", wavetestArea);
    degreeValue_Y->setFont(valueFont1);
    degreeLayout->addWidget(degreeLabel);
    degreeLayout->addWidget(degreeValue_X);
    degreeLayout->addWidget(degreeValue_Y);
    degreeLayout->addStretch();

    // 超调比例数据显示
    QHBoxLayout* overshootLayout = new QHBoxLayout();
    ElaText* overshootLabel = new ElaText("超调比:", wavetestArea);
    overshootLabel->setFont(labelFont1);
    overshootValue_X = new ElaText("N/A", wavetestArea);
    overshootValue_X->setFont(valueFont1);
    overshootValue_Y = new ElaText("N/A", wavetestArea);
    overshootValue_Y->setFont(valueFont1);
    overshootLayout->addWidget(overshootLabel);
    overshootLayout->addWidget(overshootValue_X);
    overshootLayout->addWidget(overshootValue_Y);
    overshootLayout->addStretch();

    // 添加到布局
    waveTestGrid->addWidget(peakTestTitle, 0, 0);
    waveTestGrid->addWidget(startTestBtn, 0, 1);
    waveTestGrid->addWidget(testModeLabel, 1, 0);
    waveTestGrid->addWidget(testModeSelect, 1, 1);
    waveTestGrid->addLayout(peakLayout, 2, 0, 1, 2);
    waveTestGrid->addLayout(degreeLayout, 3, 0, 1, 2);
    waveTestGrid->addLayout(overshootLayout, 4, 0, 1, 2);


    connect(SendParamBtn, &ElaPushButton::clicked, this, &Prt_AF2048::onSendParamClicked);

    // 创建循环发送定时器
    loopSendTimer = new QTimer(this);
    connect(loopSendTimer, &QTimer::timeout, this, [this]() {
        uint8_t data[9];
        int16_t X1_val = X1_Edit->text().toInt();
        int16_t Y1_val = Y1_Edit->text().toInt();
        uint8_t mode = currentMode + 1; // 0->1, 1->2, 2->3, 3->4
        Creat_data(data, mode, X1_val, Y1_val);
        COM_Send_data(data, sizeof(data));
        // 不记录发送历史以避免日志过多
    });

    // 创建UI更新定时器 - 限制UI更新频率到30Hz以提高性能
    uiUpdateTimer = new QTimer(this);
    uiUpdateTimer->setInterval(33); // 33ms = 30Hz
    connect(uiUpdateTimer, &QTimer::timeout, this, &Prt_AF2048::updateUI);
    uiUpdateTimer->start();
    
    // 初始化数据结构
    latestValidData.hasNewData = false;
    latestValidData.X1 = 0;
    latestValidData.Y1 = 0;
    latestValidData.mode = 0;
    
    // 初始化图表数据
    timeCounter = 0.0;

    // 连接循环发送按钮
    connect(LoopSendBtn, &ElaToggleButton::toggled, this, [this](bool checked) {
        if (checked) {
            // 根据频率设置定时器间隔
            int freq = FreqEdit->text().toInt();
            if (freq < 1) freq = 1;
            if (freq > 1000) freq = 1000;
            int interval = 1000 / freq; // 毫秒
            loopSendTimer->setInterval(interval);
            loopSendTimer->start();
            if (logModel) logModel->prependLogList(QString("开始循环发送 - 频率: %1Hz").arg(freq));
        } else {
            loopSendTimer->stop();
            if (logModel) logModel->prependLogList(QString("停止循环发送"));
        }
    });

    // 初始化波形测试线程
    waveTestThread = new WaveTestThread(this);
    connect(waveTestThread, &WaveTestThread::peakToPeakUpdated, this, &Prt_AF2048::onPeakToPeakUpdated);
    connect(startTestBtn, &ElaToggleButton::toggled, this, &Prt_AF2048::onStartTestToggled);


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
    customPlot->xAxis->setRange(0, 5); // 显示最近5秒数据
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

    // === 添加性能优化设置 ===
    // 禁用抗锯齿以提高性能
    customPlot->setAntialiasedElements(QCP::aeNone);
    customPlot->setNotAntialiasedElements(QCP::aeAll);

    // 优化重绘策略
    customPlot->setPlottingHints(QCP::phCacheLabels | QCP::phImmediateRefresh);

    // 设置合理的采样率
    customPlot->graph(0)->setAdaptiveSampling(true);
    customPlot->graph(1)->setAdaptiveSampling(true);

    // 减少线条细节
    customPlot->graph(0)->setLineStyle(QCPGraph::lsLine);
    customPlot->graph(1)->setLineStyle(QCPGraph::lsLine);

    // 禁用鼠标跟踪以减少开销
    customPlot->setMouseTracking(false);
    
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
    logModel->prependLogList(QString("日志记录已开启"));
    // 字体美化
    QFont infoFont; infoFont.setPointSize(13);
    X1_Info->setFont(infoFont); Y1_Info->setFont(infoFont);

    // 信号槽可后续补充
    // connect(SendParamBtn, &ElaPushButton::clicked, this, &Prt_AF2048::onSendParamClicked);
}

Prt_AF2048::~Prt_AF2048()
{
    // 停止定时器
    if (loopSendTimer) {
        loopSendTimer->stop();
    }
    if (uiUpdateTimer) {
        uiUpdateTimer->stop();
    }
}

void Prt_AF2048::SerialConnected(bool connected)
{
    // 连接状态处理
    if (connected) {
        // 连接成功时的逻辑
    } else {
        // 断开连接时的逻辑
    }
}

void Prt_AF2048::Creat_data(uint8_t *data, uint8_t mode, int16_t X1, int16_t Y1 )
{
    // 生成协议数据的逻辑
    data[0] = 0xEB; // 帧头
    data[1] = 0x90; // 帧类型
    data[2] = 0x12; //帧标识
    data[3] = mode; // 模式
    data[4] = X1 >> 8; // X1高字节
    data[5] = X1 & 0xFF; // X1低字节
    data[6] = Y1 >> 8; // Y1高字节
    data[7] = Y1 & 0xFF; // Y1低字节
    //不包括信息头和字尾的其它字节异后再与与0xFF异或
    uint8_t checksum = 0;
    for (int i = 0; i < 8; ++i) {
        checksum += data[i];
    }
    data[8] = checksum; // 校验和

}

void Prt_AF2048::Proctol_Analysis(const QByteArray &data)
{
    // 使用原子操作更新计数器，避免频繁的UI更新
    recvCount++;
    
    // 协议解析逻辑 - 修改为9字节协议
    if (data.size() < 9) {
        return;
    }
    
    const uint8_t *buf = reinterpret_cast<const uint8_t*>(data.constData());
    if (buf[0] != 0xEB || buf[1] != 0x90 ) {
        checkErrCount++;
        return;
    }
    
    // 检查校验和 - 只在数据有误时记录错误日志
    uint8_t checksum = 0;
    for (int i = 0; i < 8; ++i) {
        checksum += buf[i];
    }
    
    if (static_cast<uint8_t>(checksum) != buf[8]) {
        checkErrCount++;
        // 错误日志记录限流 - 避免过多日志影响性能
        static int errorLogCount = 0;
        if (++errorLogCount <= 10 || errorLogCount % 100 == 0) {
            if (logModel) {
                QString hexData = data.toHex(' ');
                logModel->prependLogList(QString("校验错误数据[%1]: %2").arg(errorLogCount).arg(hexData));
                logModel->prependLogList(QString("接收校验和: %1, 计算校验和: %2")
                    .arg(buf[8], 2, 16, QChar('0'))
                    .arg(static_cast<uint8_t>(checksum), 2, 16, QChar('0')));
            }
        }
        return;
    }
    
    // 解析数据 - 只更新内存中的数据，不直接更新UI
    uint8_t mode = buf[3];
    int16_t X1 = (buf[4] << 8) | buf[5];
    int16_t Y1 = (buf[6] << 8) | buf[7];
    
    // 验证模式范围
   // if (mode >= 0x01 && mode <= 0x04) {
        // 使用互斥锁保护共享数据
        QMutexLocker locker(&dataMutex);
        latestValidData.X1 = X1;
        latestValidData.Y1 = Y1;
        latestValidData.mode = mode;
        latestValidData.hasNewData = true;

        timeCounter += 0.033;
        timeData.append(timeCounter);
        x1Data.append(X1);
        y1Data.append(Y1);

        while (timeData.size() > maxDataPoints) {
            timeData.removeFirst();
            x1Data.removeFirst();
            y1Data.removeFirst();
        }
    //}
}

void Prt_AF2048::SerialDataReceived(QByteArray data)
{
    // 使用静态缓冲区避免频繁内存分配
    static QByteArray staticBuffer;
    static const int MAX_BUFFER_SIZE = 2048; // 限制缓冲区大小
    
    staticBuffer.append(data);
    
    // 防止缓冲区无限增长
    if (staticBuffer.size() > MAX_BUFFER_SIZE) {
        staticBuffer = staticBuffer.right(512); // 保留最后512字节
    }
    
    // 批量处理数据包
    while (staticBuffer.size() >= 9) {
        // 快速查找帧头 - 使用memchr优化
        int frameStart = -1;
        const char* buffer = staticBuffer.constData();
        const char* found = reinterpret_cast<const char*>(
            memchr(buffer, 0xEB, staticBuffer.size() - 1));
        
        if (found && (found - buffer) < staticBuffer.size() - 1) {
            if (static_cast<uint8_t>(found[1]) == 0x90) {
                frameStart = found - buffer;
            }
        }
        
        if (frameStart == -1) {
            // 保留最后8个字节，可能包含不完整帧头
            if (staticBuffer.size() > 8) {
                staticBuffer = staticBuffer.right(8);
            }
            break;
        }
        
        // 移除无效数据
        if (frameStart > 0) {
            staticBuffer.remove(0, frameStart);
        }
        
        // 提取并处理数据包
        if (staticBuffer.size() >= 9) {
            QByteArray packet = staticBuffer.left(9);
            staticBuffer.remove(0, 9);
            Proctol_Analysis(packet);
        }
    }
}

void Prt_AF2048::COM_Send_data(uint8_t *data, uint8_t len)
{
    // 发送数据的逻辑
    QByteArray sendData(reinterpret_cast<const char*>(data), len);
    emit Proctol_out(sendData); // 发射信号
}

// 添加槽函数实现
void Prt_AF2048::onModeChanged(int index)
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
        break;
    default:
        break;
    }
}

void Prt_AF2048::onSendParamClicked()
{
    int16_t X1_Zero = X1_Edit->text().toInt();
    int16_t Y1_Zero = Y1_Edit->text().toInt();
    // 根据当前模式执行不同操作
    switch(currentMode) {
    case 0: // 模拟闭环
        // 读取输入框参数并设置零位
            uint8_t data[9];
            Creat_data(data, 0x01, X1_Zero, Y1_Zero);
            COM_Send_data(data, sizeof(data));
            ElaMessageBar::information(ElaMessageBarType::TopRight, "零位设置", "已发送！", 1500);
            //添加操作记录
            if (logList) logModel->prependLogList(QString("设置零位: X1=%1, Y1=%2").arg(X1_Zero).arg(Y1_Zero));
        break;
    case 1: // 模拟开环
        // 读取输入框参数并设置零位
            X1_Zero = X1_Edit->text().toInt();
            Y1_Zero = Y1_Edit->text().toInt();
            Creat_data(data, 0x02, X1_Zero, Y1_Zero);
            COM_Send_data(data, sizeof(data));
            ElaMessageBar::information(ElaMessageBarType::TopRight, "零位设置", "已发送！", 1500);
            //添加操作记录
            if (logList) logModel->prependLogList(QString("设置零位: X1=%1, Y1=%2").arg(X1_Zero).arg(Y1_Zero));
        break;
    case 2: // 数字闭环
        // 读取输入框参数并设置零位
        X1_Zero = X1_Edit->text().toInt();
        Y1_Zero = Y1_Edit->text().toInt();
        Creat_data(data, 0x03, X1_Zero, Y1_Zero);
        COM_Send_data(data, sizeof(data));
        ElaMessageBar::information(ElaMessageBarType::TopRight, "零位设置", "已发送！", 1500);
        //添加操作记录
        if (logList) logModel->prependLogList(QString("设置零位: X1=%1, Y1=%2").arg(X1_Zero).arg(Y1_Zero));
        break;
    case 3: // 数字开环
        // 读取输入框参数并设置零位
        X1_Zero = X1_Edit->text().toInt();
        Y1_Zero = Y1_Edit->text().toInt();
        Creat_data(data, 0x04, X1_Zero, Y1_Zero);
        COM_Send_data(data, sizeof(data));
        ElaMessageBar::information(ElaMessageBarType::TopRight, "零位设置", "已发送！", 1500);
        //添加操作记录
        if (logList) logModel->prependLogList(QString("设置零位: X1=%1, Y1=%2").arg(X1_Zero).arg(Y1_Zero));
        break;
    default:
        break;
    }
}

// 添加UI更新函数 - 定期批量更新界面，提高高频数据处理性能
void Prt_AF2048::updateUI()
{
    // 更新接收统计信息
    if (RecvCountLabel) RecvCountLabel->setText(QString("接收帧数: %1").arg(recvCount));
    if (CheckErrLabel) CheckErrLabel->setText(QString("校验错误帧: %1").arg(checkErrCount));
    
    // 更新数据显示
    QMutexLocker locker(&dataMutex);
    if (latestValidData.hasNewData) {
        if (X1_Info) X1_Info->setText(QString::number(latestValidData.X1));
        if (Y1_Info) Y1_Info->setText(QString::number(latestValidData.Y1));
        
        // 如果波形测试开启并且线程在运行，发送数据到测试线程
        if (waveTestThread->isRunning) {
            static int dataCounter = 0;
            waveTestThread->addData(timeCounter, latestValidData.X1, latestValidData.Y1);
            if (++dataCounter % 100 == 0) {  // 每100个数据点输出一次日志
                qDebug() << "[WaveTest] 添加数据点:" << timeCounter 
                         << "X1:" << latestValidData.X1 
                         << "Y1:" << latestValidData.Y1
                         << "计数器:" << dataCounter;
            }
        }
        
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
                    // 显示固定5秒时间窗口，保持数据流畅滚动
                    double displaySpan = qMin(timeSpan, 5.0);
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

void Prt_AF2048::processBufferedData()
{
    // 预留函数，用于处理更复杂的数据缓冲逻辑
    // 如果需要处理粘包或分包情况，可以在这里实现
}

void Prt_AF2048::onStartTestToggled(bool checked)
{
    qDebug() << "[WaveTest] 波形测试状态改变:" << (checked ? "开启" : "关闭");
    if (checked) {
        // 清空之前的数据
        timeData.clear();
        x1Data.clear();
        y1Data.clear();
        timeCounter = 0.0;
        
        waveTestThread->start();
        if (logModel) logModel->prependLogList("开始波形测试");
        qDebug() << "[WaveTest] 波形测试线程已启动";
    } else {
        waveTestThread->stopTest();
        if (logModel) logModel->prependLogList("停止波形测试");
        qDebug() << "[WaveTest] 波形测试线程已停止";
    }
}

void Prt_AF2048::onPeakToPeakUpdated(double x_value, double y_value , double x_mrad , double y_mrad)
{
    peakToPeakValue_X->setText(QString::number(x_value, 'f', 2));
    peakToPeakValue_Y->setText(QString::number(y_value, 'f', 2));
    degreeValue_X->setText(QString::number(x_mrad, 'f', 4));
    degreeValue_Y->setText(QString::number(y_mrad, 'f', 4));
    //读取测试模式
        // "电/20/0.5",    // 电调镜测试 频率20Hz  幅值0.5mrad 码值±626 电压±0.191V     MODEL: 0
        // "电/10/1.0",    // 电调镜测试 频率10Hz  幅值1.0mrad 码值±1252 电压±0.382V    MODEL: 1
        // "电/5/1.5",     // 电调镜测试 频率5Hz   幅值1.5mrad 码值±1878 电压±0.573V    MODEL: 2
        // "电/1/3.0",     // 电调镜测试 频率1Hz   幅值3.0mrad 码值±3756 电压±1.147V    MODEL: 3
        // "快/200/0.5",   // 快反镜测试 频率200Hz 幅值0.5mrad 码值±626 电压±0.191V     MODEL: 4
        // "快/100/1.0",   // 快反镜测试 频率100Hz 幅值1.0mrad 码值±1252 电压±0.382V    MODEL: 5
        // "快/50/1.5",    // 快反镜测试 频率50Hz  幅值1.5mrad 码值±1878 电压±0.573V    MODEL: 6
        // "快/20/3.0"     // 快反镜测试 频率20Hz  幅值3.0mrad 码值±3756 电压±1.147V    MODEL: 7
    //计算超调比例
    double overshootX = 0.0;
    double overshootY = 0.0;

    switch (testModeSelect->currentIndex()) {
        case 0: // 电/20/0.5
            overshootX = (qAbs(x_value - 626.0) / 626.0) * 100.0;
            overshootY = (qAbs(y_value - 626.0) / 626.0) * 100.0;
            break;
        case 1: // 电/10/1.0
            overshootX = (qAbs(x_value - 1252.0) / 1252.0) * 100.0;
            overshootY = (qAbs(y_value - 1252.0) / 1252.0) * 100.0;
            break;
        case 2: // 电/5/1.5
            overshootX = (qAbs(x_value - 1878.0) / 1878.0) * 100.0;
            overshootY = (qAbs(y_value - 1878.0) / 1878.0) * 100.0;
            break;
        case 3: // 电/1/3.0
            overshootX = (qAbs(x_value - 3756.0) / 3756.0) * 100.0;
            overshootY = (qAbs(y_value - 3756.0) / 3756.0) * 100.0;
            break;
        case 4: // 快/200/0.5
            overshootX = (qAbs(x_value - 626.0) / 626.0) * 100.0;
            overshootY = (qAbs(y_value - 626.0) / 626.0) * 100.0;
            break;
        case 5: // 快/100/1.0
            overshootX = (qAbs(x_value - 1252.0) / 1252.0) * 100.0;
            overshootY = (qAbs(y_value - 1252.0) / 1252.0) * 100.0;
            break;
        case 6: // 快/50/1.5
            overshootX = (qAbs(x_value - 1878.0) / 1878.0) * 100.0;
            overshootY = (qAbs(y_value - 1878.0) / 1878.0) * 100.0;
            break;
        case 7: // 快/20/3.0
            overshootX = (qAbs(x_value - 3756.0) / 3756.0) * 100.0;
            overshootY = (qAbs(y_value - 3756.0) / 3756.0) * 100.0;
            break;
    }

    // 设置超调比显示和颜色
    QString overshootXText = QString::number(overshootX, 'f', 2) + "%";
    QString overshootYText = QString::number(overshootY, 'f', 2) + "%";
    
    // X轴超调比显示
    if (overshootX <= 10.0) {
        overshootValue_X->setStyleSheet("color: green;"); // 合格，绿色
    } else {
        overshootValue_X->setStyleSheet("color: red;"); // 不合格，红色
    }
    overshootValue_X->setText(overshootXText);

    // Y轴超调比显示
    if (overshootY <= 10.0) {
        overshootValue_Y->setStyleSheet("color: green;"); // 合格，绿色
    } else {
        overshootValue_Y->setStyleSheet("color: red;"); // 不合格，红色
    }
    overshootValue_Y->setText(overshootYText);

}
