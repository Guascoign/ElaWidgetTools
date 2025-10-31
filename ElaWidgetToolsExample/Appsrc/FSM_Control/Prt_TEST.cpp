/********************************************************************************
    * 文件名称 ：Prt_TEST.c
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2025-06-06
    * 功     能：通信协议 界面
*********************************************************************************/
#include "Prt_TEST.h"
#include "ElaScrollPageArea.h"
#include "ElaComboBox.h"
#include "ElaPushButton.h"
#include "ElaToggleButton.h"
#include "ElaText.h"
#include "ElaLineEdit.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include "ElaListView.h"
#include "ProtocolBaseWidget.h"
#include "../mainwindow.h"

Prt_TEST::Prt_TEST(QWidget *parent)
    : ProtocolBaseWidget(parent)
{
    ElaScrollPageArea* bottomLeftArea_1 = new ElaScrollPageArea(this); // 上
    ElaScrollPageArea* bottomLeftArea_2 = new ElaScrollPageArea(this); // 中
    ElaScrollPageArea* bottomLeftArea_3 = new ElaScrollPageArea(this); // 下
    ElaScrollPageArea* bottomRightArea = new ElaScrollPageArea(this);
    bottomLeftArea_1->setFixedHeight(200);
    bottomLeftArea_1->setFixedWidth(300);
    bottomLeftArea_2->setFixedHeight(210);
    bottomLeftArea_2->setFixedWidth(300);
    bottomLeftArea_3->setFixedHeight(125);
    bottomLeftArea_3->setFixedWidth(300);
    bottomRightArea->setFixedHeight(550);
    //上下布局
    QVBoxLayout* bottomLeftArea = new QVBoxLayout();
    bottomLeftArea->setContentsMargins(0, 0, 0, 0);
    bottomLeftArea->setSpacing(5);
    bottomLeftArea->addWidget(bottomLeftArea_1); // 上
    bottomLeftArea->addWidget(bottomLeftArea_2); // 中
    bottomLeftArea->addWidget(bottomLeftArea_3); // 下
    
    // 下方左右布局
    QHBoxLayout* bottomLayout = new QHBoxLayout();
    bottomLayout->setContentsMargins(0, 0, 0, 0);
    bottomLayout->setSpacing(5);
    bottomLayout->addLayout(bottomLeftArea,3); // 左侧参数设置区
    bottomLayout->addWidget(bottomRightArea,3); // 1:2 比例，可改为1:1

    // 新增：用QWidget包裹bottomLayout
    QWidget* bottomWidget = new QWidget(this);
    bottomWidget->setLayout(bottomLayout);

    // 设置Prt_TEST主布局
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(bottomWidget);
    setLayout(mainLayout);

       Unit_Select = new ElaComboBox(bottomLeftArea_2);    //单位选择
    UMode_Select = new ElaComboBox(bottomLeftArea_2);    //模式选择
    loglist= new ElaListView(bottomRightArea);//操作日志
    logModel = new T_LogModel(this);            // 日志模型
    logModel->setDisplayMode(Default);           // 可选：设置显示模式
    loglist->setModel(logModel);                // 绑定模型
    ElaText* loglist_text = new ElaText("操作日志", bottomRightArea); //操作日志文本
    ElaText* XAxis = new ElaText("X", bottomLeftArea_1); //X轴
    ElaText* YAxis = new ElaText("Y", bottomLeftArea_1); //Y轴
    XAxis_V = new ElaText("N/A V", bottomLeftArea_1); //X轴电压
    YAxis_V = new ElaText("N/A V", bottomLeftArea_1); //Y轴电压
    XAxis_mrad = new ElaText("N/A mrad", bottomLeftArea_1); //X轴角度
    YAxis_mrad = new ElaText("N/A mrad", bottomLeftArea_1); //Y轴角度
    XAxis_deg = new ElaText("N/A °", bottomLeftArea_1); //X轴角度
    YAxis_deg = new ElaText("N/A °", bottomLeftArea_1); //Y轴角度
    ElaText* XAxis_ = new ElaText("->", bottomLeftArea_2); //X占位
    ElaText* YAxis_ = new ElaText("->", bottomLeftArea_2); //Y占位
    X_Start = new ElaLineEdit(bottomLeftArea_2); //X起始位置
    Y_Start = new ElaLineEdit(bottomLeftArea_2); //Y起始位置
    X_End = new ElaLineEdit(bottomLeftArea_2); //X结束位置
    Y_End = new ElaLineEdit(bottomLeftArea_2); //Y结束位置
    X_Move = new ElaLineEdit(bottomLeftArea_3); //X移动位置
    Y_Move = new ElaLineEdit(bottomLeftArea_3); //Y移动位置
    Start_step = new ElaPushButton("生成阶跃", bottomLeftArea_2); //生成阶跃按钮

    Unit_Select->addItems({"电压 -> V", "毫弧度 -> mrad", "角度 -> °"}); // 初始化单位选择
    Unit_Select->setCurrentIndex(0); // 默认选择第一个单位
    UMode_Select->addItems({"双轴模式 MODE=0", "X轴 MODE=1","Y轴 MODE=2","扫描测试 MODE=3"}); // 初始化模式选择
    UMode_Select->setCurrentIndex(0); // 默认选择单次模式


    X_Start->setPlaceholderText("Start");
    Y_Start->setPlaceholderText("Start");
    X_End->setPlaceholderText("End");
    Y_End->setPlaceholderText("End");
    X_Move->setPlaceholderText("目标位置");
    Y_Move->setPlaceholderText("目标位置");

      // ===== bottomLeftArea_1区布局（串口状态、X/Y轴信息、协议选择、指令编辑）=====
    QGridLayout* bottomLeftGrid = new QGridLayout(bottomLeftArea_1);
    bottomLeftGrid->setContentsMargins(4, 4, 4, 4);
    bottomLeftGrid->setSpacing(2);
    // X轴/Y轴标签
    bottomLeftGrid->addWidget(XAxis, 1, 0);
    bottomLeftGrid->addWidget(YAxis, 1, 1);
    // X/Y 电压
    bottomLeftGrid->addWidget(XAxis_V, 2, 0);
    bottomLeftGrid->addWidget(YAxis_V, 2, 1);
    // X/Y mrad
    bottomLeftGrid->addWidget(XAxis_mrad, 3, 0);
    bottomLeftGrid->addWidget(YAxis_mrad, 3, 1);
    // X/Y °
    bottomLeftGrid->addWidget(XAxis_deg, 4, 0);
    bottomLeftGrid->addWidget(YAxis_deg, 4, 1);

    // ===== bot区布局（阶跃测试、移动）=====
    // 阶跃测试分组
    QVBoxLayout* stepLayout = new QVBoxLayout();
    stepLayout->setContentsMargins(8, 8, 8, 8);
    stepLayout->setSpacing(4);

    // 分组标题
    ElaText* stepTitle = new ElaText("阶跃测试", bottomLeftArea_2);
    QFont groupTitleFont;
    groupTitleFont.setPointSize(14);
    groupTitleFont.setBold(true);
    stepTitle->setFont(groupTitleFont);
    stepLayout->addWidget(stepTitle);

    // 单位选择
    QHBoxLayout* unitLayout = new QHBoxLayout();
    unitLayout->addWidget(new ElaText("单位", bottomLeftArea_2));
    unitLayout->addWidget(Unit_Select);
    stepLayout->addLayout(unitLayout);

    // 模式选择
    QHBoxLayout* modeLayout = new QHBoxLayout();
    modeLayout->addWidget(new ElaText("模式", bottomLeftArea_2));
    modeLayout->addWidget(UMode_Select);
    stepLayout->addLayout(modeLayout);

    // X轴阶跃
    QHBoxLayout* xStepLayout = new QHBoxLayout();
    xStepLayout->addWidget(new ElaText("X", bottomLeftArea_2));
    X_Start->setFixedWidth(110);
    xStepLayout->addWidget(X_Start);
    xStepLayout->addWidget(XAxis_);
    X_End->setFixedWidth(110);
    xStepLayout->addWidget(X_End);
    stepLayout->addLayout(xStepLayout);

    // Y轴阶跃
    QHBoxLayout* yStepLayout = new QHBoxLayout();
    yStepLayout->addWidget(new ElaText("Y", bottomLeftArea_2));
    Y_Start->setFixedWidth(110);
    yStepLayout->addWidget(Y_Start);
    yStepLayout->addWidget(YAxis_);
    Y_End->setFixedWidth(110);
    yStepLayout->addWidget(Y_End);
    stepLayout->addLayout(yStepLayout);

    // 生成阶跃按钮
    stepLayout->addWidget(Start_step);

    bottomLeftArea_2->setLayout(stepLayout);

    // 移动分组
    QVBoxLayout* moveLayout = new QVBoxLayout();
    moveLayout->setContentsMargins(8, 8, 8, 8);
    moveLayout->setSpacing(4);

    // 分组标题
    moveTitle = new ElaText("移动", bottomLeftArea_3);
    moveTitle->setFont(groupTitleFont);
    moveLayout->addWidget(moveTitle);

    // X轴目标位置
    QHBoxLayout* xMoveLayout = new QHBoxLayout();
    X_Move_Text = new ElaText("X轴目标位置:", bottomLeftArea_3);
    xMoveLayout->addWidget(X_Move_Text);
    X_Move->setFixedWidth(180);
    xMoveLayout->addWidget(X_Move);
    moveLayout->addLayout(xMoveLayout);

    // Y轴目标位置
    QHBoxLayout* yMoveLayout = new QHBoxLayout();
    Y_Move_Text = new ElaText("Y轴目标位置:", bottomLeftArea_3);
    yMoveLayout->addWidget(Y_Move_Text);
    Y_Move->setFixedWidth(180);
    yMoveLayout->addWidget(Y_Move);
    moveLayout->addLayout(yMoveLayout);

    // 移动按钮
    QHBoxLayout* moveBtnLayout = new QHBoxLayout();
    moveBtn = new ElaPushButton("移动", bottomLeftArea_3);
    Start_scan = new ElaToggleButton("开始扫描", bottomLeftArea_3);
    Start_scan->hide(); // 初始隐藏扫描按钮
    moveBtnLayout->addWidget(moveBtn);
    moveBtnLayout->addWidget(Start_scan);
    moveLayout->addLayout(moveBtnLayout);

    bottomLeftArea_3->setLayout(moveLayout); // 只设置一次

    // 日志区布局
    QVBoxLayout* rightLayout = new QVBoxLayout(bottomRightArea);
    rightLayout->setContentsMargins(4, 4, 4, 4);
    rightLayout->setSpacing(4);
    rightLayout->addWidget(loglist_text);
    rightLayout->addWidget(loglist);

    // 优化字体大小
    QFont labelFont;
    loglist_text->setFont(labelFont);

    QFont axisFont;
    axisFont.setPointSize(13); // X/Y轴标签
    XAxis->setFont(axisFont);
    YAxis->setFont(axisFont);

     QFont valueFont;
    valueFont.setPointSize(13); // 数值显示
    XAxis_V->setFont(valueFont);
    YAxis_V->setFont(valueFont);
    XAxis_mrad->setFont(valueFont);
    YAxis_mrad->setFont(valueFont);
    XAxis_deg->setFont(valueFont);
    YAxis_deg->setFont(valueFont);

    QFont groupFont;
    groupFont.setPointSize(13); // 阶跃测试、移动分组字体
    bottomLeftArea_2->setFont(groupFont);
    bottomLeftArea_3->setFont(groupFont);

    // 阶跃和移动内标签字体
    QFont innerLabelFont;
    innerLabelFont.setPointSize(12);
    for (auto* w : bottomLeftArea_2->findChildren<ElaText*>()) w->setFont(innerLabelFont);
    for (auto* w : bottomLeftArea_3->findChildren<ElaText*>()) w->setFont(innerLabelFont);

    // 数值输入框高度和间距调整，使更密集
    int denseHeight = 24;
    int denseSpacing = 5;
    X_Start->setFixedHeight(denseHeight);
    X_End->setFixedHeight(denseHeight);
    Y_Start->setFixedHeight(denseHeight);
    Y_End->setFixedHeight(denseHeight);
    X_Move->setFixedHeight(denseHeight);
    Y_Move->setFixedHeight(denseHeight);

    //combobox高度调整
    int comboBoxWidth = 180;
    Unit_Select->setFixedWidth(comboBoxWidth);
    UMode_Select->setFixedWidth(comboBoxWidth);

    // 阶跃测试和移动区的布局间距调小
    stepLayout->setSpacing(denseSpacing);
    moveLayout->setSpacing(denseSpacing);

    // X/Y轴阶跃行间距调小
    xStepLayout->setSpacing(denseSpacing);
    yStepLayout->setSpacing(denseSpacing);

    // 单位、模式行间距调小
    unitLayout->setSpacing(denseSpacing);
    modeLayout->setSpacing(denseSpacing);

    // 移动区行间距调小
    xMoveLayout->setSpacing(denseSpacing);
    yMoveLayout->setSpacing(denseSpacing);

    //默认lineEdit参数
    X_Start->setText("0.0");
    Y_Start->setText("0.0");
    X_End->setText("1.0");
    Y_End->setText("1.0");
    X_Move->setText("0.5");
    Y_Move->setText("0.5");
    //默认禁用
    Start_step->setEnabled(false);
    moveBtn->setEnabled(false);
    Start_scan->setEnabled(false);

    scanTimer = new QTimer(this);
    scanTimer->setInterval(1000 / 100); // 120Hz
    scanTimer->setTimerType(Qt::PreciseTimer); // 高精度
    connect(scanTimer, &QTimer::timeout, this, &Prt_TEST::on_Move_pushButton_clicked);
    //connect(Start_scan, &QPushButton::clicked, this, &Prt_TEST::onStartScanClicked);

    // 单位切换信号槽
    connect(Unit_Select, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &Prt_TEST::onUnitChanged);
     // 模式切换信号槽
    connect(UMode_Select, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &Prt_TEST::onModeChanged);

    connect(Start_step , &ElaPushButton::clicked, this, &Prt_TEST::on_Start_step_pushButton_clicked);
    connect(moveBtn, &ElaPushButton::clicked, this, &Prt_TEST::on_Move_pushButton_clicked);

    logModel->appendLogList(QString("日志记录已开启"));
}

