/** ======================================================================== **\
	Dialog

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


#if defined _inc_tdw_dpage
	#undef _inc_tdw_dpage
#endif

#if defined _TDW_DLLIB__DPAGE
	#endinput
#endif

#define _TDW_DLLIB__DPAGE

#if !defined TDW_DIALOG_PAGE_ID
	#define TDW_DIALOG_PAGE_ID (32766)
#else
	#assert (0 <= TDW_DIALOG_ID <= 32767)
#endif

#define dpage%0[%3]%1(%2) forward dP@%1@%3(%2);public dP@%1@%3(%2)
#define dP@%8\32;%1(%2) dP@%8%1(%2)

const TDW_INVALID_DIALOG_PAGE = -1;
const TDW_MAX_FUNC_NAME = 32;

static stock
	TDW_g_sPlayerInDPage[MAX_PLAYERS] = TDW_INVALID_DIALOG_PAGE,
	TDW_g_sPlayer@DialogName[MAX_PLAYERS][TDW_MAX_FUNC_NAME char];

stock DialogPage_Open(playerid, const dialog_name[])
{
	if (TDW_g_sPlayerInDPage[playerid] != TDW_INVALID_DIALOG_PAGE)
		return 0;

	TDW_g_sPlayerInDPage[playerid] = 0;

	// Store the name without tags in the array.
	TDW_g_sPlayer@DialogName[playerid][0] = EOS;
	strcat(TDW_g_sPlayer@DialogName[playerid], dialog_name, TDW_MAX_FUNC_NAME);

	// Generate a new name for the template.
	static func_name[TDW_MAX_FUNC_NAME char];
	func_name = !"dP@";
	strcat(func_name, dialog_name, TDW_MAX_FUNC_NAME);
	strcat(func_name, !"@0");

	// Call the function
	if (CallLocalFunction(func_name, !"d", playerid) == 0)
		DialogPage_Clear(playerid);

	return 1;
}

stock DialogPage_ShowDialog(playerid, const caption[], const info[],
	const button1[], const button2[])
{
	if (TDW_g_sPlayerInDPage[playerid] == TDW_INVALID_DIALOG_PAGE)
		return 0;

	const
		TDW_DIALOG_ID_ND = TDW_DIALOG_PAGE_ID,
		TDW_DIALOG_STYLE_ND = DIALOG_STYLE_MSGBOX;

	#emit PUSH.s button2
	#emit PUSH.s button1
	#emit PUSH.s info
	#emit PUSH.s caption
	#emit PUSH.c TDW_DIALOG_STYLE_ND
	#emit PUSH.c TDW_DIALOG_ID_ND
	#emit PUSH.s playerid
	#emit PUSH.c 28
	#emit SYSREQ.c ShowPlayerDialog
	#emit STACK 32
	#emit RETN
	return 0; // Make compile happy!
}

stock DialogPage_Clear(playerid)
{
	TDW_g_sPlayerInDPage[playerid] = TDW_INVALID_DIALOG_PAGE;
	TDW_g_sPlayer@DialogName[playerid]{0} = '\0';
}

stock DialogPage_Goto(playerid, index)
{
	if (TDW_g_sPlayerInDPage[playerid] == TDW_INVALID_DIALOG_PAGE)
		return 0;

	TDW_g_sPlayerInDPage[playerid] = index;

	static
		func_name[TDW_MAX_FUNC_NAME char],
		val_buffer[12 char];
	valstr(val_buffer, index, true);
	func_name = !"dP@";
	strcat(func_name, TDW_g_sPlayer@DialogName[playerid],
		TDW_MAX_FUNC_NAME);
	strcat(func_name, !"@");
	strcat(func_name, val_buffer);

	if (funcidx(func_name) != -1)
		if (CallLocalFunction(func_name, !"d", playerid) == 0)
			DialogPage_Clear(playerid);

	return 1;
}

stock DialogPage_Out(playerid)
{
	static TDW_g_sSpace[] = !" ";
	DialogPage_Clear(playerid);
	ShowPlayerDialog(playerid, -1, DIALOG_STYLE_MSGBOX, TDW_g_sSpace,
		TDW_g_sSpace, TDW_g_sSpace, TDW_g_sSpace);
	return 1;
}

// OnDialogResponse hook
public
	OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if (dialogid == TDW_DIALOG_PAGE_ID) {
		if (TDW_g_sPlayerInDPage[playerid] != TDW_INVALID_DIALOG_PAGE)
		{
			new ret;
			if (response)
				ret = ++TDW_g_sPlayerInDPage[playerid];
			else
				ret = --TDW_g_sPlayerInDPage[playerid];

			if (ret > 0) {
				static
					func_name[TDW_MAX_FUNC_NAME char],
					val_buffer[12 char];
				valstr(val_buffer, ret, true);
				func_name = !"dP@";
				strcat(func_name, TDW_g_sPlayer@DialogName[playerid],
					TDW_MAX_FUNC_NAME);
				strcat(func_name, !"@");
				strcat(func_name, val_buffer);

				if (funcidx(func_name) != -1)
					if (CallLocalFunction(func_name, !"d", playerid) == 0)
						DialogPage_Clear(playerid);
			}
		}
	}
	return
	#if defined TDWDHP@OnDialogResponse
		TDWDHP@OnDialogResponse(playerid, dialogid, response, listitem,
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

#define OnDialogResponse TDWDHP@OnDialogResponse

#if defined TDWDHP@OnDialogResponse
	forward TDWDHP@OnDialogResponse(playerid, dialogid, response, listitem,
		inputtext[]);
#endif
