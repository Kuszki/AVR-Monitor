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

#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* Parent)
: QMainWindow(Parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	aboutDialog = new AboutDialog(this);

	Weight = new QComboBox(this);
	Interval = new QDoubleSpinBox(this);
	Average = new QSpinBox(this);

	QSettings Settings("AVR-Monitor");

	Settings.beginGroup("Device");

	Weight->addItems(QStringList() << tr("Linear weights") << tr("Triangle weights") << tr("Exponential weights"));
	Weight->setCurrentIndex(Settings.value("weights", 0).toInt());
	Weight->setMinimumWidth(Weight->sizeHint().width() + 25);
	Weight->setMinimumHeight(Average->sizeHint().height());
	Weight->setEnabled(false);

	Interval->setValue(Settings.value("interval", 1.0).toDouble());
	Interval->setRange(0.01, 6.0);
	Interval->setSingleStep(0.01);
	Interval->setPrefix(tr("Delay "));
	Interval->setSuffix(tr(" s"));
	Interval->setEnabled(false);

	Average->setValue(Settings.value("average", 1).toInt());
	Average->setRange(1, 25);
	Average->setSingleStep(1);
	Average->setPrefix(tr("Moving average from "));
	Average->setSuffix(tr(" sample(s)", 0, Average->value()));
	Average->setEnabled(false);

	Settings.endGroup();

	ui->terminalWidget->setEnabled(false);

	ui->toolActions->addWidget(Interval);
	ui->toolActions->addSeparator();
	ui->toolActions->addWidget(Average);
	ui->toolActions->addSeparator();
	ui->toolActions->addWidget(Weight);

	ui->centralWidget->deleteLater();

	ui->adcWidget->SetTitleWidget(new TitleWidget(ui->adcDock));
	ui->systemWidget->SetTitleWidget(new TitleWidget(ui->systemDock));
	ui->pgaWidget->SetTitleWidget(new TitleWidget(ui->pgaDock));
	ui->shiftWidget->SetTitleWidget(new TitleWidget(ui->shiftDock));

	ui->plotWidget->SetTitleWidget(new TitleWidget(ui->plotDock));
	ui->terminalWidget->SetTitleWidget(new TitleWidget(ui->terminalDock));
	ui->sensorsWidget->SetTitleWidget(new TitleWidget(ui->sensorsDock));
	ui->eventsWidget->SetTitleWidget(new TitleWidget(ui->eventsDock));
	ui->slidersWidget->SetTitleWidget(new TitleWidget(ui->slidersDock));
	ui->devicesWidget->SetTitleWidget(new TitleWidget(ui->devicesDock));

	AppCore::getInstance()->UpdateInterval(Interval->value());
	AppCore::getInstance()->UpdateWeight(Weight->currentIndex());
	AppCore::getInstance()->UpdateAverage(Average->value());

	Settings.beginGroup("Window");

	setTabPosition(Qt::AllDockWidgetAreas, QTabWidget::TabPosition::North);
	restoreGeometry(Settings.value("size").toByteArray());

	if (isMaximized()) setGeometry(QApplication::desktop()->availableGeometry(this));

	if (!restoreState(Settings.value("layout").toByteArray()))
	{
		tabifyDockWidget(ui->plotDock, ui->sensorsDock);
		tabifyDockWidget(ui->plotDock, ui->eventsDock);
		tabifyDockWidget(ui->plotDock, ui->devicesDock);
		tabifyDockWidget(ui->plotDock, ui->slidersDock);
		tabifyDockWidget(ui->plotDock, ui->terminalDock);

		tabifyDockWidget(ui->systemDock, ui->adcDock);

		tabifyDockWidget(ui->shiftDock, ui->pgaDock);
	}

	Settings.endGroup();

	// appcore to main window connections
	connect(AppCore::getInstance(), &AppCore::onScriptTermination, [this] (void) -> void
	{
		QMessageBox::warning(this, tr("Error"), tr("Script evaluation timeout"));
	});

	// actions to app core lambdas connections
	connect(ui->actionDisconnect, &QAction::triggered, [] (void) -> void
	{
		AppCore::getInstance()->UpdateStatus(false);
	});

	// terminal to device connections
	connect(ui->terminalWidget, &TerminalWidget::onScriptExecute, [] (const QString& Code) -> void
	{
		AppCore::getDevice()->Command(KLScriptbinding::Optimize(Code));
	});

	connect(ui->terminalWidget, &TerminalWidget::onScriptValidate, [this] (const QString& Code) -> void
	{
		QMessageBox::information(this, tr("Script check"), AppCore::getValidation(Code));
	});

	// app core to this ui connections
	connect(AppCore::getInstance(), &AppCore::onEmergencyStop, boost::bind(&MainWindow::ServiceStatusChanged, this, false, false));

	// this ui to app core connections
	connect(ui->actionRun, &QAction::triggered, boost::bind(&MainWindow::ServiceStatusChanged, this, true, true));
	connect(ui->actionStop, &QAction::triggered, boost::bind(&MainWindow::ServiceStatusChanged, this, false, true));

	// main window to plot widget connections
	connect(ui->actionRun, &QAction::triggered, ui->plotWidget, &PlotWidget::RestartPlot);

	// app core to plot widget connections
	connect(AppCore::getInstance(), &AppCore::onValuesUpdate, ui->plotWidget, &PlotWidget::PlotVariables);

	// device to system widget connections
	connect(AppCore::getDevice(), &AVRBridge::onConnectionUpdate, ui->systemWidget, &SystemWidget::UpdateLink);
	connect(AppCore::getDevice(), &AVRBridge::onMasterStatusUpdate, ui->systemWidget, &SystemWidget::UpdateStatus);
	connect(AppCore::getDevice(), &AVRBridge::onShiftValuesUpdate,  ui->systemWidget, &SystemWidget::UpdateShiftValues);
	connect(AppCore::getDevice(), &AVRBridge::onShiftStatusUpdate, ui->systemWidget, &SystemWidget::UpdateShiftStatus);
	connect(AppCore::getDevice(), &AVRBridge::onGainSettingsUpdate, ui->systemWidget, &SystemWidget::UpdateGainValue);
	connect(AppCore::getDevice(), &AVRBridge::onDutyValueUpdate, ui->systemWidget, &SystemWidget::UpdateDutyValue);
	connect(AppCore::getDevice(), &AVRBridge::onFreeRamUpdate, ui->systemWidget, &SystemWidget::UpdateFreeRam);
	connect(AppCore::getDevice(), &AVRBridge::onSleepValueUpdate, ui->systemWidget, &SystemWidget::UpdateInterval);

	// system widget to device connections
	connect(ui->systemWidget, &SystemWidget::onRamCleanRequest, AppCore::getDevice(), &AVRBridge::CleanMasterRam);

	// device to adc widget connections
	connect(AppCore::getDevice(), &AVRBridge::onSensorValuesUpdate, ui->adcWidget, &AdcWidget::UpdateValues);

	// device to pga widget connections
	connect(AppCore::getDevice(), &AVRBridge::onGainSettingsUpdate, ui->pgaWidget, &GainWidget::GainChanged);
	connect(AppCore::getDevice(), &AVRBridge::onDutyValueUpdate, ui->pgaWidget, &GainWidget::DutyChanged);

	// pga to device connections
	connect(ui->pgaWidget, &GainWidget::onGainChange, AppCore::getDevice(), &AVRBridge::WriteGainSettings);
	connect(ui->pgaWidget, &GainWidget::onDutyChange, AppCore::getDevice(), &AVRBridge::WriteDutyValue);

	// device to shift widget connections
	connect(AppCore::getDevice(), &AVRBridge::onShiftValuesUpdate, ui->shiftWidget, &ShiftWidget::UpdateShiftValues);
	connect(AppCore::getDevice(), &AVRBridge::onShiftStatusUpdate, ui->shiftWidget, &ShiftWidget::UpdateShiftStatus);

	// shift widget to device connections
	connect(ui->shiftWidget, &ShiftWidget::onShiftChanged, AppCore::getDevice(), &AVRBridge::WriteShiftValues);
	connect(ui->shiftWidget, &ShiftWidget::onEnabledChanged, AppCore::getDevice(), &AVRBridge::WriteShiftStatus);

	// device to terminal log window
	connect(AppCore::getDevice(), &AVRBridge::onMessageSend, ui->terminalWidget, &TerminalWidget::AppendOutput);
	connect(AppCore::getDevice(), &AVRBridge::onMessageReceive, ui->terminalWidget, &TerminalWidget::AppendInput);

	// synchronize to app core connections
	connect(ui->actionSynchronize, &QAction::triggered, AppCore::getInstance(), &AppCore::SynchronizeDevice);

	// device to main window connections
	connect(AppCore::getDevice(), &AVRBridge::onError, this, &MainWindow::ShowErrorMessage);
	connect(AppCore::getDevice(), &AVRBridge::onConnectionUpdate, this, &MainWindow::ConnectionChanged);
	connect(AppCore::getDevice(), &AVRBridge::onMasterScriptReceive, this, &MainWindow::SaveMasterScript);

	// database reload connections
	connect(this, &MainWindow::onReloadDatabase, ui->sensorsWidget, &SensorWidget::ReloadSensors);
	connect(this, &MainWindow::onReloadDatabase, ui->eventsWidget, &EventWidget::ReloadEvents);
	connect(this, &MainWindow::onReloadDatabase, ui->devicesWidget, &DeviceWidget::ReloadDevices);
	connect(this, &MainWindow::onReloadDatabase, ui->slidersWidget, &SliderWidget::ReloadSliders);
	connect(this, &MainWindow::onReloadDatabase, ui->plotWidget, &PlotWidget::ReloadPlots);

	// connect signals by old way because of overloaded methods
	connect(Weight, SIGNAL(currentIndexChanged(int)), AppCore::getInstance(), SLOT(UpdateWeight(int)));
	connect(Interval, SIGNAL(valueChanged(double)), SLOT(IntervalValueChanged(double)));
	connect(Average, SIGNAL(valueChanged(int)), SLOT(AverageValueChanged(int)));
}

