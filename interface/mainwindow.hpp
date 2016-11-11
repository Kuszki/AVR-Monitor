/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Main window for AVR-Monitor                                            *
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

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QDesktopServices>
#include <QDoubleSpinBox>
#include <QMessageBox>
#include <QMainWindow>
#include <QFileDialog>
#include <QSettings>
#include <QComboBox>

#include <avrbridge.hpp>

#include "aboutdialog.hpp"

namespace Ui
{
	class MainWindow;
}

class MainWindow : public QMainWindow
{

		Q_OBJECT

	private:

		Ui::MainWindow *ui;

		AboutDialog* aboutDialog;

		QComboBox* Weight;
		QDoubleSpinBox* Interval;
		QSpinBox* Average;

	public:

		explicit MainWindow(QWidget* Parent = nullptr);
		virtual ~MainWindow(void) override;

	private slots:

		void ConnectDevice(void);

		void DisconnectDevice(void);

		void DownloadScript(void);

		void UploadScript(void);

		void ShowErrorMessage(const QString& Message);

		void ConnectionChanged(bool Connected);

		void SaveMasterScript(const QString& Script);

		void IntervalValueChanged(double Value);

		void AverageValueChanged(int Value);

		void ServiceStatusChanged(bool Active, bool User);

	public slots:

		void ShowAboutDialog(void);

		void ShowProjectWeb(void);

		void ToggleFulscreenMode(bool Fulscreen);

};

#endif // MAINWINDOW_HPP
