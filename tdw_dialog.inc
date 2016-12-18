/** ======================================================================== **\
	Dialog

| Description:
A library with the new syntax for working with dialogs outside of
OnDialogResponse. Besides, the library have functions that can be help in
a script.

| Version:
3.0.0

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

forward OnDialogPerformed(playerid, const dialog_name[], response,
	bool:success
);

// -----------------------------------------------------------------------------
// Constant
// -----------------------------------------------------------------------------

#define TDW_MAX_FUNCTION_NAME (31)
#define TDW_DIALOG_PLAYER_DIALOG_HIDER (0xffffffff)

#if !defined TDW_DIALOG_ID
	#define TDW_DIALOG_ID (32767)
#else
	#assert (0 <= TDW_DIALOG_ID <= 32767)
#endif

// -----------------------------------------------------------------------------
// Template
// -----------------------------------------------------------------------------

#define u@$ 0]);
#define __:%0$ u@(u[_:%0 u@$

#define dialog%1(%2) dlg@r(%1,%2)

#define dlg@r(%1,%2) dlg@s:%1(%2)
#define dlg@s: __:dlg@s1:dlg@s2:$
// with automatons
#define dlg@s1:dlg@s2:%8$%1[%3]%0(%2) %8$public dR@%1(playerid,response,listitem,inputtext[])<dialogState:%3>{return dC@%1_%3(dlg@tN:%2[]$);}stock dC@%1_%3(%2)
// without automatons
#define dlg@s2:%8$%1(%2) %8$forward dR@%1(playerid,response,listitem,inputtext[]);public dR@%1(playerid,response,listitem,inputtext[])<>return 0;public dR@%1(playerid,response,listitem,inputtext[])<dialogState:default_state>{return dC@%1(dlg@tN:%2[]$);}stock dC@%1(%2)
// delete spaces.
#define dR@%8\32%1(%2) dR@%8%1(%2)
#define dC@%8\32%1(%2) dC@%8%1(%2)
// delete [] from arguments.
#define dlg@tN:%2[%3]%0$) %2)

#define dtempl%1(%2) forward dT@%1(%2);public dT@%1(%2)
#define dT@%0\32;%1(%2) dT@%0%1(%2)

#define dfunc: #

// -----------------------------------------------------------------------------
// Vars
// -----------------------------------------------------------------------------

static stock
	TDW_g_sDialog@CallbackName[MAX_PLAYERS][TDW_MAX_FUNCTION_NAME char],
	bool:TDW_g_sDialog@IsAlreadyOpen[MAX_PLAYERS char],
	TDW_g_sSpecifier@ddds[] = !"ddds",
	TDW_g_sSpecifier@isii[] = !"isii",
	TDW_g_sCallbacks,
	TDW_g_sOnDialogPerformed[] = "OnDialogPerformed",
	TDW_g_sSpace[] = !" ",
	TDW_g_sPrefix@dR@= !"dR@";

// -----------------------------------------------------------------------------
// Hooks
// -----------------------------------------------------------------------------

// OnDialogResponse hook
public
	OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	static
		callback_name[TDW_MAX_FUNCTION_NAME],
		ret;
	if (dialogid == TDW_DIALOG_ID) {
		if (TDW_g_sDialog@IsAlreadyOpen{playerid}) {
			if (inputtext[0] == '\0') {
				// Call the function without inputtext.
				ret = CallLocalFunction(TDW_g_sDialog@CallbackName[playerid],
					TDW_g_sSpecifier@ddds, playerid, response, listitem, !"\1"
				);
			} else {
				#if defined TDW_DIALOG_PROTECT_INPUT
					new inputtext_len = strlen(inputtext);
					do {
						if (inputtext[inputtext_len] == '%')
							inputtext[inputtext_len] = '#';
					} while (--inputtext_len != 0);
				#endif
				// Call the function with inputtext.
				ret = CallLocalFunction(TDW_g_sDialog@CallbackName[playerid],
					TDW_g_sSpecifier@ddds, playerid, response, listitem,
					inputtext
				);
			}
			if (TDW_g_sCallbacks & 1) {
				strunpack(callback_name, TDW_g_sDialog@CallbackName[playerid]);
				CallLocalFunction(TDW_g_sOnDialogPerformed,
					TDW_g_sSpecifier@isii, playerid, callback_name, response,
					ret
				);
			}
			TDW_g_sDialog@IsAlreadyOpen{playerid} = false;
		} else {
			if (TDW_g_sCallbacks & 1) {
				strunpack(callback_name, TDW_g_sDialog@CallbackName[playerid]);
				CallLocalFunction(TDW_g_sOnDialogPerformed,
					TDW_g_sSpecifier@isii, playerid, callback_name, response,
					TDW_DIALOG_PLAYER_DIALOG_HIDER
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
		1;
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
		1;
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
		state dialogState:default_state;
		if (funcidx(TDW_g_sOnDialogPerformed) != -1)
			TDW_g_sCallbacks |= 1;
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
		state dialogState:default_state;
		if (funcidx(TDW_g_sOnDialogPerformed) != -1)
			TDW_g_sCallbacks |= 1;
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

// -----------------------------------------------------------------------------
// Workaround
// -----------------------------------------------------------------------------

@_TDW_DLLIB__DIALOG_@();
@_TDW_DLLIB__DIALOG_@()
{
	#emit retn
	ShowPlayerDialog(0, 0, 0, TDW_g_sSpace, TDW_g_sSpace, TDW_g_sSpace,
		TDW_g_sSpace
	);
}

static stock TDW_DLLIB__AUTOMANTON()<>{return 0;}
static stock TDW_DLLIB__AUTOMANTON()<dialogState:default_state>{return 0;}

// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------

stock
	Dialog_Open(playerid, const function_response[], style, const caption[],
		const info[], const button1[], const button2[] = !"\0")
{
	TDW_g_sDialog@CallbackName[playerid] = TDW_g_sPrefix@dR@;
	strcat(TDW_g_sDialog@CallbackName[playerid], function_response);
	TDW_g_sDialog@IsAlreadyOpen{playerid} = true;
	// All of string arguments in ShowPlayerDialog should be defined with
	// a 'const' qualifier.
	const TDW_DIALOG_ID_ND = TDW_DIALOG_ID;
	#emit push.s button2
	#emit push.s button1
	#emit push.s info
	#emit push.s caption
	#emit push.s style
	#emit push.c TDW_DIALOG_ID_ND
	#emit push.s playerid
	#emit push.c 28
	#emit sysreq.c ShowPlayerDialog
	#emit stack 32
	#emit retn
	return 0; // Make compile happy!
}

stock
	Dialog_Close(playerid)
{
	TDW_g_sDialog@CallbackName[playerid]{0} = '\0';
	TDW_g_sDialog@IsAlreadyOpen{playerid} = false;
	return ShowPlayerDialog(playerid, -1, DIALOG_STYLE_MSGBOX, TDW_g_sSpace,
		TDW_g_sSpace, TDW_g_sSpace, TDW_g_sSpace
	);
}

stock
	Dialog_Show(playerid, const template_name[])
{
	static function_name[TDW_MAX_FUNCTION_NAME char];
	function_name = !"dT@";
	strcat(function_name, template_name);
	return CallLocalFunction(function_name, !"d", playerid);
}

stock
	IsPlayerInDialog(playerid)
{
	return TDW_g_sDialog@IsAlreadyOpen{playerid};
}

stock
	Dialog_MessageBox(playerid, const caption[], const info[], const button1[],
		const button2[] = !"\0")
{
	static const __message_box_name[] = !"Message";
	return Dialog_Open(playerid, __message_box_name, DIALOG_STYLE_MSGBOX,
		caption, info, button1, button2
	);
}
