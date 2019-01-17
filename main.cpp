#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>

#include "Window.h"


int main( int argc, char **argv )
{
	QApplication app( argc, argv );

	QTranslator qtTranslator;
	qtTranslator.load("qt_" + QLocale::system().name(),
			QLibraryInfo::location(QLibraryInfo::TranslationsPath));
	app.installTranslator(&qtTranslator);

	QTranslator myappTranslator;
	if (myappTranslator.load(QLocale(), QLatin1String("uGameTools"), QLatin1String("_"), QLatin1String(":i18n")))
	{
		qDebug() << "Load Ok";
		app.installTranslator(&myappTranslator);
	}

	Window window;

	if (argc==2)
	{
		window.loadJsonFile( argv[1] );
	}

	window.show();

	return app.exec();
}


