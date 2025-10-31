/********************************************************************************
    * 文件名称 ：Prt_KF30.h
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2025-06-06
    * 功     能：通信协议 界面
*********************************************************************************/
#ifndef _PRT_KF30_H
#define _PRT_KF30_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

class Prt_KF30 : public QWidget
{
    Q_OBJECT
public:

    explicit Prt_KF30(QWidget *parent = nullptr);
    ~Prt_KF30();

signals:

public slots:

};


#endif // _PRT_KF30_H
