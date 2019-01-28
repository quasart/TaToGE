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
	float m_FontSize;

public:

	void addRow(QWidget * w);
	void deleteRow(int row);

	void showAddDialog();
	void showLoadDialog();
	void clearTable();

	std::vector<int> asIntVector(QJsonValue array, std::vector<int> default_out);
	std::vector<QString> asStringVector(QJsonValue array);

	bool loadJsonFile(QString filename);
	bool loadJson(QJsonDocument json_doc);
	QWidget * createWidget(QJsonObject const & item);

	void updateFontSize();

protected:
	void wheelEvent(QWheelEvent *event) override;
	void resizeEvent(QResizeEvent* event) override;
	QSize sizeHint() const override;

};
