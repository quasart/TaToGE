#pragma once

#include <QWidget>

#include "AddWidgetDialog.h"


class Table : public QWidget
{
Q_OBJECT

public:
	Table(QWidget * parent = nullptr);

private:
	QGridLayout & m_Layout;
	QPushButton & m_AddButton;
	AddWidgetDialog m_AddDialog;
	int m_RowCount;

public:

	void addRow(QWidget * w);
	void deleteRow(int row);

	void showAddDialog();
	void showLoadDialog();
	void clearTable();

	std::vector<int> asIntVector(QJsonValue array, std::vector<int> default_out);
	std::vector<QString> asStringVector(QJsonValue array);

	void loadJsonFile(QString filename);
	void loadJson(QJsonDocument json_doc);
	QWidget * createWidget(QJsonObject const & item);

protected:
	void resizeEvent(QResizeEvent* event) override;

};
