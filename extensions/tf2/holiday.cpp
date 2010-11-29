/**
 * vim: set ts=4 :
 * =============================================================================
 * SourceMod Team Fortress 2 Extension
 * Copyright (C) 2004-2008 AlliedModders LLC.  All rights reserved.
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * As a special exception, AlliedModders LLC gives you permission to link the
 * code of this program (as well as its derivative works) to "Half-Life 2," the
 * "Source Engine," the "SourcePawn JIT," and any Game MODs that run on software
 * by the Valve Corporation.  You must obey the GNU General Public License in
 * all respects for all other code used.  Additionally, AlliedModders LLC grants
 * this exception to all derivative works.  AlliedModders LLC defines further
 * exceptions, found in LICENSE.txt (as of this writing, version JULY-31-2007),
 * or <http://www.sourcemod.net/license.php>.
 *
 * Version: $Id$
 */

#include "holiday.h"

CDetour *getHolidayDetour = NULL;

IForward *g_getHolidayForward = NULL;

DETOUR_DECL_STATIC0(GetHoliday, int)
{
	int actualres = DETOUR_STATIC_CALL(GetHoliday)();
	if (!g_getHolidayForward)
	{
		g_pSM->LogMessage(myself, "Invalid Forward");
		return actualres;
	}

	cell_t result = 0;
	int newres = actualres;

	g_getHolidayForward->PushCellByRef(&newres);
	g_getHolidayForward->Execute(&result);
	
	if (result == Pl_Changed)
	{
		return newres;
	}

	return actualres;
}

bool InitialiseGetHolidayDetour()
{
	getHolidayDetour = DETOUR_CREATE_STATIC(GetHoliday, "GetHoliday");

	if (getHolidayDetour != NULL)
	{
		getHolidayDetour->EnableDetour();
		return true;
	}

	g_pSM->LogError(myself, "GetHoliday detour failed");
	return false;
}

void RemoveGetHolidayDetour()
{
	getHolidayDetour->Destroy();
}
