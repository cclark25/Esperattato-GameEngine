
#include "Animation/Animation.h"
#include "PixelCollision/PixelCollision.h"
#include "PixelCollision/CollisionTree.h"
#include "Camera/Camera.h"
#include "Image/Image.h"
#include "Keyboard/Keyboard.h"
#include "Node/Node.h"
#include "Process/Process.h"
#include "Screen/Screen.h"
#include "./Types.h"
#include "ThreadWorker/ThreadWorker.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <chrono>
#include <cmath>
#include <functional>
#include <iostream>
#include <map>
#include <unistd.h>
#include <memory>
#include "./XM/XM.h"

using namespace Esperatto;

bool test(CollisionTree &tree, int desiredLevel, int level = 0)
{
	if (level == desiredLevel)
	{
		cout << "At level #" << level << ": x=" << tree.first.x << endl;
		return true;
	}

	bool success = false;
	if (tree.lessThan != nullptr)
	{
		success |= test(*tree.lessThan, desiredLevel, level + 1);
	}
	if (tree.greaterThanEqualTo != nullptr)
	{
		success |= test(*tree.greaterThanEqualTo, desiredLevel, level + 1);
	}

	return success;
}

int main(int argc, char **args)
{
	al_init();

	al_init_image_addon();
	al_init_primitives_addon();
	al_install_audio();

	al_reserve_samples(1);

	auto x = PixelCollision("./Test_Files/Moma.png");
	auto y = CollisionTree({1, 1}, {10, 10}, true);

	for (int i = 2; i <= 100; i++)
	{
		y.addOne({i, i}, {10 * i, 10 * i});
	}

	for (int i = 0; test(y, i); i++)
	{
	}

	al_uninstall_audio();

	return 0;
}
