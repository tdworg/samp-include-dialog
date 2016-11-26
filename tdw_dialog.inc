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

or use that (if example == true / false):

dialog SomethingDialog <true> (playerid, listitem, inputtext[])
{
	// ...
}

And now you have to use a special functions to show a dialog to the player.

| Version:
2.0.0

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

static stock
	TDW_g_sDialog@FuncName[MAX_PLAYERS][TDW_MAX_FUNCTION_NAME char],
	bool:TDW_g_sDialog@IsOpen[MAX_PLAYERS char],
	TDW_g_sSpecifier@ddds[] = !"ddds";


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
	static __space[] = !" ";

	TDW_g_sDialog@FuncName[playerid]{0} = '\0';
	TDW_g_sDialog@IsOpen{playerid} = false;

	return
		ShowPlayerDialog(playerid, -1, DIALOG_STYLE_MSGBOX, __space, __space,
			__space, __space
		);
}


stock
	Dialog_Show(playerid, const template_name[])
{
	new function_name[TDW_MAX_FUNCTION_NAME char] = !"pdT_";
	strcat(function_name, template_name);

	return CallLocalFunction(function_name, !"d", playerid);
}


stock
	IsPlayerInDialog(playerid)
{
	return _:TDW_g_sDialog@IsOpen{playerid};
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
		if (_:TDW_g_sDialog@IsOpen{playerid})
		{
			if (inputtext[0] == '\0')
			{
				CallLocalFunction(TDW_g_sDialog@FuncName[playerid],
					TDW_g_sSpecifier@ddds, playerid, response, listitem, !"\1"
				);
			}
			else
			{
				CallLocalFunction(TDW_g_sDialog@FuncName[playerid],
					TDW_g_sSpecifier@ddds, playerid, response, listitem,
					inputtext
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

