#pragma once

#include <QHBoxLayout>

#include "widgets/Dice.h"

class DiceCup : public QWidget
{
Q_OBJECT

public:
	explicit DiceCup(QWidget * parent = nullptr)
		: QWidget(parent)
	{
		QWidget::setLayout(new QHBoxLayout(this));
		QWidget::layout()->setContentsMargins(0,0,0,0);
	}

	void addDice(Dice * d)
	{
		disconnect(d, SIGNAL(clicked()), d, SLOT(initRoll()));
		connect(d, SIGNAL(clicked()), this, SLOT(initRoll()));
		QWidget::layout()->addWidget(d);
		m_Dices.push_back(d);
	}

	std::vector<Dice *> const & dices() const { return m_Dices; }

private:
	std::vector<Dice*> m_Dices;

signals:

public slots:

	void initRoll()
	{
		for ( Dice * btn : m_Dices )
		{
			btn->initRoll();
		}
	}

};


