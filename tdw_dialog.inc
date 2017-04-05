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

#if defined _inc_tdw_dialog
	#undef _inc_tdw_dialog
#endif

#if defined _TDW_LIB_DIALOG
	#endinput
#endif

#define _TDW_LIB_DIALOG

#include "amx/amx_header"
#include "amx/amx_memory"
#include "amx/dynamic_call"

#if !defined _TDW_DIALOG_ID
	#define _TDW_DIALOG_ID (32767)
#else
	#assert (0 <= TDW_DIALOG_ID <= 32767)
#endif

#if !defined _TDW_DIALOG_SHOULD_RETURN
	#define _TDW_DIALOG_SHOULD_RETURN
#endif

#define TDW_DIALOG_MAX_FUNC_NAME (32)
#define TDW_DIALOG_PLAYER_USE_HIDER (-1)
#define TDW_DIALOG_MAX_TAG_NAME (4)

#define u@$ 0]);
#define __:%0$ u@(u[_:%0 u@$

#define ddialog_PUBLIC(%1) forward %1;public %1
#define ddialog_FLAG(%1) public E_DIALOG_FLAG:df_@%1=E_DIALOG_FLAG:0
#define ddialog_CALL_STATE(%1,%2) { state ds@st:%2;%1; }
#define ddialog_BRK:%3[%9]%0) _:ddialog_BRK:%3%0)
#define ddialog_V2_CALL(%1,%2,%3) ddialog_CALL_STATE(di@%1(_:ddialog_BRK:%3),%2)
#define ddialog_IS_FLAG(%1,%2) ((df_@%1) & (EI_DIALOG_%2))
#define ddialog_DECLARE_STATE(%1) _dialog_dummy()<%1>

#define ddialog_DEFAULT_ARGS playerid,response,listitem,inputtext[]

#define dialog%1(%2) dialog_@r:%1(%2)
#define dialog_@r: __:dialog_@v1:dialog_@v2:$

#define dialog_sg:dialogf@%0(%2\32; dialog_sg:dialogf@%0(%2

#define dialog_@v1:dialog_@v2:%8$%1<%2>%0(%3) dialogf@c(%2)|||%1(%3)
#define dialogf@c(%2) dialog_sg:dialogf@c_g(%2,0)(%2)

#define dialogf@c_g(%2,%9)(%6) @Ct:dialogf@cc(%2,t,%9)(%6)
#define dialogf@cc(%2,%7,%9)(%6) dialogfh@%7(%2,%9)(%6)

#define dialogf@cl:%0(%2)|||%1(%3) u@$ddialog_PUBLIC(di%0@%1(%3)){}__:dialogg:$stock di@%1(%3)<ds@st:__%2>

#define dialogg:%8$%0(%1)<ds@st:__%9\32; dialogg:%8$%0(%1)<ds@st:__%9

#define @Ct:dialogf@cc(t%2,%7,%9)(%6) dialogf@cl:T(%6)
#define @CT:dialogf@cc(T%2,%7,%9)(%6) dialogf@cl:T(%6)
#define @Cf:dialogf@cc(f%2,%7,%9)(%6) dialogf@cl:F(%6)
#define @CF:dialogf@cc(F%2,%7,%9)(%6) dialogf@cl:F(%6)

#define dialogfh@t(%2,%9)(%6) @CT:dialogf@cc(%2,T,%9)(%6)
#define dialogfh@T(%2,%9)(%6) @Cf:dialogf@cc(%2,f,%9)(%6)
#define dialogfh@f(%2,%9)(%6) @CF:dialogf@cc(%2,F,%9)(%6)
#define dialogfh@F(%2,%9)(%6) __end:

#define __end:%0|||%1(%2) u@$forward public __dialog__warning();public __dialog__warning()for(new __YOU_SHOULD_USE_ONLY_BOOLEAN__;;)

#define dialog_@v2:%8$%1(%3) %8$ddialog_FLAG(%1);ddialog_PUBLIC(du_@%1(ddialog_DEFAULT_ARGS)){ \
	ddialog_V2_CALL(%1,__default,%3)                                                           \
	if (ddialog_IS_FLAG(%1,SWITCH)) {                                                          \
		if (response == 0 && ddialog_IS_FLAG(%1,FALSE_IMPL))                                   \
			ddialog_V2_CALL(%1,__false,%3)                                                     \
		else if (ddialog_IS_FLAG(%1,TRUE_IMPL))                                                \
			ddialog_V2_CALL(%1,__true,%3)                                                      \
	}}                                                                                         \
	forward public di@%1(%3);stock di@%1(%3)<>{_TDW_DIALOG_SHOULD_RETURN}stock di@%1(%3)<ds@st:__default>

