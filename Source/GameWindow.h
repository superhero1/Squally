#pragma once
#include "audio/include/AudioEngine.h"
#include "cocos2d.h"

#include "Bootstrapper.h"
#include "Engine/Config/ConfigManager.h"

using namespace cocos2d;
using namespace cocos_experimental;

/**
@brief The cocos2d Application.

Private inheritance here hides part of interface from Director.
*/
class  GameWindow : private Application
{
public:
	GameWindow();
	virtual ~GameWindow();

	virtual void initGLContextAttrs();

	/**
	@brief Implement Director and Scene init code here.
	@return true Initialize success, app continue.
	@return false Initialize failed, app terminate.
	*/
	virtual bool applicationDidFinishLaunching();

	/**
	@brief Called when the application moves to the background.
	@param The pointer of the application.
	*/
	virtual void applicationDidEnterBackground();

	/**
	@brief Called when the application reenters the foreground.
	@param The pointer of the application.
	*/
	virtual void applicationWillEnterForeground();

private:
	std::string GameWindowTitle = "Squally";
};