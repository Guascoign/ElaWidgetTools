#include "UnitConvert_T.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "ElaApplication.h"
#include "ElaWindow.h"
//控件
#include "ElaText.h"
UnitConvert_T::UnitConvert_T(QWidget* parent)
    : T_BasePage(parent)
{
ElaWindow* window = dynamic_cast<ElaWindow*>(parent);

    // 预览窗口标题
    setWindowTitle("Unit Convert");
    // 顶部元素
    createCustomWidget("单位转换");

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("Unit Convert");
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

UnitConvert_T::~UnitConvert_T()
{
}
