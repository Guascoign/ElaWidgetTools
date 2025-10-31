#ifndef FRAME_DIALOG_H
#define FRAME_DIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QSpinBox>
#include <QFormLayout>
#include "ElaPushButton.h"
#include "ElaText.h"
#include "ElaAppBar.h"
#include "ElaTheme.h"
#include "ElaLineEdit.h"
#include "ElaComboBox.h"
#include "ElaSpinBox.h"

#ifdef Q_OS_WIN
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#define Q_TAKEOVER_NATIVEEVENT_H virtual bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result) override;
#else
#define Q_TAKEOVER_NATIVEEVENT_H virtual bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;
#endif
#else
#define Q_TAKEOVER_NATIVEEVENT_H
#endif

// 帧数据结构
struct FrameData
{
    QString frameType;    // 帧类型
    QString name;         // 名称
    QString dataType;     // 数据类型 (uint8/uint16/int8/int16)
    QString data;         // 数据值
    QString remark;       // 备注
    
    // 特殊配置
    QString rollMode;     // 滚码模式 (自增/自减)
    QString checkMode;    // 校验模式 (累加和/异或)
    int checkStartByte;   // 校验起始字节
    int checkEndByte;     // 校验结束字节
};

class Frame_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Frame_Dialog(QWidget* parent = nullptr);
    ~Frame_Dialog();
    
    void setFrameType(const QString& frameType); // 设置帧类型
    FrameData getFrameData() const; // 获取配置的帧数据
    void resetForm(); // 重置表单内容

protected:
    void paintEvent(QPaintEvent* event) override;
    Q_TAKEOVER_NATIVEEVENT_H

signals:
    void frameDataConfirmed(const FrameData& data); // 当用户点击确定时发出信号，携带配置数据

private slots:
    void onOkButtonClicked();     // 确定按钮点击事件
    void onCancelButtonClicked(); // 取消按钮点击事件
    void onFrameTypeChanged();    // 帧类型改变时更新界面
    void validateCheckByteRange(); // 验证校验字节范围

private:
    void setupFormLayout();       // 设置表单布局
    void updateFormForFrameType(const QString& frameType); // 根据帧类型更新表单
    
    ElaAppBar* _appBar = nullptr;
    ElaPushButton* _okButton = nullptr;
    ElaPushButton* _cancelButton = nullptr;
    ElaText* _titleText = nullptr;
    ElaThemeType::ThemeMode _themeMode;
    
    // 表单控件
    QFormLayout* _formLayout = nullptr;
    ElaComboBox* _frameTypeComboBox = nullptr;   // 帧类型选择
    ElaLineEdit* _nameEdit = nullptr;            // 名称输入
    ElaComboBox* _dataTypeComboBox = nullptr;    // 数据类型选择
    ElaLineEdit* _dataEdit = nullptr;            // 数据输入
    ElaLineEdit* _remarkEdit = nullptr;          // 备注输入
    
    // 特殊配置控件
    ElaComboBox* _rollModeComboBox = nullptr;    // 滚码模式（自增/自减）
    ElaComboBox* _checkModeComboBox = nullptr;   // 校验模式（累加和/异或）
    ElaSpinBox* _checkStartByteSpinBox = nullptr; // 校验起始字节
    ElaSpinBox* _checkEndByteSpinBox = nullptr;   // 校验结束字节
    
    // 标签控件引用（用于控制显示/隐藏）
    ElaText* _dataLabel = nullptr;         // 数据标签
    ElaText* _rollModeLabel = nullptr;
    ElaText* _checkModeLabel = nullptr;
    ElaText* _checkStartLabel = nullptr;
    ElaText* _checkEndLabel = nullptr;
    
    QString _currentFrameType; // 当前帧类型
};

#endif // FRAME_DIALOG_H
