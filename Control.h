#include <QObject>
#include <QPushButton>

#include <iostream>


class Control : public QObject
{
    Q_OBJECT

public:
    explicit Control(QPushButton * btn)
    {
    
        QObject::connect(btn, SIGNAL(clicked()), this, SLOT(exec()));
    
    }

signals:
public slots:
    void exec()
    {
        std::cout << "Ok!" << std::endl;

    }


};

