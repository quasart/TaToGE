#include <QWidget>
#include <QGridLayout>
#include <QFont>
#include <QFile>
#include <QResizeEvent>

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
			QWidget * pWidget = nullptr;
			QLayout * pLayout = nullptr;

			if (type == "Dice")
			{
				if ( item["Faces"].isArray() )
				{
					pWidget = new Dice( asStringVector(item["Faces"]) );
				}
				else
				{
					pWidget = new Dice( item["Faces"].toInt(6) );
				}
			}
			else if (type == "Timer")
			{
				pWidget = new Timer( item["Duration"].toInt(30) );
			}
			else if (type == "Counter")
			{
				// TODO parameters
				pLayout = new Counter( 
							item["Value"].toInt(0)
							//TODO asIntVector(item["Increments"])
							);
			}
			else if (type == "CountDown")
			{
				pLayout = new CountDown( item["MaxValue"].toInt() );
			}
			else if (type == "Sequence")
			{
				std::vector<QString> list = asStringVector(item["List"]);
				pLayout = new Sequence(list);
			}
			else
			{
				std::cout << "Unexpected Widget type." << std::endl;
			}


			if (pWidget)
			{
				addRow( name, pWidget );
				if ( item.contains("Style") && item["Style"].isString() )
				{
					pWidget->setStyleSheet( item["Style"].toString() );
				}
			}
			else if (pLayout)
			{
				addRow( name, pLayout );
			}

		}

	}

protected:
	void resizeEvent(QResizeEvent* event) override
	{
		QWidget::ensurePolished();
		static const size_t initial_x = QWidget::sizeHint().width();
		static const size_t initial_y = QWidget::sizeHint().height();
		static const QFont initial_font = qobject_cast<QApplication *>(QCoreApplication::instance())->font();

		float const x_ratio = (float)event->size().width() / initial_x;
		float const y_ratio = (float)event->size().height() / initial_y;
		size_t const point_size = (size_t) (initial_font.pointSize() * std::min(x_ratio,y_ratio));
		QWidget::setStyleSheet("* { font-size: " + QString::number(point_size) + "pt; }" );

		QWidget::resizeEvent(event);
	}
};
