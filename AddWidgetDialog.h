#pragma once

#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QInputDialog>
#include <QComboBox>
#include <vector>




class AddWidgetDialog : public QDialog
{
public:


	AddWidgetDialog(QWidget * parent)
		: QDialog(parent)
		, m_Layout( * new QFormLayout(this) )
	{
		QWidget::setWindowTitle("New Widget");

		m_Layout.addRow( "Widget", &m_Combo);
		m_Layout.addRow( "Name (optional)", &m_NameInput);

		{
			m_Combo.addItem( "Dice",         QJsonObject{{"Type","Dice"}, {"Faces",6}} );
			m_Combo.addItem( "Counter",      QJsonObject{{"Type","Counter"}} );
			m_Combo.addItem( "CountDown",    QJsonObject{{"Type","CountDown"}} );
			m_Combo.insertSeparator(m_Combo.count());
			m_Combo.addItem( "Timer 30 sec", QJsonObject{{"Type","Timer"}, {"Duration",30}} );
			m_Combo.addItem( "Timer 1 min",  QJsonObject{{"Type","Timer"}, {"Duration",60}} );
			m_Combo.addItem( "Timer 3 min",  QJsonObject{{"Type","Timer"}, {"Duration",60*3}} );
			m_Combo.addItem( "Timer 5 min",  QJsonObject{{"Type","Timer"}, {"Duration",60*5}} );
			m_Combo.addItem( "Timer 10 min", QJsonObject{{"Type","Timer"}, {"Duration",60*10}} );
			m_Combo.insertSeparator(m_Combo.count());
			m_Combo.addItem( "Coin flip",    QJsonObject{{"Type","Dice"}, {"Faces",QJsonArray{"Head","Tail"}}} );
			m_Combo.addItem( "Dice 4",       QJsonObject{{"Type","Dice"}, {"Faces",4}} );
			m_Combo.addItem( "Dice 6 x 1",   QJsonObject{{"Type","Dice"}, {"Faces",6}} );
			m_Combo.addItem( "Dice 6 x 2",   QJsonObject{{"Type","Dice"}, {"Faces",6}, {"Count",2}} );
			m_Combo.addItem( "Dice 6 x 3",   QJsonObject{{"Type","Dice"}, {"Faces",6}, {"Count",3}} );
			m_Combo.addItem( "Dice 6 x 4",   QJsonObject{{"Type","Dice"}, {"Faces",6}, {"Count",4}} );
			m_Combo.addItem( "Dice 8",       QJsonObject{{"Type","Dice"}, {"Faces",8}} );
			m_Combo.addItem( "Dice 10",      QJsonObject{{"Type","Dice"}, {"Faces",10}} );
			m_Combo.addItem( "Dice 12",      QJsonObject{{"Type","Dice"}, {"Faces",12}} );
			m_Combo.addItem( "Dice 20",      QJsonObject{{"Type","Dice"}, {"Faces",20}} );
			m_Combo.addItem( "Dice 100",     QJsonObject{{"Type","Dice"}, {"Faces",100}} );
			m_Combo.insertSeparator(m_Combo.count());
			m_Combo.addItem( "Videau",       QJsonObject{{"Type","Sequence"}, {"List", QJsonArray{"x1", "x2", "x4", "x8", "x16", "x32", "x64" }}} );
			m_Combo.addItem( "Magic 8 Ball", QJsonObject{{"Type","Dice"}, {"Faces", QJsonArray{"Essaye plus tard",
																								"Essaye encore",
																								"Pas d'avis",
																								"C'est ton destin",
																								"Le sort en est jeté",
																								"Une chance sur deux",
																								"Repose ta question",
																								"D'après moi oui",
																								"C'est certain",
																								"Oui absolument",
																								"Tu peux compter dessus",
																								"Sans aucun doute",
																								"Très probable",
																								"Oui",
																								"C'est bien parti",
																								"C'est non",
																								"Peu probable",
																								"Faut pas rêver",
																								"N'y compte pas",
																								"Impossible" }}} );
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
			m_JsonOutput["MaxValue"] =  QInputDialog::getInt(this, this->windowTitle(), "Value to decrease", 10,
					0, 2147483647, 1, &ok);
		}

		if (ok)
		{
			QDialog::accept();
		}
	}


};


