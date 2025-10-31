#include "Frame_Dialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QPainter>
#include "ElaText.h"
#include "ElaPushButton.h"
#include "ElaAppBar.h"
#include "ElaTheme.h"
#include "ElaLineEdit.h"
#include "ElaComboBox.h"
#include "ElaSpinBox.h"

Frame_Dialog::Frame_Dialog(QWidget* parent)
    : QDialog(parent)
{
    setFixedSize(520, 400);
    setObjectName("Frame_Dialog");
    setWindowTitle("添加帧");
    setWindowModality(Qt::ApplicationModal);

    // 自定义AppBar
    _appBar = new ElaAppBar(this);
    _appBar->setAppBarHeight(30);
    _appBar->setIsFixedSize(true);
    _appBar->setIsStayTop(true);
    _appBar->setWindowButtonFlags(ElaAppBarType::CloseButtonHint);
    _appBar->setIsDefaultClosed(false);
    connect(_appBar, &ElaAppBar::closeButtonClicked, this, [=]() {
        reject(); // 点击关闭按钮相当于取消
    });

    // 创建标题文本
    _titleText = new ElaText("帧配置", this);
    _titleText->setTextPixelSize(16);
    
    // 标题布局
    QHBoxLayout* titleLayout = new QHBoxLayout();
    titleLayout->setContentsMargins(0, 0, 0, 0);
    titleLayout->addSpacing(3);
    titleLayout->addWidget(_titleText);

    // 设置表单布局
    setupFormLayout();

    // 创建确定和取消按钮
    _okButton = new ElaPushButton("确定", this);
    _okButton->setFixedSize(90, 35);
    _okButton->setBorderRadius(6);
    // 设置确定按钮为主题色
    _okButton->setLightDefaultColor(ElaThemeColor(ElaThemeType::Light, PrimaryNormal));
    _okButton->setLightHoverColor(ElaThemeColor(ElaThemeType::Light, PrimaryHover));
    _okButton->setLightPressColor(ElaThemeColor(ElaThemeType::Light, PrimaryPress));
    _okButton->setLightTextColor(Qt::white);
    _okButton->setDarkDefaultColor(ElaThemeColor(ElaThemeType::Dark, PrimaryNormal));
    _okButton->setDarkHoverColor(ElaThemeColor(ElaThemeType::Dark, PrimaryHover));
    _okButton->setDarkPressColor(ElaThemeColor(ElaThemeType::Dark, PrimaryPress));
    _okButton->setDarkTextColor(Qt::white);
    
    _cancelButton = new ElaPushButton("取消", this);
    _cancelButton->setFixedSize(90, 35);
    _cancelButton->setBorderRadius(6);

    // 按钮布局
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    buttonLayout->addStretch();
    buttonLayout->addWidget(_okButton);
    buttonLayout->addSpacing(15);
    buttonLayout->addWidget(_cancelButton);
    buttonLayout->addStretch();

    // 主布局
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 45, 30, 30); // 增加左右边距以居中
    mainLayout->setSpacing(20);
    mainLayout->addLayout(titleLayout);
    mainLayout->addStretch(1); // 添加弹性空间
    mainLayout->addLayout(_formLayout);
    mainLayout->addStretch(1); // 添加弹性空间
    mainLayout->addLayout(buttonLayout);

    // 连接信号槽
    connect(_okButton, &ElaPushButton::clicked, this, &Frame_Dialog::onOkButtonClicked);
    connect(_cancelButton, &ElaPushButton::clicked, this, &Frame_Dialog::onCancelButtonClicked);
    
    // 初始化主题模式并监听主题变化
    _themeMode = ElaTheme::getInstance()->getThemeMode();
    connect(ElaTheme::getInstance(), &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
        update(); // 重新绘制界面
    });
}

Frame_Dialog::~Frame_Dialog()
{
}

