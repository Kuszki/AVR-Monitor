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

#ifndef DEVICEWIDGET_HPP
#define DEVICEWIDGET_HPP

#include <QPushButton>
#include <QWidget>

#include "devicedialog.hpp"
#include "deviceentry.hpp"
#include "appcore.hpp"
#include "common.hpp"

namespace Ui
{
	class DeviceWidget;
}

class DeviceWidget : public QWidget
{

		Q_OBJECT

	private:

		Ui::DeviceWidget* ui;

		DeviceDialog* Dialog;

	public:

		explicit DeviceWidget(QWidget* Parent = nullptr);
		virtual ~DeviceWidget(void) override;

	private slots:

		void AddDevice(const DeviceData& Data);

};

#endif // DEVICEWIDGET_HPP
