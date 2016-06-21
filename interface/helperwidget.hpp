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

#ifndef HELPERWIDGET_HPP
#define HELPERWIDGET_HPP

#include <QSpacerItem>
#include <QStringList>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QSettings>
#include <QWidget>
#include <QList>

#include "helperentry.hpp"

#include "../controller/codes.hpp"

class HelperWidget : public QWidget
{

		Q_OBJECT

	public:

		explicit HelperWidget(QWidget* Parent = nullptr);
		virtual ~HelperWidget(void) override;

	signals:

		void onCodePasteRequest(const QString&);

};

#endif // HELPERWIDGET_HPP
