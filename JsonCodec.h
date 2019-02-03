#pragma once

#include <vector>

#include <QWidget>
#include <QJsonObject>

class JsonCodec
{
public:
	static std::vector<int> asIntVector(QJsonValue array, std::vector<int> default_out);
	static std::vector<QString> asStringVector(QJsonValue array);

	static QJsonObject widgetJson(QWidget const & widget);
	static QWidget * createWidget(QJsonObject const & item);
};

