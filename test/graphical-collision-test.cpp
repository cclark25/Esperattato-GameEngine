#include "../src/library/core/Game.h"
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
									  6.0, al_map_rgb(128, 128, 128)});

int main()
{
	Game game = Game(config);
	game.keyboard.subscribe(KEY_EVENTS::KEY_UP, ALLEGRO_KEY_ESCAPE, 0, [&game](KEY_EVENTS e, unsigned int keycode, unsigned int keymodFlags)
							{ game.StopGame(); });

	double velocityBasis = 50;

	CollisionSquareNode first = CollisionSquareNode(
		Coordinates({0, 0}), Coordinates({42, -49}), true);
	CollisionSquare secondSquare = CollisionSquare(Coordinates({10, 0}), Coordinates({32, -49}), true);
	CollisionSquare thirdSquare = CollisionSquare(Coordinates({0, -12}), Coordinates({42, -37}), true);
	first.addSubSquare(secondSquare);
	first.addSubSquare(thirdSquare);

	auto a = Image("test/Test_Files/blue-plus.png");
	first.addChild(a);
	a.setZIndexInParent(-1);
	game.rootNode.addChild(first);
	Camera cam1(game.display);
	game.camera = cam1;
	first.addChild(game.camera);
	first.setPositionInParent(0, 0);
	first.setZIndexInParent(1);
	game.camera.setPositionInParent(-51, 43);

	vector<CollisionSquareNode *> obstacles;
	for (int i = 1; i <= 1; i++)
	{
		CollisionSquareNode *second = new CollisionSquareNode(
			Coordinates({0, 0}), Coordinates({50, -50}), true);
		// CollisionSquare secondSquare = CollisionSquare(Coordinates({0, 0}), Coordinates({25, -25}), true);
		// secondSquare.addSubSquare(CollisionSquare(Coordinates({0, 0}), Coordinates({12, -12}), true));
		// second->addSubSquare(secondSquare);

		auto b = new Image("test/Test_Files/red-box.png");
		b->setZIndexInParent(-1);
		second->addChild(*b);
		game.rootNode.addChild(*second);
		second->setPositionInParent(101 * i, -101 * i);
		obstacles.push_back(second);
	}

	Physics physics;
	Process mover = Process([&first, &physics, obstacles](double time, ThreadWorker worker)
							{
								Coordinates direction = physics.getDifference();
								auto previousPosition = first.getPositionInParent();
								first.setPositionInParent(direction.x, direction.y);
								for (CollisionSquareNode *obstacle : obstacles)
								{
									if (first.Intersects(*obstacle))
									{
										physics.setX(previousPosition.x);
										physics.setY(previousPosition.y);
										first.setPositionInParent(previousPosition.x, previousPosition.y);
										return;
									}
								}
							});

	Physics cameraPhysics;
	Process cameraMover = Process([&game, &cameraPhysics](double time, ThreadWorker worker)
								  {
									  Coordinates direction = cameraPhysics.getDifference();
									  game.camera.setPositionInParent(direction.x, direction.y);
								  });
	cameraPhysics.setYVelocity(-velocityBasis / 2);

	game.threadWork->innerQueue.push(mover);
	// game.threadWork->innerQueue.push(cameraMover);

	KeySubscription toggleHitboxes = {[&first](KEY_EVENTS e, unsigned int keycode,
															unsigned int keymodFlags)
						  {
							  first.toggleDrawBoxes(true);
						  },
						  [&first, &physics, velocityBasis](KEY_EVENTS e, unsigned int keycode,
															unsigned int keymodFlags)
						  {
							  first.toggleDrawBoxes(false);
						  }};
	game.keyboard.subscribeToggle(ALLEGRO_KEY_T, 0, toggleHitboxes);

	KeySubscription up = {[&first, &physics, velocityBasis](KEY_EVENTS e, unsigned int keycode,
															unsigned int keymodFlags)
						  {
							  physics.addYVelocity(velocityBasis);
						  },
						  [&first, &physics, velocityBasis](KEY_EVENTS e, unsigned int keycode,
															unsigned int keymodFlags)
						  {
							  physics.addYVelocity(-velocityBasis);
						  }};
	game.keyboard.subscribeToggle(ALLEGRO_KEY_W, 0, up);
	KeySubscription down = {[&first, &physics, velocityBasis](KEY_EVENTS e, unsigned int keycode,
															  unsigned int keymodFlags)
							{
								physics.addYVelocity(-velocityBasis);
							},
							[&first, &physics, velocityBasis](KEY_EVENTS e, unsigned int keycode,
															  unsigned int keymodFlags)
							{
								physics.addYVelocity(velocityBasis);
							}};
	game.keyboard.subscribeToggle(ALLEGRO_KEY_S, 0, down);
	KeySubscription right = {[&first, &physics, velocityBasis](KEY_EVENTS e, unsigned int keycode,
															   unsigned int keymodFlags)
							 {
								 physics.addXVelocity(velocityBasis);
							 },
							 [&first, &physics, velocityBasis](KEY_EVENTS e, unsigned int keycode,
															   unsigned int keymodFlags)
							 {
								 physics.addXVelocity(-velocityBasis);
							 }};
	game.keyboard.subscribeToggle(ALLEGRO_KEY_D, 0, right);
	KeySubscription left = {[&first, &physics, velocityBasis](KEY_EVENTS e, unsigned int keycode,
															  unsigned int keymodFlags)
							{
								physics.addXVelocity(-velocityBasis);
							},
							[&first, &physics, velocityBasis](KEY_EVENTS e, unsigned int keycode,
															  unsigned int keymodFlags)
							{
								physics.addXVelocity(velocityBasis);
							}};
	game.keyboard.subscribeToggle(ALLEGRO_KEY_A, 0, left);

	first.setZIndexInParent(0.01);
	first.setPositionInParent(0, 0);

	game.rootNode.setPositionInParent(0, 0);

	game.StartGame();

	return 0;
}