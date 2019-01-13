#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>

class Counter : public QWidget
{
Q_OBJECT

public:
	explicit Counter(int init = 0, std::vector<int> increments = {1,5,10}, QWidget * parent = nullptr)
		: QWidget(parent)
		, m_Value(init)
		, m_InitValue(init)
		, m_Label(*new QLabel)
	{
		QWidget::setLayout(new QHBoxLayout(this));
		QWidget::layout()->setSpacing(0);
		QWidget::layout()->setContentsMargins(0,0,0,0);

		for (int i : increments)
		{
			addIncrButton(-i);
		}

		m_Label.setText( QString::number(m_Value) );
		m_Label.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		m_Label.setMinimumWidth(50);
		m_Label.setAlignment( Qt::AlignCenter );

		QWidget::layout()->addWidget( &m_Label );
		for (int i : increments)
		{
			addIncrButton(i);
		}
	}

private:

	int m_Value;
	int m_InitValue;
	QLabel & m_Label;

protected:

	void addIncrButton(int i)
	{
			QString txt = QString::number(i);
			if (i>0)
			{
				txt = "+" + txt;
			}
			QPushButton * btn = new QPushButton(txt);
			btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
			btn->setFixedWidth(30);
			btn->setStyleSheet("font-size: 11pt;");

			QWidget::layout()->addWidget(btn);
			connect(btn, &QPushButton::clicked, [=]{ this->updateValue(i); } );
	}

	void mouseDoubleClickEvent( QMouseEvent * event ) override
	{
		m_Value = m_InitValue;
		m_Label.setText( QString::number(m_Value) );

		QWidget::mouseDoubleClickEvent(event);
	}


signals:

public slots:
	void updateValue(int i)
	{
		m_Value += i;
		m_Label.setText( QString::number(m_Value) );
	}

};


