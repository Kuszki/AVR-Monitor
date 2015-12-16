/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Script worker object for AVR-Monitor GUI                               *
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

#include "scriptworker.hpp"

ScriptWorker::ScriptWorker(KLScriptbinding* Bind, QStringList* Jobs, QMutex* Sync, QObject* Parent)
: QObject(Parent), Script(Bind), Tasks(Jobs), Locker(Sync) {}

bool ScriptWorker::isComplete(void) const
{
	return Completed;
}

void ScriptWorker::PerformEvaluations(void)
{
	Locker->lock();
	Completed = false;

	for (const auto& Task: *Tasks)
	{
		Script->SetCode(Task);
		Script->Evaluate();
	}

	Completed = true;
	Locker->unlock();

	emit onEvaluationComplete();
}
