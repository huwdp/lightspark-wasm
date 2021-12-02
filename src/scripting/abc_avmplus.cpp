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

#include "scripting/avmplus/avmplus.h"

#include "scripting/class.h"
#include "scripting/abc.h"
using namespace lightspark;

void ABCVm::registerClassesAvmplus(Global* builtin)
{
	// avm intrinsics, not documented, but implemented in avmplus
	builtin->registerBuiltin("casi32","avm2.intrinsics.memory",_MR(Class<IFunction>::getFunction(m_sys,casi32,3)));

	// if needed add AVMPLUS definitions
	if(m_sys->flashMode==SystemState::AVMPLUS)
	{
		builtin->registerBuiltin("getQualifiedClassName","avmplus",_MR(Class<IFunction>::getFunction(m_sys,getQualifiedClassName)));
		builtin->registerBuiltin("getQualifiedSuperclassName","avmplus",_MR(Class<IFunction>::getFunction(m_sys,getQualifiedSuperclassName)));
		builtin->registerBuiltin("getTimer","",_MR(Class<IFunction>::getFunction(m_sys,getTimer,0,Class<Integer>::getRef(m_sys).getPtr())));
		builtin->registerBuiltin("FLASH10_FLAGS","avmplus",_MR(abstract_ui(m_sys,0x7FF)));
		builtin->registerBuiltin("HIDE_NSURI_METHODS","avmplus",_MR(abstract_ui(m_sys,0x0001)));
		builtin->registerBuiltin("INCLUDE_BASES","avmplus",_MR(abstract_ui(m_sys,0x0002)));
		builtin->registerBuiltin("INCLUDE_INTERFACES","avmplus",_MR(abstract_ui(m_sys,0x0004)));
		builtin->registerBuiltin("INCLUDE_VARIABLES","avmplus",_MR(abstract_ui(m_sys,0x0008)));
		builtin->registerBuiltin("INCLUDE_ACCESSORS","avmplus",_MR(abstract_ui(m_sys,0x0010)));
		builtin->registerBuiltin("INCLUDE_METHODS","avmplus",_MR(abstract_ui(m_sys,0x0020)));
		builtin->registerBuiltin("INCLUDE_METADATA","avmplus",_MR(abstract_ui(m_sys,0x0040)));
		builtin->registerBuiltin("INCLUDE_CONSTRUCTOR","avmplus",_MR(abstract_ui(m_sys,0x0080)));
		builtin->registerBuiltin("INCLUDE_TRAITS","avmplus",_MR(abstract_ui(m_sys,0x0100)));
		builtin->registerBuiltin("USE_ITRAITS","avmplus",_MR(abstract_ui(m_sys,0x0200)));
		builtin->registerBuiltin("HIDE_OBJECT","avmplus",_MR(abstract_ui(m_sys,0x0400)));
		builtin->registerBuiltin("describeType","avmplus",_MR(Class<IFunction>::getFunction(m_sys,describeType)));
		builtin->registerBuiltin("describeTypeJSON","avmplus",_MR(Class<IFunction>::getFunction(m_sys,describeTypeJSON)),PACKAGE_INTERNAL_NAMESPACE);

		builtin->registerBuiltin("System","avmplus",Class<avmplusSystem>::getRef(m_sys));
		builtin->registerBuiltin("Domain","avmplus",Class<avmplusDomain>::getRef(m_sys));
		builtin->registerBuiltin("File","avmplus",Class<avmplusFile>::getRef(m_sys));

		builtin->registerBuiltin("AbstractBase","avmshell",Class<ASObject>::getRef(m_sys));
		builtin->registerBuiltin("AbstractRestrictedBase","avmshell",Class<ASObject>::getRef(m_sys));
		builtin->registerBuiltin("NativeBase","avmshell",Class<ASObject>::getRef(m_sys));
		builtin->registerBuiltin("NativeBaseAS3","avmshell",Class<ASObject>::getRef(m_sys));
		builtin->registerBuiltin("NativeSubclassOfAbstractBase","avmshell",Class<ASObject>::getRef(m_sys));
		builtin->registerBuiltin("NativeSubclassOfAbstractRestrictedBase","avmshell",Class<ASObject>::getRef(m_sys));
		builtin->registerBuiltin("NativeSubclassOfRestrictedBase","avmshell",Class<ASObject>::getRef(m_sys));
		builtin->registerBuiltin("RestrictedBase","avmshell",Class<ASObject>::getRef(m_sys));
		builtin->registerBuiltin("SubclassOfAbstractBase","avmshell",Class<ASObject>::getRef(m_sys));
		builtin->registerBuiltin("SubclassOfAbstractRestrictedBase","avmshell",Class<ASObject>::getRef(m_sys));
		builtin->registerBuiltin("SubclassOfRestrictedBase","avmshell",Class<ASObject>::getRef(m_sys));
	}

}
