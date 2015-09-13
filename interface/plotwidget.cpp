/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Ploting widget for AVR-Monitor                                         *
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

const QList<Qt::GlobalColor> PlotWidget::Colors =
{
	Qt::red, Qt::blue, Qt::green,
	Qt::yellow, Qt::magenta, Qt::cyan,
	Qt::darkRed, Qt::darkBlue, Qt::darkGreen,
	Qt::darkYellow, Qt::darkMagenta, Qt::darkCyan
};

PlotWidget::PlotWidget(QWidget* Parent)
: QWidget(Parent), ui(new Ui::PlotWidget), Dialog(new PlotDialog(this))
{
	ui->setupUi(this);

	for (auto Axis: ui->Plot->axisRect()->axes(QCPAxis::atLeft)) ui->Plot->axisRect()->removeAxis(Axis);

	for (const auto& Data: AppCore::getInstance()->GetAxes()) AddAxis(Data);
	for (const auto& Data: AppCore::getInstance()->GetPlots()) AddPlot(Data);

	ui->Plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
	ui->Plot->legend->setVisible(true);
	ui->Plot->xAxis->setRange(0, 60);

	connect(Dialog, &PlotDialog::onAxisAdd, this, &PlotWidget::AddAxis);
	connect(Dialog, &PlotDialog::onAxisChange, this, &PlotWidget::UpdateAxis);
	connect(Dialog, &PlotDialog::onAxisDelete, this, &PlotWidget::DeleteAxis);

	connect(Dialog, &PlotDialog::onPlotAdd, this, &PlotWidget::AddPlot);
	connect(Dialog, &PlotDialog::onPlotChange, this, &PlotWidget::UpdatePlot);
	connect(Dialog, &PlotDialog::onPlotDelete, this, &PlotWidget::DeletePlot);

	connect(ui->Plot->xAxis, SIGNAL(rangeChanged(const QCPRange&, const QCPRange)), SLOT(PlotRangeChanged(const QCPRange&, const QCPRange&)));
}

PlotWidget::~PlotWidget(void)
{
	delete ui;
}

void PlotWidget::PlotRangeChanged(const QCPRange& New, const QCPRange& Old)
{
	if (New.lower < 0) ui->Plot->xAxis->setRange(0, Old.size());

	ui->Start->blockSignals(true);
	ui->Stop->blockSignals(true);

	if (!ui->Start->hasFocus()) ui->Start->setValue(ui->Plot->xAxis->range().lower);
	if (!ui->Stop->hasFocus()) ui->Stop->setValue(ui->Plot->xAxis->range().upper);

	ui->Start->blockSignals(false);
	ui->Stop->blockSignals(false);

	if (Userrange) ui->Follow->setChecked(false);

	Userrange = true;
}

void PlotWidget::AddPlot(const PlotData& Data)
{
	QCPGraph* Plot = ui->Plot->addGraph(nullptr, Axes[Data.AXIS_ID]);

	QPen Pen = Axes[Data.AXIS_ID]->basePen();

	Pen.setColor(Colors.value(Plots.size(), Qt::black));

	Plot->setName(Data.Varname);
	Plot->setPen(Pen);
	Plot->setVisible(Data.Active);

	Plots.insert(Data.ID, Plot);
	Vars.insert(Data.Varlabel, Plot);

	ui->Plot->replot();
}

void PlotWidget::UpdatePlot(const PlotData& Data)
{
	QCPGraph* Plot = Plots[Data.ID];

	Plot->setName(Data.Varname);
	Plot->setVisible(Data.Active);

	Vars.remove(Vars.key(Plot));
	Vars.insert(Data.Varlabel, Plot);

	ui->Plot->replot();
}

void PlotWidget::DeletePlot(int ID)
{
	Vars.remove(Vars.key(Plots[ID]));

	ui->Plot->removeGraph(Plots.take(ID));

	int i = 0; for (auto Plot: Plots)
	{
		QPen Pen = Plot->pen();

		Pen.setColor(Colors.value(i++, Qt::black));

		Plot->setPen(Pen);
	}

	ui->Plot->replot();
}

