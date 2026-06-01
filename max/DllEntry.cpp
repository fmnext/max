//**************************************************************************/
// Copyright (c) 1998-2020 Autodesk, Inc.
// All rights reserved.
// 
// Use of this software is subject to the terms of the Autodesk license 
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.
//**************************************************************************/
// DESCRIPTION: Plugin Wizard generated plugin
// AUTHOR: 
//***************************************************************************/

#include "fmassettranslator.h"
#include "fmbundletranslator.h"

#include <filesystem>
#include <QMessageBox>

extern ClassDesc2* GetFMAssetTranslatorDesc();
extern ClassDesc2* GetFMBundleTranslatorDesc();

HINSTANCE hInstance;
int controlsInit = FALSE;

// This function is called by Windows when the DLL is loaded.  This 
// function may also be called many times during time critical operations
// like rendering.  Therefore developers need to be careful what they
// do inside this function.  In the code below, note how after the DLL is
// loaded the first time only a few statements are executed.

BOOL WINAPI DllMain(HINSTANCE hinstDLL,ULONG fdwReason,LPVOID /*lpvReserved*/)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		MaxSDK::Util::UseLanguagePackLocale();
		// Hang on to this DLL's instance handle.
		hInstance = hinstDLL;
		DisableThreadLibraryCalls(hInstance);
		// DO NOT do any initialization here. Use LibInitialize() instead.
	}
	return(TRUE);
}

// This function returns a string that describes the DLL and where the user
// could purchase the DLL if they don't have it.
__declspec( dllexport ) const TCHAR* LibDescription()
{
	return GetString(IDS_LIBDESCRIPTION);
}

// This function returns the number of plug-in classes this DLL
//TODO: Must change this number when adding a new class
__declspec( dllexport ) int LibNumberClasses()
{
	return 2;
}

// This function returns the number of plug-in classes this DLL
__declspec( dllexport ) ClassDesc* LibClassDesc(int i)
{
	switch(i)
	{
		case 0: return GetFMAssetTranslatorDesc();
		case 1: return GetFMBundleTranslatorDesc();
		default: return 0;
	}
}

// This function returns a pre-defined constant indicating the version of 
// the system under which it was compiled.  It is used to allow the system
// to catch obsolete DLLs.
__declspec( dllexport ) ULONG LibVersion()
{
	return VERSION_3DSMAX;
}

// This function is called once, right after your plugin has been loaded by 3ds Max. 
// Perform one-time plugin initialization in this method.
// Return TRUE if you deem your plugin successfully loaded, or FALSE otherwise. If 
// the function returns FALSE, the system will NOT load the plugin, it will then call FreeLibrary
// on your DLL, and send you a message.
__declspec( dllexport ) int LibInitialize(void)
{
	#pragma message(TODO("Perform initialization here."))
	/*
	char FORZA_PLUGIN_BIN_PATH[MAX_PATH];
	if (GetEnvironmentVariableA("FORZATECH_MAX_BIN", FORZA_PLUGIN_BIN_PATH, MAX_PATH))
	{
		SetDllDirectoryA(FORZA_PLUGIN_BIN_PATH);

		return TRUE;
	}

	QMessageBox msgBox(reinterpret_cast<QWidget*>(GetCOREInterface()->GetQmaxMainWindow()));
	msgBox.setWindowTitle("ForzaTech: Plugin Failure");
	msgBox.setText("The environment variable FORZATECH_MAX_BIN was not found on your system.\nPlugin will not initialize.");
	msgBox.setIcon(QMessageBox::Critical);
	msgBox.exec();

	return FALSE;
	*/
	return TRUE;
}

// This function is called once, just before the plugin is unloaded. 
// Perform one-time plugin un-initialization in this method."
// The system doesn't pay attention to a return value.
__declspec( dllexport ) int LibShutdown(void)
{
	#pragma message(TODO("Perform un-initialization here."))
	return TRUE;
}

TCHAR* GetString(int id)
{
	static TCHAR buf[256];

	if (hInstance)
	{
		return LoadString(hInstance, id, buf, _countof(buf)) ? buf : NULL;
	}

	return NULL;
}

