#include <a_samp>

#include <dialog/tdw_dialog>

dialog [] abc(playerid)
{
	printf("Called");
}

DialogResponse: []name(playerid)
{}

public OnFilterScriptInit()
{}

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
