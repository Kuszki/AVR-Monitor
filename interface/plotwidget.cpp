/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Ploting widget for AVR-Monitor                                         *
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

#include "plotwidget.hpp"
#include "ui_plotwidget.h"

const QList<Qt::GlobalColor> PlotWidget::Colors =
{
	Qt::red, Qt::blue, Qt::green,
	Qt::darkRed, Qt::darkBlue, Qt::darkGreen,
	Qt::darkYellow, Qt::darkMagenta, Qt::darkCyan,
	Qt::yellow, Qt::magenta, Qt::cyan
};

PlotWidget::PlotWidget(QWidget* Parent)
: QWidget(Parent), ui(new Ui::PlotWidget)
{
	ui->setupUi(this);

	Dialog = new PlotDialog(this);

	for (auto Axis: ui->Plot->axisRect()->axes(QCPAxis::atLeft)) ui->Plot->axisRect()->removeAxis(Axis);

	for (const auto& Data: AppCore::getInstance()->GetAxes()) AddAxis(Data);
	for (const auto& Data: AppCore::getInstance()->GetPlots()) AddPlot(Data);

	ui->leftSpacer->changeSize(ui->saveButton->sizeHint().width(), 0);
	ui->rightSpacer->changeSize(ui->settingsButton->sizeHint().width(), 0);
	ui->checkSpacer->changeSize(ui->cleanButton->sizeHint().width(), 0);
	ui->Average->setValue(QSettings("AVR-Monitor").value("plotavg", 1).toInt());
	ui->Plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
	ui->Plot->legend->setVisible(true);
	ui->Plot->xAxis->setRange(0, 60);

	Samples = ui->Average->value();

	connect(ui->Plot->xAxis, SIGNAL(rangeChanged(const QCPRange&, const QCPRange&)), SLOT(PlotRangeChanged(const QCPRange&, const QCPRange&)));

	connect(AppCore::getInstance(), &AppCore::onAxisUpdate, [this] (int Index) -> void
	{
		if (Axes.contains(Index))
		{
			if (AppCore::getInstance()->GetAxis(Index).ID == -1) DeleteAxis(Index);
			else UpdateAxis(AppCore::getInstance()->GetAxis(Index));
		}
		else AddAxis(AppCore::getInstance()->GetAxis(Index));
	});

	connect(AppCore::getInstance(), &AppCore::onPlotUpdate, [this] (int Index) -> void
	{
		if (Plots.contains(Index))
		{
			if (AppCore::getInstance()->GetPlot(Index).ID == -1) DeletePlot(Index);
			else UpdatePlot(AppCore::getInstance()->GetPlot(Index));
		}
		else AddPlot(AppCore::getInstance()->GetPlot(Index));
	});
}

PlotWidget::~PlotWidget(void)
{
	QSettings("AVR-Monitor").setValue("plotavg", ui->Average->value());

	delete ui;
}

void PlotWidget::SetTitleWidget(TitleWidget* Widget)
{
	while (ui->toolsLayout->count())
	{
		QLayoutItem* I = ui->toolsLayout->takeAt(0);

		if (QWidget* W = I->widget())
			Widget->addRightWidget(W);
		else if (QSpacerItem* S = I->spacerItem())
			Widget->addRightSpacer(S);
	}
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

	Plot->setVisible(Data.Active && Plot->valueAxis()->visible());
	Plot->setName(Data.Varname);
	Plot->setPen(Pen);

	Plots.insert(Data.ID, Plot);
	Vars.insert(Data.Varlabel, Plot);
	Values.insert(Data.Varlabel, 0.0);

	ui->Plot->replot();
}

void PlotWidget::UpdatePlot(const PlotData& Data)
{
	QCPGraph* Plot = Plots[Data.ID];
	double oldVal = Values[Vars.key(Plot)];

	Plot->setVisible(Data.Active && Plot->valueAxis()->visible());
	Plot->setName(Data.Varname);

	Vars.remove(Vars.key(Plot));
	Vars.insert(Data.Varlabel, Plot);

	Values.remove(Vars.key(Plot));
	Values.insert(Data.Varlabel, oldVal);

	ui->Plot->replot();
}

void PlotWidget::DeletePlot(int ID)
{
	Vars.remove(Vars.key(Plots[ID]));
	Values.remove(Vars.key(Plots[ID]));

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

		Plot->setVisible(Data.Active);
	}

	ui->Plot->replot();
}

void PlotWidget::DeleteAxis(int ID)
{
	for (auto Plot: Axes[ID]->graphs()) DeletePlot(Plots.key(Plot));

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
	QList<double> Keys;

	for (const auto& Plot: Plots) if (Plot->visible()) Keys.append(Plot->data()->keys());
	auto Set = Keys.toSet().values();

	if (Set.isEmpty())
	{
		QMessageBox::warning(this, tr("Error"), tr("Can't export current data - plot is empty")); return;
	}
	else
	{
		Keys.clear();
		qSort(Set);
	}

	const QString Path = QFileDialog::getSaveFileName(this, tr("Select file to save data"), QString(), tr("CSV files (*.csv)"));

	if (!Path.isEmpty())
	{
		QFile File(Path); if (File.open(QFile::WriteOnly))
		{
			QTextStream Stream(&File);

			const QChar Separator = ',';
			const double First = Set.first();

			Stream << tr("Time");

			for (const auto& Var: Vars.keys()) if (Vars[Var]->visible()) Stream << Separator << Var;

			Stream << '\n';

			for (const auto& Key: Set)
			{
				Stream << (Key - First);

				for (const auto& Plot: Vars) if (Plot->visible()) Stream << Separator << Plot->data()->value(Key).value;

				Stream << '\n';
			}
		}
		else
		{
			QMessageBox::warning(this, tr("Error"), tr("Can't open selected file"));
		}
	}
}

void PlotWidget::LegendCheckClicked(bool Active)
{
	ui->Plot->legend->setVisible(Active);
	ui->Plot->replot();
}

void PlotWidget::AverageSpinChanged(int Value)
{
	for (auto& Value: Values) Value = 0; Step = 1; Samples = Value;

	ui->Average->setSuffix(tr(" sample(s)", 0, Value));
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
	double Time = 0;

	if (Starttime.isNull()) Starttime = QTime::currentTime();
	else Time = Starttime.msecsTo(QTime::currentTime()) / 1000.0;

	for (const auto& Var: Variables)
	{
		const QString ID = (const char*) Var.ID;

		if (Vars.contains(ID))
		{
			Values[ID] += Var.Value.ToNumber();

			if (Step == Samples)
			{
				Vars[ID]->addData(Time, Values[ID] / Samples); Values[ID] = 0.0;
			}
		}
	}

	if (ui->Follow->isChecked() && ui->Plot->xAxis->range().upper < Time)
	{
		Userrange = false; ui->Plot->xAxis->setRange(Time, ui->Plot->xAxis->range().size(), Qt::AlignRight);
	}

	if (Step++ == Samples) Step = 1;

	ui->Plot->replot();
}

void PlotWidget::RestartPlot(void)
{
	for (auto Plot: Plots) Plot->clearData(); ui->Plot->replot(); Userrange = false;

	ui->Plot->xAxis->setRange(0, ui->Plot->xAxis->range().size(), Qt::AlignLeft);

	Starttime = QTime();
}
