#ifndef PROTOCOLBASEWIDGET_H
#define PROTOCOLBASEWIDGET_H

#include <QWidget>

class ProtocolBaseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ProtocolBaseWidget(QWidget* parent = nullptr) : QWidget(parent) {}
    virtual ~ProtocolBaseWidget() {}

public slots:
    virtual void SerialConnected(bool connected) = 0;
    virtual void SerialDataReceived(QByteArray data) = 0;

signals:
    void Proctol_out(const QByteArray &data);
};

#endif // PROTOCOLBASEWIDGET_H