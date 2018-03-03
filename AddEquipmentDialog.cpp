#include "AddEquipmentDialog.h"

#include <QDialogButtonBox>
#include <QDebug>
#include <QRegularExpression>

#include <QLabel>
#include <QGridLayout>
#include <QScrollArea>
#include <QRadioButton>
#include <QTimeEdit>
#include <QPushButton>

#include <QJsonArray>


QJsonObject parseDiceCode(QString code, bool * ok = nullptr)
{
	int count = 1;
	int nb_faces = 6;
	bool is_fudge = false;
	bool is_poker = false;

	QRegularExpression re("^ *"
			"([1-9])?"
			" *[Dd] *"
			"([1-9][0-9]*)?([FfPp])?"
			" *$",
			QRegularExpression::CaseInsensitiveOption);
	QRegularExpressionMatch match = re.match(code);
	if (match.hasMatch())
	{
		QString count_str = match.captured(1);
		QString nb_faces_str = match.captured(2);
		QString flag = match.captured(3);

		if (!count_str.isEmpty())
		{
			count = count_str.toInt(ok);
		}

		if (!nb_faces_str.isEmpty())
		{
			nb_faces = nb_faces_str.toInt(ok);
		}

		if (flag == "F" || flag == "f")
		{
			is_fudge = true;
		}
		if (flag == "P" || flag == "p")
		{
			is_poker = true;
		}
	}
	else
	{
		if (ok)
		{
			*ok = false;
		}
		else
		{
			qWarning() << "Invalid dice code" << code;
		}
	}

	if (ok && ! *ok)
	{
		return QJsonObject{};
	}
	else if (is_fudge)
	{
		return QJsonObject{{"Type","Dice"}, {"List",QJsonArray{" ","-","+"}}, {"Count",count}};
	}
	else if (is_poker)
	{
		return QJsonObject{{"Type","Dice"}, {"List",QJsonArray{"9","10","J","Q","K","A"}}, {"Count",count}};
	}
	else
	{
		return QJsonObject{{"Type","Dice"}, {"NbSides",nb_faces}, {"Count",count}};
	}
}

QJsonArray parseNameList(QString list_str, bool * ok)
{
	QJsonArray result = {};
	for ( QString str : list_str.split(";"))
	{
		result.push_back(str);
	}

	if (ok)
	{
		*ok &= (result.size() > 1); // found no separator.
	}

	return result;
}

void AddEquipmentDialog::addSeparator()
{
	auto line = new QFrame;
	line->setMinimumSize(QSize(0, 15));
	line->setFrameShape(QFrame::HLine);
	line->setFrameShadow(QFrame::Sunken);
	m_RadioGridLayout->addWidget( line, m_RadioGridLayout->rowCount(), 0, 1, 3);
}

QRadioButton * AddEquipmentDialog::addRadio(QString name, QJsonObject result, QString icon_name, QString description, QWidget * form)
{
	int const row = m_RadioGridLayout->rowCount();

	auto btn = new QRadioButton(name);
	QIcon icon(QIcon::fromTheme(QStringLiteral("document")));
	if (!icon_name.isEmpty()) icon = QIcon(":images/"+icon_name+".png");
	btn->setIcon(icon);
	btn->setIconSize(QSize(32, 32));
	m_RadioGridLayout->addWidget(btn, row, 0, 1, 1);

	connect( btn, &QRadioButton::toggled,  [result,this,btn,form]()
			{
				if (btn->isChecked()) m_RadioJson = result;
				if (form) form->setEnabled(btn->isChecked());
			} );

	if (form)
	{
		m_RadioGridLayout->addWidget(form, row, 1, 1, 1);
		form->setEnabled(false);
	}

	if (!description.isEmpty())
	{
		auto descr = new QLabel(description);
		descr->setWordWrap(true);
		if (form)
		{
			m_RadioGridLayout->addWidget(descr, row, 2, 1, 1);
		}
		else
		{
			m_RadioGridLayout->addWidget(descr, row, 1, 1, 2);
		}
	}

	return btn;
}

QJsonArray cards_array()
{
	QJsonArray card_array;
	for (QString s : std::vector<QString>({
			u8" \u2660",
			u8" \u2665",
			u8" \u2666",
			u8" \u2663"
			}))
		for (QString r : std::vector<QString>({
				"2", "3", "4", "5", "6", "7", "8", "9", "10",
				"J", "Q", "K", "A"
				}))
			card_array.push_back( r + s );

	return card_array;
}

