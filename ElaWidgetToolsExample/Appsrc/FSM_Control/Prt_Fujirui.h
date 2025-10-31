/********************************************************************************
    * 文件名称 ：Prt_Fujirui.h
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2025-06-06
    * 功     能：通信协议 界面
*********************************************************************************/
#ifndef _Prt_FUJIRUI_H
#define _Prt_FUJIRUI_H

#include <QWidget>
#include "ProtocolBaseWidget.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTimer>
#include <QMutex>
#include "ElaScrollPageArea.h"
#include "ElaComboBox.h"
#include "ElaPushButton.h"
#include "ElaToggleButton.h"
#include "ElaText.h"
#include "ElaLineEdit.h"
#include "ElaListView.h"
#include "ElaMessageBar.h"
#include "T_LogModel.h"
#include "QCustomplot/qcustomplot.h"
// 前置声明所有用到的控件类型
class ElaScrollPageArea;
class ElaComboBox;
class ElaPushButton;
class ElaToggleButton;
class ElaText;
class ElaLineEdit;
class ElaListView;
class ElaMessageBar;

class Prt_Fujirui : public ProtocolBaseWidget
{
    Q_OBJECT
public:
    explicit Prt_Fujirui(QWidget *parent = nullptr);
    ~Prt_Fujirui();
public slots: 
    void SerialConnected(bool connected) override;
    void SerialDataReceived(QByteArray data) override;
    void Creat_data(uint8_t *data, uint8_t mode, uint8_t sync_mode , uint8_t scan_freq , int16_t ctrl , int16_t speedpos);//生成协议
    void Proctol_Analysis(const QByteArray &data); // 协议解析
    void COM_Send_data(uint8_t *data, uint8_t len); // 发送数据
signals:
    void Proctol_out(const QByteArray &data); // 协议解析信号
private:
    /*******************界面元素**********************************/
    ElaText* X_Info = nullptr;  // 摆镜位置反馈
    ElaText* MirrorStatusInfo = nullptr;  // 摆镜状态显示
    ElaComboBox* Mode_Select = nullptr; //模式指令 00扫描模式 01位置模式
    ElaComboBox* sync_mode_Select = nullptr;// 内同步外同步设置 00外同步 01内同步
    ElaComboBox* scan_freq_Select = nullptr;// 摆扫频率参数设置 01->低频 02->中频 03->高频
    ElaLineEdit* ctrl_Edit = nullptr;// 控制参数 float ±400~800  1LSB->0.05°/s  int16 0->0 -800->49536 -400->57356 400->8000 800->16000
    ElaLineEdit* speedpos_Edit = nullptr;//匀速段角位置调节 float ±1.5° -1.5°->0 0°->32767  +1.5°->65535
    ElaText* ctrlValueLabel = nullptr;// 控制参数对应码值显示
    ElaText* ctrlHexLabel = nullptr;  // 控制参数对应16进制显示
    ElaText* speedValueLabel = nullptr;// 角位置参数对应码值显示
    ElaText* speedHexLabel = nullptr; // 角位置参数对应16进制显示
    ElaPushButton* SendParamBtn = nullptr;
    ElaListView* logList = nullptr;
    T_LogModel* logModel = nullptr; // 日志数据模型
    QCustomPlot* customPlot = nullptr; // 数据曲线图表
    ElaText* RecvCountLabel = nullptr;
    ElaText* CheckErrLabel = nullptr;
    /*******************界面变量**********************************/
    int X1 = 0; // X1位置
    int Y1 = 0; // Y1位置
    int X1_Zero_Value = 0; // X1零点位置
    int Y1_Zero_Value = 0; // Y1零点位置
    int currentMode = 0; // 模式选择
    int Last_mode = 0;           // 记录上一次模式
    int pos_cache[4] = {0, 0, 0, 0}; // 位置缓存
    int Zero_cache[4] = {0, 0, 0, 0}; // 零点位置缓存
    void onModeChanged(int index);
    void onSendParamClicked();
    void updateUI();
    void processBufferedData();
    
    // 度数转换函数
    int16_t speedToValue(double speed);    // ±400~800°/s转换 (ctrl_Edit)
    int16_t degreesToValue1_5(double degrees); // ±1.5°转换 (speedpos_Edit)
    void updateCtrlValue();     // 控制参数实时更新
    void updateSpeedValue();    // 角位置参数实时更新
    
    // 统计变量
    int recvCount = 0;
    int checkErrCount = 0;
    
    // UI更新定时器
    QTimer* uiUpdateTimer = nullptr;
    
    // 数据缓冲和线程安全
    QMutex dataMutex;
    QByteArray dataBuffer;
    
    // 图表数据
    QVector<double> timeData;
    QVector<double> x1Data;
    double timeCounter;
    const int maxDataPoints = 300; // 限制为300个数据点，约10秒数据，确保流畅度
    
    // 最新有效数据结构
    struct ValidData {
        bool hasNewData;
        int16_t posfb;
        uint8_t mirrorStatus;  // 摆镜状态：0x00正常，0x01故障
    } latestValidData;
};


#endif // _Prt_FUJIRUI_H
