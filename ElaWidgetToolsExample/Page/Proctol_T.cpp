#include "Proctol_T.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QFormLayout>
#include <QSpinBox>
#include <QLabel>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QDateTime>
#include "ElaApplication.h"
#include "ElaWindow.h"
#include "ElaScrollPageArea.h"
#include "ElaText.h"
#include "ElaPushButton.h"
#include "ElaTableView.h"
#include "ElaComboBox.h"
#include "ElaContentDialog.h"
#include "ElaLineEdit.h"
#include "ElaMessageBar.h"

//控件
#include "ElaText.h"
#include "../Appsrc/Proctol_T/Frame_Dialog.h"
Proctol_T::Proctol_T(QWidget* parent)
    : T_BasePage(parent)
{
    ElaWindow* window = dynamic_cast<ElaWindow*>(parent);
    //setTitleVisible(false);// 隐藏标题栏
    createCustomWidget("协议编辑器");

    // 创建按钮和输入框
    Add_Farme = new ElaPushButton("添加帧", this);
    Del_Farme = new ElaPushButton("删除帧", this);
    Save_Prt = new ElaPushButton("保存协议", this);
    Load_Prt = new ElaPushButton("加载协议", this);

    // 设置按钮和编辑框的样式
    Add_Farme->setFixedWidth(100);
    Del_Farme->setFixedWidth(100);
    Save_Prt->setFixedWidth(100);
    Load_Prt->setFixedWidth(100);

    // 创建协议表格
    _protocolTableView = new ElaTableView(this);
    
    // 创建帧对话框
    _frameDialog = new Frame_Dialog(this);
    
    // 创建表格模型
    QStandardItemModel* model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({"字节", "名称", "数据类型", "长度", "备注", "帧类型","帧设置"});
    //帧类型：帧头 / 标识 / 数据 / 滚码 / 校验 / 帧尾
    _protocolTableView->setModel(model);
    
    // 设置表格属性
    _protocolTableView->setAlternatingRowColors(true);
    _protocolTableView->verticalHeader()->setHidden(true);
    _protocolTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    _protocolTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    _protocolTableView->horizontalHeader()->setMinimumSectionSize(60);
    _protocolTableView->verticalHeader()->setMinimumSectionSize(30);
    _protocolTableView->setFixedHeight(400);
    
    // 设置表格字体
    QFont tableHeaderFont = _protocolTableView->horizontalHeader()->font();
    tableHeaderFont.setPixelSize(14);
    _protocolTableView->horizontalHeader()->setFont(tableHeaderFont);


    // 按钮布局（水平）
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addSpacing(20);
    buttonLayout->addWidget(Add_Farme);
    buttonLayout->addWidget(Del_Farme);
    buttonLayout->addWidget(Save_Prt);
    buttonLayout->addWidget(Load_Prt);
    buttonLayout->addStretch(); // 添加弹簧，使按钮靠左对齐

    // 表格布局
    QVBoxLayout* tableLayout = new QVBoxLayout();
    tableLayout->addWidget(new ElaText("协议帧定义:"));
    tableLayout->addWidget(_protocolTableView);

    // 主布局
    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("Serial Protocol");
    QVBoxLayout* centerLayout = new QVBoxLayout(centralWidget);
    centerLayout->setContentsMargins(10, 10, 10, 10);
    centerLayout->addLayout(buttonLayout);
    centerLayout->addSpacing(10);
    centerLayout->addLayout(tableLayout);
    centerLayout->addStretch();
    
    addCentralWidget(centralWidget, true, true, 0);

    // 连接信号
    connect(Add_Farme, &ElaPushButton::clicked, this, &Proctol_T::onAddFrameClicked);
    connect(Save_Prt, &ElaPushButton::clicked, this, &Proctol_T::onSaveProtocolClicked);
    connect(Load_Prt, &ElaPushButton::clicked, this, &Proctol_T::onLoadProtocolClicked);
    
    // 连接对话框信号
    connect(_frameDialog, &Frame_Dialog::frameDataConfirmed, this, &Proctol_T::onFrameDataConfirmed);

    // 连接表格显示信号，设置列宽
    connect(_protocolTableView, &ElaTableView::tableViewShow, this, [=]() {
        _protocolTableView->setColumnWidth(0, 60);  // 字节
        _protocolTableView->setColumnWidth(1, 120); // 名称
        _protocolTableView->setColumnWidth(2, 100); // 数据类型
        _protocolTableView->setColumnWidth(3, 80);  // 长度
        _protocolTableView->setColumnWidth(4, 150); // 备注
        _protocolTableView->setColumnWidth(5, 100); // 帧类型
        _protocolTableView->setColumnWidth(6, 200); // 帧设置
        
        // 设置表头文本居中
        QHeaderView* header = _protocolTableView->horizontalHeader();
        if (header) {
            header->setDefaultAlignment(Qt::AlignCenter);
        }
    });
}

