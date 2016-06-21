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

#ifndef SLIDERWIDGET_HPP
#define SLIDERWIDGET_HPP

#include <QPushButton>
#include <QScrollArea>
#include <QScrollBar>
#include <QWidget>

#include "sliderdialog.hpp"
#include "sliderentry.hpp"
#include "appcore.hpp"
#include "common.hpp"

namespace Ui
{
	class SliderWidget;
}

class SliderWidget : public QWidget
{

		Q_OBJECT

	private:

		Ui::SliderWidget* ui;

		SliderDialog* Dialog;

	public:

		explicit SliderWidget(QWidget* Parent = nullptr);
		virtual ~SliderWidget(void) override;

	public slots:

		void RefreshSize(void);

	private slots:

		void AddSlider(const SliderData& Data);

};

#endif // SLIDERWIDGET_HPP
