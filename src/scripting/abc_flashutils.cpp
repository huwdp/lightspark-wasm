/**************************************************************************
    Lightspark, a free flash player implementation

    Copyright (C) 2009-2013  Alessandro Pignotti (a.pignotti@sssup.it)

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

#include "scripting/flash/utils/flashutils.h"
#include "scripting/flash/utils/CompressionAlgorithm.h"
#include "scripting/flash/utils/Dictionary.h"
#include "scripting/flash/utils/Proxy.h"
#include "scripting/flash/utils/Timer.h"

#include "scripting/class.h"
#include "scripting/abc.h"
using namespace lightspark;

void ABCVm::registerClassesFlashUtils(Global* builtin)
{
	builtin->registerBuiltin("Endian","flash.utils",Class<Endian>::getRef(m_sys));
	builtin->registerBuiltin("ByteArray","flash.utils",Class<ByteArray>::getRef(m_sys));
	builtin->registerBuiltin("CompressionAlgorithm","flash.utils",Class<CompressionAlgorithm>::getRef(m_sys));
	builtin->registerBuiltin("Dictionary","flash.utils",Class<Dictionary>::getRef(m_sys));
	builtin->registerBuiltin("Proxy","flash.utils",Class<Proxy>::getRef(m_sys));
	builtin->registerBuiltin("Timer","flash.utils",Class<Timer>::getRef(m_sys));
	builtin->registerBuiltin("getQualifiedClassName","flash.utils",_MR(Class<IFunction>::getFunction(m_sys,getQualifiedClassName)));
	builtin->registerBuiltin("getQualifiedSuperclassName","flash.utils",_MR(Class<IFunction>::getFunction(m_sys,getQualifiedSuperclassName)));
	builtin->registerBuiltin("getDefinitionByName","flash.utils",_MR(Class<IFunction>::getFunction(m_sys,getDefinitionByName)));
	builtin->registerBuiltin("getTimer","flash.utils",_MR(Class<IFunction>::getFunction(m_sys,getTimer,0,Class<Integer>::getRef(m_sys).getPtr())));
	builtin->registerBuiltin("setInterval","flash.utils",_MR(Class<IFunction>::getFunction(m_sys,setInterval)));
	builtin->registerBuiltin("setTimeout","flash.utils",_MR(Class<IFunction>::getFunction(m_sys,setTimeout)));
	builtin->registerBuiltin("clearInterval","flash.utils",_MR(Class<IFunction>::getFunction(m_sys,clearInterval)));
	builtin->registerBuiltin("clearTimeout","flash.utils",_MR(Class<IFunction>::getFunction(m_sys,clearTimeout)));
	builtin->registerBuiltin("describeType","flash.utils",_MR(Class<IFunction>::getFunction(m_sys,describeType)));
	builtin->registerBuiltin("escapeMultiByte","flash.utils",_MR(Class<IFunction>::getFunction(m_sys,escapeMultiByte)));
	builtin->registerBuiltin("unescapeMultiByte","flash.utils",_MR(Class<IFunction>::getFunction(m_sys,unescapeMultiByte)));
	builtin->registerBuiltin("IExternalizable","flash.utils",InterfaceClass<IExternalizable>::getRef(m_sys));
	builtin->registerBuiltin("IDataInput","flash.utils",InterfaceClass<IDataInput>::getRef(m_sys));
	builtin->registerBuiltin("IDataOutput","flash.utils",InterfaceClass<IDataOutput>::getRef(m_sys));

	if(m_sys->flashMode==SystemState::AIR)
	{
		builtin->registerBuiltin("NativeMenuItem","flash.display",Class<NativeMenuItem>::getRef(m_sys));
	}
}
