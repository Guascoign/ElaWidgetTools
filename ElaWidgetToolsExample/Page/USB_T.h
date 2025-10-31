#ifndef USB_T_H
#define USB_T_H

#include "T_BasePage.h"
class USB_T : public T_BasePage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit USB_T(QWidget* parent = nullptr);
    ~USB_T();

private:


};

#endif // USB_T_H