Prt_TEST::~Prt_TEST()
{
}

void Prt_TEST::COM_Send_data(uint8_t *data, uint8_t len)
{
    QByteArray sendArray(reinterpret_cast<const char*>(data), len);
    emit Proctol_out(sendArray); // 发射信号
}

void Prt_TEST::SerialConnected(bool connected)
{
    if (connected) {
        // 串口连接成功时，启用相关按钮
        Start_step->setEnabled(true);
        moveBtn->setEnabled(true);
        Start_scan->setEnabled(true);
    } else {
        // 串口断开时，禁用相关按钮
        Start_step->setEnabled(false);
        moveBtn->setEnabled(false);
        Start_scan->setEnabled(false);
    }
}

void Prt_TEST::SerialDataReceived(QByteArray data)
{
    // 解析接收到的数据
    Proctol_Analysis(data);
}

//生成协议
void Prt_TEST::Creat_data(uint8_t *data,uint8_t mode, uint16_t X , uint16_t Y)
{
            data[0] = 0x22; // 起始符
            data[1] = 0x33; // 协议标识
            data[2] = mode; // 模式
            data[3] = (X >> 8) & 0xFF; // X高字节
            data[4] = X & 0xFF; // X低字节
            data[5] = (Y >> 8) & 0xFF; // Y高字节
            data[6] = Y & 0xFF; // Y低字节
            data[7] = xor_check(data + 2, 6); // 异或校验
            data[8] = 0x23; // 结束符
}

