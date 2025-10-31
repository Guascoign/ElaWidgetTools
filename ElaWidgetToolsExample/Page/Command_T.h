#ifndef COMMAND_T_H
#define COMMAND_T_H

#include "T_BasePage.h"
class Command_T : public T_BasePage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit Command_T(QWidget* parent = nullptr);
    ~Command_T();

private:


};

#endif // COMMAND_T_H