Proctol_T::~Proctol_T()
{
}

void Proctol_T::onAddFrameClicked()
{
    // 重置表单内容
    _frameDialog->resetForm();

    _frameDialog->exec(); // 仿照 T_Popup 中的 colorDialogButton 实现方式
}

void Proctol_T::onFrameDataConfirmed(const FrameData& data)
{
    // 获取表格模型
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(_protocolTableView->model());
    if (!model) return;
    
    // 验证数据范围（只对需要输入数据的帧类型进行验证）
    if ((data.frameType == "帧头" || data.frameType == "帧尾" || data.frameType == "标识" || 
         data.frameType == "数据" || data.frameType == "滚码") && 
        !data.data.isEmpty() && !validateDataRange(data.dataType, data.data)) {
        
        // 显示警告消息
        ElaMessageBar::warning(ElaMessageBarType::TopRight, "数据非法", QString("输入的数据 '%1' 超出了 %2 类型的有效范围").arg(data.data).arg(data.dataType), 3000, this);
        return; // 不添加无效数据
    }
    
    // 计算当前字节位置（基于已有数据的实际字节消耗）
    int currentByte = calculateCurrentBytePosition(model);
    
    // 确定数据类型的字节长度
    int byteLength = getDataTypeByteLength(data.dataType);
    
    // 帧设置（根据帧类型和特殊配置组装，使用结构化格式方便保存配置）
    QString frameSettings;
    if (data.frameType == "帧头" || data.frameType == "帧尾") {
        frameSettings = QString("固定值:%1").arg(data.data);
    }
    else if (data.frameType == "标识") {
        frameSettings = QString("标识:%1").arg(data.data);
    }
    else if (data.frameType == "数据") {
        frameSettings = QString("数据:%1").arg(data.data);
    }
    else if (data.frameType == "滚码") {
        frameSettings = QString("滚码:初值=%1,模式=%2").arg(data.data).arg(data.rollMode);
    }
    else if (data.frameType == "校验") {
        frameSettings = QString("校验:算法=%1,起始=%2,结束=%3")
                         .arg(data.checkMode)
                         .arg(data.checkStartByte)
                         .arg(data.checkEndByte);
    }
    else {
        frameSettings = data.data;
    }
    
    // 根据字节长度创建对应的行数
    if (byteLength == 1) {
        // u8类型，创建一行
        addSingleByteRow(model, currentByte, data.name, data.dataType, data.remark, data.frameType, frameSettings);
    }
    else if (byteLength == 2) {
        // u16类型，创建两行：高字节和低字节
        addTwoByteRows(model, currentByte, data.name, data.dataType, data.remark, data.frameType, frameSettings);
    }
    else if (byteLength == 4) {
        // u32/float类型，创建四行
        addFourByteRows(model, currentByte, data.name, data.dataType, data.remark, data.frameType, frameSettings);
    }
    else if (byteLength == 8) {
        // double类型，创建八行
        addEightByteRows(model, currentByte, data.name, data.dataType, data.remark, data.frameType, frameSettings);
    }
    
    qDebug() << "添加新帧:" << data.frameType << data.name << data.dataType << "字节长度:" << byteLength;
}

