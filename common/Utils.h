#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QTime>
#include <QTextEdit>

namespace Utils {
    inline QTextEdit* logWidget = nullptr;

    inline QString getCurrentTime() {
        return QTime::currentTime().toString("hh:mm:ss");
    }

    inline void log(const QString& msg) {
        QString logMsg = getCurrentTime() + " " + msg;

        qDebug() << logMsg;
        if (logWidget)
            logWidget->append(logMsg);
    }
};



#endif // UTILS_H