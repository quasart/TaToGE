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
			m_Combo.addItem( tr("Dice"),         QJsonObject{{"Type","Dice"}, {"Faces",6}} );
			m_Combo.addItem( tr("Counter"),      QJsonObject{{"Type","Counter"}} );
			m_Combo.addItem( tr("CountDown"),    QJsonObject{{"Type","CountDown"}} );
			m_Combo.insertSeparator(m_Combo.count());
			m_Combo.addItem( tr("30-second SandTimer"), QJsonObject{{"Type","Timer"}, {"Duration",30}} );
			m_Combo.addItem( tr( "1-minute SandTimer"), QJsonObject{{"Type","Timer"}, {"Duration",60}} );
			m_Combo.addItem( tr( "3-minute SandTimer"), QJsonObject{{"Type","Timer"}, {"Duration",60*3}} );
			m_Combo.addItem( tr( "5-minute SandTimer"), QJsonObject{{"Type","Timer"}, {"Duration",60*5}} );
			m_Combo.addItem( tr("10-minute SandTimer"), QJsonObject{{"Type","Timer"}, {"Duration",60*10}} );
			m_Combo.insertSeparator(m_Combo.count());
			m_Combo.addItem( tr("Coin flip"),      QJsonObject{{"Type","Dice"}, {"Faces",QJsonArray{tr("Head"),tr("Tail")}}} );
			m_Combo.addItem( tr("4-sided die"),   QJsonObject{{"Type","Dice"}, {"Faces",4}} );
			m_Combo.addItem( tr("6-sided die"),   QJsonObject{{"Type","Dice"}, {"Faces",6}} );
			m_Combo.addItem( tr("8-sided die"),   QJsonObject{{"Type","Dice"}, {"Faces",8}} );
			m_Combo.addItem( tr("10-sided die"),  QJsonObject{{"Type","Dice"}, {"Faces",10}} );
			m_Combo.addItem( tr("12-sided die"),  QJsonObject{{"Type","Dice"}, {"Faces",12}} );
			m_Combo.addItem( tr("20-sided die"),  QJsonObject{{"Type","Dice"}, {"Faces",20}} );
			m_Combo.addItem( tr("100-sided die"), QJsonObject{{"Type","Dice"}, {"Faces",100}} );
			m_Combo.addItem( tr("2 dice"),     QJsonObject{{"Type","Dice"}, {"Faces",6}, {"Count",2}} );
			m_Combo.addItem( tr("3 dice"),     QJsonObject{{"Type","Dice"}, {"Faces",6}, {"Count",3}} );
			m_Combo.addItem( tr("4 dice"),     QJsonObject{{"Type","Dice"}, {"Faces",6}, {"Count",4}} );
			m_Combo.insertSeparator(m_Combo.count());
			m_Combo.addItem( tr("Doubling cube"), QJsonObject{{"Type","Sequence"}, {"List", QJsonArray{
					"x1", "x2", "x4", "x8", "x16", "x32", "x64" }}} );
			m_Combo.addItem( tr("Magic 8 Ball"), QJsonObject{{"Type","Dice"}, {"Faces", QJsonArray{
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
				tr("Reply hazy, try again"),
				tr("Ask again later"),
				tr("Better not tell you now"),
				tr("Cannot predict now"),
				tr("Concentrate and ask again"),
				tr("Don't count on it"),
				tr("My reply is no"),
				tr("My sources say no"),
				tr("Outlook not so good"),
				tr("Very doubtful") }}} );
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
			m_JsonOutput["MaxValue"] =  QInputDialog::getInt(this, this->windowTitle(), tr("Value to decrease"), 10,
					0, 2147483647, 1, &ok);
		}

		if (ok)
		{
			QDialog::accept();
		}
	}


};


