#include <QApplication>
#include <QPushButton>
#include <QGridLayout>

#include <QFont>

#include "Timer.h"
#include "Counter.h"
#include "Dice.h"
#include "Sequence.h"

#include <string>

class GridFormLayout : public QGridLayout
{
public:
	GridFormLayout()
	{
		setVerticalSpacing(10);
	}

	void addRow(QString label, QWidget * i)
	{
		size_t row = QGridLayout::rowCount();
		QGridLayout::addWidget( new QLabel(label), row, 0 );
		QGridLayout::addWidget(i, row, 1);
		i->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	}

	void addRow(QString label, QLayout * i)
	{
		size_t row = QGridLayout::rowCount();
		QGridLayout::addWidget( new QLabel(label), row, 0 );
		QGridLayout::addLayout(i, row, 1);
	}

};


int main( int argc, char **argv )
{
	QApplication a( argc, argv );
	QWidget window;
	GridFormLayout layout;
	window.setLayout(&layout);

	size_t duration = 30;
	if (argc > 1)
	{
		duration = std::stoi(argv[1]);
	}

	Timer hg(nullptr, duration);
	layout.addRow("Sablier", &hg);

	Counter c(nullptr, 0, {1,5});
	layout.addRow("Verveine", &c);

	Counter c2(nullptr);
	layout.addRow("Wilfried", &c2);

	Dice d(nullptr );
	layout.addRow("Dé à 6 faces", &d);
	Dice d2(nullptr );
	layout.addRow("Dé à 6 faces", &d2);
	Dice d3(nullptr );
	layout.addRow("Dé à 6 faces", &d3);

	Dice cp(nullptr, {"Verveine", "Wilfried", "Xena", "Youri", "Zorro"});
	layout.addRow("Premier joueur", &cp);

	Sequence s1(nullptr, {"Matin", "Midi", "Soir"});
	layout.addRow("Phase de jeu", &s1);

	CountDown cd(nullptr, 9);
	layout.addRow("Nombre de vies", &cd);

	window.show();

    return a.exec();
}


