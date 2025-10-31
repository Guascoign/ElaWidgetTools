#ifndef T_LOGMODEL_H
#define T_LOGMODEL_H

#include <QAbstractListModel>
#include <QDateTime>

enum LogDisplayMode {
    Default,     // 默认模式，显示时间+函数+内容
    Simple,      // 只显示内容
    WithTime,    // 显示时间+内容
    WithFuncInfo // 显示时间+函数+内容
};

class T_LogModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit T_LogModel(QObject* parent = nullptr);
    ~T_LogModel();
    void setLogList(QStringList list);
    void appendLogList(QString log);
    void prependLogList(QString log);  // 添加到头部
    QStringList getLogList() const;
    void setDisplayMode(LogDisplayMode mode) { _displayMode = mode; }
    LogDisplayMode displayMode() const { return _displayMode; }
signals:
    void logDisplayModeChanged(LogDisplayMode mode);
protected:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;

private:
    QStringList _logList;
    LogDisplayMode _displayMode = Simple;
};

#endif // T_LOGMODEL_H