void Proctol_T::onSaveProtocolClicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, 
        "保存协议配置", 
        QString("protocol_%1.json").arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")),
        "JSON文件 (*.json)");
    
    if (!fileName.isEmpty()) {
        saveProtocolToFile(fileName);
    }
}

void Proctol_T::onLoadProtocolClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        "加载协议配置",
        "",
        "JSON文件 (*.json)");
    
    if (!fileName.isEmpty()) {
        loadProtocolFromFile(fileName);
    }
}

void Proctol_T::saveProtocolToFile(const QString& fileName)
{
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(_protocolTableView->model());
    if (!model) return;
    
    QJsonObject protocolObj;
    QJsonArray framesArray;
    
    // 遍历表格中的所有行
    for (int row = 0; row < model->rowCount(); ++row) {
        QJsonObject frameObj;
        frameObj["byte"] = model->item(row, 0)->text().toInt();
        frameObj["name"] = model->item(row, 1)->text();
        frameObj["dataType"] = model->item(row, 2)->text();
        frameObj["length"] = model->item(row, 3)->text().toInt();
        frameObj["remark"] = model->item(row, 4)->text();
        frameObj["frameType"] = model->item(row, 5)->text();
        frameObj["frameSettings"] = model->item(row, 6)->text();
        
        framesArray.append(frameObj);
    }
    
    protocolObj["frames"] = framesArray;
    protocolObj["version"] = "1.0";
    protocolObj["createdTime"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    
    QJsonDocument doc(protocolObj);
    
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
        QMessageBox::information(this, "保存成功", "协议配置已保存到: " + fileName);
    } else {
        QMessageBox::warning(this, "保存失败", "无法保存文件: " + fileName);
    }
}

void Proctol_T::loadProtocolFromFile(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "加载失败", "无法打开文件: " + fileName);
        return;
    }
    
    QByteArray data = file.readAll();
    file.close();
    
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        QMessageBox::warning(this, "加载失败", "无效的JSON文件");
        return;
    }
    
    QJsonObject protocolObj = doc.object();
    QJsonArray framesArray = protocolObj["frames"].toArray();
    
    // 清空当前表格
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(_protocolTableView->model());
    if (!model) return;
    
    model->removeRows(0, model->rowCount());
    
    // 加载配置到表格
    for (const QJsonValue& frameValue : framesArray) {
        QJsonObject frameObj = frameValue.toObject();
        
        QList<QStandardItem*> items;
        
        // 创建表格项并设置居中对齐
        QStandardItem* byteItem = new QStandardItem(QString::number(frameObj["byte"].toInt()));
        byteItem->setTextAlignment(Qt::AlignCenter);
        items << byteItem;
        
        QStandardItem* nameItem = new QStandardItem(frameObj["name"].toString());
        nameItem->setTextAlignment(Qt::AlignCenter);
        items << nameItem;
        
        QStandardItem* dataTypeItem = new QStandardItem(frameObj["dataType"].toString());
        dataTypeItem->setTextAlignment(Qt::AlignCenter);
        items << dataTypeItem;
        
        QStandardItem* lengthItem = new QStandardItem(QString::number(frameObj["length"].toInt()));
        lengthItem->setTextAlignment(Qt::AlignCenter);
        items << lengthItem;
        
        QStandardItem* remarkItem = new QStandardItem(frameObj["remark"].toString());
        remarkItem->setTextAlignment(Qt::AlignCenter);
        items << remarkItem;
        
        QStandardItem* frameTypeItem = new QStandardItem(frameObj["frameType"].toString());
        frameTypeItem->setTextAlignment(Qt::AlignCenter);
        items << frameTypeItem;
        
        // 兼容旧格式：先尝试读取frameSettings，如果没有则读取format
        QString frameSettings = frameObj["frameSettings"].toString();
        if (frameSettings.isEmpty()) {
            frameSettings = frameObj["format"].toString(); // 向后兼容
        }
        QStandardItem* frameSettingsItem = new QStandardItem(frameSettings);
        frameSettingsItem->setTextAlignment(Qt::AlignCenter);
        items << frameSettingsItem;
        
        model->appendRow(items);
    }
    
    QMessageBox::information(this, "加载成功", 
        QString("已从 %1 加载 %2 个帧配置").arg(fileName).arg(framesArray.size()));
}

