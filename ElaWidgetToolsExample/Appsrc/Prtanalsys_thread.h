/********************************************************************************
    * 文件名称 ：Prtanalsys_thread.h
    * 版     本：V1.0
    * 编写日期 ：2025-08-20
    * 功     能：串口数据解析线程
*********************************************************************************
V1.0 2025-08-20 First release @ZM
*********************************************************************************/

#ifndef PRTANALSYS_THREAD_H
#define PRTANALSYS_THREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QQueue>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>

class Prtanalsys_thread : public QThread
{
    Q_OBJECT

public:
    explicit Prtanalsys_thread();
    ~Prtanalsys_thread();

};

#endif // PRTANALSYS_THREAD_H