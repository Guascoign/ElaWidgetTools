#include "FFT_T.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "ElaApplication.h"
#include "ElaWindow.h"
//控件
#include "ElaText.h"
FFT_T::FFT_T(QWidget* parent)
    : T_BasePage(parent)
{
ElaWindow* window = dynamic_cast<ElaWindow*>(parent);

    // 预览窗口标题
    setWindowTitle("FFT");
    // 顶部元素
    createCustomWidget("FFT 计算");

    // 顶部元素
   // createCustomWidget("串口连接");

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("FFT");
    QVBoxLayout* centerLayout = new QVBoxLayout(centralWidget);
    centerLayout->setContentsMargins(0, 0, 0, 0);
    addCentralWidget(centralWidget, true, true, 0);

    // 数据统计
    ElaText* dataText = new ElaText(QString("待编写"), this);
    ElaText* dataText1 = new ElaText(QString("TODO...."), this);
    // 设置文本样式
    dataText->setTextPixelSize(15);
    dataText1->setTextPixelSize(15);
    //添加到布局
    centerLayout->addWidget(dataText);
    centerLayout->addWidget(dataText1);
    centerLayout->addStretch();
}

FFT_T::~FFT_T()
{
}
