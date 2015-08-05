/***********************************************************************
 *
 *  AVR-Monitor main. cpp file
 *  Copyright (C) 2015  Łukasz "Kuszki" Dróżdż
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see http://www.gnu.org/licenses/.
 *
 **********************************************************************/

#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* Parent)
: QMainWindow(Parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	ui->tabTerminal->setEnabled(false);
	ui->tabLog->setEnabled(false);

	avrDevice = new AVRBridge(this);

	aboutDialog = new AboutDialog(this);
	settingsDialog = new SettingsDialog(this);

	setTabPosition(Qt::AllDockWidgetAreas, QTabWidget::TabPosition::North);

	restoreState(QSettings("layout.ini", QSettings::IniFormat).value("layout").toByteArray(), 1);

	// device to system widget connections
	connect(avrDevice, &AVRBridge::onConnectionUpdate, ui->systemWidget, &SystemWidget::UpdateLink);
	connect(avrDevice, &AVRBridge::onMasterStatusUpdate, ui->systemWidget, &SystemWidget::UpdateStatus);
	connect(avrDevice, &AVRBridge::onShiftValuesUpdate,  ui->systemWidget, &SystemWidget::UpdateShiftValues);
	connect(avrDevice, &AVRBridge::onShiftStatusUpdate, ui->systemWidget, &SystemWidget::UpdateShiftStatus);
	connect(avrDevice, &AVRBridge::onGainSettingsUpdate, ui->systemWidget, &SystemWidget::UpdateGainValue);
	connect(avrDevice, &AVRBridge::onFreeRamUpdate, ui->systemWidget, &SystemWidget::UpdateFreeRam);
	connect(avrDevice, &AVRBridge::onSleepValueUpdate, ui->systemWidget, &SystemWidget::UpdateInterval);

	// system widget to device connections
	connect(ui->systemWidget, &SystemWidget::onRamCleanRequest, avrDevice, &AVRBridge::CleanMasterRam);

	// device to adc widget connections
	connect(avrDevice, &AVRBridge::onSensorValuesUpdate, ui->adcWidget, &AdcWidget::UpdateValues);

	// device to pga widget connections
	connect(avrDevice, &AVRBridge::onGainSettingsUpdate, ui->pgaWidget, &GainWidget::GainChanged);

	// pga to device connections
	connect(ui->pgaWidget, &GainWidget::onGainChange, avrDevice, &AVRBridge::WriteGainSettings);

	// device to settings connections
	connect(avrDevice, &AVRBridge::onSleepValueUpdate, settingsDialog, &SettingsDialog::UpdateMasterInterval);

	// settings to device connections
	connect(settingsDialog, &SettingsDialog::onMasterIntervalChange, avrDevice, &AVRBridge::WriteSleepValue);

	// device to shift widget connections
	connect(avrDevice, &AVRBridge::onShiftValuesUpdate, ui->shiftWidget, &ShiftWidget::UpdateShiftValues);
	connect(avrDevice, &AVRBridge::onShiftStatusUpdate, ui->shiftWidget, &ShiftWidget::UpdateShiftStatus);

	// shift widget to device connections
	connect(ui->shiftWidget, &ShiftWidget::onShiftChanged, avrDevice, &AVRBridge::WriteShiftValues);
	connect(ui->shiftWidget, &ShiftWidget::onEnabledChanged, avrDevice, &AVRBridge::WriteShiftStatus);

	// terminal to device connections SID!
	connect(ui->tabTerminal, &TerminalWidget::onScriptExecute, [this] (const QString& Code) -> void { avrDevice->Command(KLScriptbinding::Optimize(Code)); });

	// device to main window connections
	connect(avrDevice, &AVRBridge::onError, this, &MainWindow::ShowErrorMessage);
	connect(avrDevice, &AVRBridge::onConnectionUpdate, this, &MainWindow::ConnectionChanged);
	connect(avrDevice, &AVRBridge::onMasterScriptReceive, this, &MainWindow::SaveMasterScript);
}

MainWindow::~MainWindow(void)
{
	QSettings("layout.ini", QSettings::IniFormat).setValue("layout", saveState(1));

	delete avrDevice;

	delete ui;
}

void MainWindow::ConnectDevice(void)
{
	avrDevice->Connect("/dev/serial/by-id/usb-Łukasz__Kuszki__Dróżdż_AVR-Monitor_DAYYSEBT-if00-port0");
}

void MainWindow::DisconnectDevice(void)
{
	avrDevice->Disconnect();
}

void MainWindow::DownloadScript(void)
{
	avrDevice->ReadMasterScript();
}

void MainWindow::UploadScript(void)
{
	QString Name = QFileDialog::getOpenFileName(this, tr("Select script to upload"));

	if (!Name.isEmpty())
	{
		QFile File(Name);

		if (File.open(QFile::ReadOnly)) avrDevice->WriteMasterScript(File.readAll());
		else ShowErrorMessage(tr("Can't open provided script file."));
	}
}

void MainWindow::ShowErrorMessage(const QString& Message)
{
	QMessageBox::warning(this, tr("Error"), Message);
}

void MainWindow::ConnectionChanged(bool Connected)
{
	ui->actionConnect->setEnabled(!Connected);
	ui->actionDisconnect->setEnabled(Connected);

	ui->actionDownload->setEnabled(Connected);
	ui->actionUpload->setEnabled(Connected);
	ui->actionRun->setEnabled(Connected);
	ui->actionRecord->setEnabled(Connected);
	ui->actionMaster->setEnabled(Connected);
	ui->actionSettings->setEnabled(Connected);
	ui->actionSynchronize->setEnabled(Connected);

	ui->pgaWidget->setEnabled(Connected);
	ui->shiftWidget->setEnabled(Connected);
	ui->systemWidget->setEnabled(Connected);
	ui->adcWidget->setEnabled(Connected);

	ui->tabTerminal->setEnabled(Connected);
	ui->tabLog->setEnabled(Connected);
}

void MainWindow::SaveMasterScript(const QString& Script)
{
	QString Name = QFileDialog::getSaveFileName(this, tr("Select file to save script"));

	if (!Name.isEmpty())
	{
		QFile File(Name);

		if (!File.open(QFile::WriteOnly)) ShowErrorMessage(tr("Can't open provided file."));
		else
		{
			File.write(Script.toUtf8());
		}
	}
}

void MainWindow::ShowAboutDialog(void)
{
	aboutDialog->show();
}

void MainWindow::ShowSettingsDialog(void)
{
	settingsDialog->open();
}

void MainWindow::ShowProjectWeb(void)
{
	QDesktopServices::openUrl(QUrl::fromUserInput("https://github.com/Kuszki/AVR-Monitor"));
}

void MainWindow::ToggleFulscreenMode(bool Fulscreen)
{
	if (Fulscreen) showFullScreen();
	else showMaximized();
}