MainWindow::~MainWindow(void)
{
	QSettings Settings("AVR-Monitor");

	Settings.beginGroup("Window");
	Settings.setValue("size", saveGeometry());
	Settings.setValue("layout", saveState());
	Settings.endGroup();

	Settings.beginGroup("Device");
	Settings.setValue("weights", Weight->currentIndex());
	Settings.setValue("interval", Interval->value());
	Settings.setValue("average", Average->value());
	Settings.endGroup();

	delete ui;
}

void MainWindow::ConnectDevice(void)
{
	QSettings Settings("AVR-Monitor");

	Settings.beginGroup("Device");
	AppCore::getDevice()->Connect(Settings.value("port", "/dev/serial/by-id/usb-Łukasz__Kuszki__Dróżdż_AVR-Monitor_DAYYSEBT-if00-port0").toString());
	Settings.endGroup();
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

void MainWindow::SaveEnvironment(void)
{
	QString Name = QFileDialog::getSaveFileName(this, tr("Select file to save environment"), QString(), "SQLite (*.sqlite)");

	if (!Name.isEmpty())
	{
		if (!AppCore::getInstance()->SaveDatabase(Name)) ShowErrorMessage(tr("Can't open provided database file"));
	}
}

void MainWindow::LoadEnvironment(void)
{
	QString Name = QFileDialog::getOpenFileName(this, tr("Select file to load environment"), QString(), "SQLite (*.sqlite)");

	if (!Name.isEmpty())
	{
		if (!AppCore::getInstance()->LoadDatabase(Name)) ShowErrorMessage(tr("Can't open provided database file"));
		else emit onReloadDatabase();
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

	ui->terminalWidget->setEnabled(Connected);

	Weight->setEnabled(Connected);
	Interval->setEnabled(Connected);
	Average->setEnabled(Connected);
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

void MainWindow::AverageValueChanged(int Value)
{
	Average->setSuffix(tr(" sample(s)", 0, Value));

	AppCore::getInstance()->UpdateAverage(Value);
}

void MainWindow::ServiceStatusChanged(bool Active, bool User)
{
	ui->actionRun->setEnabled(!Active);
	ui->actionStop->setEnabled(Active);
	ui->actionSynchronize->setEnabled(!Active);
	ui->actionUpload->setEnabled(!Active);
	ui->actionDownload->setEnabled(!Active);
	ui->actionExport->setEnabled(!Active);
	ui->actionImport->setEnabled(!Active);

	if (User) AppCore::getInstance()->UpdateStatus(Active);
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