// 计算当前字节位置（考虑已有数据的实际字节消耗）
int Proctol_T::calculateCurrentBytePosition(QStandardItemModel* model)
{
    int totalBytes = 0;
    for (int row = 0; row < model->rowCount(); ++row) {
        QStandardItem* lengthItem = model->item(row, 3); // 长度列
        if (lengthItem) {
            totalBytes += lengthItem->text().toInt();
        }
    }
    return totalBytes;
}

// 获取数据类型的字节长度
int Proctol_T::getDataTypeByteLength(const QString& dataType)
{
    if (dataType.contains("8")) {
        return 1; // u8, int8
    } else if (dataType.contains("16")) {
        return 2; // u16, int16
    } else if (dataType.contains("32")) {
        return 4; // u32, int32
    } else if (dataType == "float") {
        return 4;
    } else if (dataType == "double") {
        return 8;
    }
    return 1; // 默认1字节
}

// 验证数据范围
bool Proctol_T::validateDataRange(const QString& dataType, const QString& data)
{
    if (data.isEmpty()) {
        return true; // 空数据视为有效（会设置默认值）
    }
    
    bool ok;
    
    // 处理十六进制和十进制输入
    QString cleanData = data.trimmed().toUpper();
    qint64 value = 0;
    
    if (cleanData.startsWith("0X")) {
        value = cleanData.mid(2).toLongLong(&ok, 16);
    } else if (cleanData.startsWith("0x")) {
        value = cleanData.mid(2).toLongLong(&ok, 16);
    } else {
        value = data.toLongLong(&ok);
    }
    
    if (!ok) {
        // 对于float和double，检查是否为有效的浮点数
        if (dataType == "float" || dataType == "double") {
            data.toDouble(&ok);
            return ok;
        }
        return false;
    }
    
    // 检查数据类型范围
    if (dataType == "uint8") {
        return value >= 0 && value <= 255;
    } else if (dataType == "int8") {
        return value >= -128 && value <= 127;
    } else if (dataType == "uint16") {
        return value >= 0 && value <= 65535;
    } else if (dataType == "int16") {
        return value >= -32768 && value <= 32767;
    } else if (dataType == "uint32") {
        return value >= 0 && value <= 4294967295LL;
    } else if (dataType == "int32") {
        return value >= -2147483648LL && value <= 2147483647LL;
    }
    
    return true; // 其他类型默认有效
}

// 添加单字节行
void Proctol_T::addSingleByteRow(QStandardItemModel* model, int bytePosition, 
                                 const QString& name, const QString& dataType, 
                                 const QString& remark, const QString& frameType, 
                                 const QString& frameSettings)
{
    QList<QStandardItem*> items;
    
    // 字节位置
    QStandardItem* byteItem = new QStandardItem(QString::number(bytePosition));
    byteItem->setTextAlignment(Qt::AlignCenter);
    items << byteItem;
    
    // 名称
    QStandardItem* nameItem = new QStandardItem(name);
    nameItem->setTextAlignment(Qt::AlignCenter);
    items << nameItem;
    
    // 数据类型
    QStandardItem* dataTypeItem = new QStandardItem(dataType);
    dataTypeItem->setTextAlignment(Qt::AlignCenter);
    items << dataTypeItem;
    
    // 长度
    QStandardItem* lengthItem = new QStandardItem("1");
    lengthItem->setTextAlignment(Qt::AlignCenter);
    items << lengthItem;
    
    // 备注
    QStandardItem* remarkItem = new QStandardItem(remark);
    remarkItem->setTextAlignment(Qt::AlignCenter);
    items << remarkItem;
    
    // 帧类型
    QStandardItem* frameTypeItem = new QStandardItem(frameType);
    frameTypeItem->setTextAlignment(Qt::AlignCenter);
    items << frameTypeItem;
    
    // 帧设置
    QStandardItem* frameSettingsItem = new QStandardItem(frameSettings);
    frameSettingsItem->setTextAlignment(Qt::AlignCenter);
    items << frameSettingsItem;
    
    model->appendRow(items);
}

