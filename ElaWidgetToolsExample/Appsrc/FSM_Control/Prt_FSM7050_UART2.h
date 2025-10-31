/********************************************************************************
    * 文件名称 ：Prt_FSM7050_UART2.h
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2025-06-13
    * 功     能：通信协议 界面
*********************************************************************************/
#ifndef _PRT_FSM7050_UART2_H
#define _PRT_FSM7050_UART2_H

#include <QWidget>
#include "ProtocolBaseWidget.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include "ElaScrollPageArea.h"
#include "ElaComboBox.h"
#include "ElaPushButton.h"
#include "ElaToggleButton.h"
#include "ElaText.h"
#include "ElaLineEdit.h"
#include "ElaListView.h"
#include "ElaMessageBar.h"
#include "T_LogModel.h"
// 前置声明所有用到的控件类型
class ElaScrollPageArea;
class ElaComboBox;
class ElaPushButton;
class ElaToggleButton;
class ElaText;
class ElaLineEdit;
class ElaListView;
class ElaMessageBar;

class Prt_FSM7050_UART2 : public ProtocolBaseWidget
{
    Q_OBJECT
public:
    explicit Prt_FSM7050_UART2(QWidget *parent = nullptr);
    ~Prt_FSM7050_UART2();
public slots: 
    void SerialConnected(bool connected) override;
    void SerialDataReceived(QByteArray data) override;
    void Creat_data(uint8_t *data);//生成协议
    void COM_Send_data(uint8_t *data, uint8_t len); // 发送数据
signals:
    void Proctol_out(const QByteArray &data); // 协议解析信号
private:
    static const int ParamCount = 11;
    ElaText* paramLabels[ParamCount];
    ElaLineEdit* paramEdits[ParamCount]; // 4~18字节参数输入框
    ElaLineEdit* recvDataEdit = nullptr;     // 实时数据显示
    ElaPushButton* SendParamBtn = nullptr;
    ElaListView* logList = nullptr;
    T_LogModel* logModel = nullptr; // 日志数据模型
    ElaText* RecvCountLabel = nullptr;
    ElaText* CheckErrLabel = nullptr;
    /*******************界面变量**********************************/
    int X1 = 0; // X1位置
    int Y1 = 0; // Y1位置
    int X2 = 0; // X2位置
    int Y2 = 0; // Y2位置
    int X1_Zero_Value = 0; // X1零点位置
    int Y1_Zero_Value = 0; // Y1零点位置
    int X2_Zero_Value = 0; // X2零点位置
    int Y2_Zero_Value = 0; // Y2零点位置
    int currentMode = 0; // 模式选择
    int Last_mode = 0;           // 记录上一次模式
    int pos_cache[4] = {0, 0, 0, 0}; // 位置缓存
    int Zero_cache[4] = {0, 0, 0, 0}; // 零点位置缓存
    void onModeChanged(int index);
    void onSendParamClicked();
    int recvCount = 0;
    int checkErrCount = 0;
    int16_t farme_count = 0; // 帧计数 0~999
    ElaToggleButton* LoopSendBtn = nullptr;
    QTimer* loopSendTimer = nullptr;
};


#endif // _PRT_FSM7050_UART2_H
