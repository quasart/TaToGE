#pragma once

#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QInputDialog>
#include <QComboBox>
#include <QPushButton>

#include <QJsonObject>
#include <QJsonArray>

#include <vector>


class AddWidgetDialog : public QDialog
{
Q_OBJECT

public:

	AddWidgetDialog(QWidget * parent);

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
	void accept();


};


