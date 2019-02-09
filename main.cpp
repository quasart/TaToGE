#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>

#include <QFile>
#include <QStandardPaths>
#include <QColor>
#include <QDebug>

#include "MainWindow.h"
#include "AppOptions.h"

int main( int argc, char **argv )
{
	qputenv("QT_STYLE_OVERRIDE",""); // to workaround warning because gtk style is not handled.
	qputenv("NO_AT_BRIDGE", "1"); // workaround too

	QApplication app( argc, argv );
	AppOptions::getInstance().parse(QCoreApplication::arguments());

	QApplication::setWindowIcon(QIcon(":images/tatoge.svg"));
	QApplication::setApplicationName("tatoge");
	QApplication::setApplicationVersion(M_APP_VERSION);

	QString const locale_name = AppOptions::getInstance().getLocale();
	QLocale const locale = locale_name.isEmpty() ? QLocale() : QLocale(locale_name);

	QTranslator qtTranslator;
	if (qtTranslator.load(locale, QLatin1String("qt"), QLatin1String("_"),
			QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
	{
		app.installTranslator(&qtTranslator);
	}

	QTranslator myappTranslator;
	if (myappTranslator.load(locale, QLatin1String("tatoge"), QLatin1String("_"), QLatin1String(":i18n")))
	{
		app.installTranslator(&myappTranslator);
	}

	AppOptions::getInstance().process(app);

	QString qss_path = QStandardPaths::locate(QStandardPaths::AppDataLocation, "tatoge.qss");
	if (qss_path.isEmpty()) qss_path = ":tatoge.qss";
	QFile qss_file(qss_path);
	qss_file.open(QFile::ReadOnly);
	QString const styleSheet = QString(qss_file.readAll()).arg(AppOptions::getInstance().getTableColor().hue());
	app.setStyleSheet(styleSheet);

	MainWindow window;
	window.show();

	return app.exec();
}


