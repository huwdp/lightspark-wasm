/**************************************************************************
    Lightspark, a free flash player implementation

    Copyright (C) 2011-2013  Alessandro Pignotti (a.pignotti@sssup.it)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
**************************************************************************/

#include <map>
#include "backends/security.h"
#include "scripting/abc.h"
#include "scripting/flash/sensors/flashsensors.h"
#include "scripting/class.h"
#include "scripting/flash/system/flashsystem.h"
#include "compat.h"
#include "backends/audio.h"

using namespace std;
using namespace lightspark;

Accelerometer::Accelerometer(Class_base* c):EventDispatcher(c) {}

void Accelerometer::sinit(Class_base* c)
{
	CLASS_SETUP_NO_CONSTRUCTOR(c, EventDispatcher, CLASS_SEALED);
	// properties
	c->setDeclaredMethodByQName("isSupported", "", Class<IFunction>::getFunction(c->getSystemState(),_isSupported),GETTER_METHOD,false);
}

void Accelerometer::buildTraits(ASObject *o)
{
}

ASFUNCTIONBODY_ATOM(Accelerometer,_isSupported)
{
	asAtomHandler::setBool(ret,false);
}
