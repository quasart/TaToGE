#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QFont>
#include <QFile>
#include <QFileDialog>
#include <QResizeEvent>
#include <QApplication>
#include <QDebug>

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

#include "widgets/Timer.h"
#include "widgets/Counter.h"
#include "widgets/Dice.h"
#include "widgets/Sequence.h"

#include "AddWidgetDialog.h"


class Window : public QWidget
{
Q_OBJECT

public:
	Window()
		: QWidget(nullptr)
		, m_Layout( * new QGridLayout(this) )
		, m_AddDialog(this)
	{
		m_Layout.setSpacing(7);

		QWidget * toolbar = new QWidget(this);
		toolbar->setLayout( new QHBoxLayout );
		toolbar->layout()->setContentsMargins(0,0,0,0);
		toolbar->setStyleSheet("font-size: 10pt; color: gray;");

		{
			QPushButton * plus_btn = new QPushButton(tr("Add widget"));
			plus_btn->setFlat(true);
			connect(plus_btn, &QPushButton::clicked, this, &Window::showAddDialog );
			toolbar->layout()->addWidget( plus_btn );
		}

		{
			QPushButton * btn = new QPushButton(tr("Load template"));
			btn->setFlat(true);
			connect(btn, &QPushButton::clicked, this, &Window::showLoadDialog );
			toolbar->layout()->addWidget( btn );
		}

		{
			QPushButton * btn = new QPushButton(tr("Clear table"));
			btn->setFlat(true);
			connect(btn, &QPushButton::clicked, this, &Window::clearTable );
			toolbar->layout()->addWidget( btn );
		}

		m_Layout.addWidget( toolbar, 0, 0, 1, 2 );
	}


private:
	QGridLayout & m_Layout;
	AddWidgetDialog m_AddDialog;


public:

	void addRow(QWidget * w)
	{
		if (!w) return;

		size_t const row = m_Layout.rowCount();

		w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

		if (w->whatsThis().isEmpty())
		{
			// Full Column span.
			m_Layout.addWidget( w, row, 0, 1, 2 );
		}
		else
		{
			QLabel * l = new QLabel(w->whatsThis());
			m_Layout.addWidget( l, row, 0 );
			m_Layout.addWidget( w, row, 1 );
		}
	}

	void deleteRow(int row)
	{
		for (int col = 0; col < m_Layout.columnCount(); ++col)
		{
			auto pItem = m_Layout.itemAtPosition(row,col);
			if (pItem && pItem->widget())
			{
				delete pItem->widget();
			}
		}
	}

	void showAddDialog()
	{
		if (m_AddDialog.exec())
		{
			loadJson(QJsonDocument(m_AddDialog.getJsonOutput()));
		}
	}

	void showLoadDialog()
	{
		QString filename = QFileDialog::getOpenFileName(this, tr("Select template file"), "./template", "JSON templates (*.json)");
		if (!filename.isEmpty())
		{
			clearTable();
			loadJsonFile(filename);
		}
	}

	void clearTable()
	{
		for (int row = 1; row < m_Layout.rowCount(); ++row)
		{
			deleteRow(row);
		}
	}

	std::vector<int> asIntVector(QJsonValue array, std::vector<int> default_out)
	{
		if (!array.isArray())
		{
			return default_out;
		}

		std::vector<int> result;
		for ( QJsonValueRef v : array.toArray() )
		{
			if (v.isDouble())
			{
				result.push_back(v.toInt());
			}
			else
			{
				qWarning() << "Unexpected type when parsing array of int.";
			}
		}
		return result;
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
				qWarning() << "Unexpected type when parsing array of string.";
			}
		}
		return result;
	}

	void loadJsonFile(QString filename)
	{
		QString val;
		QFile file;
		file.setFileName(filename);
		file.open(QIODevice::ReadOnly | QIODevice::Text);
		val = file.readAll();
		file.close();

		loadJson(QJsonDocument::fromJson(val.toUtf8()));
	}

	void loadJson(QJsonDocument json_doc)
	{

		if (json_doc.isNull())
		{
			qWarning() << "Json decoding error.";
		}
		else if (json_doc.isObject())
		{
			QJsonObject const o = json_doc.object();
			addRow( createWidget(o) );
		}
		else for ( QJsonValueRef const i : json_doc.array() )
		{
			QJsonObject const & item = i.toObject();
			addRow( createWidget(item) );
		}
	}

	QWidget * createWidget(QJsonObject const & item)
	{
		QWidget * pWidget = nullptr;

		QString type = item["Type"].toString("<UNDEFINED>");
		if (type == "Dice")
		{
			if ( item["Faces"].isArray() )
			{
				pWidget = new Dice( asStringVector(item["Faces"]),
						item["Count"].toInt(1) );
			}
			else
			{
				pWidget = new Dice( item["Faces"].toInt(6),
						item["Count"].toInt(1) );
			}
		}
		else if (type == "Timer")
		{
			pWidget = new Timer( item["Duration"].toInt(30) );
		}
		else if (type == "Counter")
		{
			pWidget = new Counter(
					item["Value"].toInt(0),
					asIntVector(item["Increments"], {1,5,10} )
					);
		}
		else if (type == "CountDown")
		{
			pWidget = new CountDown( item["MaxValue"].toInt() );
		}
		else if (type == "Sequence")
		{
			std::vector<QString> list = asStringVector(item["List"]);
			pWidget = new Sequence(list);
		}
		else if (type == "Label")
		{
			pWidget = new QLabel( item["Text"].toString("") );
		}
		else if (type == "Space")
		{
			pWidget = new QWidget();
		}
		else
		{
			qWarning() << "Unexpected Widget type " << type;
		}

		if (pWidget)
		{
			// General properties.
			pWidget->setWhatsThis( item["Name"].toString("") );
			pWidget->setStyleSheet( item["Style"].toString("") );
		}

		return pWidget;
	}

protected:
	void resizeEvent(QResizeEvent* event) override
	{
		QWidget::ensurePolished();

		const size_t initial_x = QWidget::sizeHint().width();
		const size_t initial_y = QWidget::sizeHint().height();
		const QFont initial_font = qobject_cast<QApplication *>(QCoreApplication::instance())->font();

		float const x_ratio = (float)event->size().width() / initial_x;
		float const y_ratio = (float)event->size().height() / initial_y;
		size_t const point_size = (size_t) (initial_font.pointSize() * std::min(x_ratio,y_ratio));
		QWidget::setStyleSheet("* { font-size: " + QString::number(point_size) + "pt; }" );

		QWidget::resizeEvent(event);
	}

};
