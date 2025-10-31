#include "FPGA_OTA_T.h"

#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QVBoxLayout>
#include <QCryptographicHash>
#include <QElapsedTimer>
#include "ElaApplication.h"
#include "ElaWindow.h"
#include "ElaMessageBar.h"
#include "ElaProgressBar.h"



//控件
#include "ElaText.h"
FPGA_OTA_T::FPGA_OTA_T(QWidget* parent)
    : T_BasePage(parent)
{
    ElaWindow* window = dynamic_cast<ElaWindow*>(parent);

    // 预览窗口标题
    setWindowTitle("FPGA");
    createCustomWidget("FPGA 升级");

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("FPGA");
    QVBoxLayout* centerLayout = new QVBoxLayout(centralWidget);
    centerLayout->setContentsMargins(10, 10, 10, 10);

    // *******************界面元素**********************************
    Select_File = new ElaPushButton("选择文件", this);
    Erease = new ElaPushButton("擦除", this); // 擦除按钮
    Erease->setToolTip("擦除FPGA数据，发送命令：AA01 BB02 CC33");
    ReadBack = new ElaPushButton("回读", this); // 回读按钮
    ReadBack->setToolTip("回读FPGA数据，发送命令：AA01 BB02 CC22");
    proctolToggle = new ElaToggleButton("连接串口", this);
    Start_OTA = new ElaPushButton("开始升级", this);
    Start_OTA->setToolTip("开始FPGA升级，发送AA01 BB02 CC11后发文件数据");
    File_name = new ElaText("文件名: 未选择", this);
    File_size = new ElaText("文件大小: 0 Bytes", this);
    File_path = new ElaText("文件路径：N/A", this);
    File_md5 = new ElaText("MD5: N/A", this);
    ReadBack_Bytes = new ElaText("回读字节数: N/A", this);
    ReadBack_MD5 = new ElaText("回读MD5: N/A", this);
    ReadBack_File_MD5 = new ElaText("回读文件MD5: N/A", this);
    progressBar = new ElaProgressBar(this);
    progressBar->setMinimum(0);
    progressBar->setMaximum(100);
    progressBar->setValue(0);

    // 设置文本样式
    File_name->setTextPixelSize(14);
    File_size->setTextPixelSize(14);
    File_path->setTextPixelSize(14);
    File_md5->setTextPixelSize(14);
    ReadBack_Bytes->setTextPixelSize(14);
    ReadBack_MD5->setTextPixelSize(14);
    ReadBack_File_MD5->setTextPixelSize(14);

    // 按钮横向布局
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addWidget(Select_File);
    btnLayout->addWidget(proctolToggle);
    btnLayout->addWidget(Start_OTA);
    btnLayout->addWidget(Erease);
    btnLayout->addWidget(ReadBack);

    // 添加到主布局
    centerLayout->addLayout(btnLayout);
    centerLayout->addWidget(File_name);
    centerLayout->addWidget(File_size);
    centerLayout->addWidget(File_path);
    centerLayout->addWidget(File_md5);
    centerLayout->addWidget(progressBar);
    centerLayout->addWidget(ReadBack_Bytes);
    centerLayout->addWidget(ReadBack_MD5);
    centerLayout->addWidget(ReadBack_File_MD5);
    centerLayout->addStretch();

    // 添加回读头设置
    QHBoxLayout* readHeadLayout = new QHBoxLayout();

    readHeadLayout->addStretch();
    centerLayout->addLayout(readHeadLayout);

    addCentralWidget(centralWidget, true, true, 0);

    // 选择文件按钮槽
    connect(Select_File, &ElaPushButton::clicked, this, [=]() {
        QString filePath = QFileDialog::getOpenFileName(this, "选择FPGA升级文件", QString(), "All Files (*.*)");
        if (!filePath.isEmpty()) {
            QFileInfo info(filePath);
            File_name->setText("文件名: " + info.fileName());
            File_size->setText(QString("文件大小: %1 Bytes").arg(info.size()));
            File_path->setText("文件路径：" + filePath);
            ElaMessageBar::information(ElaMessageBarType::TopRight, "选择文件", info.fileName(), 1500);
            // 计算MD5
            QFile file(filePath);
            QString md5Str = "N/A";
            if (file.open(QIODevice::ReadOnly)) {
                QCryptographicHash hash(QCryptographicHash::Md5);
                if (hash.addData(&file)) {
                    md5Str = hash.result().toHex().toUpper();
                }
                file.close();
            }
            File_md5->setText("MD5: " + md5Str);
            // 设置进度条
            progressBar->setMinimum(0);
            progressBar->setMaximum(info.size());
            progressBar->setValue(0);
        }
    });

    // 回读按钮点击
    connect(ReadBack, &ElaPushButton::clicked, this, [=]() {
        QByteArray readCmd;
        readCmd.append(static_cast<char>(0xAA));
        readCmd.append(static_cast<char>(0x01));
        readCmd.append(static_cast<char>(0xBB));
        readCmd.append(static_cast<char>(0x02));
        readCmd.append(static_cast<char>(0xCC));
        readCmd.append(static_cast<char>(0x22));
        emit sendSerialData(readCmd);
        qDebug() << "[FPGA_OTA_T] ReadBack clicked, send: AA01 BB02 CC22";
    });


    // 构造函数中全部失能
    Start_OTA->setEnabled(false);
    Erease->setEnabled(false);
    proctolToggle->setEnabled(false);
    ReadBack->setEnabled(false);

}

