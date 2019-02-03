#pragma once

#include <QMainWindow>

#include "Table.h"



class MainWindow : public QMainWindow
{
Q_OBJECT

public:
	MainWindow();

	void saveTableToFile();
	void openTableFromFile();

private:
	Table m_Table;
};
