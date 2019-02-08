#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>

#include <QFile>
#include <QStandardPaths>
#include <QColor>
#include <QDebug>

#include "MainWindow.h"


int main( int argc, char **argv )
{
	QApplication app( argc, argv );
	QApplication::setWindowIcon(QIcon(":images/tatoge.svg"));
	QApplication::setApplicationName("tatoge");
	QApplication::setApplicationVersion(M_APP_VERSION);

	QTranslator qtTranslator;
	qtTranslator.load("qt_" + QLocale::system().name(),
			QLibraryInfo::location(QLibraryInfo::TranslationsPath));
	app.installTranslator(&qtTranslator);

	QTranslator myappTranslator;
	if (myappTranslator.load(QLocale(), QLatin1String("tatoge"), QLatin1String("_"), QLatin1String(":i18n")))
	{
		app.installTranslator(&myappTranslator);
	}

	QColor table_color = argc>2 ? argv[2] : "#00aaff";

	QString qss_path = QStandardPaths::locate(QStandardPaths::AppDataLocation, "tatoge.qss");
	if (qss_path.isEmpty()) qss_path = ":tatoge.qss";
	QFile qss_file(qss_path);
	qss_file.open(QFile::ReadOnly);
	QString const styleSheet = QString(qss_file.readAll()).arg(table_color.hue());
	app.setStyleSheet(styleSheet);

	MainWindow window;
	window.show();

	return app.exec();
}


