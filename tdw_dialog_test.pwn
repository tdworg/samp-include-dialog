#include <a_samp>

#include <ysi\y_hooks>
#define TDW_DIALOG_DEFAULT_HOOK
#include <dialog/tdw_dialog>

dialog [nohook] abc(playerid, response, listitem, inputtext[])
{
	printf("Called");
}

DialogResponse: name(playerid, response, listitem, inputtext[])
{}

public OnFilterScriptInit()
{
	return 1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
	if (strcmp(cmdtext, !"/tdwdo", false, 5) == 0) {
		OpenDialog(playerid, "abc", DIALOG_STYLE_MSGBOX, "Test", "Test", "Ok", "Cancel");
		return 1;
	}
	if (strcmp(cmdtext, !"/tdwdc", false, 5) == 0) {
		CloseDialog(playerid);
		return 1;
	}
	return 0;
}


public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{

}
