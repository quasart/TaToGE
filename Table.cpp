#include "Table.h"

#include <QFile>
#include <QDebug>
#include <QResizeEvent>
#include <QApplication>
#include <QFont>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

#include "JsonCodec.h"

static const int LAST_ROW = 255; //row count arbitrary limitation to remove someday.


Table::Table(QWidget * parent)
	: QFrame(parent)
	, m_Layout( * new QGridLayout(this) )
	, m_AddButton( * new QPushButton( tr("Table is empty, click here to add widget.") ) )
	, m_AddDialog(parent ? parent : this)
	, m_RowCount(0)
	, m_FontSize(1.2)
{
	m_Layout.setSpacing(7);

	QFrame::setFrameStyle(QFrame::NoFrame);

	m_AddButton.setFlat(true);
	m_AddButton.setObjectName("AddWidgetButton");
	m_AddButton.setStyleSheet( "font-size: 10pt;");
	connect(&m_AddButton, &QPushButton::clicked, this, &Table::showAddDialog );
	m_Layout.addWidget( &m_AddButton, LAST_ROW, 0, 1, 2 );
}


void Table::addRow(QWidget * w)
{
	if (!w) return;
	if (m_RowCount == LAST_ROW) return;

	w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	w->setMinimumSize( { 300,30 } );

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

void Table::clearTable()
{
	for (int row = 0; row < m_RowCount ; ++row)
	{
		deleteRow(row);
	}
	m_RowCount = 0;
}

bool Table::loadJsonFile(QString filename)
{
	QString val;
	QFile file;
	file.setFileName(filename);
	if (not file.open(QIODevice::ReadOnly | QIODevice::Text) )
	{
		return false;
	}

	val = file.readAll();
	file.close();

	return loadJson(QJsonDocument::fromJson(val.toUtf8()));
}

bool Table::loadJson(QJsonDocument json_doc)
{
	if (json_doc.isNull())
	{
		qWarning() << "Invalid JSON cannot be decoded.";
		return false;
	}

	try
	{
		if (json_doc.isObject())
		{
			QJsonObject const o = json_doc.object();
			addRow( JsonCodec::createWidget(o) );
		}
		else for ( QJsonValueRef const i : json_doc.array() )
		{
			QJsonObject const & item = i.toObject();
			addRow( JsonCodec::createWidget(item) );
		}
		return true;
	}
	catch (std::exception const & e)
	{
		qWarning() << "Exception caught when adding Widget. Error:" << e.what() << json_doc;
		return false;
	}
}

QJsonDocument Table::toJson() const
{
	QJsonArray doc_array;

	int const col = 1;
	for (int row = 0; row < m_RowCount; ++row)
	{
		auto pItem = m_Layout.itemAtPosition(row,col);
		if (pItem && pItem->widget())
		{
			doc_array.append( JsonCodec::widgetJson(*pItem->widget()) );
		}
	}

	return QJsonDocument( doc_array );
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

