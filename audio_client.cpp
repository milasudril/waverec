#ifdef __WAND__
target[name[audio_client.o] type[object]]
#endif

#include "audio_client.h"
#include "audio_port.h"
#include <herbs/string/string.h>

namespace
	{
	class PortInitializer
		{
		public:
			PortInitializer(WaveRec::AudioClient& client
				,const Herbs::Array<Herbs::String>& ports
				,size_t size_buffer):
				 m_client(client)
				,m_ports(ports.begin())
				,m_size_buffer(size_buffer)
				{}

			bool operator()(WaveRec::AudioPort* port,size_t n,size_t N)
				{
				new(port)WaveRec::AudioPort(m_client,*m_ports,m_size_buffer);
				++m_ports;
				return 1;
				}

		private:
			WaveRec::AudioClient& m_client;
			const Herbs::String* m_ports;
			size_t m_size_buffer;
		};
	}

WaveRec::AudioClient::AudioClient(const Herbs::Array<Herbs::String>& ports
	,size_t buffer_length):
	MuStudio::Client("waverec")
	,ports(ports.length(),PortInitializer(*this,ports,buffer_length))
	,m_buffer_length(buffer_length)
	{
	}

WaveRec::AudioClient::~AudioClient()
	{
	}

int WaveRec::AudioClient::onProcess(size_t nframes)
	{
	auto port_ptr=ports.begin();
	while(port_ptr!=ports.end())
		{
		auto buffer=port_ptr->bufferGet(nframes);
		auto k=nframes;
		while(k!=0)
			{
			*(port_ptr->positionGet())=*buffer;
			port_ptr->positionIncrement();
			if(port_ptr->positionGet()==port_ptr->positionEnd())
				{
				port_ptr->buffersSwap();
				port_ptr->positionReset();
				port_ptr->readySet();
				}
			++buffer;
			--k;
			}
		++port_ptr;
		}
	return 0;
	}

void WaveRec::AudioClient::wait(size_t k)
	{
	ports[k].wait();
	}

const MuStudio::Audio::sample_t* WaveRec::AudioClient::bufferGet(size_t k)
	{
	return ports[k].bufferBackGet();
	}
