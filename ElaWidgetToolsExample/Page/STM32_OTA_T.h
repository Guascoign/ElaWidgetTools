#ifndef STM32_OTA_T_H
#define STM32_OTA_T_H

#include "T_BasePage.h"
class STM32_OTA_T : public T_BasePage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit STM32_OTA_T(QWidget* parent = nullptr);
    ~STM32_OTA_T();

private:


};

#endif // STM32_OTA_T_H