AddEquipmentDialog::AddEquipmentDialog(QWidget * parent)
	: QDialog(parent)
	, m_RadioGridLayout(new QGridLayout)
	, m_NameInput( new QLineEdit )
	, m_DiceCodeInput( new QLineEdit("2d6") )
	, m_DurationInput( new QTimeEdit({0,2,0}) )
	, m_CountDownInput( new QSpinBox )
	, m_SequenceInput( new QLineEdit(tr("Spring;Summer;Automn;Winter")) )
	, m_SortitionInput( new QLineEdit(tr("Rock;Paper;Scissors")) )
	, m_RacersInput( new QLineEdit("A;B;C;D;E;F;G;H") )
{
	// Build window

	QWidget::setWindowTitle( tr("Add Game Equipment") );
	auto mainGridLayout = new QGridLayout(this);

	mainGridLayout->addWidget( new QLabel(tr("Equipment type")), 0,0 );

	{
		m_RadioGridLayout->setHorizontalSpacing(9);
		m_RadioGridLayout->setVerticalSpacing(5);
		m_RadioGridLayout->setContentsMargins(15, 11, 16, 11);

		auto scrollAreaWidgetContents = new QWidget();
		scrollAreaWidgetContents->setGeometry(QRect(0, 0, 600, 567));
		scrollAreaWidgetContents->setLayout(m_RadioGridLayout);

		auto scrollArea = new QScrollArea;
		scrollArea->setWidget(scrollAreaWidgetContents);
		scrollArea->setStyleSheet(QStringLiteral("QRadioButton { font-weight: bold; }"));
		scrollArea->setFrameShape(QFrame::Panel);
		scrollArea->setFrameShadow(QFrame::Sunken);
		scrollArea->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
		scrollArea->setWidgetResizable(true);
		mainGridLayout->addWidget( scrollArea, 1,0 );
	}

	mainGridLayout->addWidget( new QLabel(tr("Equipment name")), 2,0 );

	{
		m_NameInput->setPlaceholderText( tr("this name is optional...") );
		mainGridLayout->addWidget( m_NameInput, 3,0 );
	}

	{
		QDialogButtonBox * btn = new QDialogButtonBox(QDialogButtonBox::Ok
													| QDialogButtonBox::Cancel);

		mainGridLayout->addWidget( btn, 4,0 );
		connect(btn, &QDialogButtonBox::accepted, this, &AddEquipmentDialog::accept);
		connect(btn, &QDialogButtonBox::rejected, this, &QDialog::reject);

		QPushButton * ok_btn = btn->button(QDialogButtonBox::Ok);
		if (ok_btn)
		{
			ok_btn->setText(tr("Add"));
			ok_btn->setIcon(QIcon::fromTheme("list-add"));
		}
	}

	// Radio buttons

	{
		addRadio( tr("Counter"),      QJsonObject{{"Type","Counter"}} , "counter", tr("To count points, typically to display score or penality."))->setChecked(true);
		addRadio( tr("CountDown"),    QJsonObject{{"Type","CountDown"}} , "counter", tr("To follow a count down, for example to display how many life are left."), m_CountDownInput );
		addSeparator();

		addRadio( tr("6-sided die"),   parseDiceCode("d6"), "dice6", tr("The classic one...") );
		addRadio( tr("8-sided die"),   parseDiceCode("d8"), "dice8" );
		addRadio( tr("20-sided die"),  parseDiceCode("d20"), "dice20" );
		addRadio( tr("4 Fudge dice"),  parseDiceCode("4dF") , "dice4df", tr("Used in Fudge role-playing system.") );
		addRadio( tr("5 Poker dice"),  parseDiceCode("5dP") , "dice5dp" );
		addRadio( tr("Other dice"),    QJsonObject{{"Type","Dice"}, {"Form","DiceCode"}}, "custom", tr("'2d6' means two 6-sided dice."), m_DiceCodeInput );
		addSeparator();

		addRadio( tr("30-second SandTimer"), QJsonObject{{"Type","Timer"}, {"Duration",30}}, "sandtimer" );
		addRadio( tr( "1-minute SandTimer"), QJsonObject{{"Type","Timer"}, {"Duration",60}}, "sandtimer" );
		addRadio( tr("Other SandTimer"),     QJsonObject{{"Type","Timer"}}, "custom", tr("Duration (mm:ss)"), m_DurationInput );
		addSeparator();

		addRadio( tr("Coin flip"),     QJsonObject{{"Type","Dice"}, {"List",QJsonArray{tr("Head"),tr("Tail")}}}, "coin" );
		addRadio( tr("Roulette"),      QJsonObject{{"Type","Dice"}, {"RollDuration",5}, {"List",QJsonArray{"0",
			 "1 " + tr("red odd low"    ),  "2 " + tr("black even low" ),  "3 " + tr("red odd low"    ),
			 "4 " + tr("black even low" ),  "5 " + tr("red odd low"    ),  "6 " + tr("black even low" ),
			 "7 " + tr("red odd low"    ),  "8 " + tr("black even low" ),  "9 " + tr("red odd low"    ),
			"10 " + tr("black even low" ), "11 " + tr("black odd low"  ), "12 " + tr("red even low"   ),
			"13 " + tr("black odd low"  ), "14 " + tr("red even low"   ), "15 " + tr("black odd low"  ),
			"16 " + tr("red even low"   ), "17 " + tr("black odd low"  ), "18 " + tr("red even low"   ),
			"19 " + tr("red odd high"   ), "20 " + tr("black even high"), "21 " + tr("red odd high"   ),
			"22 " + tr("black even high"), "23 " + tr("red odd high"   ), "24 " + tr("black even high"),
			"25 " + tr("red odd high"   ), "26 " + tr("black even high"), "27 " + tr("red odd high"   ),
			"28 " + tr("black even high"), "29 " + tr("black odd high" ), "30 " + tr("red even high"  ),
			"31 " + tr("black odd high" ), "32 " + tr("red even high"  ), "33 " + tr("black odd high" ),
			"34 " + tr("red even high"  ), "35 " + tr("black odd high" ), "36 " + tr("red even high"  ) }}}, "roulette" );
		addRadio( tr("Magic 8 Ball"), QJsonObject{{"Type","Dice"}, {"List", QJsonArray{
			tr("It is certain"),
			tr("It is decidedly so"),
			tr("Without a doubt"),
			tr("Yes - definitely"),
			tr("You may rely on it"),
			tr("As I see it, yes"),
			tr("Most likely"),
			tr("Outlook good"),
			tr("Yes"),
			tr("Signs point to yes"),
			tr("Don't count on it"),
			tr("My reply is no"),
			tr("My sources say no"),
			tr("Outlook not so good"),
			tr("Very doubtful"),
			tr("Reply hazy, try again"),
			tr("Ask again later"),
			tr("Better not tell you now"),
			tr("Cannot predict now"),
			tr("Concentrate and ask again") }}} , "8ball", tr("This magic item can answer any question."));
		addRadio( tr("Custom sortition"), QJsonObject{{"Type","Dice"}, {"Form","Sortition"}}, "custom", tr("List of possibilities, separated by semicolons (';')"), m_SortitionInput );
		addSeparator();

		addRadio( tr("Doubling cube"), QJsonObject{{"Type","Sequence"}, {"List",QJsonArray{"x1","x2","x4","x8","x16","x32","x64"}}} , "videau", tr("This one is used in Backgammon."));
		addRadio( tr("Sequence"), QJsonObject{{"Type","Sequence"}}, "custom", tr("List of steps, separated by semicolons (';')"), m_SequenceInput );
		addSeparator();

		addRadio( tr("Card deck"), QJsonObject{{"Type","CardDrawer"}, {"Cards",cards_array()}, {"NbDrawing",5}, {"DrawingTime",0}}, "cards" );
		addRadio( tr("Loto"), QJsonObject{{"Type","CardDrawer"}, {"DeckSize",49}, {"NbDrawing",6}}, "loto" );
		addRadio( tr("Snail racing"), QJsonObject{{"Type","CardDrawer"}, {"NbDrawing",5}, {"Form","Racers"}}, "custom", tr("Names of snails, separated by semicolons (';')"), m_RacersInput );
		addSeparator();

		addRadio( tr("Empty space"), QJsonObject{{"Type","Space"}} , "space", tr("Just to leave some empty room on the table."));
	}

	m_DurationInput->setDisplayFormat("mm:ss");
	m_CountDownInput->setValue(10);
	m_CountDownInput->setMinimum(1);
	m_CountDownInput->setMaximum(9999);
	m_SequenceInput->setMinimumWidth(150);
	m_SortitionInput->setMinimumWidth(150);

}


