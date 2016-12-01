/** ======================================================================== **\
	Dialog

| Description:
Adds dialogs that are not use unique ids. It helps you to make you code more
readable. Because now you have to use function to get response information.

Example:

dialog SomethingDialog(playerid, response, listitem, inputtext[])
{
	// ...
}

or use that (if response == true / false):

dialog SomethingDialog <true> (playerid, listitem, inputtext[])
{
	// ...
}

And now you have to use a special functions to show a dialog to the player.

| Version:
2.0.11

| License:
Copyright (c) 2016 Double V

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1.	The origin of this software must not be misrepresented; you must not
	claim that you wrote the original software. If you use this software
	in a product, an acknowledgement in the product documentation would be
	appreciated but is not required.
2.	Altered source versions must be plainly marked as such, and must not be
	misrepresented as being the original software.
3.	This notice may not be removed or altered from any source distribution.
\** ======================================================================== **/


#if defined _inc_tdw_dialog
	#undef _inc_tdw_dialog
#endif

#if defined _TDW_DLLIB__DIALOG
	#endinput
#endif

#define _TDW_DLLIB__DIALOG

// -----------------------------------------------------------------------------
// Default constants
// -----------------------------------------------------------------------------

#if !defined TDW_DIALOG_ID
	#define TDW_DIALOG_ID (32767)
#else
	#assert (-1 < TDW_DIALOG_ID <= 32767)
#endif

#if !defined TDW_MAX_FUNCTION_NAME
	// The max length of a function name is 31 (without a zero terminator).
	#define TDW_MAX_FUNCTION_NAME (31)
#endif

// -----------------------------------------------------------------------------
// Macros
// -----------------------------------------------------------------------------

#define dialog%1(%2) DPARSE(%1<>%2)

#define DPARSE(%1<%3>%2) forward pdR_%1(playerid,response,listitem,inputtext[]);public pdR_%1(playerid,response,listitem,inputtext[]){return g@R(%3,%1,response)(%2)
#define g@R(%3,%1,%4)(%2) _:D@A0:D@A1:%1$%2$%3$%4$
#define D@A0:D@A1:%1$%2$$%4$ _dR_%1(D@AN:%2[]$);}stock _dR_%1(%2)
#define D@A1:%1$<>%2$%3$%4$ (_:(%3)==%4)?_dR_%1(D@AN:%2[]$):0;}stock _dR_%1(%2)
#define pdR_%0\32;%1(%2) pdR_%0%1(%2)
#define _dR_%0\32;%1(%2) _dR_%0%1(%2)
#define D@AN:%2[%3]%0$) %2)


#define dtempl%1(%2) forward pdT_%1(%2);public pdT_%1(%2)
#define pdT_%0\32;%1(%2) pdT_%0%1(%2)

#define dfunc:%1, #%1,


// -----------------------------------------------------------------------------
// Vars
// -----------------------------------------------------------------------------

enum
	E_TDW_DIALOG_CALLBACKS (<<= 1)
{
	EI_CB_OnDialogPerformed
};

static stock
	TDW_g_sDialog@FuncName[MAX_PLAYERS][TDW_MAX_FUNCTION_NAME char],
	bool:TDW_g_sDialog@IsOpen[MAX_PLAYERS char],
	TDW_g_sSpecifier@ddds[] = !"ddds",
	E_TDW_DIALOG_CALLBACKS:TDW_g_sCallbacks,
	TDW_g_sOnDialogPerformed[] = !"OnDialogPerformed",
	TDW_g_sSpace[] = !" ";


// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------


stock
	Dialog_Open(playerid, const dialog_response[], style, caption[], info[],
		button[], button1[] = !"\0")
{
	TDW_g_sDialog@FuncName[playerid] = !"pdR_";
	strcat(TDW_g_sDialog@FuncName[playerid], dialog_response);

	TDW_g_sDialog@IsOpen{playerid} = true;

	return
		ShowPlayerDialog(playerid, TDW_DIALOG_ID, style, caption, info,
			button, button1
		);
}


stock
	Dialog_Close(playerid)
{
	TDW_g_sDialog@FuncName[playerid]{0} = '\0';
	TDW_g_sDialog@IsOpen{playerid} = false;

	return
		ShowPlayerDialog(playerid, -1, DIALOG_STYLE_MSGBOX, TDW_g_sSpace,
			TDW_g_sSpace, TDW_g_sSpace, TDW_g_sSpace
		);
}


stock
	Dialog_Show(playerid, const template_name[])
{
	new function_name[TDW_MAX_FUNCTION_NAME char] = !"pdT_";
	strcat(function_name, template_name);

	return
		CallLocalFunction(function_name, !"d", playerid);
}


stock
	IsPlayerInDialog(playerid)
{
	return TDW_g_sDialog@IsOpen{playerid};
}


stock
	GetPlayerUsingDialog(const dialog_name[], player_list[],
		size = sizeof player_list)
{
	new list_id, func_name[TDW_MAX_FUNCTION_NAME + 1 char] = !"pdR_";
	strcat(func_name, dialog_name);

	#if defined foreach
		foreach (new i: Player)
	#else
		for (
		#if defined GetPlayerPoolSize
			new i = GetPlayerPoolSize() + 1;
		#else
			new i = MAX_PLAYERS;
		#endif
			--i != -1;
		)
	#endif
	{
		if (_:TDW_g_sDialog@IsOpen{i} == 0)
			continue;
		if (strcmp(TDW_g_sDialog@FuncName[i], func_name, true) != 0)
			continue;
		if (list_id >= size)
			break;
		player_list[list_id++] = i;
	}
	if (list_id != 0)
		player_list[list_id] = INVALID_PLAYER_ID;

	return 1;
}


