# TDW Dialogs

### About the library
Dialogs!

### Installation
1. Download library.
2. All files move into folder `pawno/includes`. But, you can use the flag `-i<path>` to specify an alternative path.
3. Include it:
```PAWN
#include <a_samp> // standart SAMP library
#include <tdw_dialog>
```

Example:
```pawn
dialog PlayerReg<true>(playerid, inputtext[])
{
	printf("Your text: %s", inputtext[]);
}

dtempl PlayerReg(playerid)
{
	Dialog_Open(playerid, dfucn:PlayerReg, DIALOG_STYLE_MSGBOX, "Cap",
		"Info", "Button"
	);
}

public
	OnPlayerConnect(playerid)
{
	Dialog_Show(playerid, "PlayerReg");
}
```
