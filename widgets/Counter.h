#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLCDNumber>

class Counter : public QWidget
{
Q_OBJECT

public:
	explicit Counter(int init = 0, std::vector<int> increments = {1,5,10}, QWidget * parent = nullptr)
		: QWidget(parent)
		, m_Value(init)
		, m_ResetValue(init)
		, m_Label(*new QLCDNumber)
		, m_Layout(*new QHBoxLayout(this))
	{
		QWidget::setLayout(&m_Layout);
		QWidget::layout()->setSpacing(2);
		QWidget::layout()->setContentsMargins(0,0,0,0);

		for (int i : increments)
		{
			addIncrButton(-i);
		}

		m_Label.display( m_Value );
		m_Label.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		m_Label.setMinimumWidth(50);
		m_Label.setSegmentStyle( QLCDNumber::Flat );
		m_Label.setLineWidth(0);

		int const stretch_factor = increments.size() < 5 ? 100-10*increments.size() : 50;
		m_Layout.addWidget( &m_Label, stretch_factor );

		for (int i : increments)
		{
			addIncrButton(i);
		}
	}

	int value() const { return m_Value; }

	int getResetValue() const { return m_ResetValue; }
	void setResetValue(int v) { m_ResetValue = v; }

private:
	int m_Value;
	int m_ResetValue;
	QLCDNumber & m_Label;
	QHBoxLayout & m_Layout;

protected:

	void addIncrButton(int i)
	{
			QString txt = QString::number(i);
			if (i>0)
			{
				txt = "+" + txt;
			}
			QPushButton * btn = new QPushButton(txt);
			btn->setMinimumWidth(30);
			btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
			btn->setStyleSheet("font-size: 10pt;");
			m_Layout.addWidget( btn, 5 );
			connect(btn, &QPushButton::clicked, [=]{ this->updateValue(i); } );
	}

	void mouseDoubleClickEvent( QMouseEvent * event ) override
	{
		m_Value = m_ResetValue;
		m_Label.display( m_Value );

		QWidget::mouseDoubleClickEvent(event);
	}


signals:

public slots:
	void updateValue(int i)
	{
		m_Value += i;
		m_Label.display( m_Value );
	}

};


