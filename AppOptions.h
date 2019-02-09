#pragma once

#include <QCommandLineParser>
#include <QCoreApplication>
#include <QColor>

class AppOptions : public QObject, public QCommandLineParser
{
Q_OBJECT

public:
	static AppOptions & getInstance()
	{
		static AppOptions options;
		return options;
	}

	QString getLocale() const
	{
		return value(m_LocaleOption);
	}

	QString getFileToLoad() const
	{
		if (positionalArguments().isEmpty())
		{
			return "";
		}
		return positionalArguments().at(0);
	}

	QColor getTableColor() const
	{
		if (isSet(m_ColorOption))
		{
			return { value(m_ColorOption) };
		}
		else
		{
			return { "#0af" };
		}
	}

	bool isAdminHidden() const
	{
		return !positionalArguments().isEmpty() &&
			isSet(m_HideAdminOption);
	}

	bool isFullscreen() const
	{
		return isSet(m_FullScreenOption);
	}

private:
	AppOptions()
		: m_ColorOption({"c","color"}, tr("To specify table color."), tr("color"))
		, m_FullScreenOption({"f","fullscreen"}, tr("Full-screen mode."))
		, m_HideAdminOption({"H","hide_admin"}, tr("To hide table administration, only when loading table from file."))
		, m_LocaleOption({"l", "locale"}, tr("To specify locale to use."), tr("locale"))
	{
		setApplicationDescription("TaToGE - Table-Top Game Equipment");

		addOption(m_ColorOption);
		addOption(m_FullScreenOption);
		addOption(m_HideAdminOption);
		addOption(m_LocaleOption);
		addHelpOption();
		addVersionOption();

		addPositionalArgument(tr("file"), tr("Table setup file to load."), "["+tr("file")+"]");
	}

	QCommandLineOption m_ColorOption;
	QCommandLineOption m_FullScreenOption;
	QCommandLineOption m_HideAdminOption;
	QCommandLineOption m_LocaleOption;
};

