/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Device entry widget for AVR-Monitor                                    *
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

#ifndef DEVICEENTRY_HPP
#define DEVICEENTRY_HPP

#include <QWidget>

#include "devicedialog.hpp"
#include "common.hpp"

namespace Ui
{
	class DeviceEntry;
}

class DeviceEntry : public QWidget
{

		Q_OBJECT

	private:

		Ui::DeviceEntry* ui;

		QMetaObject::Connection Connection;

		DeviceDialog* Dialog;

		const int ID;

	public:

		explicit DeviceEntry(const DeviceData& Data, QWidget* Parent = nullptr);
		virtual ~DeviceEntry(void) override;

	private slots:

		void SettingsButtonClicked(void);

		void DeleteButtonClicked(void);

		void UpdateDevice(const DeviceData& Data);

		void UpdateRequest(int Index);

	signals:

		void onDeviceUpdate(const DeviceData&);

};

#endif // DEVICEENTRY_HPP
