#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>

#include "MainWindow.h"


int main( int argc, char **argv )
{
	QApplication app( argc, argv );
	QApplication::setWindowIcon(QIcon(":images/dice.svg"));
	QApplication::setApplicationName("TaToGE");
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

	MainWindow window;
	window.show();

	return app.exec();
}


