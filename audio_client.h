#ifdef __WAND__
target[name[audio_client.h] type [include]]
dependency[audio_client.o]
#endif

#ifndef WAVEREC_AUDIOCLIENT_H
#define WAVEREC_AUDIOCLIENT_H

#include <mustudio/client.h>
#include <mustudio/audio_sampletype.h>
#include <herbs/chartype/chartype.h>
#include <herbs/array_fixedsize/array_fixedsize.h>

namespace Herbs
	{
	template<class T>
	class Stringbase;
	typedef Stringbase<char_t> String;

	template<class T>
	class Array;
	}

namespace WaveRec
	{
	class AudioPort;

	class AudioClient:public MuStudio::Client
		{
		public:
			AudioClient(const Herbs::Array<Herbs::String>& ports
				,size_t buffer_length);
			~AudioClient();

			int onProcess(size_t nframes);

			size_t bufferLengthGet() const
				{return m_buffer_length;}

			const MuStudio::Audio::sample_t* bufferGet(size_t k);

			void wait(size_t k);

		private:
			Herbs::ArrayFixedsize<AudioPort> ports;
			size_t m_buffer_length;
		};

	}

#endif
