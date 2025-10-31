#ifndef FPGA_OTA_T_H
#define FPGA_OTA_T_H
#include "T_BasePage.h"

#include "ElaText.h"
#include "ElaPushButton.h"
#include "Uart_T.h"
#include "ElaToggleButton.h"
#include "ElaLineEdit.h"

class ElaPushButton;
class ElaText;
class ElaToggleButton;
class ElamessageBar;
class ElaProgressBar;
class ElaLineEdit;

class FPGA_OTA_T : public T_BasePage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit FPGA_OTA_T(QWidget* parent = nullptr);
    ~FPGA_OTA_T();
    void setupUartConnections(Uart_T* p_uartObj);    // 设置串口连接
    void onSerialDataReceived(QByteArray data); // 串口接收数据槽
signals:
    void sendSerialData(QByteArray data); //发送数据信号
private:
    /*******************界面元素**********************************/
    ElaPushButton* Select_File = nullptr;
    ElaPushButton* Start_OTA = nullptr;
    ElaPushButton* Erease = nullptr;//擦除 串口发送AA 03
    ElaPushButton* ReadBack = nullptr;//回读 串口发送AA 02
    ElaToggleButton* proctolToggle = nullptr; // 协议选择开关
    ElaText* File_name = nullptr;
    ElaText* File_size = nullptr;
    ElaText* File_md5 = nullptr;
    ElaText* File_path = nullptr;
    ElaText* ReadBack_Bytes = nullptr; // 回读字节数
    ElaText* ReadBack_MD5 = nullptr; // 回读MD5
    ElaText* ReadBack_File_MD5 = nullptr; // 回读文件名
    ElaProgressBar* progressBar = nullptr; // 进度条    
    ElaToggleButton* saveFileToggle = nullptr; // 保存文件控制开关
    
    /*******************界面变量**********************************/
    bool Com_Opened_flag = false; // 串口打开标志
    Uart_T* uartObj = nullptr;

};

#endif // FPGA_OTA_T_H
