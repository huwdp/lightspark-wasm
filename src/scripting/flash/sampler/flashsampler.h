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

#ifndef FLASHSAMPLER_H
#define FLASHSAMPLER_H

#include "asobject.h"
#include "scripting/toplevel/Array.h"

namespace lightspark
{
class Sample : public ASObject
{
public:
	Sample(Class_base* c);
	static void sinit(Class_base*);
};

class DeleteObjectSample : public Sample
{
public:
	DeleteObjectSample(Class_base* c);
	static void sinit(Class_base*);
};
class NewObjectSample : public Sample
{
public:
	NewObjectSample(Class_base* c);
	static void sinit(Class_base*);
	ASPROPERTY_GETTER(_NR<ASObject>,object);
	ASPROPERTY_GETTER(number_t,size);
};
class StackFrame : public ASObject
{
public:
	StackFrame(Class_base* c);
	static void sinit(Class_base*);
	ASFUNCTION_ATOM(_toString);
};


void clearSamples(asAtom& ret, SystemState* sys, asAtom& obj,asAtom* args, const unsigned int argslen);
void getGetterInvocationCount(asAtom& ret, SystemState* sys, asAtom& obj,asAtom* args, const unsigned int argslen);
void getInvocationCount(asAtom& ret, SystemState* sys, asAtom& obj,asAtom* args, const unsigned int argslen);
void getSetterInvocationCount(asAtom& ret, SystemState* sys, asAtom& obj,asAtom* args, const unsigned int argslen);
void getLexicalScopes(asAtom& ret, SystemState* sys, asAtom& obj,asAtom* args, const unsigned int argslen);
void getMasterString(asAtom& ret, SystemState* sys, asAtom& obj,asAtom* args, const unsigned int argslen);
void getMemberNames(asAtom& ret, SystemState* sys, asAtom& obj,asAtom* args, const unsigned int argslen);
void getSampleCount(asAtom& ret, SystemState* sys, asAtom& obj,asAtom* args, const unsigned int argslen);
void getSamples(asAtom& ret, SystemState* sys, asAtom& obj,asAtom* args, const unsigned int argslen);
void getSize(asAtom& ret, SystemState* sys, asAtom& obj,asAtom* args, const unsigned int argslen);
void getSavedThis(asAtom& ret, SystemState* sys, asAtom& obj,asAtom* args, const unsigned int argslen);
void isGetterSetter(asAtom& ret, SystemState* sys, asAtom& obj,asAtom* args, const unsigned int argslen);
void pauseSampling(asAtom& ret, SystemState* sys, asAtom& obj,asAtom* args, const unsigned int argslen);
void sampleInternalAllocs(asAtom& ret, SystemState* sys, asAtom& obj,asAtom* args, const unsigned int argslen);
void setSamplerCallback(asAtom& ret, SystemState* sys, asAtom& obj,asAtom* args, const unsigned int argslen);
void startSampling(asAtom& ret, SystemState* sys, asAtom& obj,asAtom* args, const unsigned int argslen);
void stopSampling(asAtom& ret, SystemState* sys, asAtom& obj,asAtom* args, const unsigned int argslen);





}
#endif // FLASHSAMPLER_H
