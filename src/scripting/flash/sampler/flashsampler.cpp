#include "scripting/flash/sampler/flashsampler.h"
#include "scripting/toplevel/Array.h"
#include "scripting/argconv.h"

using namespace lightspark;

Sample::Sample(Class_base* c):
	ASObject(c)
{
}

void Sample::sinit(Class_base* c)
{
	CLASS_SETUP_NO_CONSTRUCTOR(c, ASObject, CLASS_SEALED|CLASS_FINAL);
}


DeleteObjectSample::DeleteObjectSample(Class_base* c):
	Sample(c)
{
}

void DeleteObjectSample::sinit(Class_base* c)
{
	CLASS_SETUP_NO_CONSTRUCTOR(c, Sample, CLASS_SEALED|CLASS_FINAL);
}


NewObjectSample::NewObjectSample(Class_base* c):
	Sample(c)
{
}

void NewObjectSample::sinit(Class_base* c)
{
	CLASS_SETUP_NO_CONSTRUCTOR(c, Sample, CLASS_SEALED|CLASS_FINAL);
	REGISTER_GETTER(c,object);
	REGISTER_GETTER(c,size);
}
ASFUNCTIONBODY_GETTER_NOT_IMPLEMENTED(NewObjectSample,object);
ASFUNCTIONBODY_GETTER_NOT_IMPLEMENTED(NewObjectSample,size);

StackFrame::StackFrame(Class_base* c):
	ASObject(c)
{
}

void StackFrame::sinit(Class_base* c)
{
	CLASS_SETUP_NO_CONSTRUCTOR(c, ASObject, CLASS_SEALED|CLASS_FINAL);
	c->setDeclaredMethodByQName("toString","",Class<IFunction>::getFunction(c->getSystemState(),_toString),NORMAL_METHOD,true);
}
ASFUNCTIONBODY_ATOM(StackFrame,_toString)
{
	LOG(LOG_NOT_IMPLEMENTED,"flash.sampler.stackFrame.toString not implemented");
	ret = asAtomHandler::fromStringID(BUILTIN_STRINGS::EMPTY);
}


ASFUNCTIONBODY_ATOM(lightspark,clearSamples)
{
	LOG(LOG_NOT_IMPLEMENTED,"flash.sampler.clearSamples not implemented");
}
ASFUNCTIONBODY_ATOM(lightspark,getGetterInvocationCount)
{
	_NR<ASObject> o;
	_NR<ASQName> qname;
	ARG_UNPACK_ATOM (o)(qname);
	LOG(LOG_NOT_IMPLEMENTED,"flash.sampler.getGetterInvocationCount not implemented");
	asAtomHandler::setInt(ret,sys,0);
}
ASFUNCTIONBODY_ATOM(lightspark,getInvocationCount)
{
	_NR<ASObject> o;
	_NR<ASQName> qname;
	ARG_UNPACK_ATOM (o)(qname);
	LOG(LOG_NOT_IMPLEMENTED,"flash.sampler.getInvocationCount not implemented");
	asAtomHandler::setInt(ret,sys,0);
}
ASFUNCTIONBODY_ATOM(lightspark,getSetterInvocationCount)
{
	_NR<ASObject> o;
	_NR<ASQName> qname;
	ARG_UNPACK_ATOM (o)(qname);
	LOG(LOG_NOT_IMPLEMENTED,"flash.sampler.getSetterInvocationCount not implemented");
	asAtomHandler::setInt(ret,sys,0);
}
ASFUNCTIONBODY_ATOM(lightspark,getLexicalScopes)
{
	_NR<IFunction> func;
	ARG_UNPACK_ATOM (func);
	LOG(LOG_NOT_IMPLEMENTED,"flash.sampler.getLexicalScopes not implemented");
	Array* res=Class<Array>::getInstanceSNoArgs(sys);
	ret = asAtomHandler::fromObject(res);
}
ASFUNCTIONBODY_ATOM(lightspark,getMasterString)
{
	tiny_string str;
	ARG_UNPACK_ATOM (str);
	LOG(LOG_NOT_IMPLEMENTED,"flash.sampler.getMasterString not implemented");
	asAtomHandler::setNull(ret);
}
ASFUNCTIONBODY_ATOM(lightspark,getMemberNames)
{
	bool instanceNames;
	_NR<ASObject> o;
	ARG_UNPACK_ATOM (o)(instanceNames, false);
	LOG(LOG_NOT_IMPLEMENTED,"flash.sampler.getMemberNames not implemented");
	asAtomHandler::setUndefined(ret);
}
ASFUNCTIONBODY_ATOM(lightspark,getSampleCount)
{
	LOG(LOG_NOT_IMPLEMENTED,"flash.sampler.getSampleCount not implemented");
	asAtomHandler::setInt(ret,sys,-1);
}
ASFUNCTIONBODY_ATOM(lightspark,getSamples)
{
	LOG(LOG_NOT_IMPLEMENTED,"flash.sampler.getSamples not implemented");
	asAtomHandler::setUndefined(ret);
}

ASFUNCTIONBODY_ATOM(lightspark,getSize)
{
	_NR<ASObject> o;
	ARG_UNPACK_ATOM (o);
	LOG(LOG_NOT_IMPLEMENTED,"flash.sampler.getSize not implemented");
	asAtomHandler::setInt(ret,sys,0);
}
ASFUNCTIONBODY_ATOM(lightspark,getSavedThis)
{
	_NR<ASObject> o;
	ARG_UNPACK_ATOM (o);
	LOG(LOG_NOT_IMPLEMENTED,"flash.sampler.getSavedThis not implemented");
	asAtomHandler::setNull(ret);
}
ASFUNCTIONBODY_ATOM(lightspark,isGetterSetter)
{
	_NR<ASObject> o;
	_NR<ASQName> qname;
	ARG_UNPACK_ATOM (o)(qname);
	LOG(LOG_NOT_IMPLEMENTED,"flash.sampler.isGetterSetter not implemented");
	asAtomHandler::setBool(ret,false);
}
ASFUNCTIONBODY_ATOM(lightspark,pauseSampling)
{
	LOG(LOG_NOT_IMPLEMENTED,"flash.sampler.pauseSampling not implemented");
	ret = asAtomHandler::undefinedAtom;
}
ASFUNCTIONBODY_ATOM(lightspark,sampleInternalAllocs)
{
	bool b;
	ARG_UNPACK_ATOM (b);
	LOG(LOG_NOT_IMPLEMENTED,"flash.sampler.sampleInternalAllocs not implemented");
}
ASFUNCTIONBODY_ATOM(lightspark,setSamplerCallback)
{
	LOG(LOG_NOT_IMPLEMENTED,"flash.sampler.setSamplerCallback not implemented");
}
ASFUNCTIONBODY_ATOM(lightspark,startSampling)
{
	LOG(LOG_NOT_IMPLEMENTED,"flash.sampler.startSampling not implemented");
}
ASFUNCTIONBODY_ATOM(lightspark,stopSampling)
{
	LOG(LOG_NOT_IMPLEMENTED,"flash.sampler.stopSampling not implemented");
}

