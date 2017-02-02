/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Slider widget implementation for AVR-Interface                         *
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

#include "sliderwidget.hpp"
#include "ui_sliderwidget.h"

SliderWidget::SliderWidget(QWidget* Parent)
: QWidget(Parent), ui(new Ui::SliderWidget)
{
	ui->setupUi(this);

	Dialog = new SliderDialog(-1, this);

	QSettings Settings("AVR-Monitor");

	Settings.beginGroup("Sliders");

	ui->slidersLayout->setAlignment(Qt::AlignTop);
	ui->sliderCheck->setChecked(Settings.value("slider", true).toBool());

	Settings.endGroup();

	for (const auto& Data: AppCore::getInstance()->GetSliders()) AddSlider(Data);

	connect(Dialog, &SliderDialog::onDialogAccept, this, &SliderWidget::AddSlider);
	connect(ui->addButton, &QPushButton::clicked, Dialog, &SliderDialog::open);
}

SliderWidget::~SliderWidget(void)
{
	QSettings Settings("AVR-Monitor");

	Settings.beginGroup("Sliders");
	Settings.setValue("slider", ui->sliderCheck->isChecked());
	Settings.endGroup();

	delete ui;
}

void SliderWidget::SetTitleWidget(TitleWidget* Widget)
{
	ui->horizontalSpacer->changeSize(ui->addButton->sizeHint().width(), 0);
	ui->gridLayout->removeItem(ui->horizontalSpacer);

	Widget->addRightWidget(ui->sliderCheck);
	Widget->addRightSpacer(ui->horizontalSpacer);
	Widget->addRightWidget(ui->addButton);
}

void SliderWidget::RefreshSize(void)
{
	const int Items = ui->slidersLayout->minimumSize().width();
	const int Scroll = ui->scrollArea->verticalScrollBar()->width();

	ui->scrollArea->setMinimumWidth(Items + Scroll);
}

void SliderWidget::ReloadSliders(void)
{
	while (ui->slidersLayout->count()) ui->slidersLayout->takeAt(0)->widget()->deleteLater();
	for (const auto& Data: AppCore::getInstance()->GetSliders()) AddSlider(Data);
}

void SliderWidget::AddSlider(const SliderData& Data)
{
	SliderEntry* Entry = new SliderEntry(Data, this);

	Entry->EnableSlider(ui->sliderCheck->isChecked());

	ui->slidersLayout->addWidget(Entry); RefreshSize();

	connect(Entry, &SliderEntry::onSliderUpdate, this, &SliderWidget::RefreshSize);
	connect(ui->sliderCheck, &QToolButton::toggled, Entry, &SliderEntry::EnableSlider);
}
