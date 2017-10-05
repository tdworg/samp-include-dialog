#include <a_samp>
#define TDW_DIALOG_SKIP_ARGS
#include <dialog\tdw_dialog>

dialog [hook] register(playerid)
{

}

dialog [template] register(playerid)
{
	OpenDialog(playerid, dfunc:register, DIALOG_STYLE_MSGBOX,
		"Register",
		"Hello!",
		"Register", "Cancel"
	);
}

public OnPlayerConnect(playerid)
{
	ShowDialog(playerid, dfunc:register);
	return 1;
}
