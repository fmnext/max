#pragma once
#include "Max.h"
#include "maxstring.h"
#include <maxscript/maxscript.h>

//**************************************************************************/
// Copyright (c) 2024 Autodesk, Inc.
// All rights reserved.
// 
//  Use of this software is subject to the terms of the Autodesk license 
//  agreement provided at the time of installation or download, or which 
//  otherwise accompanies this software in either electronic or hard copy form.
//**************************************************************************/
// DESCRIPTION: Includes for Plugins
// AUTHOR: Apex
//***************************************************************************/

namespace fmnext
{
	std::string convertWideToStdString(const std::wstring& str);

	std::wstring convertStdStringToWide(const std::string& str);

	void PrinttoListener(const std::string& str);

	void PrinttoListener(const std::wstring& str);
}