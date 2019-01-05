#include <QWidget>
#include <QGridLayout>
#include <QFont>
#include <QFile>

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

#include "Timer.h"
#include "Counter.h"
#include "Dice.h"
#include "Sequence.h"


class Window : public QWidget
{
public:
	Window()
		: QWidget(nullptr)
		, m_Layout( * new QGridLayout(this) )
	{
		m_Layout.setVerticalSpacing(10);

	}


private:
	QGridLayout & m_Layout;


public:

	void addRow(QString label, QWidget * i)
	{
		size_t row = m_Layout.rowCount();

		if (label.isEmpty())
		{
			m_Layout.addWidget(i, row, 0, 1, 2);
		}
		else
		{
			m_Layout.addWidget( new QLabel(label), row, 0 );
			m_Layout.addWidget(i, row, 1);
		}
	}

	void addRow(QString label, QLayout * i)
	{
		size_t row = m_Layout.rowCount();
		if (label.isEmpty())
		{
			m_Layout.addLayout(i, row, 1, 1, 2);
		}
		else
		{
			m_Layout.addWidget( new QLabel(label), row, 0 );
			m_Layout.addLayout(i, row, 1);
		}
	}

	std::vector<QString> asStringVector(QJsonValue array)
	{
		std::vector<QString> result;
		for ( QJsonValueRef v : array.toArray() )
		{
			if (v.isString())
			{
				result.push_back(v.toString());
			}
			else
			{
				std::cout << "Unexpected type when parsing array of string." << std::endl;
			}
		}
		return result;
	}

	void loadJSON(QString filename)
	{
		QString val;
		QFile file;
		file.setFileName(filename);
		file.open(QIODevice::ReadOnly | QIODevice::Text);
		val = file.readAll();
		file.close();

		QJsonDocument json_file = QJsonDocument::fromJson(val.toUtf8());

		if (json_file.isNull())
		{
			std::cout << "Json decoding error." << std::endl;
		}

		for ( QJsonValueRef const i : json_file.array() )
		{
			QJsonObject const item = i.toObject();
			QString name = item["Name"].toString("");
			QString type = item["Type"].toString("unknown");

			if (type == "Dice")
			{
				if ( item["Faces"].isArray() )
				{
					std::vector<QString> face_list = asStringVector(item["Faces"]);
					addRow( name, new Dice(face_list));
				}
				else
				{
					addRow( name, new Dice( item["Faces"].toInt(6) ) );
				}
			}
			else if (type == "Timer")
			{
				addRow( name, new Timer( item["Duration"].toInt(30) ) );
			}
			else if (type == "Counter")
			{
				// TODO parameters
				addRow( name, new Counter( 
							item["Value"].toInt(0)
							//TODO asIntVector(item["Faces"])
							));
			}
			else if (type == "CountDown")
			{
				addRow( name, new CountDown( item["MaxValue"].toInt() ) );
			}
			else if (type == "Sequence")
			{
				std::vector<QString> list = asStringVector(item["List"]);
				addRow( name, new Sequence(list) );
			}
			else
			{
				std::cout << "Unexpected Widget type." << std::endl;
			}
		}

	}

};
