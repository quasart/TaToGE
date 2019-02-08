#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QStyle>



class CountDown : public QWidget
{
Q_OBJECT

public:
	explicit CountDown(size_t max_value, QWidget * parent = nullptr)
		: QWidget(parent)
		, m_Label(*new QLabel)
		, m_Layout(*new QHBoxLayout(this))
		, m_MaxValue(max_value)
		, m_Value(max_value)
	{
		QWidget::setLayout(&m_Layout);
		QWidget::layout()->setSpacing(0);
		QWidget::layout()->setContentsMargins(0,0,0,0);

		{
			QPushButton * btn = new QPushButton("\302\253");
			btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
			btn->setMinimumWidth(30);
			btn->setStyleSheet("font-size: 10pt;");
			m_Layout.addWidget(btn,5);
			connect(btn, &QPushButton::clicked, this, &CountDown::previous);
		}

		{
			m_Label.setAlignment(Qt::AlignCenter);
			m_Label.setMinimumWidth(40);
			m_Layout.addWidget(&m_Label,90);
		}

		{
			QPushButton * btn = new QPushButton("\302\273");
			btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
			btn->setMinimumWidth(30);
			btn->setStyleSheet("font-size: 10pt;");
			m_Layout.addWidget(btn,5);
			connect(btn, &QPushButton::clicked, this, &CountDown::next);
		}

		CountDown::updateLabel();
	}

	int maxValue() const { return m_MaxValue; }
	int value() const { return m_Value; }
	void setValue(int v) { m_Value = v; updateLabel(); }

protected:
	QLabel & m_Label;
	QHBoxLayout & m_Layout;
	size_t m_MaxValue;
	size_t m_Value;

	virtual void updateLabel()
	{
		m_Label.setText( QString::number(m_Value) );
	}

	void mouseDoubleClickEvent( QMouseEvent * event ) override
	{
		m_Value = m_MaxValue;
		updateLabel();

		QWidget::mouseDoubleClickEvent(event);
	}

signals:

public slots:
	void next()
	{
		if (m_Value==0)
		{
			m_Value = m_MaxValue;
		}
		else
		{
			m_Value--;
		}

		updateLabel();
	}

	void previous()
	{
		if (m_Value == m_MaxValue)
		{
			m_Value = 0;
		}
		else
		{
			m_Value++;
		}

		updateLabel();
	}
};


// --------------------------------------------------

class Sequence : public CountDown
{
Q_OBJECT

public:
	explicit Sequence(std::vector<QString> steps, QWidget * parent = nullptr)
		: CountDown(steps.size()-1, parent)
		, m_Steps(steps)
	{
		if (steps.empty())
		{
			throw std::runtime_error("Sequence constructed with no step.");
		}
		Sequence::updateLabel();
	}

	std::vector<QString> const & steps() const { return m_Steps; }

	int currentStep() const { return m_MaxValue-m_Value; }
	void setCurrentStep( int s ) { setValue(m_MaxValue-s); }

protected:
	std::vector<QString> m_Steps;

	void updateLabel() override
	{
		m_Label.setText( m_Steps.at(currentStep()) );
	}

};





