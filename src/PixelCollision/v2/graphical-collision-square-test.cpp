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

Configuration config = Configuration({
	1,
	60,
	256,
	224,
	7.0,
	6.0
});

int main() {
	Game game = Game(config);

	auto y =
	    Node(std::shared_ptr<Animation>(new Animation("src/Test_Files/test-animation.png", 8, 1, 8)));
	game.rootNode.addChild(y);

	y.setZIndexInParent(0.01);
	y.setPositionInParent(0, 0);

	
	game.rootNode.setPositionInParent(0, 0);
	game.camera.setPosition(0, 0);

	game.StartGame();

	
	return 0;
}