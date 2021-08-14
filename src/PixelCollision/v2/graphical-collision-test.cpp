#include "../../core/Game.h"
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

	Node first = Node(make_shared<CollisionSquare>(
		Coordinates({0, 0}), Coordinates({50, 50}), true));
	Node second = Node(make_shared<CollisionSquare>(
		Coordinates({0, 0}), Coordinates({50, 50}), true));
	auto firstSquare = *((CollisionSquare *)first.getDataPtr().get());
	auto secondSquare = *((CollisionSquare *)second.getDataPtr().get());

	auto a =
		Node(std::shared_ptr<Image>(new Image("src/Test_Files/green-box.png")));
	auto b =
		Node(std::shared_ptr<Image>(new Image("src/Test_Files/red-box.png")));
	first.addChild(a);
	second.addChild(b);
	game.rootNode.addChild(first);

	Camera cam1(game.display);
	game.camera = cam1;

	first.addChild(cam1);

	game.rootNode.addChild(second);

	first.setPositionInParent(0, 0);
	second.setPositionInParent(100, -100);

	Physics physics;
	Process mover = Process([&first, &physics, &firstSquare, &secondSquare](double time, ThreadWorker worker)
							{
								Coordinates direction = physics.getDifference();
								auto previousPosition = first.getPositionInParent();
								first.setPositionInParent(direction.x, direction.y);
								if (firstSquare.Intersects(secondSquare))
								{
									physics.setX(previousPosition.x);
									physics.setY(previousPosition.y);
									first.setPositionInParent(previousPosition.x, previousPosition.y);
								}
							});

	Physics cameraPhysics;
	Process cameraMover = Process([&cam1, &cameraPhysics, &firstSquare, &secondSquare](double time, ThreadWorker worker)
								  {
									  Coordinates direction = cameraPhysics.getDifference();
									  cam1.setPositionInParent(direction.x, direction.y);
								  });
	cameraPhysics.setYVelocity(-velocityBasis / 2);

	game.threadWork->innerQueue.push(mover);
	// game.threadWork->innerQueue.push(cameraMover);

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
	cam1.setPositionInParent(0, 0);

	game.StartGame();

	return 0;
}