void Frame_Dialog::onOkButtonClicked()
{
    FrameData data;
    data.frameType = _frameTypeComboBox->currentText();
    data.name = _nameEdit->text();
    data.dataType = _dataTypeComboBox->currentText();
    
    // 只有当数据输入框可见时才获取其值
    if (_dataEdit->isVisible()) {
        data.data = _dataEdit->text();
        
        // 如果是固定值类型（帧头、帧尾、标识）且数据为空，设置默认值
        if ((data.frameType == "帧头" || data.frameType == "帧尾" || data.frameType == "标识") && 
            data.data.isEmpty()) {
            if (data.dataType.contains("uint8") || data.dataType.contains("int8")) {
                data.data = "0xFF";
            } else if (data.dataType.contains("uint16") || data.dataType.contains("int16")) {
                data.data = "0xFFFF";
            } else if (data.dataType.contains("uint32") || data.dataType.contains("int32")) {
                data.data = "0xFFFFFFFF";
            } else {
                data.data = "0xFF"; // 默认情况
            }
        }
    } else {
        data.data = ""; // 校验类型不需要数据值
    }
    
    data.remark = _remarkEdit->text();
    
    // 获取特殊配置
    if (_rollModeComboBox && _rollModeComboBox->isVisible()) {
        data.rollMode = _rollModeComboBox->currentText();
    }
    if (_checkModeComboBox && _checkModeComboBox->isVisible()) {
        data.checkMode = _checkModeComboBox->currentText();
        data.checkStartByte = _checkStartByteSpinBox->value();
        data.checkEndByte = _checkEndByteSpinBox->value();
        
        // 最终验证校验字节范围
        if (data.checkStartByte >= data.checkEndByte || (data.checkEndByte - data.checkStartByte) < 1) {
            // 这里可以显示错误信息，但由于validateCheckByteRange已经自动调整，理论上不会到达这里
            return;
        }
    }
    
    emit frameDataConfirmed(data); // 发出确认信号，携带配置数据
    accept(); // 关闭对话框并返回 QDialog::Accepted
}

void Frame_Dialog::onCancelButtonClicked()
{
    reject(); // 关闭对话框并返回 QDialog::Rejected
}

void Frame_Dialog::paintEvent(QPaintEvent* event)
{
    // 仿照ElaColorDialog的paintEvent实现，确保主题匹配
    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    
    // 背景绘制
    painter.setBrush(ElaThemeColor(_themeMode, DialogBase));
    painter.drawRect(rect());
    
    painter.restore();
    QDialog::paintEvent(event);
}

#ifdef Q_OS_WIN
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
bool Frame_Dialog::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)
#else
bool Frame_Dialog::nativeEvent(const QByteArray& eventType, void* message, long* result)
#endif
{
    if (_appBar)
    {
        int ret = _appBar->takeOverNativeEvent(eventType, message, result);
        if (ret == -1)
        {
            return QDialog::nativeEvent(eventType, message, result);
        }
        return (bool)ret;
    }
    return QDialog::nativeEvent(eventType, message, result);
}
#endif

void Frame_Dialog::setFrameType(const QString& frameType)
{
    _currentFrameType = frameType;
    if (_frameTypeComboBox) {
        _frameTypeComboBox->setCurrentText(frameType);
        _frameTypeComboBox->setEnabled(true); // 允许用户在对话框中修改帧类型
        updateFormForFrameType(frameType);
    }
}

FrameData Frame_Dialog::getFrameData() const
{
    FrameData data;
    if (_frameTypeComboBox) data.frameType = _frameTypeComboBox->currentText();
    if (_nameEdit) data.name = _nameEdit->text();
    if (_dataTypeComboBox) data.dataType = _dataTypeComboBox->currentText();
    
    // 只有当数据输入框可见时才获取其值
    if (_dataEdit && _dataEdit->isVisible()) {
        data.data = _dataEdit->text();
        
        // 如果是固定值类型（帧头、帧尾、标识）且数据为空，设置默认值
        if ((data.frameType == "帧头" || data.frameType == "帧尾" || data.frameType == "标识") && 
            data.data.isEmpty()) {
            if (data.dataType.contains("uint8") || data.dataType.contains("int8")) {
                data.data = "0xFF";
            } else if (data.dataType.contains("uint16") || data.dataType.contains("int16")) {
                data.data = "0xFFFF";
            } else if (data.dataType.contains("uint32") || data.dataType.contains("int32")) {
                data.data = "0xFFFFFFFF";
            } else {
                data.data = "0xFF"; // 默认情况
            }
        }
    } else {
        data.data = "";
    }
    
    if (_remarkEdit) data.remark = _remarkEdit->text();
    
    if (_rollModeComboBox && _rollModeComboBox->isVisible()) {
        data.rollMode = _rollModeComboBox->currentText();
    }
    if (_checkModeComboBox && _checkModeComboBox->isVisible()) {
        data.checkMode = _checkModeComboBox->currentText();
        data.checkStartByte = _checkStartByteSpinBox->value();
        data.checkEndByte = _checkEndByteSpinBox->value();
    }
    
    return data;
}

