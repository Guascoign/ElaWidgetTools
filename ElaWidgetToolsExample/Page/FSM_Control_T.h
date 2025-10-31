#ifndef FSM_CONTROL_T_H
#define FSM_CONTROL_T_H

#include "T_BasePage.h"
#include <QTimer>
#include "Uart_T.h"
#include "T_LogModel.h"
#include "FSM_Control/Prt_TEST.h"
#include "FSM_Control/Prt_FSM7050_2.h"
#include "FSM_Control/Prt_FSM7050_UART1.h"
#include "FSM_Control/Prt_FSM7050_UART2.h"
#include "FSM_Control/Prt_FSM50.h"
#include "FSM_Control/Prt_AF2048.h"
#include "FSM_Control/Prt_DIG_Test.h"
#include "FSM_Control/Prt_Fujirui.h"
#include "FSM_Control/Prt_DIG45_Test.h"
#include "FSM_Control/Prt_55J.h"


class ElaComboBox;
class ElaPushButton;
class ElaToggleButton;
class ElaSpinBox;
class ElaText;
class ElaLineEdit;
class ElaListView;
class ElamessageBar;

class FSM_Control_T : public T_BasePage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit FSM_Control_T(QWidget* parent = nullptr);
    ~FSM_Control_T();
    void onSerialDataReceived(QByteArray data); // 串口接收数据槽
    void setupUartConnections(Uart_T* p_uartObj);    // 设置串口连接
private slots:
    void COM_Send_data(const QByteArray &data); // 发送数据槽
signals:
    void SerialConnected(bool connected); // 串口连接状态信号
    void SerialDataReceived(QByteArray data); // 串口数据接收信号 发送到协议测试界面
    void sendSerialData(QByteArray data); //发送数据信号
private:
    /*******************界面元素**********************************/
    ElaComboBox* proctol = nullptr; // 协议选择
    ElaToggleButton* proctolToggle = nullptr; // 协议选择开关
    ElaLineEdit* proctolout = nullptr; // 协议发送
    ElaText* portLabel_Now = nullptr; // 当前串口显示
    /*******************界面变量**********************************/
    bool Com_Opened_flag = false; // 串口打开标志
    Uart_T* uartObj = nullptr; // 串口对象指针
    Prt_TEST* prtTestWidget = nullptr;  // 协议测试界面
    Prt_FSM7050_2* prtFSM70502Widget = nullptr; // FSM7050协议界面
    Prt_FSM7050_UART1* prtFSM7050_UART1Widget = nullptr; // FSM7050 UART1协议界面
    Prt_FSM7050_UART2* prtFSM7050_UART2Widget = nullptr; // FSM7050 UART2协议界面
    Prt_FSM50* prtFSM50Widget = nullptr; // FSM50协议界面
    Prt_AF2048* prtAF2048Widget = nullptr; // AF2048协议界面
    Prt_DIG_Test* prtDIG_TestWidget = nullptr; // 数字闭环测试协议界面
    Prt_Fujirui* prtFujiruiWidget = nullptr; // 富吉睿协议界面
    Prt_DIG45_Test* prtDIG45_Test_Widget = nullptr; // 数字闭环测试-45°协议界面
    Prt_55J* prt55J_Widget = nullptr; // 55J-欧普笛协议界面
    QWidget* currentProtocolWidget = nullptr; // 当前协议对应的界面
};

#endif // FSM_CONTROL_T_H