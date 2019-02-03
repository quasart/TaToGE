#include "JsonCodec.h"

#include "widgets/Timer.h"
#include "widgets/Counter.h"
#include "widgets/Dice.h"
#include "widgets/Sequence.h"
#include "widgets/CardDrawer.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

#include <QDebug>

std::vector<int> JsonCodec::asIntVector(QJsonValue array, std::vector<int> default_out)
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

std::vector<QString> JsonCodec::asStringVector(QJsonValue array)
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

template <typename T>
QJsonArray makeQJsonArray(std::vector<T> const & v)
{
	QJsonArray array;
	for ( T const & t : v )
	{
		array.push_back(t);
	}
	return array;
}

QJsonObject JsonCodec::widgetJson(QWidget const & widget)
{
	QJsonObject item;

	if (Dice const * dice = qobject_cast<Dice const*>(&widget))
	{
		if (dice->faceLabels().empty())
		{
			item["Type"] = "Dice";
			item["NbSides"] = dice->nbSides();
		}
		else
		{
			item["Type"] = "Sortition";
			item["List"] = makeQJsonArray( dice->faceLabels() );
		}
		item["Count"] = dice->count();
	}
	else if (Timer const * timer = qobject_cast<Timer const*>(&widget))
	{
		item["Type"] = "Timer";
		item["Duration"] = timer->duration();
	}
	else if (Counter const * counter = qobject_cast<Counter const*>(&widget))
	{
		item["Type"] = "Counter";
		item["Value"] = counter->value();
		item["ResetValue"] = counter->getResetValue();
//		item["Increments"] = makeQJsonArray(counter->increments());
	}
	else if (Sequence const * seq = qobject_cast<Sequence const*>(&widget))
	{
		item["Type"] = "Sequence";
		item["List"] = makeQJsonArray( seq->steps() );
		item["CurrentStep"] = seq->currentStep();
	}
	else if (CountDown const * countdown = qobject_cast<CountDown const*>(&widget))
	{
		item["Type"] = "CountDown";
		item["MaxValue"] = countdown->maxValue();
		item["Value"] = countdown->value();
	}
	else if (CardDrawer const * drawer = qobject_cast<CardDrawer const*>(&widget))
	{
		item["Type"] = "CardDrawer";
		item["NbDrawing"] = drawer->nbDrawing();
		if (drawer->cards().empty())
		{
			item["DeckSize"] = drawer->deckSize();
		}
		else
		{
			item["Cards"] = makeQJsonArray(drawer->cards());
		}
	}
	else if (QLabel const * label = qobject_cast<QLabel const*>(&widget))
	{
		item["Type"] = "Label";
		item["Text"] = label->text();
	}

	if (!widget.whatsThis().isEmpty())
	{
		item["Name"] = widget.whatsThis();
	}
	
	if (!widget.styleSheet().isEmpty())
	{
		item["Style"] = widget.styleSheet();
	}

	return item;
}

QWidget * JsonCodec::createWidget(QJsonObject const & item)
{
	QWidget * pWidget = nullptr;
	QString const type = item["Type"].toString("<UNDEFINED>");
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
		Counter * counter = new Counter(
				item["Value"].toInt(0),
				asIntVector(item["Increments"], {1,5,10} )
				);
		if (item.contains("ResetValue"))
		{
			counter->setResetValue( item["ResetValue"].toInt(0) );
		}
		pWidget = counter;
	}
	else if (type == "Sequence")
	{
		std::vector<QString> list = asStringVector(item["List"]);
		Sequence * seq = new Sequence(list);
		if (item.contains("CurrentStep"))
		{
			seq->setCurrentStep( item["CurrentStep"].toInt(0) );
		}
		pWidget = seq;
	}
	else if (type == "CountDown")
	{
		CountDown * countdown = new CountDown( item["MaxValue"].toInt() );
		if (item.contains("Value"))
		{
			countdown->setValue( item["Value"].toInt(0) );
		}
		pWidget = countdown;
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
	else if (type == "Label" || type == "Space")
	{
		pWidget = new QLabel( item["Text"].toString(" ") );
	}
	else
	{
		throw std::runtime_error( "Invalid Widget type." );
	}

	if (pWidget)
	{
		// General properties.
		pWidget->setWhatsThis( item["Name"].toString("") );
		pWidget->setStyleSheet( item["Style"].toString("") );
	}
	return pWidget;
}



