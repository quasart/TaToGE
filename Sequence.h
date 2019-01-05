#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>

#include <iostream>

class Sequence : public QHBoxLayout
{
Q_OBJECT

public:
	explicit Sequence(QWidget * parent, std::vector<QString> labels)
		: QHBoxLayout(parent)
		, m_Label(*new QLabel(parent))
		, m_TurnLabels(labels)
		, m_Value(0)
	{
		if (labels.empty())
		{
			throw std::runtime_error("No label provided for Sequence serie.");
		}

		{
			QPushButton * btn = new QPushButton("<", parent);
			btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
			btn->setFixedWidth(30);
			QHBoxLayout::addWidget(btn);
			connect(btn, &QPushButton::clicked, this, &Sequence::previous);
		}

		m_Label.setText(labels.front());
		m_Label.setAlignment(Qt::AlignCenter);
		m_Label.setMinimumWidth(80);
		QHBoxLayout::addWidget(&m_Label);

		{
			QPushButton * btn = new QPushButton(">", parent);
			btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
			btn->setFixedWidth(30);
			QHBoxLayout::addWidget(btn);
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



class CountDown : public QHBoxLayout
{
Q_OBJECT

public:
	explicit CountDown(QWidget * parent, size_t init)
		: QHBoxLayout(parent)
		, m_Label(*new QLabel(parent))
		, m_MaxValue(init)
		, m_Value(init)
	{
		{
			QPushButton * btn = new QPushButton("<", parent);
			btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
			btn->setFixedWidth(30);
			QHBoxLayout::addWidget(btn);
			connect(btn, &QPushButton::clicked, this, &CountDown::previous);
		}

		m_Label.setText(QString::number(init));
		m_Label.setAlignment(Qt::AlignCenter);
		m_Label.setMinimumWidth(40);
		QHBoxLayout::addWidget(&m_Label);

		{
			QPushButton * btn = new QPushButton(">", parent);
			btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
			btn->setFixedWidth(30);
			QHBoxLayout::addWidget(btn);
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
