/*****************************************************************************\
Copyright (C) 2013-2014 <martinjk at outloook dot com>

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.

3. This notice may not be removed or altered from any source distribution.
\*****************************************************************************/
#pragma once

#undef WINVER
#define WINVER 0x0502

#include <Windows.h>
#include <iostream>
#include <include/D3Overlay.h>
#include <include/Config.h>

// CONFIGURATION
#define PROGRAM						"H1Z1 Tool"
#define _ATTACK_ALERT				1
#define _DEBUG_ITEMS				0
#define _3D_ENTITY_DISPLAY			1
#define	_MINIMAP					0

#define _ATTACK_NEAR_PLAYER_ALERT	1
#define _IGNORE_PLAYERS \
if(scopeobj._name != "Testuser" \
&& scopeobj._name != "Testuser2" \
)

std::string GetLanguageString(const std::string& str);
std::string GetWorkingDirectory();

/* SAMPLE:
&& scopeobject._name != "User you like" \
*/