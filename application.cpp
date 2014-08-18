#ifdef __WAND__
target[name[application.o] type[object]]
#endif

#include "application.h"
#include <waveio/writer.h>
#include <waveio/streaminfo.h>
#include <herbs/fileout/fileout.h>
#include <herbs/directory/directory.h>
#include <herbs/exceptionmissing/exceptionmissing.h>
#include <herbs/ctrl_c_handler/ctrl_c_handler.h>
#include <herbs/path/path.h>
#include <herbs/logwriter/logwriter.h>
#include <herbs/logmessage/logmessage.h>

namespace
	{
	Herbs::Array<Herbs::String> ports_get(const Herbs::Directory& dir)
		{
		Herbs::Array<Herbs::String> ret;
		auto subdir=dir.directorySetFind(STR("ports"));
		if(subdir==nullptr)
			{throw Herbs::ExceptionMissing(___FILE__,__LINE__);}

		auto dir_ptr=subdir->begin();
		while(dir_ptr!=subdir->end())
			{
			auto item_ptr=dir_ptr->itemsBegin();
			while(item_ptr!=dir_ptr->itemsEnd())
				{
				ret.append(*item_ptr);
				++item_ptr;
				}
			++dir_ptr;
			}

		return ret;
		}

	size_t buffersize_get(const Herbs::Directory& dir)
		{
		auto subdir=dir.directorySetFind(STR("buffer_size"));
		if(subdir==nullptr)
			{throw Herbs::ExceptionMissing(___FILE__,__LINE__);}

		auto dir_ptr=subdir->end();
		while(dir_ptr!=subdir->begin())
			{
			--dir_ptr;
			auto item_ptr=dir_ptr->itemsEnd();
			while(item_ptr!=dir_ptr->itemsBegin())
				{
				--item_ptr;
				return Herbs::fromString<size_t>(*item_ptr);
				}
			}
		throw Herbs::ExceptionMissing(___FILE__,__LINE__);
		}
	}

WaveRec::Application::Application(Herbs::Directory&& setup
	,Herbs::LogWriter& syslog):
	Herbs::Application(syslog)
	,ports(ports_get(setup))
	,client(ports,buffersize_get(setup))
	{
	client.activate();
	ctrl_c=&Herbs::CtrlCHandler::instanceCapture();
	}

WaveRec::Application::~Application()
	{
	Herbs::CtrlCHandler::instanceRelease();
	client.deactivate();
	}

namespace
	{
	class Output
		{
		public:
			Output(const Herbs::Path& fileout,const WaveIO::Streaminfo& info):
				file(fileout),writer(file,info)
				{}

			size_t write(const MuStudio::Audio::sample_t* buffer,size_t length)
				{return writer.write(buffer,length);}

		private:
			Herbs::FileOut file;
			WaveIO::Writer writer;
		};

	class OutputInitializer
		{
		public:
			OutputInitializer(const Herbs::Array<Herbs::String>& ports
				,MuStudio::Client& client):
				m_ports(ports.begin()),m_client(client)
				{}

			bool operator()(Output* obj,size_t n,size_t N)
				{
				new(obj)Output(Herbs::Path(m_ports->begin())|STR("wav"),
					{
					(uint32_t)(m_client.sampleRateGet())
					,1
					,WaveIO::SPEAKER_FRONT_CENTER
					});
				++m_ports;
				return 1;
				}
		private:
			const Herbs::String* m_ports;
			MuStudio::Client& m_client;
		};
	}

int WaveRec::Application::run()
	{
	sysout().write(STR("Anslut ingångar. Tryck CTRL+C för att starta."));
	ctrl_c->wait();
	sysout().write(STR("Tryck CTRL+C för att avsluta"));

	Herbs::ArrayFixedsize<Output> streams(ports.length()
		,OutputInitializer(ports,client));

	auto buffer_length=client.bufferLengthGet();
	while(!ctrl_c->statusRead())
		{
		for(size_t k=0;k<ports.length();++k)
			{
			client.wait(k);
			streams[k].write(client.bufferGet(k),buffer_length);
			}
		}

	return 0;
	}

void WaveRec::Application::destroy()
	{
	delete this;
	}
