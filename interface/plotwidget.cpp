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

#include "plotwidget.hpp"
#include "ui_plotwidget.h"
#include <QDebug>
PlotWidget::PlotWidget(QWidget* Parent)
: QWidget(Parent), ui(new Ui::PlotWidget), Dialog(new PlotDialog(this))
{
	ui->setupUi(this);

	for (auto Axis: ui->Plot->axisRect()->axes(QCPAxis::atLeft)) ui->Plot->axisRect()->removeAxis(Axis);

	for (const auto& Data: AppCore::getInstance()->GetAxes()) AddAxis(Data);
	for (const auto& Data: AppCore::getInstance()->GetPlots()) AddPlot(Data);

	ui->Plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

	ui->Plot->legend->setVisible(true);

	connect(Dialog, &PlotDialog::onAxisAdd, this, &PlotWidget::AddAxis);
	connect(Dialog, &PlotDialog::onAxisChange, this, &PlotWidget::UpdateAxis);
	connect(Dialog, &PlotDialog::onAxisDelete, this, &PlotWidget::DeleteAxis);

	connect(Dialog, &PlotDialog::onPlotAdd, this, &PlotWidget::AddPlot);
	connect(Dialog, &PlotDialog::onPlotChange, this, &PlotWidget::UpdatePlot);
	connect(Dialog, &PlotDialog::onPlotDelete, this, &PlotWidget::DeletePlot);
}

PlotWidget::~PlotWidget(void)
{
	delete ui;
}

void PlotWidget::AddPlot(const PlotData& Data)
{
	QCPGraph* Plot = ui->Plot->addGraph(nullptr, Axes[Data.AXIS_ID]);

	// TODO colors

	Plot->setName(Data.Varname);
	Plot->setPen(Axes[Data.AXIS_ID]->basePen());
	Plot->setVisible(Data.Active);

	Plots.insert(Data.ID, Plot);
	Vars.insert(Data.Varlabel, Plot);

	ui->Plot->replot();
}

void PlotWidget::UpdatePlot(const PlotData& Data)
{
	QCPGraph* Plot = Plots[Data.ID];

	// TODO colors

	Plot->setName(Data.Varname);
	Plot->setPen(Axes[Data.AXIS_ID]->basePen());
	Plot->setVisible(Data.Active);

	ui->Plot->replot();
}

void PlotWidget::DeletePlot(int ID)
{
	ui->Plot->removeGraph(Plots.take(ID));
}

void PlotWidget::AddAxis(const AxisData& Data)
{
	QCPAxis* Axis = ui->Plot->axisRect()->addAxis(QCPAxis::atLeft);

	Axis->setLabel(Data.Label ? Data.Name : QString());
	Axis->setBasePen(Qt::PenStyle(Data.Style + 1)); qDebug() << "Style:" << Data.Style;
	Axis->setRange(Data.Min, Data.Max);
	Axis->setVisible(Data.Active);

	Axes.insert(Data.ID, Axis);

	ui->Plot->replot();
}

void PlotWidget::UpdateAxis(const AxisData& Data)
{
	QCPAxis* Axis = Axes[Data.ID];

	Axis->setLabel(Data.Label ? Data.Name : QString());
	Axis->setBasePen(QPen(Data.Style + 1));
	Axis->setRange(Data.Min, Data.Max);
	Axis->setVisible(Data.Active);

	ui->Plot->replot();
}

void PlotWidget::DeleteAxis(int ID)
{
	ui->Plot->axisRect()->removeAxis(Axes.take(ID));
}

void PlotWidget::SettingsButtonClicked(void)
{
	Dialog->open();
}

void PlotWidget::CleanButtonClicked(void)
{

}

void PlotWidget::SaveButtonClicked(void)
{

}
