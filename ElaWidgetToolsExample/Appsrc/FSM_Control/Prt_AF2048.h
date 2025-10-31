/********************************************************************************
    * 文件名称 ：Prt_AF2048.h
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2025-06-06
    * 功     能：通信协议 界面
*********************************************************************************/
#ifndef _Prt_AF2048_H
#define _Prt_AF2048_H

#include <QWidget>
#include <QTimer>
#include <QMutex>
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
#include "QCustomplot/qcustomplot.h"
#include "WaveTestThread.h"

// 前置声明所有用到的控件类型
class ElaScrollPageArea;
class ElaComboBox;
class ElaPushButton;
class ElaToggleButton;
class ElaText;
class ElaLineEdit;
class ElaListView;
class ElaMessageBar;

class Prt_AF2048 : public ProtocolBaseWidget
{
    Q_OBJECT
public:
    explicit Prt_AF2048(QWidget *parent = nullptr);
    ~Prt_AF2048();
public slots: 
    void SerialConnected(bool connected) override;
    void SerialDataReceived(QByteArray data) override;
    void Creat_data(uint8_t *data, uint8_t mode, int16_t X1, int16_t Y1);//生成协议
    void Proctol_Analysis(const QByteArray &data); // 协议解析
    void COM_Send_data(uint8_t *data, uint8_t len); // 发送数据
    void onStartTestToggled(bool checked);
    void onPeakToPeakUpdated(double x_value, double y_value , double x_mrad , double y_mrad);
signals:
    void Proctol_out(const QByteArray &data); // 协议解析信号
private:
    /*******************界面元素**********************************/
    ElaText* X1_Info = nullptr;
    ElaText* Y1_Info = nullptr;
    ElaComboBox* Mode_Select = nullptr;
    ElaLineEdit* X1_Edit = nullptr;
    ElaLineEdit* Y1_Edit = nullptr;
    ElaPushButton* SendParamBtn = nullptr;
    ElaListView* logList = nullptr;
    T_LogModel* logModel = nullptr; // 日志数据模型
    ElaText* RecvCountLabel = nullptr;
    ElaText* CheckErrLabel = nullptr;
    ElaToggleButton* LoopSendBtn = nullptr;
    ElaLineEdit* FreqEdit = nullptr;
    QTimer* loopSendTimer = nullptr;
    QTimer* uiUpdateTimer = nullptr; // UI更新定时器
    
    // 波形测试相关
    WaveTestThread* waveTestThread = nullptr;
    ElaToggleButton* startTestBtn = nullptr;
    ElaText* peakToPeakValue_X = nullptr;
    ElaText* peakToPeakValue_Y = nullptr;
    ElaText* degreeValue_X = nullptr;
    ElaText* degreeValue_Y = nullptr;
    ElaText* overshootValue_X = nullptr;
    ElaText* overshootValue_Y = nullptr;
    ElaComboBox* testModeSelect = nullptr;
    
    // 图表相关
    QCustomPlot* customPlot = nullptr;
    QVector<double> timeData;
    QVector<double> x1Data;
    QVector<double> y1Data;
    double timeCounter = 0.0;
    static const int maxDataPoints = 1000; // 最大数据点数
    /*******************界面变量**********************************/
    int X1 = 0; // X1位置
    int Y1 = 0; // Y1位置
    int X1_Zero_Value = 0; // X1零点位置
    int Y1_Zero_Value = 0; // Y1零点位置
    int currentMode = 0; // 模式选择
    int Last_mode = 0;           // 记录上一次模式
    int pos_cache[4] = {0, 0, 0, 0}; // 位置缓存
    int Zero_cache[4] = {0, 0, 0, 0}; // 零点位置缓存
    
    // 高频数据处理优化
    QByteArray dataBuffer; // 数据缓冲区
    QMutex dataMutex; // 数据互斥锁
    struct LatestData {
        int16_t X1;
        int16_t Y1;
        uint8_t mode;
        bool hasNewData;
    } latestValidData; // 最新有效数据
    
    void onModeChanged(int index);
    void onSendParamClicked();
    void updateUI(); // UI更新函数
    void processBufferedData(); // 处理缓冲数据
    int recvCount = 0;
    int checkErrCount = 0;
    
};

#endif // _Prt_AF2048_H
