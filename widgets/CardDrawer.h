#pragma once

#include <QTimer>
#include <QPushButton>
#include <QHBoxLayout>

#include <chrono>
#include <random>
#include <exception>
#include <atomic>

#include "widgets/common.h"

class CardDrawer : public QWidget
{
Q_OBJECT

public:
	explicit CardDrawer(std::vector<QString> deck, size_t slot_nb = 10, QWidget * parent = nullptr)
		: CardDrawer( deck.size(), slot_nb, parent)
	{
		m_CardNames = deck;
	}

	explicit CardDrawer(size_t deck_size, size_t slot_nb = 10, QWidget * parent = nullptr)
		: QWidget(parent)
		, m_CardNames()
		, m_DeckOrder(deck_size)
		, m_Slots()
		, m_NextIndex(slot_nb)
		, m_Timer(this)
		, m_Generator(getRandomSeed())
	{
		if (deck_size==0)
		{
			throw std::runtime_error("CardDrawer created with empty deck.");
		}
		if (slot_nb==0)
		{
			throw std::runtime_error("CardDrawer created with no slot.");
		}

		for (size_t i = 0; i < m_DeckOrder.size(); ++i)
		{
			m_DeckOrder[i] = i;
		}

		QWidget::setLayout(new QHBoxLayout(this));
		QWidget::layout()->setContentsMargins(0,0,0,0);

		m_Timer.setInterval(300);
		connect(&m_Timer, SIGNAL(timeout()), this, SLOT(update()));

		for (size_t i = 0; i < slot_nb && i < deck_size; ++i)
		{
			QPushButton * btn = new QPushButton(HIDDEN_CARD);
			btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
			btn->setMinimumWidth(5);

			connect(btn, SIGNAL(clicked()), this, SLOT(update()));
			m_Slots.push_back(btn);
			QWidget::layout()->addWidget(btn);
		}
	}

	int deckSize() const { return m_DeckOrder.size(); }
	int nbDrawing() const { return m_Slots.size(); }
	int drawingTime() const { return m_Timer.interval(); }
	std::vector<QString> const & cards() const { return m_CardNames; }

	void setDrawingTime( int drawing_time_ms )
	{
		m_Timer.setInterval(drawing_time_ms);
	}

protected:
	std::vector<QString> m_CardNames;
	std::vector<size_t> m_DeckOrder;
	std::vector<QPushButton*> m_Slots;
	std::atomic<size_t> m_NextIndex;
	QTimer m_Timer;
	std::default_random_engine m_Generator;

	static constexpr const char * const HIDDEN_CARD = "?";

signals:

public slots:

	void draw()
	{
		m_NextIndex = 0;
		mixDeck();
		for (auto slot : m_Slots)
		{
			slot->setText(HIDDEN_CARD);
		}
		emit update();

		if (drawingTime() > 0)
		{
			m_Timer.start();
		}
	}

	void update()
	{
		size_t const current_index = m_NextIndex++;

		if (current_index < m_Slots.size())
		{
			size_t const drawn_card = m_DeckOrder.at(current_index);
			m_Slots[current_index]->setText( card_name(drawn_card) );
		}
		else // no draw in progress.
		{
			draw();
		}

		if (current_index == m_Slots.size()-1) // Was last slot.
		{
			m_Timer.stop();
		}
	}

protected:
	QString card_name(size_t card_id) const
	{
		return m_CardNames.empty() ? QString::number(card_id+1) : m_CardNames.at(card_id);
	}

	void mixDeck()
	{
		std::uniform_real_distribution<float> distribution;
		std::vector<float> random_indexes(m_DeckOrder.size());
		for ( float & i : random_indexes )
		{
			i = distribution(m_Generator);
		}

		std::sort( m_DeckOrder.begin(), m_DeckOrder.end(),
				[&random_indexes](size_t a, size_t b) { return random_indexes[a] < random_indexes[b]; } );
	}

};


