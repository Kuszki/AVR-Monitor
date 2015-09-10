/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  {description}                                                          *
 *  Copyright (C) 2015  Łukasz "Kuszki" Dróżdż            l.drozdz@o2.pl   *
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

#ifndef EVENTWIDGET_HPP
#define EVENTWIDGET_HPP

#include <QPushButton>
#include <QWidget>

#include "eventdialog.hpp"
#include "evententry.hpp"
#include "appcore.hpp"
#include "common.hpp"

namespace Ui
{
	class EventWidget;
}

class EventWidget : public QWidget
{
		Q_OBJECT

	private:

		Ui::EventWidget* ui;

		EventDialog* Dialog;

	public:

		explicit EventWidget(QWidget* Parent = nullptr);
		virtual ~EventWidget(void) override;

	private slots:

		void AddEvent(const EventData& Data);

};

#endif // EVENTWIDGET_HPP