#pragma once

#include <QTimer>
#include <QPushButton>
#include <QHBoxLayout>

#include <atomic>
#include <chrono>
#include <random>
#include <exception>

#include "widgets/common.h"

class Dice : public QWidget
{
Q_OBJECT

public:
	explicit Dice(size_t face_nb = 6, size_t dice_nb = 1, QWidget * parent = nullptr)
		: QWidget(parent)
		, m_FaceCount(face_nb)
		, m_Timer(this)
		, m_RollCountdown(0)
		, m_RollDuration_ms(1500)
		, m_Generator(getRandomSeed())
	{
		if (!face_nb)
		{
			throw std::runtime_error("Dice constructed with no face.");
		}
		if (!dice_nb)
		{
			throw std::runtime_error("Dice constructed with zero dice.");
		}

		QWidget::setLayout(new QHBoxLayout(this));
		QWidget::layout()->setContentsMargins(0,0,0,0);

		m_Timer.setInterval(100);
		connect(&m_Timer, SIGNAL(timeout()), this, SLOT(rollTick()));

		for (size_t i = 0; i < dice_nb; ++i)
		{
			QPushButton * btn = new QPushButton(QString::number(face_nb));
			btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
			btn->setMinimumWidth(5);

			connect(btn, SIGNAL(clicked()), this, SLOT(initRoll()));
			m_Dices.push_back(btn);
			QWidget::layout()->addWidget(btn);
		}

	}

	explicit Dice(std::vector<QString> faces, size_t dice_nb = 1, QWidget * parent = nullptr)
		: Dice(faces.size(), dice_nb, parent)
	{
		m_FaceLabels = faces;
		for ( QPushButton * btn : m_Dices )
		{
			btn->setText(faces.back());
		}
	}

	int nbSides() const { return m_FaceCount; }
	int count() const { return m_Dices.size(); }
	double rollDuration() const { return m_RollDuration_ms/1000.; }
	std::vector<QString> const & faceLabels() const { return m_FaceLabels; }

	void setRollDuration(double duration)
	{
		if (duration>=0)
		{
			m_RollDuration_ms = static_cast<size_t>(duration*1000.);
		}
	}

private:
	size_t m_FaceCount;
	std::vector<QString> m_FaceLabels;
	std::vector<QPushButton*> m_Dices;

	QTimer m_Timer;
	std::atomic<size_t> m_RollCountdown;
	size_t m_RollDuration_ms;
	std::default_random_engine m_Generator;


signals:

public slots:

	void initRoll()
	{
		m_RollCountdown = m_RollDuration_ms / m_Timer.interval();

		if (m_RollCountdown>0)
		{
			// stard rolling
			for ( QPushButton * btn : m_Dices )
			{
				btn->setStyleSheet("font-weight : 100; color: #555;");
			}

			m_Timer.start();
		}

		setRandom();
	}

	void rollTick()
	{
		if (m_RollCountdown==0)
		{
			// stop rolling
			for ( QPushButton * btn : m_Dices )
			{
				btn->setStyleSheet("");
			}

			m_Timer.stop();
		}
		else
		{
			m_RollCountdown--;
		}
		setRandom();
	}

	void setRandom()
	{
		std::uniform_int_distribution<int> distribution(1,m_FaceCount);
		int sum = 0;

		for ( QPushButton * btn : m_Dices )
		{
			int value = distribution(m_Generator);
			if ( m_FaceLabels.empty() )
			{
				btn->setText( QString::number(value) );
				sum += value;
			}
			else
			{
				btn->setText( m_FaceLabels.at(value-1) );
			}
		}

		if ( m_Dices.size() > 1
				&& m_FaceLabels.empty() )
		{
			if (m_RollCountdown==0)
			{
				setToolTip( QString::number(sum) );
			}
			else
			{
				setToolTip( "" );
			}
		}
	}

};


