#include "MainWindow.h"

#include <QIcon>
#include <QDebug>
#include <QAction>
#include <QActionGroup>
#include <QMenuBar>
#include <QMessageBox>
#include <QCoreApplication>
#include <QStyle>
#include <QJsonDocument>
#include <QFileDialog>


MainWindow::MainWindow()
	: m_Table(this)
{
	setCentralWidget(&m_Table);

	QMenu * fileMenu = menuBar()->addMenu(tr("&File"));
	{
		QAction * action = fileMenu->addAction(tr("&Load table layout from file..."));
		action->setShortcuts(QKeySequence::Open);
		action->setIcon(QIcon::fromTheme("document-open"));
		connect(action, &QAction::triggered, this, &MainWindow::openTableFromFile );
	}
	{
		QAction * action = fileMenu->addAction(tr("&Save as..."));
		action->setShortcuts(QKeySequence::Save);
		action->setIcon(QIcon::fromTheme("document-save-as"));
		connect(action, &QAction::triggered, this, &MainWindow::saveTableToFile );
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
				tr("TaToGE: Table-Top Game Equipement.") + "\n\n" +
				tr("This application provides a bunch of simple widgets to emulate the common game equipment needed when playing board games or card games.") + "\n\n" +
				tr("Click on 'Add a widget' to set equipement on table before using them.")); }  );
	}
	{
		QAction * action = helpMenu->addAction(tr("&About"));
		action->setIcon(QIcon::fromTheme("help-about"));
		connect(action, &QAction::triggered, [this](){ QMessageBox::information(this, "",
				"<h2>" + QCoreApplication::applicationName() + " <font size=-1>" + tr("Version: ") + QCoreApplication::applicationVersion() + "</font></h2>" +
				"<p>" + tr("Copyright &copy;2019 Alfred Massard.") + "<br>" +
				tr("This is free software, and you are welcome to redistribute it under GNU General Public License v3.0 conditions.")+ "</p>" +
				"<p>&nbsp;</p>" +
				"<p><a href='https://github.com/quasart-/TaToGE/issues'>" + tr("Report a bug") + "</a>" +
				" (" + tr("Build: ") + QCoreApplication::applicationVersion() + " " +  __DATE__ + " " +__TIME__ + ")"
				); }  );
	}

	QStringList args = QCoreApplication::arguments();
	if (args.count() > 1)
	{
		m_Table.loadJsonFile( args.at(1) );
	}
}

void MainWindow::saveTableToFile()
{
	QString filename = QFileDialog::getSaveFileName(
			this,
			tr("Save file"),
			"table.json",
			tr("JSON templates (*.json)"));
	if (filename.isEmpty())
	{
		return;
	}

	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		return;
	}

	QTextStream out(&file);
	QJsonDocument const json_doc = m_Table.toJson();
	out << json_doc.toJson(QJsonDocument::Indented);
}

void MainWindow::openTableFromFile()
{
	QString const filename = QFileDialog::getOpenFileName(
			this,
			tr("Select file"),
			"./examples",
			tr("JSON templates (*.json)"));
	if (!filename.isEmpty())
	{
		m_Table.clearTable();
		m_Table.loadJsonFile(filename);
	}
}


