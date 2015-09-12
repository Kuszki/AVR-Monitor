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

#ifndef COMMON_HPP
#define COMMON_HPP

#include <QString>

struct SensorData
{
	int ID = -1;

	QString Name;
	QString Label;
	QString Unit;
	QString Script;

	bool Active = false;
};

struct EventData
{
	int ID = -1;

	QString Name;
	QString Script;

	bool Active;
};

struct DeviceData
{
	int ID = -1;

	QString Name;

	int Output;

	bool Active;
};

struct AxisData
{
	int ID = -1;

	QString Name;

	int Style;

	double Min;
	double Max;

	bool Label;
	bool Active;
};

struct PlotData
{
	int ID = -1;

	int AXIS_ID;
	int SENSOR_ID;

	bool Active;

	QString Varname;
	QString Varlabel;
	QString Axisname;
};

#endif // COMMON_HPP
