#include <QSignalMapper>
#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>

#include <iostream>

class Counter : public QHBoxLayout
{
Q_OBJECT

public:
	explicit Counter(int init = 0, std::vector<unsigned short> increments = {1,5,10}, QWidget * parent = nullptr)
		: m_Value(init)
		, m_Label(parent)
	{
		for (int i : increments)
		{
			addIncrButton(-i);
		}

		m_Label.setText( QString::number(m_Value) );
		m_Label.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		m_Label.setMinimumWidth(50);
		m_Label.setAlignment( Qt::AlignCenter );
		QHBoxLayout::addWidget( &m_Label );
		QHBoxLayout::setSpacing(2);

		for (int i : increments)
		{
			addIncrButton(i);
		}
	}

private:

	int m_Value;
	QLabel m_Label;

protected:

	void addIncrButton(int i)
	{
			QString txt = QString::number(i);
			if (i>0)
			{
				txt = "+" + txt;
			}
			QPushButton * btn = new QPushButton(txt);
			btn->setFixedWidth(30);

			QFont f = btn->font();
			f.setPointSize( 10 );
			btn->setFont(f);

			QHBoxLayout::addWidget(btn, Qt::AlignCenter);
			connect(btn, &QPushButton::clicked, [=]{ this->updateValue(i); } );
	}


signals:

public slots:
	void updateValue(int i)
	{
		m_Value += i;
		m_Label.setText( QString::number(m_Value) );
	}

};


