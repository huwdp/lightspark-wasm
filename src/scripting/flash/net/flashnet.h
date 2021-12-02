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

#ifndef SCRIPTING_FLASH_NET_FLASHNET_H
#define SCRIPTING_FLASH_NET_FLASHNET_H 1

#include "compat.h"
#include "asobject.h"
#include "scripting/flash/events/flashevents.h"
#include "thread_pool.h"
#include "backends/netutils.h"
#include "timer.h"
#include "backends/decoder.h"
#include "backends/audio.h"
#include "NetStreamInfo.h"
#include "scripting/flash/utils/ByteArray.h"

namespace lightspark
{

class URLRequest: public ASObject
{
private:
	enum METHOD { GET=0, POST };
	METHOD method;
	tiny_string url;
	_NR<ASObject> data;
	tiny_string digest;
	tiny_string validatedContentType() const;
	tiny_string getContentTypeHeader() const;
	void validateHeaderName(const tiny_string& headerName) const;
	ASPROPERTY_GETTER_SETTER(tiny_string,contentType);
	ASPROPERTY_GETTER_SETTER(_R<Array>,requestHeaders);
public:
	URLRequest(Class_base* c, const tiny_string u="", const tiny_string m="GET", _NR<ASObject> d = NullRef);
	void finalize() override;
	static void sinit(Class_base*);
	static void buildTraits(ASObject* o);
	ASFUNCTION_ATOM(_constructor);
	ASFUNCTION_ATOM(_getURL);
	ASFUNCTION_ATOM(_setURL);
	ASFUNCTION_ATOM(_getMethod);
	ASFUNCTION_ATOM(_setMethod);
	ASFUNCTION_ATOM(_setData);
	ASFUNCTION_ATOM(_getData);
	ASFUNCTION_ATOM(_getDigest);
	ASFUNCTION_ATOM(_setDigest);
	URLInfo getRequestURL() const;
	std::list<tiny_string> getHeaders() const;
	void getPostData(std::vector<uint8_t>& data) const;
};

class URLRequestMethod: public ASObject
{
public:
	URLRequestMethod(Class_base* c):ASObject(c){}
	static void sinit(Class_base*);
};

class URLVariables: public ASObject
{
private:
	void decode(const tiny_string& s);
	tiny_string toString_priv();
public:
	URLVariables(Class_base* c):ASObject(c){}
	URLVariables(Class_base* c,const tiny_string& s);
	static void sinit(Class_base*);
	static void buildTraits(ASObject* o);
	ASFUNCTION_ATOM(_constructor);
	ASFUNCTION_ATOM(decode);
	ASFUNCTION_ATOM(_toString);
	tiny_string toString();
};

class URLLoaderDataFormat: public ASObject
{
public:
	URLLoaderDataFormat(Class_base* c):ASObject(c){}
	static void sinit(Class_base*);
};


class SharedObjectFlushStatus: public ASObject
{
public:
	SharedObjectFlushStatus(Class_base* c):ASObject(c){}
	static void sinit(Class_base*);
};

class SharedObject: public EventDispatcher
{
private:
	tiny_string name;
public:
	SharedObject(Class_base* c);
	bool destruct() override;
	bool doFlush();
	static void sinit(Class_base*);
	ASFUNCTION_ATOM(getLocal);
	ASFUNCTION_ATOM(getRemote);
	ASFUNCTION_ATOM(flush);
	ASFUNCTION_ATOM(clear);
	ASFUNCTION_ATOM(close);
	ASFUNCTION_ATOM(connect);
	ASFUNCTION_ATOM(setProperty);
	ASPROPERTY_GETTER_SETTER(_R<ASObject>,client);
	ASPROPERTY_GETTER(_NR<ASObject>,data);
	ASFUNCTION_ATOM(_getDefaultObjectEncoding);
	ASFUNCTION_ATOM(_setDefaultObjectEncoding);
	ASPROPERTY_SETTER(number_t,fps);
	ASPROPERTY_GETTER_SETTER(uint32_t,objectEncoding);
	ASFUNCTION_ATOM(_getPreventBackup);
	ASFUNCTION_ATOM(_setPreventBackup);
	ASFUNCTION_ATOM(_getSize);
};

class IDynamicPropertyWriter
{
public:
	static void linkTraits(Class_base* c);
};
class IDynamicPropertyOutput
{
public:
	static void linkTraits(Class_base* c);
};
// this is an internal class not described in the specs, but avmplus defines it for dynamic property writing
class DynamicPropertyOutput:public ASObject
{
public:
	DynamicPropertyOutput(Class_base* c):ASObject(c){}
	static void sinit(Class_base*);
	ASFUNCTION_ATOM(writeDynamicProperty);
};

class ObjectEncoding: public ASObject
{
public:
	ObjectEncoding(Class_base* c):ASObject(c){}
	enum ENCODING { AMF0=0, AMF3=3, DEFAULT=3 };
	static void sinit(Class_base*);
	ASPROPERTY_GETTER_SETTER(_NR<ASObject>, dynamicPropertyWriter);
};

class URLLoader;

class URLLoaderThread : public DownloaderThreadBase
{
private:
	_R<URLLoader> loader;
	void execute() override;
public:
	URLLoaderThread(_R<URLRequest> _request, _R<URLLoader> _loader);
};

class URLLoader: public EventDispatcher, public IDownloaderThreadListener, public ILoadable
{
private:
	tiny_string dataFormat;
	_NR<ASObject> data;
	Mutex spinlock;
	URLLoaderThread *job;
	uint64_t timestamp_last_progress;
public:
	URLLoader(Class_base* c);
	void finalize() override;
	static void sinit(Class_base*);
	static void buildTraits(ASObject* o);
	void threadFinished(IThreadJob *job) override;
	void setData(_NR<ASObject> data);
	ASObject* getData() const
	{
		return data.getPtr();
	}
	tiny_string getDataFormat();
	void setDataFormat(const tiny_string& newFormat);
	void setBytesTotal(uint32_t b) override;
	void setBytesLoaded(uint32_t b) override;
	ASFUNCTION_ATOM(_constructor);
	ASFUNCTION_ATOM(load);
	ASFUNCTION_ATOM(close);
	ASFUNCTION_ATOM(_getDataFormat);
	ASFUNCTION_ATOM(_getData);
	ASFUNCTION_ATOM(_setData);
	ASFUNCTION_ATOM(_setDataFormat);
	ASPROPERTY_GETTER_SETTER(uint32_t, bytesLoaded);
	ASPROPERTY_GETTER_SETTER(uint32_t, bytesTotal);
};

class Responder: public ASObject
{
private:
	asAtom result;
	asAtom status;
public:
	Responder(Class_base* c):ASObject(c),result(asAtomHandler::invalidAtom),status(asAtomHandler::invalidAtom){}
	static void sinit(Class_base*);
	void finalize() override;
	ASFUNCTION_ATOM(_constructor);
	ASFUNCTION_ATOM(onResult);
};

class NetConnection: public EventDispatcher, public IThreadJob
{
friend class NetStream;
private:
	enum PROXY_TYPE { PT_NONE, PT_HTTP, PT_CONNECT_ONLY, PT_CONNECT, PT_BEST };
	//Indicates whether the application is connected to a server through a persistent RMTP connection/HTTP server with Flash Remoting
	bool _connected;
	tiny_string protocol;
	URLInfo uri;
	//Data for remoting support (NetConnection::call)
	// The message data to be sent asynchronously
	std::vector<uint8_t> messageData;
	Mutex downloaderLock;
	Downloader* downloader;
	_NR<Responder> responder;
	uint32_t messageCount;
	//The connection is to a flash media server
	ObjectEncoding::ENCODING objectEncoding;
	PROXY_TYPE proxyType;
	//IThreadJob interface
	void execute() override;
	void threadAbort() override;
	void jobFence() override;
public:
	NetConnection(Class_base* c);
	void finalize() override;
	static void sinit(Class_base*);
	static void buildTraits(ASObject* o);
	ASFUNCTION_ATOM(_constructor);
	ASFUNCTION_ATOM(connect);
	ASFUNCTION_ATOM(call);
	ASFUNCTION_ATOM(_getConnected);
	ASFUNCTION_ATOM(_getConnectedProxyType);
	ASFUNCTION_ATOM(_getDefaultObjectEncoding);
	ASFUNCTION_ATOM(_setDefaultObjectEncoding);
	ASFUNCTION_ATOM(_getObjectEncoding);
	ASFUNCTION_ATOM(_setObjectEncoding);
	ASFUNCTION_ATOM(_getProtocol);
	ASFUNCTION_ATOM(_getProxyType);
	ASFUNCTION_ATOM(_setProxyType);
	ASFUNCTION_ATOM(_getURI);
	ASFUNCTION_ATOM(close);
	ASPROPERTY_GETTER_SETTER(NullableRef<ASObject>,client);
	ASPROPERTY_GETTER_SETTER(uint32_t,maxPeerConnections);
	ASPROPERTY_GETTER(tiny_string,nearID);
	void afterExecution(_R<Event> e) override;
};

class NetStreamAppendBytesAction: public ASObject
{
public:
	NetStreamAppendBytesAction(Class_base* c):ASObject(c){}
	static void sinit(Class_base*);
};


class SoundTransform;
class NetStream: public EventDispatcher, public IThreadJob, public ITickJob
{
private:
	bool tickStarted;
	//Indicates whether the NetStream is paused
	bool paused;
	//Indicates whether the NetStream has been closed/threadAborted. This is reset at every play() call.
	//We initialize this value to true, so we can check that play() hasn't been called without being closed first.
	volatile bool closed;

