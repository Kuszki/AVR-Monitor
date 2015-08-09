/***********************************************************************
 *
 * {description}
 * Copyright (C) {year}  {fullname}
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 **********************************************************************/

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QDesktopServices>
#include <QDoubleSpinBox>
#include <QMessageBox>
#include <QMainWindow>
#include <QFileDialog>
#include <QSettings>

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
		QDoubleSpinBox* Interval;

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

	public slots:

		void ShowAboutDialog(void);

		void ShowProjectWeb(void);

		void ToggleFulscreenMode(bool Fulscreen);

};

#endif // MAINWINDOW_HPP
