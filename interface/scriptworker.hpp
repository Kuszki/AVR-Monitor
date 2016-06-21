/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Script worker object for AVR-Monitor GUI                               *
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

#ifndef SCRIPTWORKER_HPP
#define SCRIPTWORKER_HPP

#include <QStringList>
#include <QObject>
#include <QMutex>

#include "KLLibs.hpp"

class ScriptWorker : public QObject
{

		Q_OBJECT

	private:

		volatile bool Completed = true;

		KLScriptbinding* Script;
		QStringList* Tasks;
		QMutex* Locker;

	public:

		explicit ScriptWorker(KLScriptbinding* Bind, QStringList* Jobs, QMutex* Sync = nullptr, QObject* Parent = nullptr);

		bool isComplete(void) const;

	public slots:

		void PerformEvaluations(void);

	signals:

		void onEvaluationComplete(void);

};

#endif // SCRIPTWORKER_HPP
