/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Ploting options dialog for AVR-Monitor                                 *
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

#include "plotdialog.hpp"
#include "ui_plotdialog.h"

PlotDialog::PlotDialog(QWidget* Parent)
: QDialog(Parent), ui(new Ui::PlotDialog)
{
	ui->setupUi(this);

	ui->Sensors->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
}

PlotDialog::~PlotDialog(void)
{
	delete ui;
}

void PlotDialog::AddAxis(const AxisData& Axis)
{
	ui->Axes->addItem(Axis.Name, QVariant(Axis.ID));

	emit onAxisAdd(Axis);
}

void PlotDialog::UpdateAxis(const AxisData& Axis)
{
	const int ID = ui->Axes->findData(Axis.ID);

	ui->Axes->setItemText(ID, Axis.Name);

	emit onAxisChange(Axis);
}

void PlotDialog::UpdatePlot(QListWidgetItem* Plot)
{
	PlotData Data = AppCore::getInstance()->GetPlot(Plot->data(Qt::UserRole).toInt());

	Data.Active = Plot->checkState() == Qt::Checked;

	if (AppCore::getInstance()->UpdatePlot(Data))
	{
		emit onPlotChange(Data);
	}
	else
	{
		QMessageBox::warning(this, tr("Error"), tr("Can't update plot in database - %1").arg(AppCore::getError()));
	}
}

void PlotDialog::UpdateList(void)
{
	ui->Plots->blockSignals(true);

	ui->Plots->clear();

	for (const auto& Plot: AppCore::getInstance()->GetPlots()) if (Plot.AXIS_ID == ui->Axes->currentData())
	{
		QListWidgetItem* Item = new QListWidgetItem(QString("%1 (%2)").arg(Plot.Varname).arg(Plot.Varlabel), ui->Plots);

		Item->setFlags(Item->flags() | Qt::ItemIsUserCheckable);
		Item->setData(Qt::UserRole, QVariant(Plot.ID));
		Item->setCheckState(Plot.Active ? Qt::Checked : Qt::Unchecked);
	}

	ui->Plots->blockSignals(false);
}

void PlotDialog::AddButtonClicked(void)
{
	AxisDialog* Dialog = new AxisDialog(-1, this);

	connect(Dialog, &AxisDialog::onDialogAccept, this, &PlotDialog::AddAxis);
	connect(Dialog, &QDialog::accepted, Dialog, &QObject::deleteLater);
	connect(Dialog, &QDialog::rejected, Dialog, &QObject::deleteLater);

	Dialog->open();
}

void PlotDialog::DeleteButtonClicked(void)
{
	if (ui->Axes->currentIndex() < 0)
	{
		QMessageBox::warning(this, tr("Error"), tr("Select a valid axis to delete"));
	}
	else if (QMessageBox::question(this, tr("Delete axis"), tr("Are you sure you wany to delete selected axis (%1)?").arg(ui->Axes->currentText())) == QMessageBox::Yes)
	{
		const int ID = ui->Axes->currentData().toInt();

		if (AppCore::getInstance()->DeleteAxis(ID))
		{
			ui->Axes->removeItem(ui->Axes->currentIndex());

			emit onAxisDelete(ID);
		}
		else
		{
			QMessageBox::warning(this, tr("Error"), tr("Can't delete axis - %1").arg(AppCore::getError()));
		}
	}
}

void PlotDialog::SettingsButtonClicked(void)
{
	if (ui->Axes->currentIndex() < 0)
	{
		QMessageBox::warning(this, tr("Error"), tr("Select a valid axis to modify"));
	}
	else
	{
		AxisDialog* Dialog = new AxisDialog(ui->Axes->currentData().toInt(), this);

		connect(Dialog, &AxisDialog::onDialogAccept, this, &PlotDialog::UpdateAxis);
		connect(Dialog, &QDialog::accepted, Dialog, &QObject::deleteLater);
		connect(Dialog, &QDialog::rejected, Dialog, &QObject::deleteLater);

		Dialog->open();
	}
}

void PlotDialog::InsertButtonClicked(void)
{
	const int ID = ui->Sensors->currentRow(); if (ID < 0) return;

	PlotData Plot;

	Plot.AXIS_ID = ui->Axes->currentData().toInt();
	Plot.SENSOR_ID = ui->Sensors->item(ID, 0)->data(Qt::UserRole).toInt();
	Plot.Active = true;

	if (AppCore::getInstance()->AddPlot(Plot))
	{
		Plot = AppCore::getInstance()->GetPlot(Plot.ID);

		UpdateList(); emit onPlotAdd(Plot);
	}
	else
	{
		QMessageBox::warning(this, tr("Error"), tr("Can't add plot to database - %1").arg(AppCore::getError()));
	}
}

void PlotDialog::RemoveButtonClicked(void)
{
	QListWidgetItem* Item = ui->Plots->currentItem(); if (!Item) return;

	if (AppCore::getInstance()->DeletePlot(Item->data(Qt::UserRole).toInt()))
	{
		emit onPlotDelete(Item->data(Qt::UserRole).toInt()); delete Item;
	}
	else
	{
		QMessageBox::warning(this, tr("Error"), tr("Can't delete plot from database - %1").arg(AppCore::getError()));
	}
}

void PlotDialog::open(void)
{
	while (ui->Sensors->rowCount()) ui->Sensors->removeRow(0);
	while (ui->Axes->count()) ui->Axes->removeItem(0);

	for (const auto& Sensor: AppCore::getInstance()->GetSensors())
	{
		ui->Sensors->insertRow(0);

		ui->Sensors->setItem(0, 0, new QTableWidgetItem(Sensor.Name));
		ui->Sensors->setItem(0, 1, new QTableWidgetItem(Sensor.Label));
		ui->Sensors->setItem(0, 2, new QTableWidgetItem(Sensor.Unit));

		ui->Sensors->item(0, 0)->setData(Qt::UserRole, QVariant(Sensor.ID));
	}

	for (const auto& Axis: AppCore::getInstance()->GetAxes())
	{
		ui->Axes->addItem(Axis.Name, QVariant(Axis.ID));
	}

	QDialog::open();
}
