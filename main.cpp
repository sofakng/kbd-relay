#include <stdio.h>
#include <string.h>

struct Client
{
	bool connected;
	char buffer[64];
	int bufferPosition;
};

Client clients[8];

bool hasCommand(char* cmd);
bool ProcessCmd(char* cmd);
bool ProcessLocalCmd(char* device, char* action);
bool ProcessKeyboardCommand(int modifiers, int key);

int main()
{
	// Cmd: USB1_KBD_A

	// USB KBD/MOUSE
	//  0 = local
	//  1 = i2c device #1
	//  2 = i2c device #2
	// Bluetooth KBD/MOUSE
	//  1 = (default)
	
	// ALT+F4?
	// Modifiers_KEY               _KEYBD
	strcpy(clients[0].buffer, "USB1_KEYBD_LCTRL+LALT_F4\r");

	ProcessCmd(strupr(clients[0].buffer));


	getchar();

	return 0;
}

bool ProcessCmd(char* cmd)
{
	// valid targets:  USB#, BTH#
	char* target = strtok(cmd, "_");

	if (!target)
		return false;

	// valid devices, KEYBD, MOUSE
	char* device = strtok(NULL, "_");

	if (!device)
		return false;

	char* action = strtok(NULL, "\r");

	if (!action)
		return false;

	printf("Target: [%s], device: [%s], action: [%s]", target, device, action);

	// or strcasecmp
	if (stricmp(target, "USB1") == 0)
		ProcessLocalCmd(device, action);

	return false;
}

/*

Binary Input	Function
0					Disconnect if connected from the host.
0x1 -	 0xF		Converted to special keys like home, page up, backspace, etc.
0x10 - 0x7E		Translation mode: printable ASCII characters.
0x7F				Toggle virtual keyboard on iPhone.
0x80 - 0xDF		Interprets input as actual scan code.
0xE0 - 0xE7		Sends modifier keys Left Shift, Left Alt, Right Shift, etc.
0xE8 - 0xEF		Interprets input as actual scan code.
0xF0 - 0xFC		Reserved for custom reports.
0xFD				Raw mode: input is RAW report.
0xFE				Interpretive mode: input is shorthand report.
0xFF				Sends output report to UART.
*/

bool ProcessLocalCmd(char* device, char* action)
{
	if (stricmp(device, "KEYBD") == 0)
	{
		// valid modifiers: RGUI, RALT, RSHIFT, RCTRL, 
		//                  LGUI, LALT, LSHIFT, LCTRL
		char* modifiers = strtok(action, "_");
		
		if (!modifiers)
			return false;

		char* keyCode = strtok(NULL, "\r");

		if (!keyCode)
			return false;

		int mod = 0;

		if (strstr(modifiers, "RGUI"))
			mod = mod | 0x80;
		if (strstr(modifiers, "RALT"))
			mod = mod | 0x40;
		if (strstr(modifiers, "RSHIFT"))
			mod = mod | 0x20;
		if (strstr(modifiers, "RCTRL"))
			mod = mod | 0x10;
		if (strstr(modifiers, "LGUI"))
			mod = mod | 0x8;
		if (strstr(modifiers, "LALT"))
			mod = mod | 0x4;
		if (strstr(modifiers, "LSHIFT"))
			mod = mod | 0x2;
		if (strstr(modifiers, "LCTRL"))
			mod = mod | 0x1;

		return ProcessKeyboardCommand(mod, (int)*action);
	}


	return true;
}

bool ProcessKeyboardCommand(int modifiers, int key)
{
	// check for printable ASCII character
	//  32 = 0x20
	// 126 = 0x7E

	if (key >= 32 && key <= 126)
	{

	}

	return false;
}