#pragma once

#include <QTimer>
#include <QProgressBar>
#include <QObject>
#include <QMouseEvent>

#include <chrono>

class Timer : public QProgressBar
{
Q_OBJECT

public:
	explicit Timer(size_t duration_sec = 30, QWidget * parent = nullptr )
		: QProgressBar(parent)
		, m_Timer(this)
	{
		QProgressBar::setMaximum(duration_sec * 1000);
		QProgressBar::setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		QProgressBar::setFocusPolicy(Qt::StrongFocus);

		connect(&m_Timer, SIGNAL(timeout()), this, SLOT(update()));
	}

	void start()
	{
		QProgressBar::setValue( QProgressBar::maximum() );
		m_BeginTime = std::chrono::steady_clock::now();
		size_t refresh_time_ms = QProgressBar::maximum() / 100 / 5;
		
		// We don't need more precision.
		if (refresh_time_ms < 50)
		{
			refresh_time_ms = 50;
		}

		m_Timer.start(refresh_time_ms);
	}

	void stop()
	{
		QProgressBar::reset();
		m_Timer.stop();
	}

	bool isRunning() const
	{
		return m_Timer.isActive();
	}

	void toggle()
	{
		if (isRunning())
		{
			stop();
		}
		else
		{
			start();
		}
	}

private:
	QTimer m_Timer;
	std::chrono::time_point<std::chrono::steady_clock> m_BeginTime;


protected:
	void mouseReleaseEvent(QMouseEvent *event) override
	{
		if (event->button() == Qt::LeftButton)
		{
			toggle();
		}

		QProgressBar::mouseReleaseEvent(event);
	}

	void keyPressEvent(QKeyEvent * event) override
	{
		if (event->key() == Qt::Key_Space)
		{
			toggle();
		}
		QProgressBar::keyPressEvent(event);
	}

signals:
	// TODO finish signal ?

public slots:
	void update()
	{
		auto now = std::chrono::steady_clock::now();
		size_t elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now-m_BeginTime).count();
		size_t max = QProgressBar::maximum();

		if (elapsed >= max)
		{
			stop();
			//TODO play sound
		}
		else
		{
			QProgressBar::setValue( max - elapsed );
		}
	}
};


