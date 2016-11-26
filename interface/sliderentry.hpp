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

#ifndef SLIDERENTRY_HPP
#define SLIDERENTRY_HPP

#include <QWidget>

#include "sliderdialog.hpp"
#include "common.hpp"

namespace Ui
{
	class SliderEntry;
}

class SliderEntry : public QWidget
{

		Q_OBJECT

	private:

		Ui::SliderEntry* ui = nullptr;

		SliderDialog* Dialog;

		const int ID;

	public:

		explicit SliderEntry(const SliderData& Data, QWidget* Parent = nullptr);
		virtual ~SliderEntry(void) override;

	private slots:

		void SliderValueChange(int Value);

		void SpinValueChange(double Value);

		void SettingsButtonClicked(void);

		void DeleteButtonClicked(void);

		void UpdateSlider(const SliderData& Data);

		void UpdateRequest(int Index);

	signals:

		void onSliderUpdate(const SliderData&);

		void onValueUpdate(double);

};

#endif // SLIDERENTRY_HPP
