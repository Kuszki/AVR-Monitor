/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Sensor widget for AVR-Monitor                                          *
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

#ifndef SENSORWIDGET_HPP
#define SENSORWIDGET_HPP

#include <QPushButton>
#include <QScrollArea>
#include <QScrollBar>
#include <QWidget>

#include "sensordialog.hpp"
#include "sensorentry.hpp"
#include "appcore.hpp"
#include "common.hpp"

namespace Ui
{
	class SensorWidget;
}

class SensorWidget : public QWidget
{

		Q_OBJECT

	private:

		Ui::SensorWidget* ui;

		SensorDialog* Dialog;

	public:

		explicit SensorWidget(QWidget* Parent = nullptr);
		virtual ~SensorWidget(void) override;

	public slots:

		void RefreshSize(void);

	private slots:

		void AddSensor(const SensorData& Data);

};

#endif // SENSORWIDGET_HPP
