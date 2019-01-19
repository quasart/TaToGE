#pragma once

#include <QMainWindow>

#include <QIcon>
#include <QAction>
#include <QActionGroup>
#include <QMenuBar>
#include <QMessageBox>

#include "Table.h"



class MainWindow : public QMainWindow
{
Q_OBJECT

public:
	MainWindow()
		: m_Table(this)
	{
		setWindowIcon(QIcon(":images/dice.svg"));
		setCentralWidget(&m_Table);

		QMenu * fileMenu = menuBar()->addMenu(tr("&File"));
		{
			QAction * action = fileMenu->addAction(tr("&Load table layout from file..."));
			action->setShortcuts(QKeySequence::Open);
			connect(action, &QAction::triggered, &m_Table, &Table::showLoadDialog );
		}
		{
			QAction * action = fileMenu->addAction(tr("&New empty table"));
			action->setShortcuts(QKeySequence::New);
			connect(action, &QAction::triggered, &m_Table, &Table::clearTable );
		}
		{
			QAction * action = fileMenu->addAction(tr("&Exit"));
			action->setShortcuts(QKeySequence::Quit);
			connect(action, &QAction::triggered, this, &MainWindow::close );
		}

		{
			QAction * addAction = menuBar()->addAction(tr("Add a &widget"));
			connect(addAction, &QAction::triggered, &m_Table, &Table::showAddDialog );
		}
		{
			QAction * helpMenu = menuBar()->addAction(tr("&Help"));
			helpMenu->setShortcuts(QKeySequence::HelpContents);
			connect(helpMenu, &QAction::triggered, [this](){ QMessageBox::information(this, "",
					tr("This application provides a bunch of simple widgets to emulate the common game equipment needed when playing board games or card games.") + "\n\n" +
					tr("Click on 'Add a widget' to set equipement on table before using them.") + "\n\n" +
					tr("Created by Alfred Massard")); }  );
		}

		QStringList args = QCoreApplication::arguments();
		if (args.count() > 1)
		{
			m_Table.loadJsonFile( args.at(1) );
		}
	}

private:
	Table m_Table;
};