void AddEquipmentDialog::accept()
{
	bool ok = true;
	QJsonObject out = m_RadioJson;

	if (out["Type"] == "CountDown"
		&& !out.contains("MaxValue"))
	{
		out["MaxValue"] = m_CountDownInput->value();
	}
	else if (out["Type"] == "Timer"
		&& !out.contains("Duration"))
	{
		out["Duration"] =  QTime{0,0,0}.secsTo( m_DurationInput->time() );
	}
	else if (out["Type"] == "Dice"
		&& out["Form"] == "Sortition")
	{
		out["List"] = parseNameList(m_SortitionInput->text(), &ok);
		if (ok)
		{
			m_SortitionInput->setStyleSheet( "" );
		}
		else
		{
			m_SortitionInput->setStyleSheet( "color: red;" );
		}
	}
	else if (out["Type"] == "Sequence"
		&& !out.contains("List"))
	{
		out["List"] = parseNameList(m_SequenceInput->text(), &ok);
		if (ok)
		{
			m_SequenceInput->setStyleSheet( "" );
		}
		else
		{
			m_SequenceInput->setStyleSheet( "color: red;" );
		}
	}
	else if (out["Type"] == "Dice"
		&& out["Form"] == "DiceCode")
	{
		out = parseDiceCode(m_DiceCodeInput->text(), &ok);
		if (ok)
		{
			m_DiceCodeInput->setStyleSheet( "" );
		}
		else
		{
			m_DiceCodeInput->setStyleSheet( "color: red;" );
		}
	}
	else if (out["Type"] == "CardDrawer"
		&& out["Form"] == "Racers")
	{
		out["Cards"] = parseNameList(m_RacersInput->text(), &ok);
		if (ok)
		{
			m_RacersInput->setStyleSheet( "" );
		}
		else
		{
			m_RacersInput->setStyleSheet( "color: red;" );
		}
	}

	if (!m_NameInput->text().isEmpty())
	{
		out["Name"] = m_NameInput->text();
	}

	if (ok)
	{
		m_JsonOutput = out;
		QDialog::accept();
		m_NameInput->setText("");
	}
}

