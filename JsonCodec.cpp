#include "JsonCodec.h"

#include "widgets/Timer.h"
#include "widgets/Counter.h"
#include "widgets/Dice.h"
#include "widgets/DiceCup.h"
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

Dice * createDice(QJsonObject const & item)
{
	Dice * dice = nullptr;

	if (item.contains("List"))
	{
		dice = new Dice( JsonCodec::asStringVector(item["List"]) );
	}
	else
	{
		dice = new Dice( item["NbSides"].toInt(6) );
		dice->setFirstFace( item["FirstFace"].toInt(1) );
	}

	dice->setRollDuration( item["RollDuration"].toDouble(1.5) );
	return dice;
}

QJsonObject buildDiceJson(Dice const * d)
{
	QJsonObject item;
	item["Type"] = "Dice";
	if (d->faceLabels().empty())
	{
		item["NbSides"] = d->nbSides();
		item["FirstFace"] = d->firstFace();
	}
	else
	{
		item["List"] = makeQJsonArray( d->faceLabels() );
	}
	item["RollDuration"] = d->rollDuration();
	return item;
}

QJsonObject JsonCodec::widgetJson(QWidget const & widget)
{
	QJsonObject item;

	if (Dice const * dice = qobject_cast<Dice const*>(&widget))
	{
		item = buildDiceJson(dice);
	}
	else if (DiceCup const * dicecup = qobject_cast<DiceCup const*>(&widget))
	{
		QJsonArray dice_array;
		for ( Dice * d : dicecup->dices() )
		{
			dice_array.push_back( buildDiceJson(d) );
		}
		item["Type"] = "DiceCup";
		item["List"] = dice_array;
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
		item["Increments"] = makeQJsonArray(counter->increments());
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
		item["DrawingTime"] = drawer->drawingTime();
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
	else
	{
		qWarning() << "Unexpected widget during serialization: " << &widget;
		item["Type"] = "Space";
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
		const int count = item["Count"].toInt(1);
		if (count==1)
		{
			pWidget = createDice(item);
		}
		else
		{
			DiceCup * dg = new DiceCup();
			for (int i=0; i<count && i<20; ++i)
			{
				dg->addDice( createDice(item) );
			}
			pWidget = dg;
		}
	}
	else if (type == "DiceCup")
	{
		DiceCup * dg = new DiceCup();
		for (QJsonValueRef group_elem : item["List"].toArray())
		{
			dg->addDice( createDice(group_elem.toObject()) );
		}
		pWidget = dg;
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
		CardDrawer * drawer;
		if (item.contains("Cards"))
		{
			drawer = new CardDrawer( asStringVector(item["Cards"]), item["NbDrawing"].toInt(5) );
		}
		else
		{
			drawer = new CardDrawer( item["DeckSize"].toInt(10), item["NbDrawing"].toInt(5) );
		}
		drawer->setDrawingTime(item["DrawingTime"].toInt(300));
		pWidget = drawer;
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



