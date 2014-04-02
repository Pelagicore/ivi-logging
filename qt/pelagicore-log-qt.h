#pragma once

#include <QtGlobal>
#include <QDebug>

#include "pelagicore-log.h"

namespace pelagicore {

/**
 * The constructor of this class registers a Qt log message handler which redirects logs to pelagicore logging
 */
template<LogContext* context> class LogQtMessageHandler {

	LOG_SET_CLASS_CONTEXT(*context);

public:
	LogQtMessageHandler() {
		qInstallMessageHandler(qtToLogMessageHandler);
	}

	static void qtToLogMessageHandler(QtMsgType type, const QMessageLogContext& logContext,
					  const QString& s) {
		switch (type) {
		case QtDebugMsg :
			log_debug() << s;
			break;
		case QtWarningMsg :
			log_warning() << s;
			break;
		case QtCriticalMsg :
			log_warn() << s;
			break;
		default :
			log_error() << s;
		}
	}

};

#ifdef ENABLE_CONSOLE_LOGGING
template<typename T>
ConsoleLogData& operator<<(ConsoleLogData& data, const T& value) {
	QString s;
	QDebug d(&s);
	d << value;
	data << s.toStdString().c_str();
	return data;
}
#endif

#ifdef ENABLE_DLT_LOGGING
template<typename T>
DltLogData& operator<<(DltLogData& data, const T& value) {
	QString s;
	QDebug d(&s);
	d << value;
	data << s.toStdString().c_str();
	return data;
}
#endif

}
