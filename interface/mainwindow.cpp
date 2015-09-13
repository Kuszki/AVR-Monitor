/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Main window for AVR-Monitor                                            *
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

#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* Parent)
: QMainWindow(Parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	Interval = new QDoubleSpinBox(this);

	Interval->setPrefix(tr("Delay "));
	Interval->setSuffix(tr(" s"));
	Interval->setRange(0.05, 6.0);
	Interval->setSingleStep(0.05);
	Interval->setEnabled(false);

	// connect interval by old way because of overloaded methods
	connect(Interval, SIGNAL(valueChanged(double)), SLOT(IntervalValueChanged(double)));

	Interval->setValue(QSettings("settings.ini", QSettings::IniFormat).value("interval", 1.0).toDouble());
	AppCore::getInstance()->UpdateInterval(Interval->value());

	ui->tabTerminal->setEnabled(false);
	ui->tabLog->setEnabled(false);

	ui->toolActions->addWidget(Interval);

	aboutDialog = new AboutDialog(this);

	setTabPosition(Qt::AllDockWidgetAreas, QTabWidget::TabPosition::North);

	restoreState(QSettings("layout.ini", QSettings::IniFormat).value("layout").toByteArray(), 1);

	// this ui to app core connections
	connect(ui->actionRun, &QAction::triggered, [this] (void) -> void
	{
		AppCore::getInstance()->UpdateStatus(true);

		ui->actionRun->setEnabled(false);
		ui->actionStop->setEnabled(true);
		ui->actionSynchronize->setEnabled(false);
		ui->actionUpload->setEnabled(false);
		ui->actionDownload->setEnabled(false);
	});

	connect(ui->actionStop, &QAction::triggered, [this] (void) -> void
	{
		AppCore::getInstance()->UpdateStatus(false);

		ui->actionRun->setEnabled(true);
		ui->actionStop->setEnabled(false);
		ui->actionSynchronize->setEnabled(true);
		ui->actionUpload->setEnabled(true);
		ui->actionDownload->setEnabled(true);
	});

	connect(ui->actionDisconnect, &QAction::triggered, [this] (void) -> void
	{
		AppCore::getInstance()->UpdateStatus(false);
	});

	// terminal to device connections SID!
	connect(ui->tabTerminal, &TerminalWidget::onScriptExecute, [this] (const QString& Code) -> void
	{
		AppCore::getDevice()->Command(KLScriptbinding::Optimize(Code));
	});

	connect(ui->tabTerminal, &TerminalWidget::onScriptValidate, [this] (const QString& Code) -> void
	{
		QMessageBox::information(this, tr("Script check"), AppCore::getValidation(Code));
	});

	// main window to plot widget connections
	connect(ui->actionRun, &QAction::triggered, ui->tabPlot, &PlotWidget::RestartPlot);
	connect(AppCore::getInstance(), &AppCore::onValuesUpdate, ui->tabPlot, &PlotWidget::PlotVariables);
	connect(AppCore::getInstance(), &AppCore::onSensorUpdate, ui->tabPlot, &PlotWidget::UpdateSensors);

	// device to system widget connections
	connect(AppCore::getDevice(), &AVRBridge::onConnectionUpdate, ui->systemWidget, &SystemWidget::UpdateLink);
	connect(AppCore::getDevice(), &AVRBridge::onMasterStatusUpdate, ui->systemWidget, &SystemWidget::UpdateStatus);
	connect(AppCore::getDevice(), &AVRBridge::onShiftValuesUpdate,  ui->systemWidget, &SystemWidget::UpdateShiftValues);
	connect(AppCore::getDevice(), &AVRBridge::onShiftStatusUpdate, ui->systemWidget, &SystemWidget::UpdateShiftStatus);
	connect(AppCore::getDevice(), &AVRBridge::onGainSettingsUpdate, ui->systemWidget, &SystemWidget::UpdateGainValue);
	connect(AppCore::getDevice(), &AVRBridge::onFreeRamUpdate, ui->systemWidget, &SystemWidget::UpdateFreeRam);
	connect(AppCore::getDevice(), &AVRBridge::onSleepValueUpdate, ui->systemWidget, &SystemWidget::UpdateInterval);

	// system widget to device connections
	connect(ui->systemWidget, &SystemWidget::onRamCleanRequest, AppCore::getDevice(), &AVRBridge::CleanMasterRam);

	// device to adc widget connections
	connect(AppCore::getDevice(), &AVRBridge::onSensorValuesUpdate, ui->adcWidget, &AdcWidget::UpdateValues);

	// device to pga widget connections
	connect(AppCore::getDevice(), &AVRBridge::onGainSettingsUpdate, ui->pgaWidget, &GainWidget::GainChanged);

	// pga to device connections
	connect(ui->pgaWidget, &GainWidget::onGainChange, AppCore::getDevice(), &AVRBridge::WriteGainSettings);

	// device to shift widget connections
	connect(AppCore::getDevice(), &AVRBridge::onShiftValuesUpdate, ui->shiftWidget, &ShiftWidget::UpdateShiftValues);
	connect(AppCore::getDevice(), &AVRBridge::onShiftStatusUpdate, ui->shiftWidget, &ShiftWidget::UpdateShiftStatus);

	// shift widget to device connections
	connect(ui->shiftWidget, &ShiftWidget::onShiftChanged, AppCore::getDevice(), &AVRBridge::WriteShiftValues);
	connect(ui->shiftWidget, &ShiftWidget::onEnabledChanged, AppCore::getDevice(), &AVRBridge::WriteShiftStatus);

	// device to log window
	connect(AppCore::getDevice(), &AVRBridge::onMessageSend, ui->tabLog, &LogWidget::AppendOutput);
	connect(AppCore::getDevice(), &AVRBridge::onMessageReceive, ui->tabLog, &LogWidget::AppendInput);

	// synchronize to app core connections
	connect(ui->actionSynchronize, &QAction::triggered, AppCore::getInstance(), &AppCore::SynchronizeDevice);

	// device to main window connections
	connect(AppCore::getDevice(), &AVRBridge::onError, this, &MainWindow::ShowErrorMessage);
	connect(AppCore::getDevice(), &AVRBridge::onConnectionUpdate, this, &MainWindow::ConnectionChanged);
	connect(AppCore::getDevice(), &AVRBridge::onMasterScriptReceive, this, &MainWindow::SaveMasterScript);
}