void PlotWidget::AddAxis(const AxisData& Data)
{
	QCPAxis* Axis = ui->Plot->axisRect()->addAxis(QCPAxis::atLeft);

	Axis->setLabel(Data.Label ? Data.Name : QString());
	Axis->setBasePen(Qt::PenStyle(Data.Style + 1));
	Axis->setRange(Data.Min, Data.Max);
	Axis->setAutoTicks(true);
	Axis->setVisible(Data.Active);
	Axis->grid()->setVisible(true);

	Axes.insert(Data.ID, Axis);

	ui->Plot->replot();
}

void PlotWidget::UpdateAxis(const AxisData& Data)
{
	QCPAxis* Axis = Axes[Data.ID];

	Axis->setLabel(Data.Label ? Data.Name : QString());
	Axis->setBasePen(QPen(Qt::PenStyle(Data.Style + 1)));
	Axis->setRange(Data.Min, Data.Max);
	Axis->setVisible(Data.Active);

	for (auto Plot: Plots) if (Plot->valueAxis() == Axis)
	{
		QPen Pen = Plot->pen();

		Pen.setStyle(Qt::PenStyle(Data.Style + 1));

		Plot->setPen(Pen);
	}

	ui->Plot->replot();
}

void PlotWidget::DeleteAxis(int ID)
{
	for (auto Plot: Axes[ID]->graphs()) ui->Plot->removeGraph(Plot);

	int i = 0; for (auto Plot: Plots)
	{
		QPen Pen = Plot->pen();

		Pen.setColor(Colors.value(i++, Qt::black));

		Plot->setPen(Pen);
	}

	ui->Plot->axisRect()->removeAxis(Axes.take(ID));

	ui->Plot->replot();
}

void PlotWidget::SettingsButtonClicked(void)
{
	Dialog->open();
}

void PlotWidget::CleanButtonClicked(void)
{
	RestartPlot();
}

void PlotWidget::SaveButtonClicked(void)
{
	const QString Path = QFileDialog::getSaveFileName(this, tr("Select file to save data"), QString(), tr("CSV files (*.csv)"));

	if (!Path.isEmpty())
	{
		const QChar groupSeparator = ',';

		QFile File(Path); File.open(QFile::WriteOnly);
		QString Buff(tr("Time")); QList<double> Keys;

		for (const auto& Var: Vars.keys()) if (Vars[Var]->visible()) Buff.append(groupSeparator).append(Var);
		for (const auto& Plot: Plots) if (Plot->visible()) Keys.append(Plot->data()->keys());

		Buff.append('\n');

		auto Set = Keys.toSet().values(); qSort(Set);

		for (const auto& Key: Set)
		{
			Buff.append(QString::number(Key));
			for (const auto& Plot: Vars) if (Plot->visible()) Buff.append(groupSeparator).append(QString::number(Plot->data()->value(Key).value));
			Buff.append('\n');
		}

		File.write(Buff.toUtf8());
	}
}

void PlotWidget::RangeSpinChanged(void)
{
	if (ui->Start->value() > ui->Stop->value())
	{
		ui->Start->setValue(ui->Plot->xAxis->range().lower);
		ui->Stop->setValue(ui->Plot->xAxis->range().upper);
	}
	else
	{
		ui->Plot->xAxis->setRange(ui->Start->value(), ui->Stop->value());
	}

	ui->Plot->replot();
}

void PlotWidget::PlotVariables(const KLVariables& Variables)
{
	const double Time = Starttime.msecsTo(QTime::currentTime()) / 1000.0;

	for (const auto& Var: Variables)
	{
		const QString ID = (const char*) Var.ID;

		if (Vars.contains(ID)) Vars[ID]->addData(Time, Var.Value.ToNumber());
	}

	if (ui->Follow->isChecked() && ui->Plot->xAxis->range().upper < Time)
	{
		Userrange = false; ui->Plot->xAxis->setRange(Time, ui->Plot->xAxis->range().size(), Qt::AlignRight);
	}

	ui->Plot->replot();
}

void PlotWidget::UpdateSensors(void)
{
	const auto Data = AppCore::getInstance()->GetPlots();

	for (auto ID: Plots.keys()) if (!Data.contains(ID)) DeletePlot(ID);

	for (const auto& Plot: Data) UpdatePlot(Plot);
}

void PlotWidget::RestartPlot(void)
{
	Userrange = false; ui->Plot->xAxis->setRange(0, ui->Plot->xAxis->range().size(), Qt::AlignLeft);

	for (auto Plot: Plots)
	{
		Plot->clearData();
	}

	ui->Plot->replot();

	Starttime = QTime::currentTime();
}
