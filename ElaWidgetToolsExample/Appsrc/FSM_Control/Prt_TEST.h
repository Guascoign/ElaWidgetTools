/********************************************************************************
    * 文件名称 ：Prt_TEST.h
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2025-06-06
    * 功     能：通信协议 界面
*********************************************************************************/
#ifndef PRT_TEST_H
#define PRT_TEST_H

#include <QWidget>
#include "ProtocolBaseWidget.h"

#include "T_LogModel.h"
#include <QTimer>

// 前置声明所有用到的控件类型
class ElaScrollPageArea;
class ElaComboBox;
class ElaPushButton;
class ElaToggleButton;
class ElaText;
class ElaLineEdit;
class ElaListView;

class Prt_TEST : public ProtocolBaseWidget
{
    Q_OBJECT
public:
    explicit Prt_TEST(QWidget *parent = nullptr);
    ~Prt_TEST();

public slots:
    void SerialConnected(bool connected) override;
    void SerialDataReceived(QByteArray data) override;
    void on_Start_step_pushButton_clicked();//阶跃测试
    void on_Move_pushButton_clicked();//移动测试
    void onUnitChanged(int index); // 单位切换槽
    void onModeChanged(int index); // 模式切换槽
    void Creat_data(uint8_t *data,uint8_t mode, uint16_t X , uint16_t Y);//生成协议
    void Proctol_Analysis(const QByteArray &data); // 协议解析
    void COM_Send_data(uint8_t *data, uint8_t len); // 发送数据
signals:
    void Proctol_out(const QByteArray &data); // 协议解析信号
private:
    /*******************界面元素**********************************/
    ElaComboBox* Unit_Select = nullptr; // 单位选择
    ElaComboBox* UMode_Select = nullptr; // 模式选择
    ElaComboBox* step_mode_comboBox = nullptr; // 阶跃模式选择
    ElaListView* loglist = nullptr; // 操作日志
    ElaLineEdit* X_Start = nullptr; // X起始位置
    ElaLineEdit* Y_Start = nullptr; // Y起始位置
    ElaLineEdit* X_End = nullptr; // X结束位置
    ElaLineEdit* Y_End = nullptr; // Y结束位置
    ElaLineEdit* X_Move = nullptr; // X移动位置
    ElaLineEdit* Y_Move = nullptr; // Y移动位置
    ElaPushButton* Start_step = nullptr; // 生成阶跃按钮
    ElaPushButton* moveBtn = nullptr; // 移动按钮
    ElaToggleButton* Start_scan = nullptr; // 扫描按钮
    ElaText* XAxis_V = nullptr; // X轴电压
    ElaText* YAxis_V = nullptr; // Y轴电压
    ElaText* XAxis_mrad = nullptr; // X轴角度
    ElaText* YAxis_mrad = nullptr; // Y轴角度
    ElaText* XAxis_deg = nullptr; // X轴角度
    ElaText* YAxis_deg = nullptr; // Y轴角度
    ElaText* moveTitle = nullptr; // 移动标题
    ElaText* X_Move_Text = nullptr; // X轴移动文本
    ElaText* Y_Move_Text = nullptr; // Y轴移动文本
    T_LogModel* logModel = nullptr; // 日志数据模型
    /*******************界面变量**********************************/
    uint8_t xor_check(uint8_t *data, uint8_t len);//异或校验
    bool Step_test_flag = false;//阶跃测试标志
    int lastUnitIndex = 0; // 上一个单位索引
    float Pos_cache[3] = {0};    // 0:双轴 1:单X 2:单Y
    float Speed_cache[2] = {10 , 100};  // 3:速度模式
    int Last_mode = 0;           // 记录上一次模式
    QTimer *scanTimer = nullptr;
    void onStartScanClicked();

};

#endif // PRT_TEST_H