#define di@%1\32; di@%1
#define diT@%1\32; diT@%1
#define diF@%1\32; diF@%1
#define df_@%1\32; df_@%1
#define du_@%1\32; du_@%1

#define d%0<%1> #%1

enum E_DIALOG_FLAG (<<= 1)
{
	EI_DIALOG_SWITCH,     // if the dialog function has case-functions:
	EI_DIALOG_TRUE_IMPL,  // - true-case
	EI_DIALOG_FALSE_IMPL  // - false-case
};

enum E_DIALOG_CALLBACKS (<<= 1)
{
	EI_DIALOG_CB_OnDialogPerformed = 1
};

forward OnDialogPerformed(playerid, const dialog_name[], response, bool:success);

forward public _dialog_dummy();

ddialog_DECLARE_STATE(             ){}
ddialog_DECLARE_STATE(ds@st:__true ){}
ddialog_DECLARE_STATE(ds@st:__false){}

static stock
	TDW_g_sAmxHdr[AMX_HDR],
	bool:TDW_g_sDialogOpened[MAX_PLAYERS char],
	TDW_g_sDialogName[MAX_PLAYERS][TDW_DIALOG_MAX_FUNC_NAME char],
	E_DIALOG_CALLBACKS:TDW_g_sCallbacks,
	TDW_g_sSpace[] = " ";

static stock const
	TDW_g_sSpecifier@ddds[] = !"ddds",
	TDW_g_sOnDialogPerformed[] = !"OnDialogPerformed",
	TDW_g_sSwitcherTag[] = !"du_@";

static stock Dialog_Init()
{
	GetAmxHeader(TDW_g_sAmxHdr);

	new
		num_publics = GetNumPublics(TDW_g_sAmxHdr),
		name[TDW_DIALOG_MAX_FUNC_NAME],
		flag_addr,
		E_DIALOG_FLAG:flag_value;

	for (new i = 0; i < num_publics; ++i)
	{
		if (GetPublicNameFromIndex(i, name) && strcmp(name, TDW_g_sSwitcherTag, _, TDW_DIALOG_MAX_TAG_NAME) == 0)
		{
			// Get the flag address
			name[1] = 'f';
			flag_addr = GetPubVarAddressFromName(name);
			if (flag_addr == -1)
				continue;

			flag_value = E_DIALOG_FLAG:ReadAmxMemory(flag_addr);

			name[1] = 'i';

			name[2] = 'T';  // true
			if (GetPublicIndexFromName(name) != -1)
			{
				flag_value |= EI_DIALOG_TRUE_IMPL | EI_DIALOG_SWITCH;
			}

			name[2] = 'F';  // false
			if (GetPublicIndexFromName(name) != -1)
			{
				flag_value |= EI_DIALOG_TRUE_IMPL | EI_DIALOG_SWITCH;
			}

			WriteAmxMemory(flag_addr, _:flag_value);
		}
	}
}

// OnFilterScriptInit/OnGameModeInit hook
#if defined FILTERSCRIPT
	public OnFilterScriptInit()
	{
		if (funcidx(TDW_g_sOnDialogPerformed) != -1)
			TDW_g_sCallbacks |= EI_DIALOG_CB_OnDialogPerformed;

		Dialog_Init();

	#if defined TDW_DLG@OnFilterScriptInit
		return TDW_DLG@OnFilterScriptInit();
	#else
		return 1;
	#endif
	}

	#if defined _ALS_OnFilterScriptInit
		#undef OnFilterScriptInit
	#else
		#define _ALS_OnFilterScriptInit
	#endif

	#define OnFilterScriptInit TDW_DLG@OnFilterScriptInit

	#if defined TDW_DLG@OnFilterScriptInit
		forward TDW_DLG@OnFilterScriptInit();
	#endif
