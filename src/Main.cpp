#include "stdafx.h"
#include "TestAppDelegate.h"

#define MYAPPLICATION_NAME L"Test"

#if defined(ENGINE_TARGET_WIN32)
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main(int argc, const char* argv[])
#endif
{
	ParticleSystem::SetTexturesPath("textures/Particles");

#if defined(ENGINE_TARGET_WIN32)
	Core::fileSystem.SetWriteDirectory("./write_directory");
#else
    Core::fileSystem.SetWriteDirectory(IO::Path::GetSpecialFolderPath(SpecialFolder::LocalDocuments));
#endif
    
#if defined(ENGINE_TARGET_WIN32)
    std::string base_path = "base_p";
#else
    std::string base_dir = "Contents/Resources/base_p";
    std::string work_dir = std::string([[[NSBundle mainBundle] bundlePath] UTF8String]);
    std::string base_path = IO::Path::Combine(work_dir, base_dir);
#endif
    
    Core::fileSystem.MountDirectory(base_path);

    Log::log.AddSink(new Log::DebugOutputLogSink());
	Log::log.AddSink(new Log::HtmlFileLogSink("log.htm", true));

#if defined(ENGINE_TARGET_WIN32)
	Core::Application::APPLICATION_NAME = MYAPPLICATION_NAME;
	Core::RunApplicationWithDelegate(new TestAppDelegate());
#else
	Core::RunApplicationWithDelegate(argc, argv, new TestAppDelegate());
#endif
	
	return 0;
}