void Frame_Dialog::setupFormLayout()
{
    _formLayout = new QFormLayout();
    _formLayout->setSpacing(15);
    _formLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    _formLayout->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
    _formLayout->setFormAlignment(Qt::AlignHCenter | Qt::AlignTop);
    _formLayout->setHorizontalSpacing(20);
    _formLayout->setVerticalSpacing(15);
    
    // 帧类型选择
    _frameTypeComboBox = new ElaComboBox(this);
    _frameTypeComboBox->addItems({"帧头", "标识", "数据", "滚码", "校验", "帧尾"});
    _frameTypeComboBox->setFixedWidth(250);
    _frameTypeComboBox->setFixedHeight(35);
    ElaText* frameTypeLabel = new ElaText("帧类型:", this);
    frameTypeLabel->setTextPixelSize(14);
    frameTypeLabel->setFixedWidth(100);
    _formLayout->addRow(frameTypeLabel, _frameTypeComboBox);
    
    // 名称输入
    _nameEdit = new ElaLineEdit(this);
    _nameEdit->setPlaceholderText("请输入名称");
    _nameEdit->setFixedWidth(250);
    _nameEdit->setFixedHeight(35);
    ElaText* nameLabel = new ElaText("名称:", this);
    nameLabel->setTextPixelSize(14);
    nameLabel->setFixedWidth(100);
    _formLayout->addRow(nameLabel, _nameEdit);
    
    // 数据类型选择
    _dataTypeComboBox = new ElaComboBox(this);
    _dataTypeComboBox->addItems({"uint8", "uint16", "int8", "int16"});
    _dataTypeComboBox->setFixedWidth(250);
    _dataTypeComboBox->setFixedHeight(35);
    ElaText* dataTypeLabel = new ElaText("数据类型:", this);
    dataTypeLabel->setTextPixelSize(14);
    dataTypeLabel->setFixedWidth(100);
    _formLayout->addRow(dataTypeLabel, _dataTypeComboBox);
    
    // 数据输入
    _dataEdit = new ElaLineEdit(this);
    _dataEdit->setPlaceholderText("请输入数据值");
    _dataEdit->setFixedWidth(250);
    _dataEdit->setFixedHeight(35);
    _dataLabel = new ElaText("数据:", this);
    _dataLabel->setTextPixelSize(14);
    _dataLabel->setFixedWidth(100);
    _formLayout->addRow(_dataLabel, _dataEdit);
    
    // 备注输入
    _remarkEdit = new ElaLineEdit(this);
    _remarkEdit->setPlaceholderText("请输入备注");
    _remarkEdit->setFixedWidth(250);
    _remarkEdit->setFixedHeight(35);
    ElaText* remarkLabel = new ElaText("备注:", this);
    remarkLabel->setTextPixelSize(14);
    remarkLabel->setFixedWidth(100);
    _formLayout->addRow(remarkLabel, _remarkEdit);
    
    // 滚码模式选择（默认隐藏）
    _rollModeComboBox = new ElaComboBox(this);
    _rollModeComboBox->addItems({"自增", "自减"});
    _rollModeComboBox->setFixedWidth(250);
    _rollModeComboBox->setFixedHeight(35);
    _rollModeComboBox->setVisible(false);
    _rollModeLabel = new ElaText("滚码模式:", this);
    _rollModeLabel->setTextPixelSize(14);
    _rollModeLabel->setFixedWidth(100);
    _rollModeLabel->setVisible(false);
    _formLayout->addRow(_rollModeLabel, _rollModeComboBox);
    
    // 校验模式选择（默认隐藏）
    _checkModeComboBox = new ElaComboBox(this);
    _checkModeComboBox->addItems({"累加和", "异或"});
    _checkModeComboBox->setFixedWidth(250);
    _checkModeComboBox->setFixedHeight(35);
    _checkModeComboBox->setVisible(false);
    _checkModeLabel = new ElaText("校验模式:", this);
    _checkModeLabel->setTextPixelSize(14);
    _checkModeLabel->setFixedWidth(100);
    _checkModeLabel->setVisible(false);
    _formLayout->addRow(_checkModeLabel, _checkModeComboBox);
    
    // 校验起始字节（默认隐藏）
    _checkStartByteSpinBox = new ElaSpinBox(this);
    _checkStartByteSpinBox->setRange(0, 255);
    _checkStartByteSpinBox->setValue(0); // 设置默认值为0
    _checkStartByteSpinBox->setFixedWidth(250);
    _checkStartByteSpinBox->setFixedHeight(35);
    _checkStartByteSpinBox->setVisible(false);
    _checkStartLabel = new ElaText("校验起始字节:", this);
    _checkStartLabel->setTextPixelSize(14);
    _checkStartLabel->setFixedWidth(100);
    _checkStartLabel->setVisible(false);
    _formLayout->addRow(_checkStartLabel, _checkStartByteSpinBox);
    
    // 校验结束字节（默认隐藏）
    _checkEndByteSpinBox = new ElaSpinBox(this);
    _checkEndByteSpinBox->setRange(0, 255);
    _checkEndByteSpinBox->setValue(1); // 设置默认值为1
    _checkEndByteSpinBox->setFixedWidth(250);
    _checkEndByteSpinBox->setFixedHeight(35);
    _checkEndByteSpinBox->setVisible(false);
    _checkEndLabel = new ElaText("校验结束字节:", this);
    _checkEndLabel->setTextPixelSize(14);
    _checkEndLabel->setFixedWidth(100);
    _checkEndLabel->setVisible(false);
    _formLayout->addRow(_checkEndLabel, _checkEndByteSpinBox);
    
    // 连接帧类型改变信号
    connect(_frameTypeComboBox, QOverload<const QString&>::of(&ElaComboBox::currentTextChanged),
            this, &Frame_Dialog::onFrameTypeChanged);
    
    // 连接校验字节范围验证信号
    connect(_checkStartByteSpinBox, QOverload<int>::of(&ElaSpinBox::valueChanged),
            this, &Frame_Dialog::validateCheckByteRange);
    connect(_checkEndByteSpinBox, QOverload<int>::of(&ElaSpinBox::valueChanged),
            this, &Frame_Dialog::validateCheckByteRange);
}

