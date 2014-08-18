#ifdef __WAND__
target[name[audio_port.o] type[object]]
#endif

#include "audio_port.h"
#include <herbs/string/string.h>

namespace
	{
	Herbs::Stringbase<char> portname_make(const Herbs::String& str)
		{
		Herbs::Stringbase<char> ret;
		auto ptr=str.begin();
		while(ptr!=str.end())
			{
			ret.append(*ptr);
			++ptr;
			}
		ret.terminate();
		return ret;
		}
	}

WaveRec::AudioPort::AudioPort(MuStudio::Client& client
	,const Herbs::String port_name,size_t size_buffer):
	 buffer_a(size_buffer),buffer_b(size_buffer)
	,buffer_front(&buffer_a),buffer_back(&buffer_b)
	,port(client,portname_make(port_name).begin())
	{
	positionReset();
	}