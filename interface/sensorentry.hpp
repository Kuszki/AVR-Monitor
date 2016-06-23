/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Sensor entry widget for AVR-Monitor                                    *
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

#ifndef SENSORENTRY_HPP
#define SENSORENTRY_HPP

#include <QWidget>

#include "sensordialog.hpp"
#include "common.hpp"

namespace Ui
{
	class SensorEntry;
}

class SensorEntry : public QWidget
{

		Q_OBJECT

	private:

		Ui::SensorEntry* ui;

		SensorDialog* Dialog;

		const int ID;

	public:

		explicit SensorEntry(const SensorData& Data, QWidget* Parent = nullptr);
		virtual ~SensorEntry(void) override;

	private slots:

		void SettingsButtonClicked(void);

		void DeleteButtonClicked(void);

		void UpdateSensor(const SensorData& Data);

	signals:

		void onSensorUpdate(const SensorData&);


};

#endif // SENSORENTRY_HPP
