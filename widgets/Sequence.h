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
		, m_MaxValue(max_value)
		, m_Value(max_value)
	{
		QWidget::setLayout(new QHBoxLayout(this));
		QWidget::layout()->setSpacing(0);
		QWidget::layout()->setContentsMargins(0,0,0,0);

		{
			QPushButton * btn = new QPushButton(parent);
			btn->setIcon(style()->standardIcon(QStyle::SP_ArrowBack));
			btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
			btn->setFixedWidth(30);
			btn->setFlat(true);
			QWidget::layout()->addWidget(btn);
			connect(btn, &QPushButton::clicked, this, &CountDown::previous);
		}

		{
			m_Label.setAlignment(Qt::AlignCenter);
			m_Label.setMinimumWidth(40);
			QWidget::layout()->addWidget(&m_Label);
		}

		{
			QPushButton * btn = new QPushButton(parent);
			btn->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));
			btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
			btn->setFixedWidth(30);
			btn->setFlat(true);
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

public:
	explicit Sequence(std::vector<QString> labels, QWidget * parent = nullptr)
		: CountDown(labels.size()-1, parent)
		, m_TurnLabels(labels)
	{
		if (labels.empty())
		{
			throw std::runtime_error("Sequence constructed with no step.");
		}
		Sequence::updateLabel();
	}

	std::vector<QString> m_TurnLabels;

protected:
	void updateLabel() override
	{
		size_t const sequence_index = m_TurnLabels.size()-1-m_Value;
		m_Label.setText( m_TurnLabels.at(sequence_index) );
	}

};





