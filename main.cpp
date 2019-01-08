#include <QApplication>

#include "Window.h"


int main( int argc, char **argv )
{
	QApplication a( argc, argv );

	Window window;

	QString filename = argc>1 ? argv[1] : "template/all.json";
	window.loadJSON(filename);

	window.show();

	return a.exec();
}


