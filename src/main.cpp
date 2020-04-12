
#include "Animation/Animation.h"
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
#include <chrono>
#include <cmath>
#include <functional>
#include <iostream>
#include <map>
#include <unistd.h>
#include <memory>

using namespace Esperatto;

int main(int argc, char **args) {
	al_init();
	
	al_init_image_addon();
	al_init_primitives_addon();

	Keyboard keyboard;

	keyboard.subscribe(
	    Keyboard::KEY_EVENTS::KEY_DOWN, ALLEGRO_KEY_H, 0,
	    [](Keyboard::KEY_EVENTS e, unsigned int keycode, unsigned int keymod) {
			std::cout << "'H' button pressed down." << endl;
	    });
	keyboard.subscribe(
	    Keyboard::KEY_EVENTS::KEY_DOWN, ALLEGRO_KEY_H, 0,
	    [](Keyboard::KEY_EVENTS e, unsigned int keycode, unsigned int keymod) {
			std::cout << "'H' button pressed down, but second function." << endl;
	    });
	keyboard.subscribe(
	    Keyboard::KEY_EVENTS::KEY_UP, ALLEGRO_KEY_H, 0,
	    [](Keyboard::KEY_EVENTS e, unsigned int keycode, unsigned int keymod) {
			std::cout << "'H' button released." << endl;
	    });

	Node root = Node(std::shared_ptr<char>(new char(1)));

	root.setPositionInParent(0, 0);
	root.setCenterOfRotation(128, 112);

	Esperatto::Screen d(256, 224, 360);
	Node last = root;
	shared_ptr<ThreadWork> w = shared_ptr<ThreadWork>(new ThreadWork());
	std::srand(time(NULL));
	for (int i = 0; i < 20; i++) {
		Node newNode(shared_ptr<Animation>(new Animation("./Test_Files/Down.png", 1, 1, 1)));
		newNode.setPositionInParent(128, 112);
		newNode.setZIndexInParent(0.01);
		newNode.setCenterOfRotation(0, 0);
		root.addChild(newNode);
		int random = rand(), random2 = rand();

		Process p(
		    [newNode, random, random2](double passedTime, ThreadWorker worker) {
			    const double pixelPerSecond = 100 * ((random2 % 5) / 2.0);
			    Node n(newNode);

			    n.move(sin(random) * passedTime * pixelPerSecond,
			           cos(random) * passedTime * pixelPerSecond);

			    return;
		    });
		w->second.push(p);

		last = newNode;
	}
	std::cout << "Size: " << w->second.size() << endl;

	ThreadWorker worker(w);
	worker.take_off_standby();

	const int i = 256;
	d.setFullscreen(true);
	d.setPixelStretch(7.0 / 6.0, 1);
	Camera cam(d);
	auto before = chrono::high_resolution_clock::now();
	cam.toggleAnchor(false);
	for (int j = 0; j < i; j++) {
		cam.drawToScreen(root);
		// last.move(-2, 0);
		if (j < i / 4) {
			cam.move(1, 0);
		} else if (j < i / 2) {
			cam.rotate(3.14159 / (i));
		} else if (j < 3 * i / 4) {
			cam.zoomIn(0.01);
		} else {
			cam.zoomIn(-0.01);
		}
	}
	double total = chrono::duration_cast<chrono::duration<double>>(
	                   chrono::high_resolution_clock::now() - before)
	                   .count();

	std::cout << "Average Framerate: " << i / total << endl;
	
	return 0;
}