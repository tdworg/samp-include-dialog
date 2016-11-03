/** ======================================================================== **\
| Version:
0.0.1

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


static stock
	bool:TDW_g_sIsOpened@Dialog[MAX_PLAYERS char],
	TDW_g_sAddresses@Dialog[MAX_PLAYERS];


#define dialog:%1(%2) forward %1@_wD(%2);public %1@_wD(%2)
#define Dialog_Open%0(%1,%2,%3,%4,%5,%6) _Dialog_Open(%1,#%2@_wD,%3,%4,%5,%6)

stock
	_Dialog_Open(playerid, const function[], style, caption[], info[],
		button1[], button2[] = !"")
{
	new func_idx = funcidx(function);

	// If the function doesn't exist.
	if (func_idx == -1)
		return 0;

	new addr;

	// Get the offset to dat, and invert it to get the offset to start of prefix
	#emit lctrl 1
	#emit neg
	// Public function list comes 32 bytes after the start of prefix. Store
	// the address to the public function list
	#emit add.c 32
	#emit stor.s.pri addr
	#emit lref.s.alt addr
	// Get the pointer to the function at the given index
	#emit lctrl 1
	#emit neg
	#emit add
	#emit load.s.alt func_idx // Load the index to alt
	#emit shl.c.alt 3
	#emit add
	#emit stor.s.pri addr
	#emit lref.s.pri addr // Obtain the address
	#emit stor.s.pri addr

	#emit push.pri

	// Get the cell address of the array
	#emit const.alt TDW_g_sAddresses@Dialog
	#emit load.s.pri playerid
	#emit idxaddr

	#emit pop.alt

	#emit xchg // exchange the values of two registers
	#emit stor.i // write the value to the array

	TDW_g_sIsOpened@Dialog{playerid} = true;

	ShowPlayerDialog(playerid, TDW_DIALOG_REGISTER_ID, style, caption, info,
		button1, button2
	);
	return 1;
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



#if defined TDWDH@OnDialogResponse
forward TDWDH@OnDialogResponse(playerid, dialogid, response, listitem,
	inputtext[]);
#endif

public
	OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if (dialogid == TDW_DIALOG_REGISTER_ID)
	{
		if (TDW_g_sIsOpened@Dialog{playerid} == false)
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
		0; // Return 0;
	#endif
}

#if defined _ALS_OnDialogResponse
	#undef OnDialogResponse
#else
	#define _ALS_OnDialogResponse
#endif

#define OnDialogResponse TDWDH@OnDialogResponse