stock
	Dialog_CloseForAll(const dialog_name[])
{
	new func_name[TDW_MAX_FUNCTION_NAME + 1 char] = !"pdR_";
	strcat(func_name, dialog_name);

	#if defined foreach
		foreach (new i: Player)
	#else
		for (
		#if defined GetPlayerPoolSize
			new i = GetPlayerPoolSize() + 1;
		#else
			new i = MAX_PLAYERS;
		#endif
			--i != -1;
		)
	#endif
	{
		if (_:TDW_g_sDialog@IsOpen{i} == 0)
			continue;
		if (strcmp(TDW_g_sDialog@FuncName[i], func_name, true) != 0)
			continue;

		TDW_g_sDialog@FuncName[i]{0} = '\0';
		TDW_g_sDialog@IsOpen{i} = false;

		ShowPlayerDialog(i, -1, DIALOG_STYLE_MSGBOX, TDW_g_sSpace, TDW_g_sSpace,
			TDW_g_sSpace, TDW_g_sSpace
		);
	}
}

// -----------------------------------------------------------------------------
// Internal
// -----------------------------------------------------------------------------

// OnDialogResponse hook
public
	OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if (dialogid == TDW_DIALOG_ID)
	{
		if (TDW_g_sDialog@IsOpen{playerid})
		{
			static ret;
			if (inputtext[0] == '\0')
			{
				ret = CallLocalFunction(TDW_g_sDialog@FuncName[playerid],
					TDW_g_sSpecifier@ddds, playerid, response, listitem, !"\1"
				);
			}
			else
			{
				new len = strlen(inputtext);

				while (len != 0)
				{
					if (inputtext[len] == '%')
						inputtext[len] = '#';
					len--;
				}

				ret = CallLocalFunction(TDW_g_sDialog@FuncName[playerid],
					TDW_g_sSpecifier@ddds, playerid, response, listitem,
					inputtext
				);
			}
			if (TDW_g_sCallbacks & EI_CB_OnDialogPerformed)
			{
				static dialog_name[TDW_MAX_FUNCTION_NAME];
				strunpack(dialog_name, TDW_g_sDialog@FuncName[playerid]);
				CallLocalFunction(TDW_g_sOnDialogPerformed, !"isii",
					playerid, dialog_name, response, ret
				);
			}
		}
	}
	return
	#if defined TDWDH@OnDialogResponse
		TDWDH@OnDialogResponse(playerid, dialogid, response, listitem,
			inputtext
		);
	#else
		1; // return 1;
	#endif
}

#if defined _ALS_OnDialogResponse
	#undef OnDialogResponse
#else
	#define _ALS_OnDialogResponse
#endif

#define OnDialogResponse TDWDH@OnDialogResponse

#if defined TDWDH@OnDialogResponse
	forward TDWDH@OnDialogResponse(playerid, dialogid, response, listitem,
		inputtext[]);
#endif


// OnPlayerDisconnect hook
public
	OnPlayerDisconnect(playerid, reason)
{
	#pragma unused reason
	Dialog_Close(playerid);
	return
	#if defined TDWDH@OnPlayerDisconnect
		TDWDH@OnPlayerDisconnect(playerid, reason);
	#else
		1; // return 1;
	#endif
}

#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif

#define OnPlayerDisconnect TDWDH@OnPlayerDisconnect

#if defined TDWDH@OnPlayerDisconnect
	forward TDWDH@OnPlayerDisconnect(playerid, reason);
#endif


// OnFilterScriptInit/OnGameModeInit hook
#if defined FILTERSCRIPT
	public
		OnFilterScriptInit()
	{

		if (funcidx(TDW_g_sOnDialogPerformed) != -1)
		{
			TDW_g_sCallbacks |= EI_CB_OnDialogPerformed;
		}

		return
		#if defined TDWDH@OnFilterScriptInit
			TDWDH@OnFilterScriptInit();
		#else
			1;
		#endif
	}

	#if defined _ALS_OnFilterScriptInit
		#undef OnFilterScriptInit
	#else
		#define _ALS_OnFilterScriptInit
	#endif

	#define OnFilterScriptInit TDWDH@OnFilterScriptInit

	#if defined TDWDH@OnFilterScriptInit
		forward TDWDH@OnFilterScriptInit();
	#endif
#else
	public
		OnGameModeInit()
	{

		if (funcidx(TDW_g_sOnDialogPerformed) != -1)
		{
			TDW_g_sCallbacks |= EI_CB_OnDialogPerformed;
		}

		return
		#if defined TDWDH@OnGameModeInit
			TDWDH@OnGameModeInit();
		#else
			1;
		#endif
	}

	#if defined _ALS_OnGameModeInit
		#undef OnGameModeInit
	#else
		#define _ALS_OnGameModeInit
	#endif

	#define OnGameModeInit TDWDH@OnGameModeInit

	#if defined TDWDH@OnGameModeInit
		forward TDWDH@OnGameModeInit();
	#endif
#endif


forward OnDialogPerformed(playerid, const dialog_name[], response,
	bool:success
);