MainWindow::~MainWindow(void)
{
	QSettings("layout.ini", QSettings::IniFormat).setValue("layout", saveState(1));
	QSettings("settings.ini", QSettings::IniFormat).setValue("interval", Interval->value());

	delete ui;
}

void MainWindow::ConnectDevice(void)
{
	const QString Port = QSettings("settings.ini", QSettings::IniFormat).value("port", "/dev/serial/by-id/usb-Łukasz__Kuszki__Dróżdż_AVR-Monitor_DAYYSEBT-if00-port0").toString();

	AppCore::getDevice()->Connect(Port);
}

void MainWindow::DisconnectDevice(void)
{
	AppCore::getDevice()->Disconnect();
}

void MainWindow::DownloadScript(void)
{
	AppCore::getDevice()->ReadMasterScript();
}

void MainWindow::UploadScript(void)
{
	QString Name = QFileDialog::getOpenFileName(this, tr("Select script to upload"));

	if (!Name.isEmpty())
	{
		QFile File(Name);

		if (File.open(QFile::ReadOnly)) AppCore::getDevice()->WriteMasterScript(File.readAll());
		else ShowErrorMessage(tr("Can't open provided script file"));
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
	ui->actionStop->setEnabled(false);
	ui->actionSynchronize->setEnabled(Connected);

	ui->pgaWidget->setEnabled(Connected);
	ui->shiftWidget->setEnabled(Connected);
	ui->systemWidget->setEnabled(Connected);
	ui->adcWidget->setEnabled(Connected);

	ui->tabTerminal->setEnabled(Connected);
	ui->tabLog->setEnabled(Connected);

	Interval->setEnabled(Connected);
}

void MainWindow::SaveMasterScript(const QString& Script)
{
	QString Name = QFileDialog::getSaveFileName(this, tr("Select file to save script"));

	if (!Name.isEmpty())
	{
		QFile File(Name);

		if (!File.open(QFile::WriteOnly)) ShowErrorMessage(tr("Can't open provided file"));
		else
		{
			File.write(Script.toUtf8());
		}
	}
}

void MainWindow::IntervalValueChanged(double Value)
{
	AppCore::getInstance()->UpdateInterval(Value);
}

void MainWindow::ShowAboutDialog(void)
{
	aboutDialog->show();
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