// 添加双字节行（高字节和低字节）
void Proctol_T::addTwoByteRows(QStandardItemModel* model, int bytePosition, 
                               const QString& name, const QString& dataType, 
                               const QString& remark, const QString& frameType, 
                               const QString& frameSettings)
{
    // 高字节行
    QList<QStandardItem*> highByteItems;
    
    QStandardItem* highByteItem = new QStandardItem(QString::number(bytePosition));
    highByteItem->setTextAlignment(Qt::AlignCenter);
    highByteItems << highByteItem;
    
    QStandardItem* highNameItem = new QStandardItem(name);
    highNameItem->setTextAlignment(Qt::AlignCenter);
    highByteItems << highNameItem;
    
    QString highDataType;
    if (dataType.contains("uint16")) {
        highDataType = "uint16_H";
    } else if (dataType.contains("int16")) {
        highDataType = "int16_H";
    } else {
        highDataType = dataType + "_H";
    }
    QStandardItem* highDataTypeItem = new QStandardItem(highDataType);
    highDataTypeItem->setTextAlignment(Qt::AlignCenter);
    highByteItems << highDataTypeItem;
    
    QStandardItem* highLengthItem = new QStandardItem("1");
    highLengthItem->setTextAlignment(Qt::AlignCenter);
    highByteItems << highLengthItem;
    
    QStandardItem* highRemarkItem = new QStandardItem(remark);
    highRemarkItem->setTextAlignment(Qt::AlignCenter);
    highByteItems << highRemarkItem;
    
    QStandardItem* highFrameTypeItem = new QStandardItem(frameType);
    highFrameTypeItem->setTextAlignment(Qt::AlignCenter);
    highByteItems << highFrameTypeItem;
    
    QStandardItem* highFrameSettingsItem = new QStandardItem(frameSettings);
    highFrameSettingsItem->setTextAlignment(Qt::AlignCenter);
    highByteItems << highFrameSettingsItem;
    
    model->appendRow(highByteItems);
    
    // 低字节行
    QList<QStandardItem*> lowByteItems;
    
    QStandardItem* lowByteItem = new QStandardItem(QString::number(bytePosition + 1));
    lowByteItem->setTextAlignment(Qt::AlignCenter);
    lowByteItems << lowByteItem;
    
    QStandardItem* lowNameItem = new QStandardItem(name);
    lowNameItem->setTextAlignment(Qt::AlignCenter);
    lowByteItems << lowNameItem;
    
    QString lowDataType;
    if (dataType.contains("uint16")) {
        lowDataType = "uint16_L";
    } else if (dataType.contains("int16")) {
        lowDataType = "int16_L";
    } else {
        lowDataType = dataType + "_L";
    }
    QStandardItem* lowDataTypeItem = new QStandardItem(lowDataType);
    lowDataTypeItem->setTextAlignment(Qt::AlignCenter);
    lowByteItems << lowDataTypeItem;
    
    QStandardItem* lowLengthItem = new QStandardItem("1");
    lowLengthItem->setTextAlignment(Qt::AlignCenter);
    lowByteItems << lowLengthItem;
    
    QStandardItem* lowRemarkItem = new QStandardItem(remark);
    lowRemarkItem->setTextAlignment(Qt::AlignCenter);
    lowByteItems << lowRemarkItem;
    
    QStandardItem* lowFrameTypeItem = new QStandardItem(frameType);
    lowFrameTypeItem->setTextAlignment(Qt::AlignCenter);
    lowByteItems << lowFrameTypeItem;
    
    QStandardItem* lowFrameSettingsItem = new QStandardItem(frameSettings);
    lowFrameSettingsItem->setTextAlignment(Qt::AlignCenter);
    lowByteItems << lowFrameSettingsItem;
    
    model->appendRow(lowByteItems);
}

