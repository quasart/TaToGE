#pragma once

#include <QDialog>
#include <QGridLayout>
#include <QLineEdit>

#include <vector>




class AddWidgetDialog : public QDialog
{
public:

	struct JsonTemplates
	{
		const char * name;
		QJsonObject json;
	};

	static JsonTemplates const & getTemplate(size_t i)
	{
		// TODO load this map from json file
		static const std::vector<JsonTemplates> s_templates = {
			{ "Dice 6", {{ "Type" , "Dice" }} },
			{ "2 x Dice 6", {{ "Type" , "Dice"}, {"Count" , 2 }} },
			{ "3 x Dice 6", {{ "Type" , "Dice"}, {"Count" , 3 }} },
			{ "Compteur", {{ "Type" , "Counter" }} },
			{ "SandTimer 30 sec", {{ "Type" , "Timer"}, {"Duration" , 30 }} },
			{ "SandTimer 1 min", {{ "Type" , "Timer"}, {"Duration" , 60 }} },
		};

		return s_templates[i];
	}

	AddWidgetDialog(QWidget * parent)
		: QDialog(parent)
		, m_Layout( * new QGridLayout(this) )
	{
		QWidget::setWindowTitle("New Widget");

		{
			size_t const row = m_Layout.rowCount();
			m_Layout.addWidget( new QLabel("Name (optional)"), row, 0 );
			m_Layout.addWidget( &m_NameInput, row, 1);
		}

		for (size_t template_id = 0; template_id < 6; ++template_id )
		{
			size_t const row = m_Layout.rowCount();
			QPushButton * btn = new QPushButton( getTemplate(template_id).name );
			btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
			m_Layout.addWidget( btn, row, 0, 1, 2 );
			connect(btn, &QPushButton::clicked, [=]{ this->onTemplateAdd(template_id); } );
		}

	}

	QJsonObject getJsonOutput() const { return m_JsonOutput; }

private:
	QGridLayout & m_Layout;
	QLineEdit m_NameInput;
	QJsonObject m_JsonOutput;

	void onTemplateAdd(size_t template_id)
	{
		m_JsonOutput = getTemplate(template_id).json;

		if (!m_NameInput.text().isEmpty())
		{
			m_JsonOutput["Name"] = m_NameInput.text();
			m_NameInput.setText("");
		}

		QDialog::accept();
	}

};


