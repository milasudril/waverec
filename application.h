#ifdef __WAND__
target[name[application.h] type[include]]
dependency[application.o]
#endif

#ifndef WAVEREC_APPLICATION_H
#define WAVEREC_APPLICATION_H

#include "audio_client.h"
#include <herbs/application/application.h>
#include <herbs/array/array.h>

namespace Herbs
	{
	class CtrlCHandler;

	template<class T>
	class Stringbase;

	typedef Stringbase<char_t> String;
	}

namespace WaveRec
	{
	class Application:public Herbs::Application
		{
		public:
			Application(Herbs::Directory&& setup,Herbs::LogWriter& syslog);
			int run();
			void destroy();

			~Application();

		private:
			Herbs::Array<Herbs::String> ports;
			AudioClient client;
			Herbs::CtrlCHandler* ctrl_c;
		};
	}

#endif
