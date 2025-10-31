/********************************************************************************
    * 文件名称 ：proctol_T.h
    * 版     本：V1.0
    * 编写日期 ：2025-08-20
    * 功     能：串口协议配置
*********************************************************************************
V1.0 2025-08-20 First release @ZM
*********************************************************************************/
#ifndef PROCTOL_T_H
#define PROCTOL_T_H

#include "T_BasePage.h"
#include "ElaText.h"
#include "ElaPushButton.h"
#include "ElaToggleButton.h"
#include "ElaLineEdit.h"
#include "ElaTableView.h"
#include "ElaComboBox.h"

// 前向声明
struct FrameData;

class ElaPushButton;
class ElaText;
class ElaToggleButton;  
class ElamessageBar;
class ElaProgressBar;
class ElaLineEdit;
class ElaTableView;
class ElaComboBox;
class Frame_Dialog;

class Proctol_T : public T_BasePage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit Proctol_T(QWidget* parent = nullptr);
    ~Proctol_T();

private:
    /*******************界面元素**********************************/
    ElaPushButton* Add_Farme = nullptr; //添加一帧协议
    ElaPushButton* Del_Farme = nullptr; //删除一帧协议
    ElaPushButton* Save_Prt = nullptr;   //保存协议至cfg
    ElaPushButton* Load_Prt = nullptr;   //从cfg加载协议
    ElaTableView* _protocolTableView = nullptr; //协议帧表格
    Frame_Dialog* _frameDialog = nullptr; // 添加帧对话框

private slots:
    void onAddFrameClicked(); //添加帧按钮点击事件
    void onFrameDataConfirmed(const FrameData& data); // 处理确认的帧数据
    void onSaveProtocolClicked(); // 保存协议配置
    void onLoadProtocolClicked(); // 加载协议配置

private:
    void saveProtocolToFile(const QString& fileName); // 保存协议到文件
    void loadProtocolFromFile(const QString& fileName); // 从文件加载协议
    
    // 字节计算和行添加相关方法
    int calculateCurrentBytePosition(class QStandardItemModel* model); // 计算当前字节位置
    int getDataTypeByteLength(const QString& dataType); // 获取数据类型字节长度
    bool validateDataRange(const QString& dataType, const QString& data); // 验证数据范围
    void addSingleByteRow(class QStandardItemModel* model, int bytePosition, 
                         const QString& name, const QString& dataType, 
                         const QString& remark, const QString& frameType, 
                         const QString& frameSettings); // 添加单字节行
    void addTwoByteRows(class QStandardItemModel* model, int bytePosition, 
                       const QString& name, const QString& dataType, 
                       const QString& remark, const QString& frameType, 
                       const QString& frameSettings); // 添加双字节行
    void addFourByteRows(class QStandardItemModel* model, int bytePosition, 
                        const QString& name, const QString& dataType, 
                        const QString& remark, const QString& frameType, 
                        const QString& frameSettings); // 添加四字节行
    void addEightByteRows(class QStandardItemModel* model, int bytePosition, 
                         const QString& name, const QString& dataType, 
                         const QString& remark, const QString& frameType, 
                         const QString& frameSettings); // 添加八字节行


};

#endif // PROCTOL_T_H