FPGA_OTA_T::~FPGA_OTA_T()
{
}

void FPGA_OTA_T::setupUartConnections(Uart_T* p_uartObj)
{
    this->uartObj = p_uartObj;
    Uart_T* pUart = this->uartObj;
    FPGA_OTA_T* pFPGAT = this;

    // 连接串口打开信号
    connect(pUart, &Uart_T::Com_Opened, this, [=](const QString& portName) {
        qDebug() << "[FPGA_OTA_T] Serial port opened:" << portName;
        if (proctolToggle) proctolToggle->setEnabled(true); // 只使能协议连接开关
        if (Start_OTA) Start_OTA->setEnabled(false);
        if (Erease) Erease->setEnabled(false);
        if (ReadBack) ReadBack->setEnabled(true); // 使能回读按钮
        Com_Opened_flag = true;
    });

    // 连接串口关闭信号
    connect(pUart, &Uart_T::Com_Closed, this, [=]() {
        qDebug() << "[FPGA_OTA_T] Serial port closed";
        if (proctolToggle) proctolToggle->setEnabled(false);
        if (Start_OTA) Start_OTA->setEnabled(false);
        if (Erease) Erease->setEnabled(false);
        if (ReadBack) ReadBack->setEnabled(false); // 禁用回读按钮
        Com_Opened_flag = false;
    });

    // 协议连接开关控制发送和擦除按钮
    connect(proctolToggle, &ElaToggleButton::toggled, this, [=](bool checked){
        if (checked) {
            if (Start_OTA) Start_OTA->setEnabled(true);
            if (Erease) Erease->setEnabled(true);                
            connect(pFPGAT, &FPGA_OTA_T::sendSerialData, pUart, &Uart_T::Send_Data, Qt::UniqueConnection);
            connect(pUart, &Uart_T::RecvData, pFPGAT, &FPGA_OTA_T::onSerialDataReceived, Qt::UniqueConnection);
            ElaMessageBar::success(ElaMessageBarType::TopRight, "输出已连接", "Done！", 1500);
        } else {
            if (Start_OTA) Start_OTA->setEnabled(false);
            if (Erease) Erease->setEnabled(false);
            connect(pFPGAT, &FPGA_OTA_T::sendSerialData, pUart, &Uart_T::Send_Data, Qt::UniqueConnection);
            connect(pUart, &Uart_T::RecvData, pFPGAT, &FPGA_OTA_T::onSerialDataReceived, Qt::UniqueConnection);
            ElaMessageBar::success(ElaMessageBarType::TopRight, "输出已断开", "Done！", 1500);
        }
    });

        connect(Erease, &ElaPushButton::clicked, this, [=]() {
            QByteArray eraseCmd;
            eraseCmd.append(static_cast<char>(0xAA));
            eraseCmd.append(static_cast<char>(0x01));
            eraseCmd.append(static_cast<char>(0xBB));
            eraseCmd.append(static_cast<char>(0x02));
            eraseCmd.append(static_cast<char>(0xCC));
            eraseCmd.append(static_cast<char>(0x33));
            //if (pUart) pUart->Send_Data(eraseCmd); // 通过串口模块转发到发送线程
            emit sendSerialData(eraseCmd); // 发射信号 发送线程直接发送
            qDebug() << "[FPGA_OTA_T] Erease clicked, send: AA01 BB02 CC33";
        });

        connect(Start_OTA, &ElaPushButton::clicked, this, [=]() {
            QString filePath = File_path ? File_path->text().mid(QString("文件路径：").length()) : QString();
            if (filePath.isEmpty() || filePath == "N/A") {
                ElaMessageBar::error(ElaMessageBarType::TopRight, "传输失败！", "未选择文件", 1500);
                return;
            }
            QFile file(filePath);
            if (!file.open(QIODevice::ReadOnly)) {
                ElaMessageBar::error(ElaMessageBarType::TopRight, "传输失败！", "文件打开失败", 1500);
                return;
            }
            Start_OTA->setEnabled(false);
            // 发送AA 01
            QByteArray startCmd;
            startCmd.append(static_cast<char>(0xAA));
            startCmd.append(static_cast<char>(0x01));
            startCmd.append(static_cast<char>(0xBB));
            startCmd.append(static_cast<char>(0x02));
            startCmd.append(static_cast<char>(0xCC));
            startCmd.append(static_cast<char>(0x11));
            //if (pUart) pUart->Send_Data(startCmd);//通过串口模块转发到发送线程
            emit sendSerialData(startCmd); // 发射信号 发送线程直接发送
            QThread::msleep(10);

            QElapsedTimer timer;
            timer.start();
            const int chunkSize = 4096;
            qint64 totalBytes = file.size();
            qint64 sentBytes = 0;
            progressBar->setMinimum(0);
            progressBar->setMaximum(totalBytes);
            progressBar->setValue(0);

            while (!file.atEnd()) {
                QByteArray chunk = file.read(chunkSize);
                //if (pUart) pUart->Send_Data(chunk);
                emit sendSerialData(chunk); // 发射信号 发送线程直接发送
                sentBytes += chunk.size();
                progressBar->setValue(sentBytes);
                QThread::msleep(10);
            }
            file.close();
            progressBar->setValue(totalBytes); // 发送完成，进度条100%
            QFileInfo info(filePath);
            double seconds = timer.elapsed() / 1000.0;
            ElaMessageBar::success(
                ElaMessageBarType::TopRight,
                "传输完成！",
                QString("%1\n耗时：%2 秒").arg(info.fileName()).arg(seconds, 0, 'f', 2),
                2000
            );
            Start_OTA->setEnabled(true);
        });
}

