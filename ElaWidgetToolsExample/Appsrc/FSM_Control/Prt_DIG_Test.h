/********************************************************************************
    * 文件名称 ：Prt_DIG_Test.h
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2025-06-06
    * 功     能：通信协议 界面
*********************************************************************************/
#ifndef _Prt_DIG_Test_H
#define _Prt_DIG_Test_H

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

class Prt_DIG_Test : public ProtocolBaseWidget
{
    Q_OBJECT
public:
    explicit Prt_DIG_Test(QWidget *parent = nullptr);
    ~Prt_DIG_Test();
public slots: 
    void SerialConnected(bool connected) override;
    void SerialDataReceived(QByteArray data) override;
    void Creat_data(uint8_t *data, uint8_t mode, int16_t X1, int16_t Y1);//生成协议
    void Proctol_Analysis(const QByteArray &data); // 协议解析
    void COM_Send_data(uint8_t *data, uint8_t len); // 发送数据
signals:
    void Proctol_out(const QByteArray &data); // 协议解析信号
private:
    /*******************界面元素**********************************/
    ElaText* X1_Info = nullptr;
    ElaText* Y1_Info = nullptr;
    ElaText* X1_Zero = nullptr;
    ElaText* Y1_Zero = nullptr;
    ElaComboBox* Mode_Select = nullptr;
    ElaLineEdit* X1_Edit = nullptr;
    ElaLineEdit* Y1_Edit = nullptr;
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
    QVector<double> y1Data;
    QVector<double> x1DAData;
    QVector<double> y1DAData;
    double timeCounter;
    const int maxDataPoints = 300; // 限制为300个数据点，约10秒数据，确保流畅度
    
    // 最新有效数据结构
    struct ValidData {
        bool hasNewData;
        int16_t X1;
        int16_t Y1;
        int16_t X1_DActrl; // X-DA控制
        int16_t Y1_DActrl; // Y-DA控制
        int16_t X1_Zero;
        int16_t Y1_Zero;
        uint8_t mode;
    } latestValidData;
};


#endif // _Prt_DIG_Test_H
