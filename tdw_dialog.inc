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

And now you have to use a special functions to show a dialog to the player.

| Version:
1.2.0

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


#if !defined TDW_DIALOG_ID
	// This dialog id needs to retrieve data from OnDialogResponse (playerid,
	// response, listitem, inputtext), and then to send the data to a callback.
	//
	// Note:
	// 32700 is the limit of the dialogs in SA:MP.
	#define TDW_DIALOG_ID (32700)
#else
	#assert (-1 < TDW_DIALOG_ID <= 32700)
#endif

#if !defined TDW_MAX_FUNCTION_NAME
	// The max length of a function name is 31 (without a zero terminator).
	#define TDW_MAX_FUNCTION_NAME (31)
#endif

#if !defined TDW_DIALOG_INVALID_ADDRESS
	#define TDW_DIALOG_INVALID_ADDRESS (0xffffffff)
#endif

static stock
	TDW_g_sAddresses@Dialog[MAX_PLAYERS];


#define dialog:%1(%2) forward _wD@%1(%2);public _wD@%1(%2)


stock
	Dialog_Open(playerid, const function_name[], style, caption[], info[],
		button1[], button2[] = !"")
{
	new address = GetDialogPublicAddressFromName(function_name);

	if (address == -1)
		return 0;

	TDW_g_sAddresses@Dialog[playerid] = address;

	return ShowPlayerDialog(playerid, TDW_DIALOG_ID, style, caption, info,
		button1, button2
	);
}


stock
	Dialog_CloseForPlayer(playerid)
{
	static space[] = !" ";

	TDW_g_sAddresses@Dialog[playerid] = TDW_DIALOG_INVALID_ADDRESS;

	return ShowPlayerDialog(playerid, -1, DIALOG_STYLE_MSGBOX, space, space,
		space, space
	);
}


stock
	Dialog_CloseForAll(const function_name[])
{
	new address = GetDialogPublicAddressFromName(function_name);

	if (address == -1)
		return 0;

	new list_index;

	for (new i = _TDW_FOREACH_MAX_PLAYERS_; --i != -1; )
	{
		if (TDW_g_sAddresses@Dialog[i] != address)
			continue;

		TDW_g_sAddresses@Dialog[playerid] = TDW_DIALOG_INVALID_ADDRESS;

		ShowPlayerDialog(playerid, -1, DIALOG_STYLE_MSGBOX, space, space,
			space, space
		);
	}
}


stock
	Dialog_GetPlayersUsingDialog(const dialog_name[], list[],
		size = sizeof list)
{
	new address = GetDialogPublicAddressFromName(function_name);

	if (address == -1)
		return 0;

	new list_index;

	for (new i = _TDW_FOREACH_MAX_PLAYERS_; --i != -1; )
	{
		if (TDW_g_sAddresses@Dialog[i] != address)
			continue;
		if (list_index >= size)
			break;
		list[list_index++] = i;
	}

	if (list_index != 0)
		list[list_index] = INVALID_PLAYER_ID;

	return 1;
}





#if defined TDWDH@OnDialogResponse
forward TDWDH@OnDialogResponse(playerid, dialogid, response, listitem,
	inputtext[]);
#endif

public
	OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if (dialogid == TDW_DIALOG_ID)
	{
		if (TDW_g_sAddresses@Dialog[playerid] == TDW_DIALOG_INVALID_ADDRESS)
			return 0;

		if (inputtext[0] == '\0')
		{
			// Push '\0'
			#emit push.c 0
		}
		else
		{
			// Push the string
			#emit push.s inputtext
		}

		#emit push.s listitem
		#emit push.s response
		#emit push.s playerid
		#emit push.c 16

		#emit lctrl 6
		#emit add.c 40
		#emit push.pri
		// Call the function
		#emit const.alt TDW_g_sAddresses@Dialog
		#emit load.s.pri playerid
		#emit lidx
		#emit sctrl 6
	}

	return
	#if defined TDWDH@OnDialogResponse
		TDWDH@OnDialogResponse(playerid, dialogid, response, listitem,
			inputtext
		);
	#else
		1; // Return 1;
	#endif
}

#if defined _ALS_OnDialogResponse
	#undef OnDialogResponse
#else
	#define _ALS_OnDialogResponse
#endif

#define OnDialogResponse TDWDH@OnDialogResponse


#if defined TDWDH@OnPlayerConnect
forward TDWDH@OnPlayerConnect(playerid);
#endif

public
	OnPlayerConnect(playerid)
{
	TDW_g_sAddresses@Dialog[playerid] = TDW_DIALOG_INVALID_ADDRESS;
	return
	#if defined TDWDH@OnPlayerConnect
		TDWDH@OnPlayerConnect(playerid);
	#else
		1; // Return 1;
	#endif
}

#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif

#define OnPlayerConnect TDWDH@OnPlayerConnect



static stock
	GetDialogPublicAddressFromName(const function_name[])
{
	static fn_name[TDW_MAX_FUNCTION_NAME + 1 char];
	fn_name =  !"_wD@";

	strcat(fn_name, function_name, sizeof fn_name);

	new function_index = funcidx(fn_name);

	if (function_index == -1)
	{
		return -1;
	}
	{}

	new addr;

	#emit lctrl 1
	#emit neg

	#emit add.c 32
	#emit stor.s.pri addr
	#emit lref.s.alt addr

	#emit lctrl 1
	#emit neg
	#emit add
	#emit load.s.alt function_index
	#emit shl.c.alt 3
	#emit add
	#emit stor.s.pri addr
	#emit lref.s.pri addr

	#emit stack 0x8
	#emit retn

	return 0;
}
