
#include "Animation/Animation.h"
// #include "Camera/Camera.h"
// #include "Image/Image.h"
// #include "Keyboard/Keyboard.h"
// #include "Node/Node.h"
// #include "Process/Process.h"
// #include "Screen/Screen.h"
// #include "./Types.h"
// #include "ThreadWorker/ThreadWorker.h"
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
// #include "./XM/XM.h"
#include "core/Game.h"

using namespace Esperatto;

// void playSong()
// {
// 	auto xm = XM::create_context_from_file(48000, "./Test_Files/kam_-_mario_brothers.xm");
// 	auto sample = XM::copyToBuffer(xm);
// 	bool played = al_play_sample(sample, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
// 	cout << "Sample " << (played ? "" : "not ") << "played." << endl;
// }

Configuration config = Configuration({
	1,
	60,
	256,
	224,
	7.0,
	6.0
});

int main(int argc, char **args)
{
	cout << "Here" << endl;
	Game game = Game(config);

	// thread(playSong).join();

	bool escapePressed = false;
	// Keyboard keyboard;
	// keyboard.subscribe(
	// 	Keyboard::KEY_EVENTS::KEY_UP, ALLEGRO_KEY_ESCAPE, 0,
	// 	[&escapePressed](Keyboard::KEY_EVENTS e, unsigned int keycode, unsigned int keymod) {
	// 		escapePressed = true;
	// 	});

	Node last = game.rootNode;
	
	std::srand(time(NULL));
	for (int i = 0; i < 1; i++)
	{
		Node newNode(shared_ptr<Animation>(new Animation("./Test_Files/test-animation.png", 8, 2, 8)));
		newNode.setPositionInParent(0 + 32 * (i % 8), 0 + 40 * (i / 8));
		newNode.setZIndexInParent(0.01);
		newNode.setCenterOfRotation(0, 0);
		game.rootNode.addChild(newNode);

		auto *motionVector = new Coordinates();
		(*motionVector).x = 0;
		(*motionVector).y = 0;
		auto nodeptr = &newNode;
		Process p(
			[newNode, motionVector](double passedTime, ThreadWorker worker) {
				Node n(newNode);
				double hyp = sqrt(pow((*motionVector).y, 2) + pow((*motionVector).x, 2));

				double angle = asin((*motionVector).y / hyp);
				if (hyp > 0)
				{
					n.move(((*motionVector).x < 0 ? -1 : 1) * 100 * passedTime * cos(angle), 100 * passedTime * sin(angle));
				}
				return;
			});
		// w->innerQueue.push(p);

		// keyboard.subscribe(
		// 	Keyboard::KEY_EVENTS::KEY_DOWN, ALLEGRO_KEY_W, 0,
		// 	[&newNode, motionVector](Keyboard::KEY_EVENTS e, unsigned int keycode, unsigned int keymod) {
		// 		(*motionVector).y -= 1;
		// 	});
		// keyboard.subscribe(
		// 	Keyboard::KEY_EVENTS::KEY_UP, ALLEGRO_KEY_W, 0,
		// 	[&newNode, motionVector](Keyboard::KEY_EVENTS e, unsigned int keycode, unsigned int keymod) {
		// 		(*motionVector).y += 1;
		// 	});

		// keyboard.subscribe(
		// 	Keyboard::KEY_EVENTS::KEY_DOWN, ALLEGRO_KEY_S, 0,
		// 	[&newNode, motionVector](Keyboard::KEY_EVENTS e, unsigned int keycode, unsigned int keymod) {
		// 		(*motionVector).y += 1;
		// 	});
		// keyboard.subscribe(
		// 	Keyboard::KEY_EVENTS::KEY_UP, ALLEGRO_KEY_S, 0,
		// 	[&newNode, motionVector](Keyboard::KEY_EVENTS e, unsigned int keycode, unsigned int keymod) {
		// 		(*motionVector).y -= 1;
		// 	});
		// ///////
		// keyboard.subscribe(
		// 	Keyboard::KEY_EVENTS::KEY_DOWN, ALLEGRO_KEY_D, 0,
		// 	[&newNode, motionVector](Keyboard::KEY_EVENTS e, unsigned int keycode, unsigned int keymod) {
		// 		(*motionVector).x += 1;
		// 	});
		// keyboard.subscribe(
		// 	Keyboard::KEY_EVENTS::KEY_UP, ALLEGRO_KEY_D, 0,
		// 	[&newNode, motionVector](Keyboard::KEY_EVENTS e, unsigned int keycode, unsigned int keymod) {
		// 		(*motionVector).x -= 1;
		// 	});

		// keyboard.subscribe(
		// 	Keyboard::KEY_EVENTS::KEY_DOWN, ALLEGRO_KEY_A, 0,
		// 	[&newNode, motionVector](Keyboard::KEY_EVENTS e, unsigned int keycode, unsigned int keymod) {
		// 		(*motionVector).x -= 1;
		// 	});
		// keyboard.subscribe(
		// 	Keyboard::KEY_EVENTS::KEY_UP, ALLEGRO_KEY_A, 0,
		// 	[&newNode, motionVector](Keyboard::KEY_EVENTS e, unsigned int keycode, unsigned int keymod) {
		// 		(*motionVector).x += 1;
		// 	});

		last = newNode;
	}


	// std::cout << "Size: " << w->second.size() << endl;
	// Process p1(
	// 	[root](double passedTime, ThreadWorker worker) {
	// 		// cout << "Here 1" << endl;
	// 		return;
	// 	});
	// w->push(p1);
	// Process p2(
	// 	[root](double passedTime, ThreadWorker worker) {
	// 		// cout << "Here 2" << endl;
	// 		return;
	// 	});
	// w->push(p2);
	// ThreadWorker worker1(w, 1s / 120.00), worker2(w, 1s / 2);
	// worker1.take_off_standby();
	// worker2.take_off_standby();


	const int i = 256;
	Esperatto::Screen d(256, 224, 360);
	d.setFullscreen(true);
	d.setPixelStretch(7.0 / 6.0, 1);
	Camera cam(d);
	auto before = chrono::high_resolution_clock::now();
	// cam.toggleAnchor(true);

	// for (int j = 0; j < i; j++)
	// while (!escapePressed)
	// {
	// 	cam.drawToScreen(root);

		// last.move(-2, 0);
		// if (j < i / 4)
		// {
		// 	cam.move(1, 0);
		// }
		// else if (j < i / 2)
		// {
		// 	cam.rotate(3.14159 / (i));
		// }
		// else if (j < 3 * i / 4)
		// {
		// 	cam.zoomIn(0.01);
		// }
		// else
		// {
		// 	cam.zoomIn(-0.01);
		// }
	// }
	// double total = chrono::duration_cast<chrono::duration<double>>(
	// 				   chrono::high_resolution_clock::now() - before)
	// 				   .count();

	// std::cout << "Average Framerate: " << i / total << endl;

	// usleep(10000000);

	al_uninstall_audio();

	return 0;
}