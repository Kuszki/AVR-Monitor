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

#include "titlewidget.hpp"
#include "ui_titlewidget.h"

TitleWidget::TitleWidget(QDockWidget* Parent)
: QWidget(Parent), ui(new Ui::TitleWidget)
{
	ui->setupUi(this);

	ui->leftSpacer->changeSize(ui->closeButton->sizeHint().width(), 0);
	ui->rightSpacer->changeSize(ui->closeButton->sizeHint().width(), 0);

	Parent->setTitleBarWidget(this);

	setWindowTitle(Parent->windowTitle());
	setWindowIcon(Parent->windowIcon());

	connect(Parent, &QWidget::windowTitleChanged, this, &TitleWidget::setWindowTitle);
	connect(Parent, &QWidget::windowIconChanged, this, &TitleWidget::setWindowIcon);
}

TitleWidget::~TitleWidget(void)
{
	delete ui;
}

void TitleWidget::addLeftWidget(QWidget* Widget, int Stretch, Qt::Alignment Alignment)
{
	ui->leftLayout->addWidget(Widget, Stretch, Alignment);
}

void TitleWidget::addRightWidget(QWidget* Widget, int Stretch, Qt::Alignment Alignment)
{
	ui->rightLayout->addWidget(Widget, Stretch, Alignment);
}

void TitleWidget::addLeftLayout(QLayout* Layout, int Stretch)
{
	ui->leftLayout->addLayout(Layout, Stretch);
}

void TitleWidget::addRightLayout(QLayout* Layout, int Stretch)
{
	ui->rightLayout->addLayout(Layout, Stretch);
}

void TitleWidget::addLeftSpacer(QSpacerItem* Spacer)
{
	ui->leftLayout->addSpacerItem(Spacer);
}

void TitleWidget::addRightSpacer(QSpacerItem* Spacer)
{
	ui->rightLayout->addSpacerItem(Spacer);
}

void TitleWidget::addLeftItem(QLayoutItem* Item)
{
	ui->leftLayout->addItem(Item);
}

void TitleWidget::addRightItem(QLayoutItem* Item)
{
	ui->rightLayout->addItem(Item);
}

QHBoxLayout* TitleWidget::getLeftLayout(void)
{
	return ui->leftLayout;
}

QHBoxLayout* TitleWidget::getRightLayout(void)
{
	return ui->rightLayout;
}

void TitleWidget::setWindowTitle(const QString& Title)
{
	ui->Title->setText(Title);
}

void TitleWidget::setWindowIcon(const QIcon& Icon)
{
	ui->Icon->setPixmap(Icon.pixmap(24, 24));
}

void TitleWidget::CloseButtonClicked(void)
{
	QDockWidget* Dock = qobject_cast<QDockWidget*>(parent());

	Dock->close();
}

void TitleWidget::ViewButtonClicked(void)
{
	QDockWidget* Dock = qobject_cast<QDockWidget*>(parent());

	if (Dock->isFloating())
	{
		if (Dock->geometry() == QApplication::desktop()->availableGeometry(Dock))
		{
			Dock->setGeometry(Dock->property("last-geometry").toRect());
		}
		else
		{
			Dock->setProperty("last-geometry", Dock->geometry());
			Dock->setGeometry(QApplication::desktop()->availableGeometry(Dock));
		}
	}
	else Dock->setFloating(true);
}
