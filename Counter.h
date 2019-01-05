#include <QSignalMapper>
#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>

#include <iostream>

class Counter : public QWidget
{
Q_OBJECT

public:
	explicit Counter(QWidget * parent, int init = 0, std::vector<int> increments = {-1,1} )
		: QWidget(parent)
		, m_Value(init)
		, m_Label(*new QLabel(QString::number(m_Value), this) )
	{
		QHBoxLayout *layout = new QHBoxLayout(this);
		layout->addWidget( &m_Label );

		for (int i : increments)
		{
			QPushButton * btn = new QPushButton(QString::number(i), this);
			btn->setFixedWidth(20);
			layout->addWidget(btn);
			connect(btn, &QPushButton::clicked, [=]{ this->updateValue(i); } );
		}
	}


private:

	int m_Value;
	QLabel & m_Label;

protected:

signals:
	// TODO finish signal ?

public slots:
	void updateValue(int i)
	{
		m_Value += i;
		m_Label.setText( QString::number(m_Value) );
	}

};


