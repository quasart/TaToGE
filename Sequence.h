#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>

#include <iostream>

class Sequence : public QWidget
{
Q_OBJECT

public:
	explicit Sequence(std::vector<QString> labels, QWidget * parent = nullptr)
		: QWidget(parent)
		, m_Label(*new QLabel)
		, m_TurnLabels(labels)
		, m_Value(0)
	{
		if (labels.empty())
		{
			throw std::runtime_error("No label provided for Sequence serie.");
		}

		QWidget::setLayout(new QHBoxLayout(this));
		QWidget::layout()->setSpacing(0);
		QWidget::layout()->setContentsMargins(0,0,0,0);

		{
			QPushButton * btn = new QPushButton("<", parent);
			btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
			btn->setFixedWidth(30);
			QWidget::layout()->addWidget(btn);
			connect(btn, &QPushButton::clicked, this, &Sequence::previous);
		}

		m_Label.setText(labels.front());
		m_Label.setAlignment(Qt::AlignCenter);
		m_Label.setMinimumWidth(80);
		QWidget::layout()->addWidget(&m_Label);

		{
			QPushButton * btn = new QPushButton(">", parent);
			btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
			btn->setFixedWidth(30);
			QWidget::layout()->addWidget(btn);
			connect(btn, &QPushButton::clicked, this, &Sequence::next);
		}

	}

private:
	QLabel & m_Label;
	std::vector<QString> m_TurnLabels;
	size_t m_Value;

protected:

signals:

public slots:
	void previous()
	{
		if (m_Value==0)
		{
			m_Value = m_TurnLabels.size();
		}
		m_Value--;

		m_Label.setText( m_TurnLabels.at(m_Value) );
	}

	void next()
	{
		m_Value++;
		if (m_Value==m_TurnLabels.size())
		{
			m_Value = 0;
		}
		m_Label.setText( m_TurnLabels.at(m_Value) );
	}
};


// --------------------------------------------------



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

		m_Label.setText(QString::number(max_value));
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

	}

private:
	QLabel & m_Label;
	size_t m_MaxValue;
	size_t m_Value;

protected:

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

		m_Label.setText( QString::number(m_Value) );
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

		m_Label.setText( QString::number(m_Value) );
	}
};
