
#include "../.././Types.h"
#include "../.././XM/XM.h"
#include "../../Animation/Animation.h"
#include "../../Camera/Camera.h"
#include "../../Image/Image.h"
#include "../../Keyboard/Keyboard.h"
#include "../../Node/Node.h"
#include "../../PixelCollision/v2/collisionSquare.h"
#include "../../Process/Process.h"
#include "../../Screen/Screen.h"
#include "../../ThreadWorker/ThreadWorker.h"
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

void testIntersection(Esperatto::Coordinates a1, Esperatto::Coordinates a2,
                      Esperatto::Coordinates b1, Esperatto::Coordinates b2,
                      bool shouldIntersect) {
	Node first = Node(shared_ptr<CollisionSquare>(new CollisionSquare(
	    Coordinates({0, 0}), Coordinates({a2.x - a1.x, a2.y - a1.y}), true)));
	Node second = Node(shared_ptr<CollisionSquare>(new CollisionSquare(
	    Coordinates({0, 0}), Coordinates({b2.x - b1.x, b2.y - b1.y}), true)));
	auto firstSquare = *((CollisionSquare *)first.getDataPtr().get());
	auto secondSquare = *((CollisionSquare *)second.getDataPtr().get());

	first.setPositionInParent(a1.x, a1.y);
	second.setPositionInParent(b1.x, b1.y);

	bool intersects = firstSquare.Intersects(secondSquare);
	bool inverseIntersects = firstSquare.Intersects(secondSquare);

	if (intersects != inverseIntersects) {
		throw std::logic_error(
		    "Square ({" + to_string(a1.x) + ", " + to_string(a1.y) + "}, {" +
		    to_string(a2.x) + ", " + to_string(a2.y) + "})" +
		    (intersects ? " intersects " : " does not intersect ") + "({" +
		    to_string(b1.x) + ", " + to_string(b1.y) + "}, {" +
		    to_string(b2.x) + ", " + to_string(b2.y) +
		    "}) but not vice-versa.");
	}

	if (intersects != shouldIntersect) {
		throw std::logic_error(
		    "Square ({" + to_string(a1.x) + ", " + to_string(a1.y) + "}, {" +
		    to_string(a2.x) + ", " + to_string(a2.y) + "}) should" +
		    (shouldIntersect ? " intersect " : " not intersect ") + "({" +
		    to_string(b1.x) + ", " + to_string(b1.y) + "}, {" +
		    to_string(b2.x) + ", " + to_string(b2.y) + "}).");
	}
}

int main(int argc, char **args) {
	al_init();

	al_init_image_addon();
	al_init_primitives_addon();
	al_install_audio();

	al_reserve_samples(1);

	auto firstSquare = CollisionSquare(
	    Coordinates({0, 0}), Coordinates({10,10}), true);

	auto secondSquare = CollisionSquare(
	    Coordinates({0, 0}), Coordinates({10,10}), true);

	auto thirdSquare = CollisionSquare({0,0}, {5,5}, true);
	auto fourthSquare = CollisionSquare({6,6}, {10,10}, true);

	firstSquare.addSubSquare(thirdSquare);
	secondSquare.addSubSquare(fourthSquare);

	std::cout << (firstSquare.Intersects(secondSquare) ? "Intersects" : "Does not intersect") << std::endl;
	std::cout << (thirdSquare.Intersects(fourthSquare) ? "Intersects" : "Does not intersect") << std::endl;
	std::cout << thirdSquare.toString() << std::endl;
	std::cout << fourthSquare.toString() << std::endl;

	al_uninstall_audio();

	return 0;
}