void Prt_TEST::on_Move_pushButton_clicked()
{
    int modeIndex = UMode_Select->currentIndex();
    if (modeIndex == 0 || modeIndex == 1 || modeIndex == 2) {
        int unitIndex = Unit_Select->currentIndex();
        float X_move = X_Move->text().toFloat();
        float Y_move = Y_Move->text().toFloat();
        // 先转换为V
        auto toV = [](float val, int unit) {
            if (unit == 0) return val; // V
            if (unit == 1) return val * 10.0f / 1.5f * 180.0f / static_cast<float>(M_PI) / 1000.0f; // mrad -> V
            if (unit == 2) return val * 10.0f / 1.5f; // angle -> V
            return val;
        };
        float X_move_V = toV(X_move, unitIndex);
        float Y_move_V = toV(Y_move, unitIndex);
        // 限幅检测
        if(X_move_V < -10.0f || X_move_V > 10.0f || Y_move_V < -10.0f || Y_move_V > 10.0f)
        {
            qDebug() << "Out of range";
            return;
        }
        // float转int16
        int16_t X_move_int = static_cast<int16_t>(X_move_V / 10.0f * 32767);
        int16_t Y_move_int = 0.00f; //static_cast<int16_t>(Y_move_V / 10.0f * 32767);
        uint8_t data[9];
        Creat_data(data, 0x00, X_move_int, Y_move_int); //复位
        COM_Send_data(data, 9); //发送数据
        // 记录日志
        if (loglist) logModel->appendLogList(QString("移动 X->%1 Y->%2").arg(X_move).arg(Y_move));
    }
    else if (modeIndex == 3) {
        float X_speed = X_Move->text().toFloat();
        float Y_speed = Y_Move->text().toFloat();
        // 限幅检测 速度50-180  -> 幅值电压200mV-720mV(峰峰值) 小于50无效不动作
        // 50 -> 0~+100mV~0~-100mV~0    180 -> +360mV~0~+360mV~0  -50-> 0~-100mV~0~+100mV~0 -180 -> -360mV~0~-360mV~0
        if(X_speed > 180.0f) //||  Y_speed > 180.0f)
        {
            qDebug() << "Out of range";
            return;
        }
        // float转uint16  0x00 -> 50.0  0xFFFF -> 180.0
        int16_t X_speed_int = static_cast<int16_t>(X_speed / 180.0f * 32767.0f);
        int16_t Y_speed_int = static_cast<int16_t>(Y_speed / 180.0f * 32767.0f);
        uint8_t data[9];
        //速度模式
        Creat_data(data, 0x03, X_speed_int, Y_speed_int); //复位
        COM_Send_data(data, 9); //发送数据
        // 记录日志
        if (loglist) logModel->appendLogList(QString("扫描速度 X->%1 频率->%2").arg(X_speed).arg(Y_speed));
    }


}


