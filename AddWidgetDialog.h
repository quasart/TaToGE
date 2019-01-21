#pragma once

#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QInputDialog>
#include <QComboBox>
#include <QJsonObject>
#include <QJsonArray>

#include <vector>


class AddWidgetDialog : public QDialog
{
Q_OBJECT

public:

	AddWidgetDialog(QWidget * parent)
		: QDialog(parent)
		, m_Layout( * new QFormLayout(this) )
	{
		QWidget::setWindowTitle(tr("New Widget"));

		m_Layout.addRow( tr("Widget"), &m_Combo);
		m_Layout.addRow( tr("Name (optional)"), &m_NameInput);

		{
			m_Combo.addItem( tr("Counter"),      QJsonObject{{"Type","Counter"}} );
			m_Combo.setItemData(m_Combo.count()-1, tr("To count points, typically to display score or penality."), Qt::ToolTipRole);
			m_Combo.addItem( tr("CountDown..."),    QJsonObject{{"Type","CountDown"}} );
			m_Combo.setItemData(m_Combo.count()-1, tr("To follow a count down, for example to display how many life are left before loosing."), Qt::ToolTipRole);
			m_Combo.insertSeparator(m_Combo.count());

			m_Combo.addItem( tr("30-second SandTimer"), QJsonObject{{"Type","Timer"}, {"Duration",30}} );
			m_Combo.addItem( tr( "1-minute SandTimer"), QJsonObject{{"Type","Timer"}, {"Duration",60}} );
			m_Combo.addItem( tr( "3-minute SandTimer"), QJsonObject{{"Type","Timer"}, {"Duration",60*3}} );
			m_Combo.addItem( tr( "5-minute SandTimer"), QJsonObject{{"Type","Timer"}, {"Duration",60*5}} );
			m_Combo.addItem( tr("Other SandTimer..."),  QJsonObject{{"Type","Timer"}} );
			m_Combo.insertSeparator(m_Combo.count());

			m_Combo.addItem( tr("4-sided die"),   QJsonObject{{"Type","Dice"}, {"NbSides",4}} );
			m_Combo.addItem( tr("6-sided die"),   QJsonObject{{"Type","Dice"}, {"NbSides",6}} );
			m_Combo.setCurrentIndex( m_Combo.count()-1);
			m_Combo.addItem( tr("8-sided die"),   QJsonObject{{"Type","Dice"}, {"NbSides",8}} );
			m_Combo.addItem( tr("10-sided die"),  QJsonObject{{"Type","Dice"}, {"NbSides",10}} );
			m_Combo.addItem( tr("12-sided die"),  QJsonObject{{"Type","Dice"}, {"NbSides",12}} );
			m_Combo.addItem( tr("20-sided die"),  QJsonObject{{"Type","Dice"}, {"NbSides",20}} );
			m_Combo.addItem( tr("100-sided die"), QJsonObject{{"Type","Dice"}, {"NbSides",100}} );
			m_Combo.addItem( tr("2 dice"),        QJsonObject{{"Type","Dice"}, {"NbSides",6}, {"Count",2}} );
			m_Combo.addItem( tr("3 dice"),        QJsonObject{{"Type","Dice"}, {"NbSides",6}, {"Count",3}} );
			m_Combo.addItem( tr("4 dice"),        QJsonObject{{"Type","Dice"}, {"NbSides",6}, {"Count",4}} );
			m_Combo.addItem( tr("Other dice..."), QJsonObject{{"Type","Dice"}} );
			m_Combo.insertSeparator(m_Combo.count());

			m_Combo.addItem( tr("Coin flip"),     QJsonObject{{"Type","Sortition"}, {"List",QJsonArray{tr("Head"),tr("Tail")}}} );
			m_Combo.addItem( tr("Roulette"),      QJsonObject{{"Type","Sortition"}, {"List",QJsonArray{"0",
					 "1 (R)",  "2 (B)", "3 (R)",
					 "4 (B)",  "5 (R)", "6 (B)",
					 "7 (R)",  "8 (B)", "9 (R)",
					"10 (B)", "11 (B)", "12 (R)",
					"13 (B)", "14 (R)", "15 (B)",
					"16 (R)", "17 (B)", "18 (R)",
					"19 (R)", "20 (B)", "21 (R)",
					"22 (B)", "23 (R)", "24 (B)",
					"25 (R)", "26 (B)", "27 (R)",
					"28 (B)", "29 (B)", "30 (R)",
					"31 (B)", "32 (R)", "33 (B)",
					"34 (R)", "35 (B)", "36 (R)" }}} );
			m_Combo.addItem( tr("Magic 8 Ball"), QJsonObject{{"Type","Sortition"}, {"List", QJsonArray{
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
				tr("Concentrate and ask again") }}} );
			m_Combo.setItemData(m_Combo.count()-1, tr("This magic item can answer any question."), Qt::ToolTipRole);
			m_Combo.addItem( tr("Other sortition..."), QJsonObject{{"Type","Sortition"}} );
			m_Combo.insertSeparator(m_Combo.count());

			m_Combo.addItem( tr("Doubling cube"), QJsonObject{{"Type","Sequence"}, {"List",QJsonArray{"x1","x2","x4","x8","x16","x32","x64"}}} );
			m_Combo.setItemData(m_Combo.count()-1, tr("This one is used in Backgammon."), Qt::ToolTipRole);
			m_Combo.addItem( tr("Other sequence..."), QJsonObject{{"Type","Sequence"}} );
		}

		{
			QDialogButtonBox * btn = new QDialogButtonBox(QDialogButtonBox::Ok
														| QDialogButtonBox::Cancel);

			m_Layout.addRow( btn );
			connect(btn, &QDialogButtonBox::accepted, this, &AddWidgetDialog::accept);
			connect(btn, &QDialogButtonBox::rejected, this, &QDialog::reject);
		}
	}

