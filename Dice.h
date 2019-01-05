#include <QTimer>
#include <QPushButton>

#include <iostream>
#include <atomic>
#include <chrono>
#include <random>
#include <exception>


class Dice : public QPushButton
{
Q_OBJECT

public:
	explicit Dice(size_t face_nb = 6, QWidget * parent = nullptr)
		: QPushButton(QString::number(face_nb), parent)
		, m_FaceCount(face_nb)
		, m_Timer(this)
		, m_RollTime()
		, m_Generator(std::random_device{}())
	{
		if (!face_nb)
		{
			throw std::runtime_error("Dice constructed with no face.");
		}
		connect(&m_Timer, SIGNAL(timeout()), this, SLOT(update()));
		connect(this, SIGNAL(clicked()), this, SLOT(roll()));

		QPushButton::setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	}

	explicit Dice(std::vector<QString> faces, QWidget * parent = nullptr)
		: Dice(faces.size(), parent)
	{
		m_FaceLabels = faces;
		QPushButton::setText(faces.back());
	}

	bool isRunning() const
	{
		return m_Timer.isActive();
	}

private:
	size_t m_FaceCount;
	std::vector<QString> m_FaceLabels;

	QTimer m_Timer;
	std::atomic<size_t> m_RollTime;
	std::default_random_engine m_Generator;

protected:

signals:

public slots:

	void roll()
	{
		m_RollTime = 10;
		QPushButton::setEnabled(false);
		setRandom();

		using namespace std::literals::chrono_literals;
		m_Timer.start(100ms);
	}

	void update()
	{
		if (m_RollTime==0)
		{
			m_Timer.stop();
			QPushButton::setEnabled(true);
		}
		else
		{
			m_RollTime--;
		}
		setRandom();
	}

	void setRandom()
	{
		std::uniform_int_distribution<int> distribution(1,m_FaceCount);
		int value = distribution(m_Generator);

		if ( m_FaceLabels.empty() )
		{
			setText( QString::number(value) );
		}
		else
		{
			setText( m_FaceLabels.at(value-1) );
		}
	}

};


