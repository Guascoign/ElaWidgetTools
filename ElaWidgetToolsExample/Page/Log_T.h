#ifndef LOG_T_H
#define LOG_T_H

#include "T_BasePage.h"
class Log_T : public T_BasePage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit Log_T(QWidget* parent = nullptr);
    ~Log_T();

private:


};

#endif // LOG_T_H
