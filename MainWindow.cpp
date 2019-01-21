#include "MainWindow.h"

#include <QIcon>
#include <QAction>
#include <QActionGroup>
#include <QMenuBar>
#include <QMessageBox>

MainWindow::MainWindow()
	: m_Table(this)
{
	setCentralWidget(&m_Table);

	QMenu * fileMenu = menuBar()->addMenu(tr("&File"));
	{
		QAction * action = fileMenu->addAction(tr("&Load table layout from file..."));
		action->setShortcuts(QKeySequence::Open);
		action->setIcon(QIcon::fromTheme("document-open"));
		connect(action, &QAction::triggered, &m_Table, &Table::showLoadDialog );
	}
	{
		QAction * action = fileMenu->addAction(tr("&New empty table"));
		action->setShortcuts(QKeySequence::New);
		action->setIcon(QIcon::fromTheme("document-revert"));
		connect(action, &QAction::triggered, &m_Table, &Table::clearTable );
	}
	{
		QAction * action = fileMenu->addAction(tr("&Exit"));
		action->setShortcuts(QKeySequence::Quit);
		action->setIcon(QIcon::fromTheme("application-exit", style()->standardIcon(QStyle::SP_DialogCloseButton)));
		connect(action, &QAction::triggered, this, &MainWindow::close );
	}

	{
		QAction * addAction = menuBar()->addAction(tr("Add a &widget"));
		connect(addAction, &QAction::triggered, &m_Table, &Table::showAddDialog );
	}


	QMenu * helpMenu = menuBar()->addMenu(tr("&Help"));
	{
		QAction * action = helpMenu->addAction(tr("&General description"));
		action->setShortcuts(QKeySequence::HelpContents);
		action->setIcon(QIcon::fromTheme("help-contents"));
		connect(action, &QAction::triggered, [this](){ QMessageBox::information(this, "",
				tr("This application provides a bunch of simple widgets to emulate the common game equipment needed when playing board games or card games.") + "\n\n" +
				tr("Click on 'Add a widget' to set equipement on table before using them.")); }  );
	}
	{
		QAction * action = helpMenu->addAction(tr("&About"));
		action->setIcon(QIcon::fromTheme("help-about"));
		connect(action, &QAction::triggered, [this](){ QMessageBox::information(this, "",
				QCoreApplication::applicationName() + "\n" +
				tr("Created by Alfred Massard") + ".\n\n" +
				tr("Version: ") + QCoreApplication::applicationVersion() + "\n" +
				tr("Build: ") + __DATE__ + " " +__TIME__
				); }  );
	}

	QStringList args = QCoreApplication::arguments();
	if (args.count() > 1)
	{
		m_Table.loadJsonFile( args.at(1) );
	}
}
