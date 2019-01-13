#include <QApplication>

#include "Window.h"


int main( int argc, char **argv )
{
	QApplication a( argc, argv );

	Window window;

	if (argc==2)
	{
		window.loadJsonFile( argv[1] );
		window.show();
	}
	else
	{
		window.show();
		window.showAddDialog();
	}

	return a.exec();
}


