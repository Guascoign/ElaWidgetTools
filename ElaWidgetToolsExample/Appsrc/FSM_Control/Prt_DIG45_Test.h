/********************************************************************************
    * 文件名称 ：Prt_DIG45_Test.h
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2025-06-06
    * 功     能：通信协议 界面
*********************************************************************************/
#ifndef _Prt_DIG45_Test_H
#define _Prt_DIG45_Test_H

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

class Prt_DIG45_Test : public ProtocolBaseWidget
{
    Q_OBJECT
public:
    explicit Prt_DIG45_Test(QWidget *parent = nullptr);
    ~Prt_DIG45_Test();
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
    ElaText* X1_FB = nullptr;
    ElaText* X1_TAG = nullptr;
    ElaText* X1_ERR = nullptr;
    ElaText* X1_CTL = nullptr;
    ElaText* Y1_FB = nullptr;
    ElaText* Y1_TAG = nullptr;
    ElaText* Y1_ERR = nullptr;
    ElaText* Y1_CTL = nullptr;
    ElaComboBox* Mode_Select = nullptr;
    ElaLineEdit* X1_Edit = nullptr;
    ElaLineEdit* Y1_Edit = nullptr;
    ElaPushButton* SendParamBtn = nullptr;
    ElaListView* logList = nullptr;
    T_LogModel* logModel = nullptr; // 日志数据模型
    QCustomPlot* X_customPlot = nullptr; // 数据曲线图表
    QCustomPlot* Y_customPlot = nullptr; // 数据曲线图表
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
    QVector<double> X_FB_L;
    QVector<double> X_TAG_L;
    QVector<double> X_CTL_L;
    QVector<double> X_ERR_L;

    QVector<double> Y_FB_L;
    QVector<double> Y_TAG_L;
    QVector<double> Y_CTL_L;
    QVector<double> Y_ERR_L;

    double timeCounter;
    const int maxDataPoints = 90; // 限制为90个数据点，约3秒数据，确保流畅度
    
    // 最新有效数据结构
    struct ValidData {
        bool hasNewData;
        int16_t X1_FB; // X-反馈
        int16_t X1_TAG;// X-目标
        int16_t X1_CTL; // X-DA控制
        int16_t X1_ERR; // Y-DA控制

        int16_t Y1_FB; // Y-反馈
        int16_t Y1_TAG;// Y-目标
        int16_t Y1_CTL; // Y-DA控制
        int16_t Y1_ERR; // Y-DA控制
        
        uint8_t mode;
    } latestValidData;
};


#endif // _Prt_DIG45_Test_H
