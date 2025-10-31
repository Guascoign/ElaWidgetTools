#include "Bluetooth_T.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "ElaApplication.h"
#include "ElaWindow.h"
#include "ElaComboBox.h"
#include "ElaPushButton.h"
#include "ElaToggleButton.h"
#include "ElaPlainTextEdit.h"
#include "ElaText.h"

Bluetooth_T::Bluetooth_T(QWidget* parent)
    : T_BasePage(parent)
{
    ElaWindow* window = dynamic_cast<ElaWindow*>(parent);

    setWindowTitle("Bluetooth");
    createCustomWidget("蓝牙助手");

    // 主体水平布局
    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("Bluetooth");
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(8, 8, 8, 8);
    mainLayout->setSpacing(12);

    // 左侧参数设置区
    QWidget* leftWidget = new QWidget(centralWidget);
    leftWidget->setObjectName("bluetoothLeftPanel");
    QVBoxLayout* leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->setSpacing(12);

    // 设备选择
    ElaText* deviceLabel = new ElaText("蓝牙设备", leftWidget);
    deviceLabel->setTextPixelSize(12);
    ElaComboBox* deviceCombo = new ElaComboBox(leftWidget);
    ElaPushButton* scanBtn = new ElaPushButton("扫描", leftWidget);
    scanBtn->setFixedHeight(28);
    QHBoxLayout* deviceLayout = new QHBoxLayout();
    deviceLayout->addWidget(deviceLabel);
    deviceLayout->addStretch();
    deviceLayout->addWidget(scanBtn);
    leftLayout->addLayout(deviceLayout);
    leftLayout->addWidget(deviceCombo);

    // 服务选择
    ElaText* serviceLabel = new ElaText("服务", leftWidget);
    serviceLabel->setTextPixelSize(12);
    ElaComboBox* serviceCombo = new ElaComboBox(leftWidget);
    leftLayout->addWidget(serviceLabel);
    leftLayout->addWidget(serviceCombo);

    // 特征选择
    ElaText* charLabel = new ElaText("特征", leftWidget);
    charLabel->setTextPixelSize(12);
    ElaComboBox* charCombo = new ElaComboBox(leftWidget);
    leftLayout->addWidget(charLabel);
    leftLayout->addWidget(charCombo);

    // 间距
    leftLayout->addSpacing(20);

    // 连接按钮
    ElaToggleButton* connectBtn = new ElaToggleButton("连接", leftWidget);
    connectBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    connectBtn->setFixedHeight(40);
    connectBtn->setIsToggled(false);
    connectBtn->setText("连接");
    connect(connectBtn, &ElaToggleButton::toggled, [=](bool checked) {
        if (checked) {
            connectBtn->setText("断开");
        } else {
            connectBtn->setText("连接");
        }
    });
    leftLayout->addWidget(connectBtn);

    leftLayout->addStretch();

    // 底部按钮区
    QHBoxLayout* bottomBtnLayout = new QHBoxLayout();
    ElaPushButton* docBtn = new ElaPushButton(leftWidget);
    docBtn->setIcon(QIcon(":/icons/doc.svg"));
    docBtn->setFixedSize(28, 28);
    bottomBtnLayout->addStretch();
    bottomBtnLayout->addWidget(docBtn);
    leftLayout->addLayout(bottomBtnLayout);

    leftWidget->setFixedWidth(260);
    mainLayout->addWidget(leftWidget);

    // 右侧收发显示区
    QWidget* rightWidget = new QWidget(centralWidget);
    QVBoxLayout* rightLayout = new QVBoxLayout(rightWidget);
    rightLayout->setSpacing(10);

    // 顶部大文本框
    ElaPlainTextEdit* recvEdit = new ElaPlainTextEdit(rightWidget);
    recvEdit->setPlaceholderText("数据接收区...");
    rightLayout->addWidget(recvEdit, 5);

    // 中部状态区
    QHBoxLayout* statusLayout = new QHBoxLayout();
    ElaText* statusLabel = new ElaText("状态: 未连接", rightWidget);
    statusLabel->setTextPixelSize(15);
    ElaText* rxLabel = new ElaText("Rx: 0 Bytes", rightWidget);
    ElaText* txLabel = new ElaText("Tx: 0 Bytes", rightWidget);
    rxLabel->setTextPixelSize(15);
    txLabel->setTextPixelSize(15);
    statusLayout->addWidget(statusLabel);
    statusLayout->addWidget(rxLabel);
    statusLayout->addWidget(txLabel);
    statusLayout->addStretch();
    rightLayout->addLayout(statusLayout);

    // 底部发送区
    QWidget* sendArea = new QWidget(rightWidget);
    QVBoxLayout* sendVLayout = new QVBoxLayout(sendArea);
    sendVLayout->setContentsMargins(0, 0, 0, 12);
    sendVLayout->setSpacing(0);

    // 编辑框和右下角按钮
    QWidget* sendStack = new QWidget(sendArea);
    sendStack->setAttribute(Qt::WA_StyledBackground);
    QGridLayout* stackGrid = new QGridLayout(sendStack);
    stackGrid->setContentsMargins(0, 0, 0, 0);
    stackGrid->setSpacing(0);

    ElaPlainTextEdit* sendEdit = new ElaPlainTextEdit(sendStack);
    sendEdit->setPlaceholderText("输入要发送的数据...");
    stackGrid->addWidget(sendEdit, 0, 0);

    // 右下角按钮层
    QWidget* btnFloatWidget = new QWidget(sendStack);
    btnFloatWidget->setAttribute(Qt::WA_TranslucentBackground);
    QHBoxLayout* btnFloatLayout = new QHBoxLayout(btnFloatWidget);
    btnFloatLayout->addStretch();
    ElaToggleButton* autoSendBtn = new ElaToggleButton("自动发送", btnFloatWidget);
    ElaPushButton* sendBtn = new ElaPushButton("发送", btnFloatWidget);
    sendBtn->setFixedSize(80, 36);
    autoSendBtn->setFixedSize(80, 36);
    btnFloatLayout->addWidget(autoSendBtn, 0, Qt::AlignRight | Qt::AlignBottom);
    btnFloatLayout->addWidget(sendBtn, 0, Qt::AlignRight | Qt::AlignBottom);
    btnFloatLayout->setContentsMargins(0, 0, 10, 8);
    btnFloatLayout->setSpacing(4);
    btnFloatLayout->setAlignment(Qt::AlignRight | Qt::AlignBottom);
    btnFloatWidget->setLayout(btnFloatLayout);

    stackGrid->addWidget(btnFloatWidget, 0, 0, Qt::AlignRight | Qt::AlignBottom);

    sendVLayout->addWidget(sendStack);

    rightLayout->addWidget(sendArea, 1);

    mainLayout->addWidget(rightWidget, 1);

    addCentralWidget(centralWidget, true, true, 0);
}

Bluetooth_T::~Bluetooth_T()
{
}