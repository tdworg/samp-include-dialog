# TDW Dialog

The library provides a new syntax for handling dialog responses.

## Installation
1. Download library.
2. All files move into folder `pawno/includes`. But, you can use the flag `-i<path>` to specify an alternative path.
3. Include it:
```PAWN
#include <a_samp> // standard SAMP library
#include <tdw_dialog>
```

### Example:
```pawn
#include <a_samp>
#include <tdw_dialog>

dialog register(playerid, response, listitem, inputtext[])
{
	if (!response)
		return;
	SendClientMessage(playerid, -1, "Registered");
}

public OnPlayerConnect(playerid)
{
	OpenDialog(playerid, "register", DIALOG_STYLE_MSGBOX,
		"Register",
		"Hello!",
		"Register", "Cancel"
	);

	return 1;
}
```