//协议解析
void Prt_TEST::Proctol_Analysis(const QByteArray &data)
{
    // 协议格式: data[0]=0x22, data[1]=0x33, data[2]=mode, data[3]=XH, data[4]=XL, data[5]=YH, data[6]=YL, data[7]=xor, data[8]=0x23
    if (data.size() < 9) return;
    const uint8_t *buf = reinterpret_cast<const uint8_t*>(data.constData());
    if (buf[0] != 0x22 || buf[1] != 0x33 || buf[8] != 0x23) return;
    uint8_t xor_data = 0;
    for (int i = 2; i <= 6; ++i) xor_data ^= buf[i];
    if (xor_data != buf[7]) return;

    int16_t x = (int16_t)((buf[3] << 8) | buf[4]);
    int16_t y = (int16_t)((buf[5] << 8) | buf[6]);
    float x_val = x / 32767.0f * 10.0f;
    float y_val = y / 32767.0f * 10.0f;
    XAxis_V->setText(QString::number(x_val, 'f', 6) + "V");
    YAxis_V->setText(QString::number(y_val, 'f', 6) + "V");
    //转换成角度±10V x_val 对应 ±1.5°
    float x_angle = x_val / 10.0f * 1.5f;
    float y_angle = y_val / 10.0f * 1.5f;
    XAxis_deg->setText(QString::number(x_angle, 'f', 6) + "°");
    YAxis_deg->setText(QString::number(y_angle, 'f', 6) + "°");
    //转换成弧度(毫弧度)
    float x_rad = x_angle * M_PI / 180.0f * 1000.0f;
    float y_rad = y_angle * M_PI / 180.0f * 1000.0f;
    XAxis_mrad->setText(QString::number(x_rad, 'f', 3) + "mrad");
    YAxis_mrad->setText(QString::number(y_rad, 'f', 3) + "mrad");
}
void Prt_TEST::onStartScanClicked()
{
    //频率0 退出
    if(Y_Move->text().toInt() == 0)
    {
        scanTimer->stop();
        Start_scan->setText("开始");
        return;
    }
    if (!scanTimer->isActive()) {
        // 仅在模式3下动态设置定时器频率
        if (step_mode_comboBox->currentIndex() == 3) {
            bool ok = false;
            int freq = Y_Move->text().toInt(&ok);
            if (ok && freq > 0) {
                scanTimer->stop();
                scanTimer->setInterval(1000 / freq);
            }
        }
        scanTimer->start();
        Start_scan->setText("关闭");
    } else {
        scanTimer->stop();
        Start_scan->setText("开始");
    }
}

