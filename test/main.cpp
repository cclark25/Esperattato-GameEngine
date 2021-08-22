
#include "./Types.h"
#include "./XM/XM.h"
#include "Animation/Animation.h"
#include "Camera/Camera.h"
#include "Image/Image.h"
#include "Keyboard/Keyboard.h"
#include "Node/Node.h"
#include "PixelCollision/CollisionTree.h"
#include "PixelCollision/PixelCollision.h"
#include "Process/Process.h"
#include "Screen/Screen.h"
#include "ThreadWorker/ThreadWorker.h"
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

bool test(CollisionTree &tree, int desiredLevel, int level = 0,
          double totalSpacing = 0) {
	if (totalSpacing == 0) {
		totalSpacing =
		    std::pow(2, std::ceil(std::log2((double)tree.collisionCount))) * 9;
	}

	if (level == desiredLevel) {
		cout << " ";

		cout << setw(3) << setfill('0') << tree.first.x << "\t( left: "
		     << (tree.lessThan == nullptr ? "null"
		                                  : to_string(tree.lessThan->first.x))
		     << ", total length: "
		     << (tree.lessThan == nullptr ? 0 : tree.lessThan->getCount())
		     << " )\t"

		     << "( right: "
		     << (tree.greaterThanEqualTo == nullptr
		             ? "null"
		             : to_string(tree.greaterThanEqualTo->first.x))
		     << ", total length: "
		     << (tree.greaterThanEqualTo == nullptr
		             ? 0
		             : tree.greaterThanEqualTo->getCount())
		     << " )\t";
		return true;
	}

	bool success = false;
	if (tree.lessThan != nullptr) {
		success |= test(*tree.lessThan, desiredLevel, level + 1, totalSpacing);
	}
	if (tree.greaterThanEqualTo != nullptr) {
		success |= test(*tree.greaterThanEqualTo, desiredLevel, level + 1,
		                totalSpacing);
	}

	return success;
}

int main(int argc, char **args) {
	al_init();

	al_init_image_addon();
	al_init_primitives_addon();
	al_install_audio();

	al_reserve_samples(1);

	auto x = PixelCollision("./Test_Files/Moma.png");
	auto y = CollisionTree({1, 1}, {10, 10}, true);

	for (int i = 2; i <= 100; i++) {
		y.addOne({i, i}, {10 * i, 10 * i});
		// test(y, i);
		for (int i = 0; test(y, i); i++) {
			cout << endl;
		}
		cout << "##########################" << endl;
	}

	// for (int i = 0; test(y, i); i++) {
	// 	cout << endl;
	// }

	al_uninstall_audio();

	return 0;
}
