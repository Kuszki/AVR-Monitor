/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Ploting options dialog for AVR-Monitor                                 *
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

#ifndef PLOTDIALOG_HPP
#define PLOTDIALOG_HPP

#include <QListWidgetItem>
#include <QDialog>

#include "axisdialog.hpp"

namespace Ui
{
	class PlotDialog;
}

class PlotDialog : public QDialog
{

		Q_OBJECT

	private:

		Ui::PlotDialog* ui;

	public:

		explicit PlotDialog(QWidget* Parent = nullptr);
		virtual ~PlotDialog(void) override;

	private slots:

		void AddAxis(const AxisData& Axis);
		void UpdateAxis(const AxisData& Axis);

		void UpdatePlot(QListWidgetItem* Plot);

		void UpdateList(void);

		void AddButtonClicked(void);
		void DeleteButtonClicked(void);
		void SettingsButtonClicked(void);

		void InsertButtonClicked(void);
		void RemoveButtonClicked(void);

	public slots:

		virtual void open(void) override;

	signals:

		void onPlotAdd(const PlotData&);
		void onPlotChange(const PlotData&);
		void onPlotDelete(int);

		void onAxisAdd(const AxisData&);
		void onAxisChange(const AxisData&);
		void onAxisDelete(int);

};

#endif // PLOTDIALOG_HPP