void Prt_TEST::onModeChanged(int index)
{   
    // 0->双轴 1->单X轴 2->单Y轴 3->速度
        // 切换前保存
    if (Last_mode >= 0 && Last_mode <= 2) {
        Pos_cache[0] = X_Move->text().toFloat();
        Pos_cache[1] = Y_Move->text().toFloat();
    } else if (Last_mode == 3) {
        Speed_cache[0] = X_Move->text().toFloat();
        Speed_cache[1] = Y_Move->text().toFloat();
    }

    if (index == 0) {
        X_Start->setEnabled(true);
        X_End->setEnabled(true);
        Y_Start->setEnabled(true);
        Y_End->setEnabled(true);
        Start_step->setEnabled(true);
        Unit_Select->setEnabled(true);
        X_Move->setEnabled(true);
        Y_Move->setEnabled(true);
        moveTitle->setText("移动");
        X_Move_Text->setText("X轴位置");
        Y_Move_Text->setText("Y轴位置");
        moveBtn->setText("移动");
        Start_scan->hide();//隐藏扫描按钮
    } else if (index == 1) {
        X_Start->setEnabled(true);
        X_End->setEnabled(true);
        Y_Start->setEnabled(false);
        Y_End->setEnabled(false);
        Start_step->setEnabled(true);
        Unit_Select->setEnabled(true);
        X_Move->setEnabled(true);
        Y_Move->setEnabled(false);
        moveTitle->setText("移动");
        X_Move_Text->setText("X轴位置");
        Y_Move_Text->setText("Y轴位置");
        moveBtn->setText("移动");
        Start_scan->hide();//隐藏扫描按钮
    } else if (index == 2) {
        X_Start->setEnabled(false);
        X_End->setEnabled(false);
        Y_Start->setEnabled(true);
        Y_End->setEnabled(true);
        Start_step->setEnabled(true);
        Unit_Select->setEnabled(true);
        X_Move->setEnabled(false);
        Y_Move->setEnabled(true);
        moveTitle->setText("移动");
        X_Move_Text->setText("X轴位置");
        Y_Move_Text->setText("Y轴位置");
        moveBtn->setText("移动");
        Start_scan->hide();//隐藏扫描按钮
    }
    else if(index == 3) {
        X_Start->setEnabled(false);
        X_End->setEnabled(false);
        Y_Start->setEnabled(false);
        Y_End->setEnabled(false);
        Start_step->setEnabled(false);
        Unit_Select->setEnabled(false);
        X_Move->setEnabled(true);
        Y_Move->setEnabled(true);
        //groupBox_2文本修改
        moveTitle->setText("扫描速度(±180)");
        X_Move_Text->setText("X轴速度");
        Y_Move_Text->setText("发送频率");
        moveBtn->setText("触发波形");
        Start_scan->show();//显示扫描按钮
    }
        // 切换后恢复
    if (index >= 0 && index <= 2) {
        X_Move->setText(QString::number(Pos_cache[0]));
        Y_Move->setText(QString::number(Pos_cache[1]));
    } else if (index == 3) {
        X_Move->setText(QString::number(Speed_cache[0]));
        Y_Move->setText(QString::number(Speed_cache[1]));
    }
    Last_mode = index; // 更新上一个模式
}

