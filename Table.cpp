#include "Table.h"

#include <QFile>
#include <QFileDialog>
#include <QDebug>
#include <QResizeEvent>
#include <QApplication>
#include <QFont>
#include <QGridLayout>

#include "widgets/Timer.h"
#include "widgets/Counter.h"
#include "widgets/Dice.h"
#include "widgets/Sequence.h"
#include "widgets/CardDrawer.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

static const int LAST_ROW = 255; //row count arbitrary limitation to remove someday.


Table::Table(QWidget * parent)
	: QWidget(parent)
	, m_Layout( * new QGridLayout(this) )
	, m_AddButton( * new QPushButton( tr("Table is empty, click here to add widget.") ) )
	, m_AddDialog(parent ? parent : this)
	, m_RowCount(0)
	, m_FontSize(1.2)
{
	m_Layout.setSpacing(7);

	m_AddButton.setFlat(true);
	m_AddButton.setStyleSheet("font-size: 10pt; color: gray;");
	connect(&m_AddButton, &QPushButton::clicked, this, &Table::showAddDialog );
	m_Layout.addWidget( &m_AddButton, LAST_ROW, 0, 1, 2 );
}


void Table::addRow(QWidget * w)
{
	if (!w) return;
	if (m_RowCount == LAST_ROW) return;

	w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	if (w->whatsThis().isEmpty())
	{
		// Full Column span.
		m_Layout.addWidget( w, m_RowCount, 0, 1, 2 );
	}
	else
	{
		QLabel * l = new QLabel(w->whatsThis());
		l->setMinimumHeight(w->minimumHeight());
		m_Layout.addWidget( l, m_RowCount, 0 );
		m_Layout.addWidget( w, m_RowCount, 1 );
	}
	m_AddButton.setText( tr("Add a widget") );
	++m_RowCount;
	
	updateFontSize();
}

void Table::deleteRow(int row)
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

void Table::showAddDialog()
{
	if (m_AddDialog.exec())
	{
		loadJson(QJsonDocument(m_AddDialog.getJsonOutput()));
	}
}

void Table::showLoadDialog()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Select file"), "./examples", "JSON templates (*.json)");
	if (!filename.isEmpty())
	{
		clearTable();
		loadJsonFile(filename);
	}
}

void Table::clearTable()
{
	for (int row = 0; row < m_RowCount ; ++row)
	{
		deleteRow(row);
	}
	m_AddButton.show();
	m_RowCount = 0;
}

std::vector<int> Table::asIntVector(QJsonValue array, std::vector<int> default_out)
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

std::vector<QString> Table::asStringVector(QJsonValue array)
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

void Table::loadJsonFile(QString filename)
{
	QString val;
	QFile file;
	file.setFileName(filename);
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	val = file.readAll();
	file.close();

	loadJson(QJsonDocument::fromJson(val.toUtf8()));
}

void Table::loadJson(QJsonDocument json_doc)
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

QWidget * Table::createWidget(QJsonObject const & item)
{
	QWidget * pWidget = nullptr;

	QString type = item["Type"].toString("<UNDEFINED>");
	if (type == "Dice")
	{
		pWidget = new Dice( item["NbSides"].toInt(6),
				item["Count"].toInt(1) );
	}
	else if (type == "Sortition")
	{
		pWidget = new Dice( asStringVector(item["List"]),
				item["Count"].toInt(1) );
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
	else if (type == "CardDrawer")
	{
		if (item.contains("Cards"))
		{
			pWidget = new CardDrawer( asStringVector(item["Cards"]), item["NbDrawing"].toInt(5) );
		}
		else
		{
			pWidget = new CardDrawer( item["DeckSize"].toInt(10), item["NbDrawing"].toInt(5) );
		}
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
		pWidget->setMinimumSize( { 300,30 } );
	}

	return pWidget;
}

void Table::updateFontSize()
{
	QWidget::ensurePolished();
	const QFont app_font = qobject_cast<QApplication *>(QCoreApplication::instance())->font();

	float const x_ratio = (float)size().width() / sizeHint().width();
	float const y_ratio = (float)size().height() / sizeHint().height();
	float const ratio = std::min(x_ratio,y_ratio);
	size_t const point_size = (size_t) (app_font.pointSize() * m_FontSize * ratio);
	if (point_size)
	{
		QWidget::setStyleSheet("* { font-size: " + QString::number(point_size) + "pt; }" );
	}
}

void Table::wheelEvent(QWheelEvent *event)
{
	if ( event->modifiers() & Qt::ControlModifier )
	{
		m_FontSize *= 1. + (float) event->angleDelta().y() / 1200.;
		updateFontSize();

		event->accept();
	}
}

void Table::resizeEvent(QResizeEvent* event)
{
	updateFontSize();
	QWidget::resizeEvent(event);
}

QSize Table::sizeHint() const
{
	return { 350, 50*(m_RowCount+1) } ;
}

