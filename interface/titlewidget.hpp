/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Title bar widget for dock widgets in GTK3 Titlebat style               *
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

#ifndef TITLEWIDGET_HPP
#define TITLEWIDGET_HPP

#include <QDesktopWidget>
#include <QApplication>
#include <QVBoxLayout>
#include <QDockWidget>
#include <QPixmap>
#include <QWidget>
#include <QLabel>

namespace Ui
{
	class TitleWidget;
}

class TitleWidget : public QWidget
{

		Q_OBJECT

	private:

		Ui::TitleWidget* ui;

	public:

		explicit TitleWidget(QDockWidget* Parent = nullptr);
		virtual ~TitleWidget(void) override;

		void addLeftWidget(QWidget* Widget, int Stretch = 0, Qt::Alignment Alignment = Qt::Alignment());
		void addRightWidget(QWidget* Widget, int Stretch = 0, Qt::Alignment Alignment = Qt::Alignment());

		void addLeftLayout(QLayout* Layout, int Stretch = 0);
		void addRightLayout(QLayout* Layout, int Stretch = 0);

		void addLeftSpacer(QSpacerItem* Spacer);
		void addRightSpacer(QSpacerItem* Spacer);

		void addLeftItem(QLayoutItem* Item);
		void addRightItem(QLayoutItem* Item);

		QHBoxLayout* getLeftLayout(void);
		QHBoxLayout* getRightLayout(void);

	public slots:

		void setWindowTitle(const QString& Title);
		void setWindowIcon(const QIcon& Icon);

	private slots:

		void CloseButtonClicked(void);
		void ViewButtonClicked(void);

};

#endif // TITLEWIDGET_HPP