void Frame_Dialog::updateFormForFrameType(const QString& frameType)
{
    // 隐藏所有特殊配置控件及其标签
    _rollModeComboBox->setVisible(false);
    _rollModeLabel->setVisible(false);
    _checkModeComboBox->setVisible(false);
    _checkModeLabel->setVisible(false);
    _checkStartByteSpinBox->setVisible(false);
    _checkStartLabel->setVisible(false);
    _checkEndByteSpinBox->setVisible(false);
    _checkEndLabel->setVisible(false);
    
    // 默认显示数据输入框和标签
    _dataEdit->setVisible(true);
    _dataLabel->setVisible(true);
    
    // 清空数据类型选项并根据帧类型重新设置
    _dataTypeComboBox->clear();
    
    // 根据帧类型设置数据类型选项和显示对应的配置
    if (frameType == "帧头" || frameType == "帧尾") {
        // 帧头和帧尾只能是uint8，只需要：数据、名称、备注
        _dataTypeComboBox->addItems({"uint8" , "uint16"});
        _dataEdit->setPlaceholderText("请输入数据值");
        _dataEdit->setVisible(true);
    }
    else if (frameType == "标识") {
        // 标识只配置uint8，只需要：数据、名称、备注
        _dataTypeComboBox->addItems({"uint8", "uint16"});
        _dataEdit->setPlaceholderText("请输入标识值");
        _dataEdit->setVisible(true);
    }
    else if (frameType == "数据") {
        // 数据可以是所有类型，需要：数据类型、数据、名称、备注
        _dataTypeComboBox->addItems({"uint8","uint16","uint32", "int8", "int16" ,"int32" , "float", "double" });
        _dataEdit->setPlaceholderText("请输入数据值");
        _dataEdit->setVisible(true);
    }
    else if (frameType == "滚码") {
        // 滚码需要：数据类型(uint8/uint16)、自增/自减、名称、备注
        _dataTypeComboBox->addItems({"uint8", "uint16"});
        _rollModeComboBox->setVisible(true);
        _rollModeLabel->setVisible(true);
        _dataEdit->setPlaceholderText("初始值");
        _dataEdit->setText("0"); // 设置滚码初值默认为0
        _dataEdit->setVisible(true);
    }
    else if (frameType == "校验") {
        // 校验需要：数据类型(uint8/uint16)、累加和/异或、名称、备注、校验起始字节、结束字节
        _dataTypeComboBox->addItems({"uint8", "uint16"});
        _checkModeComboBox->setVisible(true);
        _checkModeLabel->setVisible(true);
        _checkStartByteSpinBox->setVisible(true);
        _checkStartLabel->setVisible(true);
        _checkEndByteSpinBox->setVisible(true);
        _checkEndLabel->setVisible(true);
        
        // 设置默认的校验字节范围
        _checkStartByteSpinBox->setValue(0);
        _checkEndByteSpinBox->setValue(1);
        
        // 隐藏数据输入框和标签
        _dataEdit->setVisible(false);
        _dataLabel->setVisible(false);
    }
    
    // 调整窗口大小
    if (frameType == "校验") {
        setFixedSize(520, 520);
    } else if (frameType == "滚码") {
        setFixedSize(520, 470);
    } else {
        setFixedSize(520, 400);
    }
}

