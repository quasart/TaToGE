#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>




class CountDown : public QWidget
{
Q_OBJECT

public:
	explicit CountDown(size_t max_value, QWidget * parent = nullptr)
		: QWidget(parent)
		, m_Label(*new QLabel)
		, m_MaxValue(max_value)
		, m_Value(max_value)
	{
		QWidget::setLayout(new QHBoxLayout(this));
		QWidget::layout()->setSpacing(0);
		QWidget::layout()->setContentsMargins(0,0,0,0);

		{
			QPushButton * btn = new QPushButton("<", parent);
			btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
			btn->setFixedWidth(30);
			QWidget::layout()->addWidget(btn);
			connect(btn, &QPushButton::clicked, this, &CountDown::previous);
		}

		m_Label.setAlignment(Qt::AlignCenter);
		m_Label.setMinimumWidth(40);
		QWidget::layout()->addWidget(&m_Label);

		{
			QPushButton * btn = new QPushButton(">", parent);
			btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
			btn->setFixedWidth(30);
			QWidget::layout()->addWidget(btn);
			connect(btn, &QPushButton::clicked, this, &CountDown::next);
		}

		CountDown::updateLabel();
	}

protected:
	QLabel & m_Label;
	size_t m_MaxValue;
	size_t m_Value;

	virtual void updateLabel()
	{
		m_Label.setText( QString::number(m_Value) );
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

public:
	explicit Sequence(std::vector<QString> labels, QWidget * parent = nullptr)
		: CountDown(labels.size(), parent)
		, m_TurnLabels(labels)
	{
		Sequence::updateLabel();
	}

	std::vector<QString> m_TurnLabels;

protected:
	void updateLabel() override
	{
		size_t sequence_index = m_MaxValue-m_Value; // Reversed because m_Value is downcounting.

		if (sequence_index < m_TurnLabels.size())
		{
			m_Label.setText( m_TurnLabels[sequence_index] );
		}
		else
		{
			m_Label.setText("-");
		}
	}

};