// 添加四字节行
void Proctol_T::addFourByteRows(QStandardItemModel* model, int bytePosition, 
                                const QString& name, const QString& dataType, 
                                const QString& remark, const QString& frameType, 
                                const QString& frameSettings)
{
    // 对于float、uint32、int32类型，名称和备注处理不同
    bool isStandardType = (dataType == "float" || dataType == "uint32" || dataType == "int32");
    
    for (int i = 0; i < 4; ++i) {
        QList<QStandardItem*> items;
        
        QStandardItem* byteItem = new QStandardItem(QString::number(bytePosition + i));
        byteItem->setTextAlignment(Qt::AlignCenter);
        items << byteItem;
        
        // 名称：标准类型不添加字节后缀，其他类型添加
        QString itemName = isStandardType ? name : QString("%1_B%2").arg(name).arg(i);
        QStandardItem* nameItem = new QStandardItem(itemName);
        nameItem->setTextAlignment(Qt::AlignCenter);
        items << nameItem;
        
        QString byteDataType = QString("%1_B%2").arg(dataType).arg(i);
        QStandardItem* dataTypeItem = new QStandardItem(byteDataType);
        dataTypeItem->setTextAlignment(Qt::AlignCenter);
        items << dataTypeItem;
        
        QStandardItem* lengthItem = new QStandardItem("1");
        lengthItem->setTextAlignment(Qt::AlignCenter);
        items << lengthItem;
        
        // 备注：标准类型不添加字节后缀，其他类型添加
        QString itemRemark = isStandardType ? remark : QString("%1(字节%2)").arg(remark).arg(i);
        QStandardItem* remarkItem = new QStandardItem(itemRemark);
        remarkItem->setTextAlignment(Qt::AlignCenter);
        items << remarkItem;
        
        QStandardItem* frameTypeItem = new QStandardItem(frameType);
        frameTypeItem->setTextAlignment(Qt::AlignCenter);
        items << frameTypeItem;
        
        QStandardItem* frameSettingsItem = new QStandardItem(frameSettings);
        frameSettingsItem->setTextAlignment(Qt::AlignCenter);
        items << frameSettingsItem;
        
        model->appendRow(items);
    }
}

// 添加八字节行
void Proctol_T::addEightByteRows(QStandardItemModel* model, int bytePosition, 
                                 const QString& name, const QString& dataType, 
                                 const QString& remark, const QString& frameType, 
                                 const QString& frameSettings)
{
    // 对于double类型，名称和备注处理不同
    bool isDoubleType = (dataType == "double");
    
    for (int i = 0; i < 8; ++i) {
        QList<QStandardItem*> items;
        
        QStandardItem* byteItem = new QStandardItem(QString::number(bytePosition + i));
        byteItem->setTextAlignment(Qt::AlignCenter);
        items << byteItem;
        
        // 名称：double类型不添加字节后缀，其他类型添加
        QString itemName = isDoubleType ? name : QString("%1_B%2").arg(name).arg(i);
        QStandardItem* nameItem = new QStandardItem(itemName);
        nameItem->setTextAlignment(Qt::AlignCenter);
        items << nameItem;
        
        QString byteDataType = QString("%1_B%2").arg(dataType).arg(i);
        QStandardItem* dataTypeItem = new QStandardItem(byteDataType);
        dataTypeItem->setTextAlignment(Qt::AlignCenter);
        items << dataTypeItem;
        
        QStandardItem* lengthItem = new QStandardItem("1");
        lengthItem->setTextAlignment(Qt::AlignCenter);
        items << lengthItem;
        
        // 备注：double类型不添加字节后缀，其他类型添加
        QString itemRemark = isDoubleType ? remark : QString("%1(字节%2)").arg(remark).arg(i);
        QStandardItem* remarkItem = new QStandardItem(itemRemark);
        remarkItem->setTextAlignment(Qt::AlignCenter);
        items << remarkItem;
        
        QStandardItem* frameTypeItem = new QStandardItem(frameType);
        frameTypeItem->setTextAlignment(Qt::AlignCenter);
        items << frameTypeItem;
        
        QStandardItem* frameSettingsItem = new QStandardItem(frameSettings);
        frameSettingsItem->setTextAlignment(Qt::AlignCenter);
        items << frameSettingsItem;
        
        model->appendRow(items);
    }
}
