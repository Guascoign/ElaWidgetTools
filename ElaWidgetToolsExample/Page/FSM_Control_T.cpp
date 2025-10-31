#include "FSM_Control_T.h"
#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QTimer>
#include "ElaApplication.h"
#include "ElaWindow.h"
#include "ElaComboBox.h"
#include "ElaPushButton.h"
#include "ElaToggleButton.h"
#include "ElaScrollPageArea.h"
#include "ElaSpinBox.h"
#include "ElaText.h"
#include "ElaListView.h"
#include "ElaLineEdit.h"
#include <Qgroupbox.h>
#include "ElaMessageBar.h"
#include "../mainwindow.h"

FSM_Control_T::FSM_Control_T(QWidget* parent)
    : T_BasePage(parent)
{
    ElaWindow* window = dynamic_cast<ElaWindow*>(parent);
    //setWindowTitle("FSM Control123");
    setTitleVisible(false);// 隐藏标题栏
    // createCustomWidget("FSM 控制");
    // 主体水平布局

    // 三个区 上部串口连接 协议选择 下左参数设置 下右操作记录+图表
    ElaScrollPageArea* topArea = new ElaScrollPageArea(this);
    
    topArea->setFixedHeight(110);
    topArea->setFixedWidth(420);
   
    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("FSM Control");
    QVBoxLayout* centerLayout = new QVBoxLayout(centralWidget);

    centerLayout->addWidget(topArea);
    //centerLayout->addLayout(bottomLayout);
    centerLayout->setContentsMargins(0, 0, 0, 0);
    addCentralWidget(centralWidget, true, true, 0);

    ElaText* portLabel = new ElaText("当前串口:", topArea); //串口显示
    portLabel->setFixedWidth(100); // 设置宽度以适应文本
    portLabel_Now = new ElaText("N/A", topArea); //串口显示
    portLabel_Now->setFixedWidth(200); // 设置宽度以适应文本
    ElaText* proctol_text = new ElaText("当前协议:", topArea); //当前协议
    proctol_text->setFixedWidth(100); // 设置宽度以适应文本
    ElaText* proctolsend_text = new ElaText("协议发送:", topArea); //协议发送
    proctolsend_text->setFixedWidth(100); // 设置宽度以适应文本
    proctol = new ElaComboBox(topArea);    //协议选择
    proctolToggle = new ElaToggleButton("协议连接", topArea); //串口连接开关 连接发送和接收emit到串口模块
    proctolout = new ElaLineEdit(topArea); //发送的指令
 
    //初始化内容
    proctol->addItems({
        "Test Demo" ,
        "FSM7050-一拖二",
        "FSM7050-一拖一-UART1",
        "FSM7050-一拖一-UART2",
        "FSM50-安光所",
        "AF2048",
        "数字闭环测试",
        "富吉睿(配合测试版本)",
        "数字闭环测试-45°",
        "55J-欧普笛",
        }); // 初始化协议文本
    proctol->setCurrentIndex(0); 
    proctolout->setPlaceholderText("输出指令...");

    //默认禁用
    proctolToggle->setEnabled(false);

    // ===== topArea区布局（串口状态、协议选择、指令编辑）=====
    // 使用 QGridLayout 实现左标签右控件紧贴且整体靠左
    QGridLayout* topGrid = new QGridLayout(topArea);
    topGrid->setContentsMargins(4, 4, 4, 4);
    topGrid->setHorizontalSpacing(1); // 控制左右间距
    topGrid->setVerticalSpacing(1);

    // 第一行：串口
    topGrid->addWidget(portLabel,      0, 0, 1, 1,  Qt::AlignLeft);
    topGrid->addWidget(portLabel_Now,  0, 1, 1, 1,  Qt::AlignLeft);
    topGrid->addWidget(proctolToggle,  0, 2, 1, 1,  Qt::AlignLeft);

    // 第二行：协议
    topGrid->addWidget(proctol_text,   1, 0, 1, 1,  Qt::AlignLeft);
    topGrid->addWidget(proctol,        1, 1, 1, 2,  Qt::AlignLeft);

    // 第三行：协议发送
    topGrid->addWidget(proctolsend_text, 2, 0, 1, 1,  Qt::AlignLeft);
    topGrid->addWidget(proctolout,       2, 1, 1, 2,  Qt::AlignHCenter);
    proctolout->setFixedWidth(233); // 设置高度以适应文本
    proctol->setFixedWidth(245);

    // 右侧弹簧，内容整体靠左
    topGrid->setColumnStretch(3, 1);

    // 优化字体大小
    QFont labelFont;
    QFont labelFont1;
    labelFont.setPointSize(14); // 串口状态、协议等主标签
    labelFont1.setPointSize(7); // 协议发送、操作日志等次级标签
    portLabel->setFont(labelFont);
    portLabel_Now->setFont(labelFont1);
    proctol_text->setFont(labelFont);
    proctolsend_text->setFont(labelFont);
    proctolsend_text->setFixedWidth(300);

    // 添加协议界面
    //Test Demo -> 0
    prtTestWidget = new Prt_TEST(centralWidget);
    prtTestWidget->hide();
    centerLayout->addWidget(prtTestWidget);
    //FSM7050-一拖二 -> 1
    prtFSM70502Widget = new Prt_FSM7050_2(centralWidget);
    prtFSM70502Widget->hide();
    centerLayout->addWidget(prtFSM70502Widget);
    //FSM7050-一拖一-UART1 -> 2
    prtFSM7050_UART1Widget = new Prt_FSM7050_UART1(centralWidget);
    prtFSM7050_UART1Widget->hide();
    centerLayout->addWidget(prtFSM7050_UART1Widget);
    //FSM7050-一拖一-UART2 -> 3
    prtFSM7050_UART2Widget = new Prt_FSM7050_UART2(centralWidget);
    prtFSM7050_UART2Widget->hide();
    centerLayout->addWidget(prtFSM7050_UART2Widget);
    //FSM50-安光所 -> 4
    prtFSM50Widget = new Prt_FSM50(centralWidget);
    prtFSM50Widget->hide();
    centerLayout->addWidget(prtFSM50Widget);
    //AF2048 -> 5
    prtAF2048Widget = new Prt_AF2048(centralWidget);
    prtAF2048Widget->hide();
    centerLayout->addWidget(prtAF2048Widget);
    //数字闭环测试 -> 6
    prtDIG_TestWidget = new Prt_DIG_Test(centralWidget);
    prtDIG_TestWidget->hide();
    centerLayout->addWidget(prtDIG_TestWidget);
    //富吉睿(配合测试版本) -> 7
    prtFujiruiWidget = new Prt_Fujirui(centralWidget);
    prtFujiruiWidget->hide();
    centerLayout->addWidget(prtFujiruiWidget);
    //数字闭环测试-45° -> 8
    prtDIG45_Test_Widget = new Prt_DIG45_Test(centralWidget);
    prtDIG45_Test_Widget->hide();
    centerLayout->addWidget(prtDIG45_Test_Widget);
    //55J-欧普笛 -> 9
    prt55J_Widget = new Prt_55J(centralWidget);
    prt55J_Widget->hide();
    centerLayout->addWidget(prt55J_Widget);


    // 弹簧
    centerLayout->addStretch();

    // 默认显示第一个协议界面
    currentProtocolWidget = prtTestWidget;
    currentProtocolWidget->show();
    //设置初始连接
    connect(this, SIGNAL(SerialConnected(bool)), currentProtocolWidget, SLOT(SerialConnected(bool)));
    connect(this, SIGNAL(SerialDataReceived(QByteArray)), currentProtocolWidget, SLOT(SerialDataReceived(QByteArray)));
    connect(currentProtocolWidget, SIGNAL(Proctol_out(QByteArray)), this, SLOT(COM_Send_data(QByteArray)));
    // 协议切换信号槽
    connect(proctol, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index){
        // 1. 断开上一个协议界面的信号槽
        if (currentProtocolWidget) {
            // 断开串口连接信号
            disconnect(this, &FSM_Control_T::SerialConnected, currentProtocolWidget, nullptr);
            // 断开串口数据信号
            disconnect(this, &FSM_Control_T::SerialDataReceived, currentProtocolWidget, nullptr);
            // 断开协议界面向外的协议解析信号
            disconnect(currentProtocolWidget, SIGNAL(Proctol_out(QByteArray)), this, SLOT(COM_Send_data(QByteArray)));
            currentProtocolWidget->hide();
            qDebug() << "[FSM_Control_T] Protocol widget disconnected and hidden.";
        }
        // 2. 切换协议界面
        switch(index) {
            case 0: // Test Demo
                currentProtocolWidget = prtTestWidget;
                break;
            case 1: // FSM7050-一拖二
                currentProtocolWidget = prtFSM70502Widget;
                break;
            case 2: // FSM7050-一拖一-UART1
                currentProtocolWidget = prtFSM7050_UART1Widget;
                break;
            case 3: // FSM7050-一拖一-UART2
                currentProtocolWidget = prtFSM7050_UART2Widget;
                break;
            case 4: // FSM50-安光所
                currentProtocolWidget = prtFSM50Widget;
                break;
            case 5: // AF2048
                currentProtocolWidget = prtAF2048Widget;
                break;
            case 6: // 数字闭环测试
                currentProtocolWidget = prtDIG_TestWidget;
                break;
            case 7: // 富吉睿(配合测试版本)
                currentProtocolWidget = prtFujiruiWidget;
                break;
            case 8: 
                currentProtocolWidget = prtDIG45_Test_Widget;
                break;
            case 9: 
                currentProtocolWidget = prt55J_Widget;
                break;
            default:
                currentProtocolWidget = nullptr;
                break;
        }
        // 3. 连接当前协议界面的信号槽
        if (currentProtocolWidget) {
            // 连接串口连接状态信号
            connect(this, SIGNAL(SerialConnected(bool)), currentProtocolWidget, SLOT(SerialConnected(bool)));
            // 连接串口数据接收信号
            connect(this, SIGNAL(SerialDataReceived(QByteArray)), currentProtocolWidget, SLOT(SerialDataReceived(QByteArray)));
            // 连接协议界面向外的协议解析信号
            connect(currentProtocolWidget, SIGNAL(Proctol_out(QByteArray)), this, SLOT(COM_Send_data(QByteArray)));
            currentProtocolWidget->show();
            qDebug() << "[FSM_Control_T] Protocol widget connected and shown:" << currentProtocolWidget->metaObject()->className();
        }
    });

}