#else
	public OnGameModeInit()
	{
		if (funcidx(TDW_g_sOnDialogPerformed) != -1)
			TDW_g_sCallbacks |= EI_DIALOG_CB_OnDialogPerformed;

		Dialog_Init();

	#if defined TDW_DLG@OnGameModeInit
		return TDW_DLG@OnGameModeInit();
	#else
		return 1;
	#endif
	}

	#if defined _ALS_OnGameModeInit
		#undef OnGameModeInit
	#else
		#define _ALS_OnGameModeInit
	#endif

	#define OnGameModeInit TDW_DLG@OnGameModeInit

	#if defined TDW_DLG@OnGameModeInit
		forward TDW_DLG@OnGameModeInit();
	#endif
#endif

// OnDialogResponse hook
public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if (dialogid == _TDW_DIALOG_ID)
	{
		new retval;

		if (_:TDW_g_sDialogOpened{playerid} == 0)
		{
			retval = TDW_DIALOG_PLAYER_USE_HIDER;
		}
		else
		{
			TDW_g_sDialogOpened{playerid} = false;

			if (inputtext[0] == '\0')
			{
				retval = CallLocalFunction(TDW_g_sDialogName[playerid],
					TDW_g_sSpecifier@ddds, playerid, response, listitem, !"\1");
			}
			else
			{
				new len = strlen(inputtext);

				do
				{
					if (inputtext[len] == '#')
						inputtext[len] = '%';
				}
				while (--len != 0);

				retval = CallLocalFunction(TDW_g_sDialogName[playerid],
					TDW_g_sSpecifier@ddds, playerid, response, listitem, inputtext);
			}
		}

		if (TDW_g_sCallbacks & EI_DIALOG_CB_OnDialogPerformed)
		{
			static name[TDW_DIALOG_MAX_FUNC_NAME];

			strunpack(name, TDW_g_sDialogName[playerid]);
			CallLocalFunction(TDW_g_sOnDialogPerformed, !"isii", playerid, name, response, retval);
		}
	}

#if defined TDW_DLG@OnDialogResponse
	return TDW_DLG@OnDialogResponse(playerid, dialogid, response, listitem,
		inputtext);
#else
	return 1;
#endif
}

#if defined _ALS_OnDialogResponse
	#undef OnDialogResponse
#else
	#define _ALS_OnDialogResponse
#endif

#define OnDialogResponse TDW_DLG@OnDialogResponse

#if defined TDW_DLG@OnDialogResponse
	forward TDW_DLG@OnDialogResponse(playerid, dialogid, response, listitem,
		inputtext[]);
#endif

@__TDW_DIALOG_FIX_@();
@__TDW_DIALOG_FIX_@()
{
	#emit retn
	ShowPlayerDialog(0, 0, 0, TDW_g_sSpace, TDW_g_sSpace, TDW_g_sSpace, TDW_g_sSpace);
}

stock Dialog_Open(playerid, const func[], style, const caption[], const info[],
	const button1[], const button2[] = !"\0")
{
	TDW_g_sDialogName[playerid] = TDW_g_sSwitcherTag;
	strcat(TDW_g_sDialogName[playerid], func);
	TDW_g_sDialogOpened{playerid} = true;

	const TDW_DIALOG_ID_CONST = _TDW_DIALOG_ID;
	const PUSH_ARG_BYTES = 28;
	const POP_ARG_BYTES = PUSH_ARG_BYTES + 4;

	#emit push.s button2
	#emit push.s button1
	#emit push.s info
	#emit push.s caption
	#emit push.s style
	#emit push.c TDW_DIALOG_ID_CONST
	#emit push.s playerid
	#emit push.c PUSH_ARG_BYTES
	#emit sysreq.c ShowPlayerDialog
	#emit stack POP_ARG_BYTES
	#emit retn

	return 0;
}

stock Dialog_Close(playerid)
{
	TDW_g_sDialogName[playerid]{0} = '\0';
	TDW_g_sDialogOpened{playerid} = false;

	return ShowPlayerDialog(playerid, -1, DIALOG_STYLE_MSGBOX, TDW_g_sSpace,
		TDW_g_sSpace, TDW_g_sSpace, TDW_g_sSpace);
}

stock IsPlayerInDialog(playerid)
{
	return TDW_g_sDialogOpened{playerid};
}

stock Dialog_MessageBox(playerid, const caption[], const info[], const button1[], const button2[] = !"\0")
{
	static const message_box_name[] = !"MessageBox";

	return Dialog_Open(playerid, message_box_name, DIALOG_STYLE_MSGBOX, caption, info, button1, button2);
}
