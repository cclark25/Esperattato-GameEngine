#include "../core/Game.h"
#include "cmath"
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
								y.setPositionInParent(direction.x,direction.y);
							});

	game.threadWork->innerQueue.push(mover);

	KeySubscription up = {[&y, &physics, velocityBasis](KEY_EVENTS e, unsigned int keycode,
														unsigned int keymodFlags)
						  {
							  physics.addYVelocity(-velocityBasis);
						  },
						  [&y, &physics, velocityBasis](KEY_EVENTS e, unsigned int keycode,
														unsigned int keymodFlags)
						  {
							  physics.addYVelocity(velocityBasis);
						  }};
	game.keyboard.subscribeToggle(ALLEGRO_KEY_W, 0, up);
	KeySubscription down = {[&y, &physics, velocityBasis](KEY_EVENTS e, unsigned int keycode,
														  unsigned int keymodFlags)
							{
								physics.addYVelocity(velocityBasis);
							},
							[&y, &physics, velocityBasis](KEY_EVENTS e, unsigned int keycode,
														  unsigned int keymodFlags)
							{
								physics.addYVelocity(-velocityBasis);
							}};
	game.keyboard.subscribeToggle(ALLEGRO_KEY_S, 0, down);
	KeySubscription right = {[&y, &physics, velocityBasis](KEY_EVENTS e, unsigned int keycode,
														   unsigned int keymodFlags)
							 {
								 physics.addXVelocity(velocityBasis);
							 },
							 [&y, &physics, velocityBasis](KEY_EVENTS e, unsigned int keycode,
														   unsigned int keymodFlags)
							 {
								 physics.addXVelocity(-velocityBasis);
							 }};
	game.keyboard.subscribeToggle(ALLEGRO_KEY_D, 0, right);
	KeySubscription left = {[&y, &physics, velocityBasis](KEY_EVENTS e, unsigned int keycode,
														  unsigned int keymodFlags)
							{
								physics.addXVelocity(-velocityBasis);
							},
							[&y, &physics, velocityBasis](KEY_EVENTS e, unsigned int keycode,
														  unsigned int keymodFlags)
							{
								physics.addXVelocity(velocityBasis);
							}};
	game.keyboard.subscribeToggle(ALLEGRO_KEY_A, 0, left);

	y.setZIndexInParent(0.01);
	y.setPositionInParent(0, 0);

	game.rootNode.setPositionInParent(0, 0);
	game.camera.setPositionInParent(0, 0);

	game.StartGame();

	return 0;
}