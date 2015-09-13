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

#ifndef PLOTWIDGET_HPP
#define PLOTWIDGET_HPP

#include <QWidget>
#include <QTime>
#include <QMap>

#include <KLLibs.hpp>
#include <qcustomplot.hpp>

#include "plotdialog.hpp"

namespace Ui
{
	class PlotWidget;
}

class PlotWidget : public QWidget
{

		Q_OBJECT

	private:

		static const QList<Qt::GlobalColor> Colors;

		Ui::PlotWidget* ui;

		PlotDialog* Dialog;

		QMap<QString, QCPGraph*> Vars;
		QMap<int, QCPGraph*> Plots;
		QMap<int, QCPAxis*> Axes;

		QTime Starttime;

		bool Userrange = true;

	public:

		explicit PlotWidget(QWidget* Parent = nullptr);
		virtual ~PlotWidget(void) override;

	private slots:

		void PlotRangeChanged(const QCPRange& New, const QCPRange& Old);

		void AddPlot(const PlotData& Data);
		void UpdatePlot(const PlotData& Data);
		void DeletePlot(int ID);

		void AddAxis(const AxisData& Data);
		void UpdateAxis(const AxisData& Data);
		void DeleteAxis(int ID);

		void SettingsButtonClicked(void);
		void CleanButtonClicked(void);
		void SaveButtonClicked(void);

		void RangeSpinChanged(void);

	public slots:

		void PlotVariables(const KLVariables& Variables);

		void UpdateSensors(void);

		void RestartPlot(void);

};

#endif // PLOTWIDGET_HPP