void FPGA_OTA_T::onSerialDataReceived(QByteArray data)
{
    static QByteArray readBackBuffer;
    static bool isReading = false;

    // 检查是否为回读命令响应（6字节头部：AA 01 BB 02 CC 22）
    if (!isReading && data.size() >= 2) {
        // 判断是否为6字节回读头
        if (static_cast<uint8_t>(data[0]) == 0xAA &&
            static_cast<uint8_t>(data[1]) == 0x04 ) {
            
            readBackBuffer.clear();
            isReading = true;
            data = data.mid(2); // 去掉6字节回读头
            qDebug() << "[FPGA_OTA_T] 检测到2字节回读头：AA 04，开始接收数据";
        }
    }

    if (isReading) {
        readBackBuffer.append(data);

        // 显示回读字节数
        if (ReadBack_Bytes) {
            ReadBack_Bytes->setText(QString("回读字节数: %1").arg(readBackBuffer.size()));
        }

        // 显示回读数据的MD5
        if (ReadBack_MD5) {
            QCryptographicHash hash(QCryptographicHash::Md5);
            hash.addData(readBackBuffer);
            ReadBack_MD5->setText("回读MD5: " + hash.result().toHex().toUpper());
        }

        // 获取原文件大小
        qint64 fileSize = 0;
        if (File_size) {
            QString sizeStr = File_size->text().mid(QString("文件大小: ").length());
            sizeStr = sizeStr.left(sizeStr.indexOf(" "));
            fileSize = sizeStr.toLongLong();
        }

        // 取前fileSize字节计算MD5
        if (fileSize > 0 && readBackBuffer.size() >= fileSize) {
            QByteArray fileData = readBackBuffer.left(fileSize);
            QCryptographicHash hash(QCryptographicHash::Md5);
            hash.addData(fileData);
            QString fileMd5 = hash.result().toHex().toUpper();
            if (ReadBack_File_MD5) {
                ReadBack_File_MD5->setText("回读文件MD5: " + fileMd5);
            }

            // 比较MD5
            QString origMd5 = File_md5 ? File_md5->text().mid(QString("MD5: ").length()) : "";
            if (fileMd5 == origMd5) {
                ElaMessageBar::success(ElaMessageBarType::TopRight, "校验通过", "回读数据MD5校验通过！", 2000);
                qDebug() << "[FPGA_OTA_T] MD5校验通过";
            } else {
                ElaMessageBar::error(ElaMessageBarType::TopRight, "校验失败", 
                    QString("回读数据MD5校验失败！\n原文件MD5: %1\n回读MD5: %2").arg(origMd5).arg(fileMd5), 3000);
                qDebug() << "[FPGA_OTA_T] MD5校验失败 - 原文件:" << origMd5 << "回读:" << fileMd5;
            }
            isReading = false; // 结束本次回读
        }
    }
}
