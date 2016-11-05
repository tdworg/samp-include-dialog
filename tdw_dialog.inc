/** ======================================================================== **\
| Version:
1.0.0

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


#if !defined TDW_DIALOG_REGISTER_ID
	#define TDW_DIALOG_REGISTER_ID (32700)
#endif

#if !defined MAX_FUNCTION_NAME
	#define MAX_FUNCTION_NAME (32)
#endif

#if !defined _TDW_FOREACH_MAX_PLAYERS_
	#if defined GetPlayerPoolSize
		#define _TDW_FOREACH_MAX_PLAYERS_ (GetPlayerPoolSize() + 1)
	#else
		#define _TDW_FOREACH_MAX_PLAYERS_ (GetMaxPlayers())
	#endif
#endif



static stock
	bool:TDW_g_sIsOpened@Dialog[MAX_PLAYERS char],
	TDW_g_sAddresses@Dialog[MAX_PLAYERS];



#define dialog%1(%2) forward %1@_wD(%2);public %1@_wD(%2)

#define OpenDialog%0(%1,%2,%3,%4,%5,%6) _Dialog_Open(%1,_wD@S:%2$,%3,%4,%5,%6)

// Remove the spaces
#define _wD@S: _wD@S0:_wD@S1:
#define _wD@S0:_wD@S1:%0\32;%2$ _wD@S:%2$
#define _wD@S1:%2$ wD@C:%2$

// Add suffix
#define wD@C: wD@C0:wD@C1:
#define wD@C0:wD@C1:%0\34;%2\34;%3$ #%2@_wD
#define wD@C1:%2$ #%2@_wD

#define Dialog_GetPlayersUsingDialog%0(%1,%2)\
	_Dialog_GetPlayersUsingDialog(_wD@S:%1$,%2)

#define Dialog_CloseForAll%0(%1)\
	_Dialog_CloseForAll(_wD@S:%1$)



static stock
	GetPublicAddressFromName(const function_name[])
{
	new function_index = funcidx(function_name);

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



stock
	_Dialog_Open(playerid, const function_name[], style, caption[], info[],
		button1[], button2[] = !"")
{
	new address = GetPublicAddressFromName(function_name);

	if (address == -1)
		return 0;

	TDW_g_sAddresses@Dialog[playerid] = address;
	TDW_g_sIsOpened@Dialog{playerid} = true;

	return ShowPlayerDialog(playerid, TDW_DIALOG_REGISTER_ID, style, caption,
		info, button1, button2
	);
}


stock
	Dialog_Close(playerid)
{
	static space[] = !" ";

	TDW_g_sIsOpened@Dialog{playerid} = false;

	return ShowPlayerDialog(playerid, -1, DIALOG_STYLE_MSGBOX, space, space,
		space, space
	);
}


stock
	_Dialog_GetPlayersUsingDialog(const function_name[], player_list[],
		size = sizeof player_list)
{
	new address = GetPublicAddressFromName(function_name);

	if (address == -1)
		return 0;

	new list_index;

	for (new i = _TDW_FOREACH_MAX_PLAYERS_; --i != -1;)
	{
		if (TDW_g_sAddresses@Dialog[i] != address)
			continue;
		if (_:TDW_g_sIsOpened@Dialog{i} == 0)
			continue;
		if (list_index >= size)
			break;
		player_list[list_index++] = i;
	}

	if (list_index != 0)
		player_list[list_index] = INVALID_PLAYER_ID;

	return 1;
}


stock
	_Dialog_CloseForAll(const function_name[])
{
	new address = GetPublicAddressFromName(function_name);

	if (address == -1)
		return 0;

	new list_index;

	for (new i = _TDW_FOREACH_MAX_PLAYERS_; --i != -1;)
	{
		if (TDW_g_sAddresses@Dialog[i] != address)
			continue;
		if (_:TDW_g_sIsOpened@Dialog{i} == 0)
			continue;
		Dialog_Close(i);
	}

	return 1;
}



#if defined TDWDH@OnDialogResponse
forward TDWDH@OnDialogResponse(playerid, dialogid, response, listitem,
	inputtext[]);
#endif

public
	OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if (dialogid == TDW_DIALOG_REGISTER_ID)
	{
		if (_:TDW_g_sIsOpened@Dialog{playerid} == 0)
			return 0;
		else
		{
			Dialog_Close(playerid);
		}

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
	TDW_g_sIsOpened@Dialog{playerid} = false;
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



stock
	Dialog_Open(playerid, const function_name[], style, caption[], info[],
		button1[], button2[] = !"")
{
	static buffer[MAX_FUNCTION_NAME char];
	buffer[0] = '\0';
	strcat(buffer, function_name, sizeof buffer);
	strcat(buffer, !"@_wD", sizeof buffer);
	return _Dialog_Open(playerid, buffer, style, caption, info, button1,
		button2
	);
}