	QJsonObject getJsonOutput() const { return m_JsonOutput; }

	QJsonArray getNameList(bool * ok)
	{
		QJsonArray result = {};
		QString const list_str = QInputDialog::getText(this, this->windowTitle(),
					tr("Please provide list of names, separated by semicolon ';'"), 
					QLineEdit::Normal, "", ok);
		for ( QString str : list_str.split(";"))
		{
			result.push_back(str);
		}
		*ok &= (result.size() > 1); // found no separator.
		return result;
	}

private:
	QFormLayout & m_Layout;
	QLineEdit m_NameInput;
	QComboBox m_Combo;
	QJsonObject m_JsonOutput;

public slots:
	void accept()
	{
		m_JsonOutput = m_Combo.itemData( m_Combo.currentIndex() ).toJsonObject();
		bool ok = true;

		if (!m_NameInput.text().isEmpty())
		{
			m_JsonOutput["Name"] = m_NameInput.text();
			m_NameInput.setText("");
		}

		if (m_JsonOutput["Type"] == "CountDown"
			&& !m_JsonOutput.contains("MaxValue"))
		{
			m_JsonOutput["MaxValue"] =  QInputDialog::getInt(this, this->windowTitle(),
					tr("Initial value to decrease"), 10,
					0, 2147483647, 1, &ok);
		}
		else if (m_JsonOutput["Type"] == "Timer"
			&& !m_JsonOutput.contains("Duration"))
		{
			m_JsonOutput["Duration"] =  QInputDialog::getInt(this, this->windowTitle(),
					tr("Timer duration in seconds"), 60,
					1, 2147483647, 1, &ok);
		}
		else if (m_JsonOutput["Type"] == "Sortition"
			&& !m_JsonOutput.contains("List"))
		{
			m_JsonOutput["List"] = getNameList(&ok);
		}
		else if (m_JsonOutput["Type"] == "Sequence"
			&& !m_JsonOutput.contains("List"))
		{
			m_JsonOutput["List"] = getNameList(&ok);
		}
		else if (m_JsonOutput["Type"] == "Dice"
			&& !m_JsonOutput.contains("NbSides") && !m_JsonOutput.contains("Count"))
		{
			m_JsonOutput["NbSides"] = QInputDialog::getInt(this, this->windowTitle(),
					tr("Number of sides of the die"), 6,
					0, 2147483647, 1, &ok);
			if (ok)
			m_JsonOutput["Count"] = QInputDialog::getInt(this, this->windowTitle(),
					tr("Number of dice to roll?"), 1,
					1, 10, 1, &ok);
		}

		if (ok)
		{
			QDialog::accept();
		}
	}


};


