#include "T_LogModel.h"

T_LogModel::T_LogModel(QObject* parent)
    : QAbstractListModel{parent}
{
}

T_LogModel::~T_LogModel()
{
}

int T_LogModel::rowCount(const QModelIndex& parent) const
{
    return this->_logList.count();
}

// QVariant T_LogModel::data(const QModelIndex& index, int role) const
// {
//     if (role == Qt::DisplayRole)
//     {
//         return _logList[index.row()];
//     }
//     return QVariant();
// }

QVariant T_LogModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        // 日志格式: [时间](函数: 函数名, 行数: 行号) -> 内容
        QString log = _logList[index.row()];

        // 提取时间
        int timeStart = log.indexOf('[') + 1;
        int timeEnd = log.indexOf(']');
        QString time = (timeStart > 0 && timeEnd > timeStart) ? log.mid(timeStart, timeEnd - timeStart) : "";

        // 提取函数
        int funcStart = log.indexOf("(函数:") + 4;
        int funcEnd = log.indexOf(')', funcStart);
        QString func;
        if (funcStart >= 4 && funcEnd > funcStart) {
            func = log.mid(funcStart, funcEnd - funcStart);
        }

        // 提取内容
        int arrow = log.indexOf("->");
        QString info = (arrow != -1) ? log.mid(arrow + 2).trimmed() : log;

        // 根据显示模式返回
        switch (_displayMode) {
        case Default:
            return log;
        case Simple:
            return info;
        case WithTime:
            return QString("[%1] %2").arg(time, info);
        case WithFuncInfo:
            return QString("[%1] [%2] %3").arg(time, func, info);
        default:
            return log;
        }
    }
    return QVariant();
}

void T_LogModel::setLogList(QStringList list)
{
    beginResetModel();
    this->_logList = list;
    endResetModel();
}

void T_LogModel::appendLogList(QString log)
{
    beginResetModel();
    this->_logList.append(log);
    endResetModel();
}

void T_LogModel::prependLogList(QString log)
{
    beginResetModel();
    this->_logList.prepend(log);
    endResetModel();
}

QStringList T_LogModel::getLogList() const
{
    return this->_logList;
}
