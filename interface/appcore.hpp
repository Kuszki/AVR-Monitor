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

#ifndef APPCORE_HPP
#define APPCORE_HPP

#include <QSqlDatabase>
#include <QStringList>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QObject>
#include <QRegExp>
#include <QTimer>
#include <QList>

#include <KLLibs.hpp>
#include <avrbridge.hpp>

#include "common.hpp"

class AppCore final: public QObject
{

		Q_OBJECT

	private:

		static AppCore* THIS;
		static AVRBridge* Device;

		static QString LastError;

		QSqlDatabase Database;

		KLScriptbinding Script;
		KLVariables Adc;

		QStringList Tasks;
		QString Initscript;

		unsigned char Values;

		QTimer Interval;

		void UpdateScriptTasks(void);
		void UpdateDefaultOutputs(void);

		bool SensorScriptOk(const QString& Code);
		bool EventScriptOk(const QString& Code);

	public:

		AppCore(void);
		~AppCore(void);

		QString GetScript(void);

		bool AddSensor(SensorData& Data);
		bool UpdateSensor(SensorData& Data);
		bool DeleteSensor(int ID);

		SensorData GetSensor(int ID);
		QList<SensorData> GetSensors(void);

		bool AddEvent(EventData& Data);
		bool UpdateEvent(EventData& Data);
		bool DeleteEvent(int ID);

		EventData GetEvent(int ID);
		QList<EventData> GetEvents(void);

		bool AddDevice(DeviceData& Data);
		bool UpdateDevice(DeviceData& Data);
		bool DeleteDevice(int ID);

		DeviceData GetDevice(int ID);
		QList<DeviceData> GetDevices(void);

		bool AddAxis(AxisData& Data);
		bool UpdateAxis(AxisData& Data);
		bool DeleteAxis(int ID);

		AxisData GetAxis(int ID);
		QList<AxisData> GetAxes(void);

		bool AddPlot(PlotData& Data);
		bool UpdatePlot(PlotData& Data);
		bool DeletePlot(int ID);

		PlotData GetPlot(int ID);
		QList<PlotData> GetPlots(void);

		void ConnectVariable(const QString& Var, const boost::function<void (double)>& Callback);
		void DisconnectVariable(const QString& Var);

		static AppCore* getInstance(void);
		static AVRBridge* getDevice(void);
		static QString getError(void);

	public slots:

		void UpdateVariables(const KLVariables& Vars);

		void PerformTasks(const KLVariables& Vars);

		void UpdateStatus(bool Active);

		void UpdateInterval(double Time);

		void SynchronizeDevice(void);

	signals:

		void onScriptUpdate(void);

};

#endif // APPCORE_HPP
