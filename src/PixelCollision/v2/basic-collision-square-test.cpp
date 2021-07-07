#define DEBUG a
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
	Node first = Node(make_shared<CollisionSquare>(
	    Coordinates({0, 0}), Coordinates({a2.x - a1.x, a2.y - a1.y}), true));
	Node second = Node(make_shared<CollisionSquare>(
	    Coordinates({0, 0}), Coordinates({b2.x - b1.x, b2.y - b1.y}), true));
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

	// Test a complete overlap.
	testIntersection({0, 0}, {10, 10}, {0, 0}, {10, 10}, true);

	// Test partial complete overlaps.
	testIntersection({0, 0}, {10, 10}, {1, 0}, {10, 10}, true);
	testIntersection({0, 0}, {10, 10}, {0, 1}, {10, 10}, true);
	testIntersection({0, 0}, {10, 10}, {1, 1}, {10, 10}, true);
	testIntersection({0, 0}, {10, 10}, {0, 0}, {9, 10}, true);
	testIntersection({0, 0}, {10, 10}, {0, 0}, {10, 9}, true);
	testIntersection({0, 0}, {10, 10}, {0, 0}, {9, 9}, true);

	// Test full inter overlap.
	testIntersection({0, 0}, {10, 10}, {1, 1}, {9, 9}, true);

	// Test partial outer overlap.
	testIntersection({0, 0}, {10, 10}, {-1, 0}, {10, 10}, true);
	testIntersection({0, 0}, {10, 10}, {0, -1}, {10, 10}, true);
	testIntersection({0, 0}, {10, 10}, {-1, -1}, {10, 10}, true);
	testIntersection({0, 0}, {10, 10}, {0, 0}, {10, 11}, true);
	testIntersection({0, 0}, {10, 10}, {0, 0}, {11, 10}, true);
	testIntersection({0, 0}, {10, 10}, {0, 0}, {11, 11}, true);

	// Test complete outer overlap.
	testIntersection({0, 0}, {10, 10}, {-1, -1}, {11, 11}, true);

	// Test non-intersections.

	testIntersection({0, 0}, {10, 10}, {11, 0}, {20, 10}, false);
	testIntersection({0, 0}, {10, 10}, {0, 11}, {10, 20}, false);
	testIntersection({0, 0}, {10, 10}, {11, 11}, {21, 21}, false);

	// Test barely intersecting.

	testIntersection({0, 0}, {10, 10}, {10, 0}, {20, 10}, true);
	testIntersection({0, 0}, {10, 10}, {0, 10}, {10, 20}, true);

	std::cout << "Test completed." << std::endl;

	al_uninstall_audio();

	return 0;
}