	uint32_t streamTime;
	URLInfo url;
	double frameRate;
	//The NetConnection used by this NetStream
	_NR<NetConnection> connection;
	Downloader* downloader;
	VideoDecoder* videoDecoder;
	AudioDecoder* audioDecoder;
	AudioStream *audioStream;
	// only used when in DataGenerationMode
	StreamCache* datagenerationfile;
	bool datagenerationthreadstarted;
	Mutex mutex;
	Mutex countermutex;
	//IThreadJob interface for long jobs
	void execute() override;
	void threadAbort() override;
	void jobFence() override;
	//ITickJob interface to frame advance
	void tick() override;
	void tickFence() override;
	bool isReady() const;
	_NR<ASObject> client;

	ASPROPERTY_GETTER_SETTER(NullableRef<SoundTransform>,soundTransform);
	number_t oldVolume;

	enum CONNECTION_TYPE { CONNECT_TO_FMS=0, DIRECT_CONNECTIONS };
	CONNECTION_TYPE peerID;

	bool checkPolicyFile;
	bool rawAccessAllowed;

	uint32_t framesdecoded;
	uint32_t prevstreamtime;
	number_t playbackBytesPerSecond;
	number_t maxBytesPerSecond;

	struct bytespertime {
		uint64_t timestamp;
		uint32_t bytesread;
	};
	std::deque<bytespertime> currentBytesPerSecond;
	enum DATAGENERATION_EXPECT_TYPE { DATAGENERATION_HEADER=0,DATAGENERATION_PREVTAG,DATAGENERATION_FLVTAG };
	DATAGENERATION_EXPECT_TYPE datagenerationexpecttype;
	_NR<ByteArray> datagenerationbuffer;
	StreamDecoder* streamDecoder;
public:
	NetStream(Class_base* c);
	~NetStream();
	void finalize() override;
	static void sinit(Class_base*);
	static void buildTraits(ASObject* o);
	ASFUNCTION_ATOM(_constructor);
	ASFUNCTION_ATOM(play);
	ASFUNCTION_ATOM(play2);
	ASFUNCTION_ATOM(resume);
	ASFUNCTION_ATOM(pause);
	ASFUNCTION_ATOM(togglePause);
	ASFUNCTION_ATOM(close);
	ASFUNCTION_ATOM(seek);
	ASFUNCTION_ATOM(_getBytesLoaded);
	ASFUNCTION_ATOM(_getBytesTotal);
	ASFUNCTION_ATOM(_getTime);
	ASFUNCTION_ATOM(_getCurrentFPS);
	ASFUNCTION_ATOM(_getClient);
	ASFUNCTION_ATOM(_setClient);
	ASFUNCTION_ATOM(_getCheckPolicyFile);
	ASFUNCTION_ATOM(_setCheckPolicyFile);
	ASFUNCTION_ATOM(attach);
	ASFUNCTION_ATOM(appendBytes);
	ASFUNCTION_ATOM(appendBytesAction);
	ASFUNCTION_ATOM(_getInfo);
	ASFUNCTION_ATOM(publish);
	ASPROPERTY_GETTER(number_t, backBufferLength);
	ASPROPERTY_GETTER_SETTER(number_t, backBufferTime);
	ASPROPERTY_GETTER(number_t, bufferLength);
	ASPROPERTY_GETTER_SETTER(number_t, bufferTime);
	ASPROPERTY_GETTER_SETTER(number_t, bufferTimeMax);
	ASPROPERTY_GETTER_SETTER(number_t, maxPauseBufferTime);
	ASPROPERTY_GETTER_SETTER(bool, useHardwareDecoder);