void Frame_Dialog::onFrameTypeChanged()
{
    QString frameType = _frameTypeComboBox->currentText();
    updateFormForFrameType(frameType);
}

void Frame_Dialog::resetForm()
{
    // 清空输入框
    if (_nameEdit) _nameEdit->clear();
    if (_dataEdit) {
        _dataEdit->clear();
        // 如果当前帧类型是滚码，设置默认值为0
        if (_frameTypeComboBox && _frameTypeComboBox->currentText() == "滚码") {
            _dataEdit->setText("0");
        }
    }
    if (_remarkEdit) _remarkEdit->clear();
    
    // 重置下拉框到第一项
    if (_dataTypeComboBox && _dataTypeComboBox->count() > 0) {
        _dataTypeComboBox->setCurrentIndex(0);
    }
    if (_rollModeComboBox) _rollModeComboBox->setCurrentIndex(0);
    if (_checkModeComboBox) _checkModeComboBox->setCurrentIndex(0);
    
    // 重置数值框
    if (_checkStartByteSpinBox) _checkStartByteSpinBox->setValue(0);
    if (_checkEndByteSpinBox) _checkEndByteSpinBox->setValue(1);
}

void Frame_Dialog::validateCheckByteRange()
{
    if (!_checkStartByteSpinBox || !_checkEndByteSpinBox) return;
    
    int startByte = _checkStartByteSpinBox->value();
    int endByte = _checkEndByteSpinBox->value();
    
    // 检查起始字节不能高于结束字节
    if (startByte >= endByte) {
        // 如果是起始字节改变导致的冲突，调整结束字节
        if (sender() == _checkStartByteSpinBox) {
            _checkEndByteSpinBox->setValue(startByte + 1);
        }
        // 如果是结束字节改变导致的冲突，调整起始字节
        else if (sender() == _checkEndByteSpinBox) {
            _checkStartByteSpinBox->setValue(endByte - 1);
        }
    }
    
    // 确保结束字节 - 起始字节 >= 1
    startByte = _checkStartByteSpinBox->value();
    endByte = _checkEndByteSpinBox->value();
    
    if (endByte - startByte < 1) {
        if (sender() == _checkStartByteSpinBox) {
            _checkEndByteSpinBox->setValue(startByte + 1);
        } else if (sender() == _checkEndByteSpinBox) {
            _checkStartByteSpinBox->setValue(endByte - 1);
        }
    }
}