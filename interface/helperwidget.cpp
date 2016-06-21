/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Script editor helper widget for AVR-Monitor                            *
 *  Copyright (C) 2015  Łukasz "Kuszki" Dróżdż  l.drozdz@openmailbox.org   *
 *                                                                         *
 *  This program is free software: you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the  Free Software Foundation, either  version 3 of the  License, or   *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This  program  is  distributed  in the hope  that it will be useful,   *
 *  but WITHOUT ANY  WARRANTY;  without  even  the  implied  warranty of   *
 *  MERCHANTABILITY  or  FITNESS  FOR  A  PARTICULAR  PURPOSE.  See  the   *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have  received a copy  of the  GNU General Public License   *
 *  along with this program. If not, see http://www.gnu.org/licenses/.     *
 *                                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "helperwidget.hpp"

HelperWidget::HelperWidget(QWidget* Parent)
: QWidget(Parent)
{
	static const QStringList valCommon =
	{
		tr("Declare variable"),
		tr("Export variable"),
		tr("Set variable"),
		tr("If statement"),
		tr("If + else statement"),
		tr("While statement")
	};

	static const QStringList dscCommon =
	{
		tr("var name;"),
		tr("export name;"),
		tr("set name value;"),
		tr("if test;\n\t# statements if true\nfi;"),
		tr("if test;\n\t# statements if true\nelse;\n\t# statements if false\nfi;"),
		tr("while test;\n\t# loop\ndone;")
	};

	static const QStringList valSystem =
	{
		tr("Put shift data"),
		tr("Enable pin"),
		tr("Disable pin"),
		tr("Set output state")
	};

	static const QStringList dscSystem =
	{
		tr("call put data;"),
		tr("call put pin, 1;"),
		tr("call put pin, 0;"),
		tr("call out status;")
	};

	static const QStringList valDevice =
	{
		tr("Pull sensors data"),
		tr("Pull device status"),
		tr("Set PGA gain"),
		tr("Set sleep time"),
		tr("Set default output"),
		tr("Clear RAM"),
		tr("Set master status"),
		tr("Download script")
	};

	static const QStringList dscDevice =
	{
		tr("call get;"),
		tr("call sys;"),
		tr("call pga id, gain;"),
		tr("call dev %1, time; # sleep = time * 100 ms").arg(DEV_SLEEP),
		tr("call dev %1, value;").arg(DEV_DEFAULT),
		tr("call dev %1, %2;").arg(DEV_SPEC).arg(CLEAN_RAM),
		tr("call dev %1, status;").arg(DEV_MASTER),
		tr("call dev %1, 1;").arg(DEV_SCRIPT)
	};

	static const QStringList catList =
	{
		tr("Common actions"), tr("System actions"), tr("Device actions")
	};

	static const QList<QStringList> valMap = { valCommon, valSystem, valDevice  };
	static const QList<QStringList> dscMap = { dscCommon, dscSystem, dscDevice  };

	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	QTabWidget* toolBox = new QTabWidget(this);

	mainLayout->addWidget(toolBox);
	mainLayout->setMargin(0);

	toolBox->setTabPosition(QTabWidget::East);

	setLayout(mainLayout);

	for (int i = 0; i < catList.size(); i++)
	{
		QWidget* Tools = new QWidget(this);
		QVBoxLayout* Layout = new QVBoxLayout(Tools);

		toolBox->addTab(Tools, catList.at(i));

		for (int j = 0; j < valMap.at(i).size(); j++)
		{
			HelperEntry* Entry = new HelperEntry(valMap.at(i).at(j), dscMap.at(i).at(j), this);

			Layout->addWidget(Entry);

			connect(Entry, &HelperEntry::onScriptPaste, [this] (const QString& Code) -> void { emit onCodePasteRequest(Code); });
		}

		Layout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));
	}
}

HelperWidget::~HelperWidget(void) {}
