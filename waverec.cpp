#ifdef __WAND__
target[name[waverec.exe] type[application] platform[;Windows]]
target[name[waverec] type[application]]
#endif

#include "application.h"
#include <herbs/maindefault/maindefault.h>

Herbs::Application* Herbs::Application::instanceCreate(
	Herbs::LogWriter& syslog,Herbs::Directory&& setup)
	{
	return new WaveRec::Application(std::move(setup),syslog);
	}
