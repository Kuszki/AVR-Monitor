/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Main AppCore module for AVR-Monitor                                    *
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

#ifndef APPCORE_HPP
#define APPCORE_HPP

#define EmitError { LastError = Query.lastError().text(); return false; }
#define EmitNotFound { LastError = tr("Item with selected ID doesn't exists"); return false; }

#define VREADONLY KLVariables::NUMBER, KLVariables::KLSCALLBACK(), false

#define DBNAME "database.sqlite"

#define VARDUMP(V) \
	for (const auto* Space = &V; Space; Space = Space->Parent) \
	for (const auto& Var : *Space) qDebug() << Var.Index << "=" << Var.Value.ToNumber();

#include <QRegExpValidator>
#include <QSqlDatabase>
#include <QStringList>
#include <QSqlQuery>
#include <QSqlError>
#include <QSettings>
#include <QVariant>
#include <QObject>
#include <QRegExp>
#include <QThread>
#include <QMutex>
#include <QTimer>
#include <QList>

#include <avrbridge.hpp>

#include "common.hpp"
#include "scriptworker.hpp"

class AppCore final : public QObject
{

		Q_OBJECT

	private:

		static AppCore* THIS;
		static AVRBridge* Device;
		static QRegExpValidator* Validator;

		static QString LastError;

		QSqlDatabase Database;

		QMap<KLString, QVector<double>> History;
		QVector<double> Weights;

		QMap<int, SensorData> Sensors;
		QMap<int, EventData> Events;
		QMap<int, DeviceData> Devices;
		QMap<int, AxisData> Axes;
		QMap<int, PlotData> Plots;
		QMap<int, SliderData> Sliders;

		KLVariables SlidersVar;
		KLVariables SensorsVar;
		KLVariables AdcVar;

		KLScriptbinding Script;

		QStringList Tasks;

		unsigned Weight = 0;
		unsigned Samples = 1;
		bool Done = true;

		QMutex Locker;
		QThread Thread;
		ScriptWorker Worker;

		QTimer Watchdog;
		QTimer Interval;

	private:

		bool SensorScriptOk(const QString& Code, const QString& Label = QString());
		bool EventScriptOk(const QString& Code);

	public:

		AppCore(void);
		~AppCore(void);

		QString GetScript(void);

		bool SaveDatabase(const QString& Path);
		bool LoadDatabase(const QString& Path);

		bool AddSensor(SensorData& Data);
		bool UpdateSensor(SensorData& Data);
		bool DeleteSensor(int ID);
		bool DisableSensor(int ID);

		const SensorData& GetSensor(int ID);
		const QMap<int, SensorData>& GetSensors(void);

		bool AddEvent(EventData& Data);
		bool UpdateEvent(EventData& Data);
		bool DeleteEvent(int ID);
		bool DisableEvent(int ID);

		const EventData& GetEvent(int ID);
		const QMap<int, EventData>& GetEvents(void);

		bool AddDevice(DeviceData& Data);
		bool UpdateDevice(DeviceData& Data);
		bool DeleteDevice(int ID);
		bool DisableDevice(int ID);

		const DeviceData& GetDevice(int ID);
		const QMap<int, DeviceData>& GetDevices(void);

		bool AddAxis(AxisData& Data);
		bool UpdateAxis(AxisData& Data);
		bool DeleteAxis(int ID);
		bool DisableAxis(int ID);

		const AxisData& GetAxis(int ID);
		const QMap<int, AxisData>& GetAxes(void);

		bool AddPlot(PlotData& Data);
		bool UpdatePlot(PlotData& Data);
		bool DeletePlot(int ID);
		bool DisablePlot(int ID);

		const PlotData& GetPlot(int ID);
		const QMap<int, PlotData>& GetPlots(void);

		bool AddSlider(SliderData& Data);
		bool UpdateSlider(SliderData& Data);
		bool DeleteSlider(int ID);
		bool DisableSlider(int ID);

		SliderData GetSlider(int ID);
		QMap<int, SliderData> GetSliders(void);

		void ConnectVariable(const QString& Var, const boost::function<void (double)>& Callback);
		void DisconnectVariable(const QString& Var);

		static QString getValidation(const QString& Code);

		static QRegExpValidator* getValidator(void);
		static AppCore* getInstance(void);
		static AVRBridge* getDevice(void);
		static QString getError(void);

	private slots:

		void UpdateVariables(const KLVariables& Vars);

		void PerformTasks(const KLVariables& Vars);

		void UpdateInvalidItems(void);

		void UpdateScriptTasks(void);

		void CompleteEvaluations(void);

		void TerminateEvaluations(void);

	public slots:

		void UpdateAverage(int Count);

		void UpdateWeight(int Type);

		void UpdateStatus(bool Active);

		void UpdateInterval(double Time);

		void UpdateVariable(int ID, double Value);

		void SynchronizeDevice(void);

	signals:

		void onValuesUpdate(const KLVariables&);

		void onScriptUpdate(void);
		void onSensorUpdate(int);
		void onEventUpdate(int);
		void onDeviceUpdate(int);
		void onAxisUpdate(int);
		void onPlotUpdate(int);
		void onSliderUpdate(int);

		void onEmergencyStop(void);

		void onEvaluationRequest(void);
		void onScriptTermination(void);

};

#endif // APPCORE_HPP
