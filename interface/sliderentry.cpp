/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Slider entry implementation for AVR-Interface                          *
 *  Copyright (C) 2016  Łukasz "Kuszki" Dróżdż  l.drozdz@openmailbox.org   *
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

#include "sliderentry.hpp"
#include "ui_sliderentry.h"

SliderEntry::SliderEntry(const SliderData& Data, QWidget* Parent)
: QWidget(Parent), ui(new Ui::SliderEntry), ID(Data.ID)
{
	ui->setupUi(this); UpdateSlider(Data);

	Dialog = new SliderDialog(ID, this);

	connect(AppCore::getInstance(), &AppCore::onSliderUpdate, this, &SliderEntry::UpdateRequest);

	connect(this, &SliderEntry::onValueUpdate, [this] (double Value) -> void
	{
		AppCore::getInstance()->UpdateVariable(ID, Value);
	});
}

SliderEntry::~SliderEntry(void)
{
	delete ui;
}

void SliderEntry::SliderValueChange(int Value)
{
	const double Min = ui->Spin->minimum();
	const double Step = ui->Spin->singleStep();
	const double Set = Value * Step + Min;

	ui->Spin->blockSignals(true);
	ui->Spin->setValue(Set);
	ui->Spin->blockSignals(false);

	emit onValueUpdate(Set);
}

void SliderEntry::SpinValueChange(double Value)
{
	const double Min = ui->Spin->minimum();
	const double Max = ui->Spin->maximum();

	const int Full = ui->Slider->maximum();
	const int Pos = ((Value - Min) * Full) / (Max - Min);

	ui->Slider->blockSignals(true);
	ui->Slider->setValue(Pos);
	ui->Slider->blockSignals(false);

	emit onValueUpdate(Value);
}

void SliderEntry::SettingsButtonClicked(void)
{
	Dialog->open();
}

void SliderEntry::DeleteButtonClicked(void)
{
	if (QMessageBox::question(this, tr("Delete slider"), tr("Are you sure you wany to delete selected slider (%1)?").arg(ui->Name->text())) == QMessageBox::Yes)
	{
		if (AppCore::getInstance()->DeleteSlider(ID)) deleteLater();
		else QMessageBox::warning(this, tr("Error"), tr("Can't delete slider - %1").arg(AppCore::getError()));
	}
}

void SliderEntry::UpdateSlider(const SliderData& Data)
{
	if (Data.ID != ID) return;

	const int Start = ((Data.Init - Data.Min)* Data.Steps) / (Data.Max - Data.Min);
	const double Step = (Data.Max - Data.Min) / (Data.Steps - 1);

	ui->Slider->blockSignals(true);
	ui->Spin->blockSignals(true);

	ui->Name->setText(Data.Name);
	ui->Name->setEnabled(Data.Active);

	ui->Spin->setSingleStep(Step);
	ui->Spin->setMinimum(Data.Min);
	ui->Spin->setMaximum(Data.Max);
	ui->Spin->setValue(Data.Init);
	ui->Spin->setEnabled(Data.Active);

	ui->Slider->setMaximum(Data.Steps - 1);
	ui->Slider->setValue(Start);
	ui->Slider->setEnabled(Data.Active);

	ui->Slider->blockSignals(false);
	ui->Spin->blockSignals(false);

	emit onSliderUpdate(Data);
}

void SliderEntry::UpdateRequest(int Index)
{
	if (Index == ID) UpdateSlider(AppCore::getInstance()->GetSlider(ID));
}
