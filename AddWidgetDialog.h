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
			{ "Coin flip",    {{"Type","Dice"}, {"Faces",QJsonArray{"Head","Tail"}}} },
			{ "Dice 4",       {{"Type","Dice"}, {"Faces",4}} },
			{ "Dice 6 x 1",   {{"Type","Dice"}, {"Faces",6}} },
			{ "Dice 6 x 2",   {{"Type","Dice"}, {"Faces",6}, {"Count",2}} },
			{ "Dice 6 x 3",   {{"Type","Dice"}, {"Faces",6}, {"Count",3}} },
			{ "Dice 6 x 4",   {{"Type","Dice"}, {"Faces",6}, {"Count",4}} },
			{ "Dice 8",       {{"Type","Dice"}, {"Faces",8}} },
			{ "Dice 10",      {{"Type","Dice"}, {"Faces",10}} },
			{ "Dice 12",      {{"Type","Dice"}, {"Faces",12}} },
			{ "Dice 20",      {{"Type","Dice"}, {"Faces",20}} },
			{ "Dice 100",     {{"Type","Dice"}, {"Faces",100}} },
			{ "Counter",      {{"Type","Counter"}} },
			{ "Timer 30 sec", {{"Type","Timer"}, {"Duration",30}} },
			{ "Timer 1 min",  {{"Type","Timer"}, {"Duration",60}} },
			{ "Timer 3 min",  {{"Type","Timer"}, {"Duration",60*3}} },
			{ "Timer 5 min",  {{"Type","Timer"}, {"Duration",60*5}} },
			{ "Timer 10 min", {{"Type","Timer"}, {"Duration",60*10}} },
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

		for (size_t template_id = 0; template_id < 17; ++template_id )
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


