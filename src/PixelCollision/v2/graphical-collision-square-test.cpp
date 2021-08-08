#include "../../Animation/Animation.h"
#include "../../Camera/Camera.h"
#include "../../Image/Image.h"
#include "../../Keyboard/Keyboard.h"
#include "../../Node/Node.h"
#include "../../PixelCollision/CollisionTree.h"
#include "../../PixelCollision/PixelCollision.h"
#include "../../Process/Process.h"
#include "../../Screen/Screen.h"
#include "../../ThreadWorker/ThreadWorker.h"
#include "../../Types.h"
#include "../../XM/XM.h"
#include "../../core/Game.h"
#include "../../Physics/headers/Physics.h"
#include "cmath"
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <chrono>
#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <unistd.h>

using namespace Esperatto;

Configuration config = Configuration({1,
									  60,
									  256,
									  224,
									  7.0,
									  6.0});

int main()
{
	Game game = Game(config);
	game.keyboard.subscribe(KEY_EVENTS::KEY_UP, ALLEGRO_KEY_ESCAPE, 0, [&game](KEY_EVENTS e, unsigned int keycode, unsigned int keymodFlags)
							{ game.StopGame(); });

	double velocityBasis = 50;

	auto y =
		Node(std::shared_ptr<Animation>(new Animation("src/Test_Files/test-animation.png", 8, 1, 8)));
	game.rootNode.addChild(y);

	Physics physics;
	Process mover = Process([&y, &physics](double time, ThreadWorker worker)
							{
								Coordinates direction = physics.getDifference();
								y.move(direction.x, direction.y);
							});

	game.threadWork->innerQueue.push(mover);

	KeySubscription up = {[&y, &physics, velocityBasis](KEY_EVENTS e, unsigned int keycode,
										 unsigned int keymodFlags)
						  {
							  physics.yVelocity += -velocityBasis;
						  },
						  [&y, &physics, velocityBasis](KEY_EVENTS e, unsigned int keycode,
										 unsigned int keymodFlags)
						  {
							  physics.yVelocity -= -velocityBasis;
						  }};
	game.keyboard.subscribeToggle(ALLEGRO_KEY_W, 0, up);
	KeySubscription down = {[&y, &physics, velocityBasis](KEY_EVENTS e, unsigned int keycode,
										 unsigned int keymodFlags)
						  {
							  physics.yVelocity += velocityBasis;
						  },
						  [&y, &physics, velocityBasis](KEY_EVENTS e, unsigned int keycode,
										 unsigned int keymodFlags)
						  {
							  physics.yVelocity -= velocityBasis;
						  }};
	game.keyboard.subscribeToggle(ALLEGRO_KEY_S, 0, down);
	KeySubscription right = {[&y, &physics, velocityBasis](KEY_EVENTS e, unsigned int keycode,
										 unsigned int keymodFlags)
						  {
							  physics.xVelocity += velocityBasis;
						  },
						  [&y, &physics, velocityBasis](KEY_EVENTS e, unsigned int keycode,
										 unsigned int keymodFlags)
						  {
							  physics.xVelocity -= velocityBasis;
						  }};
	game.keyboard.subscribeToggle(ALLEGRO_KEY_D, 0, right);
	KeySubscription left = {[&y, &physics, velocityBasis](KEY_EVENTS e, unsigned int keycode,
										 unsigned int keymodFlags)
						  {
							  physics.xVelocity += -velocityBasis;
						  },
						  [&y, &physics, velocityBasis](KEY_EVENTS e, unsigned int keycode,
										 unsigned int keymodFlags)
						  {
							  physics.xVelocity -= -velocityBasis;
						  }};
	game.keyboard.subscribeToggle(ALLEGRO_KEY_A, 0, left);

	
	y.setZIndexInParent(0.01);
	y.setPositionInParent(0, 0);

	game.rootNode.setPositionInParent(0, 0);
	game.camera.setPosition(0, 0);

	// game.StartGame();

	auto changeOfAccel = RateOfChange(10);
	auto acceleration = RateOfChange(changeOfAccel);
	RateOfChange velocity(acceleration);

	cout << "Value: " << velocity.getResult(1) << endl;

	return 0;
}