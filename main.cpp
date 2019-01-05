#include <QApplication>
#include <QPushButton>
#include <QFormLayout>

#include <QFont>

#include "Control.h"
#include "Hourglass.h"
#include "Counter.h"

#include <string>

int main( int argc, char **argv )
{
	QApplication a( argc, argv );
	QWidget window;
	QFormLayout layout;

	size_t duration = 3000;
	if (argc > 1)
	{
		duration = std::stoi(argv[1]);
	}



	Hourglass hg(nullptr, duration);
	layout.addRow("Sablier", &hg);

	Counter c(nullptr, 0, {-5,-1,1,5});
	layout.addRow("Verveine", &c);

	Counter c2(nullptr, 0, {-5,-1,1,5});
	layout.addRow("Wilfried", &c2);


	window.setLayout(&layout);
	window.show();

    return a.exec();
}