void Prt_TEST::onUnitChanged(int index)
{
    // 获取当前参数
    auto getVal = [](QLineEdit* edit) {
        return edit->text().toFloat();
    };
    float x1 = getVal(X_Start);
    float x2 = getVal(X_End);
    float y1 = getVal(Y_Start);
    float y2 = getVal(Y_End);
    float xm = getVal(X_Move);
    float ym = getVal(Y_Move);

    // 先全部转为V
    auto toV = [](float val, int unit) {
        if (unit == 0) return val; // V
        if (unit == 1) return static_cast<float>(val * 10.0f / 1.5f * 180.0f / M_PI / 1000.0f); // mrad -> V
        if (unit == 2) return val * 10.0f / 1.5f; // angle -> V
        return val;
    };
    x1 = toV(x1, lastUnitIndex);
    x2 = toV(x2, lastUnitIndex);
    y1 = toV(y1, lastUnitIndex);
    y2 = toV(y2, lastUnitIndex);
    xm = toV(xm, lastUnitIndex);
    ym = toV(ym, lastUnitIndex);

    // 再从V转为目标单位
    auto fromV = [](float val, int unit) {
        if (unit == 0) return val; // V
        if (unit == 1) return val / 10.0f * 1.5f * static_cast<float>(M_PI) / 180.0f * 1000.0f; // V -> mrad
        if (unit == 2) return val / 10.0f * 1.5f; // V -> angle
        return val;
    };
    x1 = fromV(x1, index);
    x2 = fromV(x2, index);
    y1 = fromV(y1, index);
    y2 = fromV(y2, index);
    xm = fromV(xm, index);
    ym = fromV(ym, index);

    // 更新界面
    auto setVal = [index](QLineEdit* edit, float val) {
        int precision = 6;
        if (index == 1) precision = 3; // mrad 3位
        else if (index == 2) precision = 5; // angle 1位
        else if (index == 0) precision = 5; // V 6位
        edit->setText(QString::number(val, 'f', precision));
    };
    setVal(X_Start, x1);
    setVal(X_End, x2);
    setVal(Y_Start, y1);
    setVal(Y_End, y2);
    setVal(X_Move, xm);
    setVal(Y_Move, ym);

    lastUnitIndex = index; // 更新上一次单位索引
}

