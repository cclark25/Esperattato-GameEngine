#ifndef ESPERATTO_GAME
#define ESPERATTO_GAME
#include "../Node/Node.h"
#include "../Node/NodeSubTypes.h"
#include "../ThreadWorker/ThreadWorker.h"
#include "../Camera/Camera.h"
#include "../Keyboard/Keyboard.h"
#include "../Animation/Animation.h"
#include "../Camera/Camera.h"
#include "../Image/Image.h"
#include "../Keyboard/Keyboard.h"
#include "../Node/Node.h"
#include "../PixelCollision/CollisionTree.h"
#include "../PixelCollision/PixelCollision.h"
#include "../Process/Process.h"
#include "../Screen/Screen.h"
#include "../ThreadWorker/ThreadWorker.h"
#include "../Types.h"
#include "../XM/XM.h"
#include "../core/Game.h"
#include "../Physics/headers/Physics.h"
#include "../PixelCollision/v2/collisionSquare.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <vector>

namespace _Esperatto {
	class RootNode : public Esperatto::Subdata {};
} // namespace _Esperatto

namespace Esperatto {
	struct Configuration {
		/**
		 * @param workerCount - Number of ThreadWorkers to create to process
		 * data in the game.
		 */
		unsigned int workerCount;
		/**
		 * @param framesPerSecond - Target frames per second to run the game in.
		 */
		unsigned int framesPerSecond;
		/**
		 * @param screenWidth - Width of the screen in pixels.
		 */
		unsigned int screenWidth;
		/**
		 * @param screenHeight - Height of the screen in pixels.
		 */
		unsigned int screenHeight;

		/**
		 * @param screenPixelStretchX - Stretch each pixel horizontally by this factor.
		 * @param screenPixelStretchY - Stretch each pixel vertically by this factor.
		 * 
		 * Note: a ratio of 7/6 would emulate the SNES's graphic capabilities.
		 */
		double screenPixelStretchX;
		double screenPixelStretchY;
	};
	class Game {
	  private:
		vector<ThreadWorker> workers;
		bool shouldStop = false;
		Configuration config;
	  public:
		Node rootNode =
		    Node(shared_ptr<_Esperatto::RootNode>(new _Esperatto::RootNode()));
		shared_ptr<ThreadWork> threadWork =
		    shared_ptr<ThreadWork>(new ThreadWork());
		Esperatto::Screen display;
		Camera camera;
		Keyboard keyboard;

		Game(Configuration &config);
		void StartGame();
		void StopGame();
	};
} // namespace Esperatto

#endif