	void sendClientNotification(const tiny_string& name, std::list<asAtom>& arglist);

	//Interface for video
	/**
	  	Get the frame width

		@pre lock on the object should be acquired and object should be ready
		@return the frame width
	*/
	uint32_t getVideoWidth() const;
	/**
	  	Get the frame height

		@pre lock on the object should be acquired and object should be ready
		@return the frame height
	*/
	uint32_t getVideoHeight() const;
	/**
	  	Get the frame rate

		@pre lock on the object should be acquired and object should be ready
		@return the frame rate
	*/
	double getFrameRate();
	/**
	  	Get the texture containing the current video Frame
		@pre lock on the object should be acquired and object should be ready
		@return a TextureChunk ready to be blitted
	*/
	const TextureChunk& getTexture() const;
	/**
	  	Get the stream time

		@pre lock on the object should be acquired and object should be ready
		@return the stream time
	*/
	uint32_t getStreamTime();
	/**
	  	Get the length of loaded data

		@pre lock on the object should be acquired and object should be ready
		@return the length of loaded data
	*/
	uint32_t getReceivedLength();
	/**
	  	Get the length of loaded data

		@pre lock on the object should be acquired and object should be ready
		@return the total length of the data
	*/
	uint32_t getTotalLength();
	/**
	  	Acquire the mutex to guarantee validity of data

		@return true if the lock has been acquired
	*/
	bool lockIfReady();
	/**
	  	Release the lock

		@pre the object should be locked
	*/
	void unlock();
};

class LocalConnection: public EventDispatcher
{
public:
	LocalConnection(Class_base* c);
	static void sinit(Class_base*);
	ASFUNCTION_ATOM(_constructor);
	ASPROPERTY_GETTER(bool,isSupported);
	ASFUNCTION_ATOM(allowDomain);
	ASFUNCTION_ATOM(allowInsecureDomain);
	ASFUNCTION_ATOM(send);
	ASFUNCTION_ATOM(connect);
	ASFUNCTION_ATOM(close);
	ASFUNCTION_ATOM(domain);
	ASPROPERTY_GETTER_SETTER(_NR<ASObject>,client);
};

class NetGroup: public EventDispatcher
{
public:
	NetGroup(Class_base* c);
	static void sinit(Class_base*);
	ASFUNCTION_ATOM(_constructor);
};

class FileReference: public EventDispatcher
{
public:
	FileReference(Class_base* c);
	static void sinit(Class_base*);
	ASFUNCTION_ATOM(_constructor);
};
class FileFilter: public ASObject
{
public:
	FileFilter(Class_base* c);
	static void sinit(Class_base*);
	ASFUNCTION_ATOM(_constructor);
	ASPROPERTY_GETTER_SETTER(tiny_string,description);
	ASPROPERTY_GETTER_SETTER(tiny_string,extension);
	ASPROPERTY_GETTER_SETTER(tiny_string,macType);
};

class DRMManager: public EventDispatcher
{
public:
	DRMManager(Class_base* c);
	static void sinit(Class_base*);
	ASPROPERTY_GETTER(bool,isSupported);
};

void navigateToURL(asAtom& ret,SystemState* sys,asAtom& obj,asAtom* args, const unsigned int argslen);
void sendToURL(asAtom& ret,SystemState* sys,asAtom& obj,asAtom* args, const unsigned int argslen);
void registerClassAlias(asAtom& ret,SystemState* sys,asAtom& obj,asAtom* args, const unsigned int argslen);
void getClassByAlias(asAtom& ret,SystemState* sys,asAtom& obj,asAtom* args, const unsigned int argslen);

}

#endif /* SCRIPTING_FLASH_NET_FLASHNET_H */
