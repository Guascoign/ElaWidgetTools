#ifndef CALCULATOR_T_H
#define CALCULATOR_T_H

#include "T_BasePage.h"
class Calculator_T : public T_BasePage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit Calculator_T(QWidget* parent = nullptr);
    ~Calculator_T();

private:


};

#endif // CALCULATOR_T_H
