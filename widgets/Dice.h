#pragma once

#include <QTimer>
#include <QPushButton>
#include <QHBoxLayout>

#include <QMouseEvent>

#include <atomic>
#include <chrono>
#include <random>
#include <exception>

#include "widgets/common.h"

class Dice : public QPushButton
{
Q_OBJECT

public:
	explicit Dice(size_t face_nb = 6, QWidget * parent = nullptr)
		: QPushButton(QString::number(face_nb),parent)
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

		m_Timer.setInterval(100);
		connect(&m_Timer, SIGNAL(timeout()), this, SLOT(rollTick()));

		this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		this->setMinimumWidth(5);
		connect(this, SIGNAL(clicked()), this, SLOT(initRoll()));

	}

	explicit Dice(std::vector<QString> faces, QWidget * parent = nullptr)
		: Dice(faces.size(), parent)
	{
		m_FaceLabels = faces;
		this->setText(faces.back());
	}

	int nbSides() const { return m_FaceCount; }
	int firstFace() const { return m_FirstFace; }
	double rollDuration() const { return m_RollDuration_ms/1000.; }
	std::vector<QString> const & faceLabels() const { return m_FaceLabels; }

	void setFirstFace(int firstFace)
	{
		m_FirstFace = firstFace;
		setText( QString::number(m_FaceCount-1+m_FirstFace) );
	}

	void setRollDuration(double duration)
	{
		if (duration>=0)
		{
			m_RollDuration_ms = static_cast<size_t>(duration*1000.);
		}
	}

private:
	size_t m_FaceCount;
	int m_FirstFace = 1;
	std::vector<QString> m_FaceLabels;

	QTimer m_Timer;
	std::atomic<size_t> m_RollCountdown;
	size_t m_RollDuration_ms;
	std::default_random_engine m_Generator;


signals:

public slots:

	void initRoll()
	{
		if (!isEnabled())
			return;

		m_RollCountdown = m_RollDuration_ms / m_Timer.interval();

		if (m_RollCountdown>0)
		{
			// start rolling
			setStyleSheet("font-weight : 100; color: #555;");
			m_Timer.start();
		}

		setRandom();
	}

	void rollTick()
	{
		if (m_RollCountdown==0)
		{
			// stop rolling
			setStyleSheet("");

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
		std::uniform_int_distribution<int> distribution(0,m_FaceCount-1);

		int value = distribution(m_Generator);
		if ( m_FaceLabels.empty() )
		{
			setText( QString::number(value+m_FirstFace) );
		}
		else
		{
			setText( m_FaceLabels.at(value) );
		}
	}

protected:
	bool event(QEvent *event) override
	{
		if (event->type() == QEvent::MouseButtonPress)
		{
			QMouseEvent* mouseEvent = (QMouseEvent*)event;
			if ( mouseEvent->modifiers() & Qt::ControlModifier &&
					mouseEvent->button() == Qt::LeftButton )
			{
				setEnabled(!isEnabled());
				return true;
			}
		}
		else if (event->type() == QEvent::MouseButtonDblClick)
		{
			// Disabled to avoid click when enabling button.
			return false;
		}

		return QPushButton::event(event);
	}

};


