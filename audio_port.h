#ifdef __WAND__
target[name[audio_port.h] type[include]]
dependency[audio_port.o]
#endif

#ifndef WAVEREC_AUDIOPORT_H
#define WAVEREC_AUDIOPORT_H

#include <mustudio/audio_input_exported.h>
#include <herbs/array_fixedsize/array_fixedsize.h>
#include <herbs/eventauto/eventauto.h>
#include <herbs/chartype/chartype.h>

namespace Herbs
	{
	template<class T>
	class Stringbase;
	typedef Stringbase<char_t> String;
	}

namespace WaveRec
	{
	class AudioPort
		{
		public:
			AudioPort(MuStudio::Client& client,const Herbs::String port_name
				,size_t size_buffer);

			void wait()
				{data_ready.wait();}

			void readySet()
				{data_ready.set();}

			const MuStudio::Audio::sample_t* bufferBackGet()
				{return buffer_back->begin();}

			void buffersSwap()
				{std::swap(buffer_front,buffer_back);}

			void positionReset()
				{position=buffer_front->begin();}

			void positionIncrement()
				{++position;}

			MuStudio::Audio::sample_t* positionGet()
				{return position;}

			MuStudio::Audio::sample_t* positionEnd()
				{return buffer_front->end();}

			const MuStudio::Audio::sample_t* bufferGet(size_t n_frames)
				{return port.bufferGet(n_frames);}

		private:
			Herbs::ArrayFixedsize<MuStudio::Audio::sample_t> buffer_a;
			Herbs::ArrayFixedsize<MuStudio::Audio::sample_t> buffer_b;
			Herbs::ArrayFixedsize<MuStudio::Audio::sample_t>* buffer_front;
			Herbs::ArrayFixedsize<MuStudio::Audio::sample_t>* buffer_back;
			MuStudio::Audio::sample_t* position;
			MuStudio::Audio::InputExported port;
			Herbs::EventAuto data_ready;
		};
	}

#endif
