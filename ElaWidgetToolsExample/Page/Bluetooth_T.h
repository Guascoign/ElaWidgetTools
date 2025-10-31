#ifndef BLUETOOTH_T_H
#define BLUETOOTH_T_H

#include "T_BasePage.h"
#include <QWidget>
#include <QQueue>
#include <QThread>
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

class ElaComboBox;
class ElaPushButton;
class ElaToggleButton;
class ElaPlainTextEdit;
class ElaSpinBox;
class ElaText;
class Bluetooth_T : public T_BasePage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit Bluetooth_T(QWidget* parent = nullptr);
    ~Bluetooth_T();

private:


};

#endif // BLUETOOTH_T_H