FSM_Control_T::~FSM_Control_T()
{
}

void FSM_Control_T::setupUartConnections(Uart_T* p_uartObj)
{
    this->uartObj = p_uartObj;
    Uart_T *pUart = this->uartObj;
    FSM_Control_T *pFsmControl = this;

    // 连接串口打开信号
    connect(pUart, &Uart_T::Com_Opened, this, [=](const QString &portName){
        qDebug() << "[FSM_Control_T] Serial port opened:" << portName;
        portLabel_Now->setText(portName);
        Com_Opened_flag = true;
        proctolToggle->setEnabled(true);
    });

    // 连接串口关闭信号
    connect(pUart, &Uart_T::Com_Closed, this, [=](){
        qDebug() << "[FSM_Control_T] Serial port closed";
        portLabel_Now->setText("N/A");
        Com_Opened_flag = false;
        proctolToggle->setEnabled(false);
        // 断开协议信号连接（如果还连着）
        disconnect(pFsmControl, &FSM_Control_T::sendSerialData, pUart, &Uart_T::Send_Data);
        disconnect(pUart, &Uart_T::RecvData, pFsmControl, &FSM_Control_T::onSerialDataReceived);
    });

    // 协议连接按钮控制协议信号与串口信号的连接
    connect(proctolToggle, &ElaToggleButton::toggled, this, [=](bool checked){
        if (checked) {
            // 只在串口已打开时连接
            if (pUart) {
                connect(pFsmControl, &FSM_Control_T::sendSerialData, pUart, &Uart_T::Send_Data, Qt::UniqueConnection);
                connect(pUart, &Uart_T::RecvData, pFsmControl, &FSM_Control_T::onSerialDataReceived, Qt::UniqueConnection);
                //禁用协议选择 信息发送按钮等输出
                proctol->setEnabled(false);
                emit SerialConnected(true); // 发射串口连接信号
                ElaMessageBar::success(ElaMessageBarType::TopRight, "输出已连接", "Done！", 1500);
            }
        } else {
            disconnect(pFsmControl, &FSM_Control_T::sendSerialData, pUart, &Uart_T::Send_Data);
            disconnect(pUart, &Uart_T::RecvData, pFsmControl, &FSM_Control_T::onSerialDataReceived);
            proctol->setEnabled(true);
            emit SerialConnected(false); // 发射串口断开信号
            ElaMessageBar::success(ElaMessageBarType::TopRight, "输出已断开", "Done！", 1500);
        }
    });
}

void FSM_Control_T::onSerialDataReceived(QByteArray data)
{
    //传递数据到协议测试界面进行解析
    if (currentProtocolWidget) {
        emit SerialDataReceived(data); // 发射信号到协议测试界面
    } else {
        qDebug() << "[FSM_Control_T] No current protocol widget to handle data.";
    }
}

void FSM_Control_T::COM_Send_data(const QByteArray &data)
{
    // 更新指令到proctolout
    proctolout->setText(QString("0x%1").arg(data.toHex().toUpper()));
    emit sendSerialData(data); // 发射信号
}