uint8_t Prt_TEST::xor_check(uint8_t *data, uint8_t len)
{
    uint8_t xor_data = 0;
    for (uint8_t i = 0; i < len; i++)
    {
        xor_data ^= data[i];
    }
    return xor_data;
}


void Prt_TEST::on_Start_step_pushButton_clicked()
{
    // 获取当前单位索引
    int unitIndex = Unit_Select->currentIndex();

    // 获取阶跃测试参数
    float X_step_start = X_Start->text().toFloat();
    float X_step_end = X_End->text().toFloat();
    float Y_step_start = Y_Start->text().toFloat();
    float Y_step_end = Y_End->text().toFloat();

    // 先转换为V
    auto toV = [](float val, int unit) {
        if (unit == 0) return val; // V
        if (unit == 1) return val * 10.0f / 1.5f * 180.0f / static_cast<float>(M_PI) / 1000.0f; // mrad -> V
        if (unit == 2) return val * 10.0f / 1.5f; // angle -> V
        return val;
    };
    float X_step_start_V = toV(X_step_start, unitIndex);
    float X_step_end_V = toV(X_step_end, unitIndex);
    float Y_step_start_V = toV(Y_step_start, unitIndex);
    float Y_step_end_V = toV(Y_step_end, unitIndex);

    // 限幅检测
    if(X_step_start_V < -10.0f || X_step_start_V > 10.0f || X_step_end_V < -10.0f || X_step_end_V > 10.0f
        || Y_step_start_V < -10.0f || Y_step_start_V > 10.0f || Y_step_end_V < -10.0f || Y_step_end_V > 10.0f)
    {
        qDebug() << "Out of range";
        return;
    }

    // float转int16
    int16_t X_step_start_int = static_cast<int16_t>(X_step_start_V / 10.0f * 32767);
    int16_t X_step_end_int = static_cast<int16_t>(X_step_end_V / 10.0f * 32767);
    int16_t Y_step_start_int = static_cast<int16_t>(Y_step_start_V / 10.0f * 32767);
    int16_t Y_step_end_int = static_cast<int16_t>(Y_step_end_V / 10.0f * 32767);

    uint8_t data[9];
    // UMode_Select判断轴数量 0->双轴 1->单X轴 2->单Y轴
    int8_t axis_num = UMode_Select->currentIndex();

    if(Step_test_flag)//阶跃已触发 点击归位
    {
        Creat_data(data, axis_num, X_step_start_int, Y_step_start_int); //复位
        COM_Send_data(data, 9); //发送数据

        // 记录日志
        if (loglist) logModel->appendLogList(QString("复位阶跃 X->%1 Y->%2").arg(X_step_start).arg(Y_step_start));
        //复位阶跃测试
        Step_test_flag = false;
        Start_step->setText("生成阶跃");
        X_Start->setEnabled(false);
        X_End->setEnabled(true);
        Y_Start->setEnabled(false);
        Y_End->setEnabled(true);
        return;
    }
    else{
        Creat_data(data, axis_num, X_step_end_int, Y_step_end_int); //移动
        COM_Send_data(data, 9); //发送数据
        // 记录日志
        if (loglist) logModel->appendLogList(QString("生成阶跃 X->%1 Y->%2").arg(X_step_end).arg(Y_step_end));
        //设置阶跃测试标志
        Step_test_flag = true;
        //修改文本 复位阶跃
        Start_step->setText("复位阶跃");
        X_Start->setEnabled(true);
        X_End->setEnabled(false);
        Y_Start->setEnabled(true);
        Y_End->setEnabled(false);
    }
}
