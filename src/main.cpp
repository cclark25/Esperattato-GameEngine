// #include "./ThreadPool/ThreadWorker.h"
// #include "./ThreadPool/Process.h"
#include "Camera/Camera.h"
#include "Image/Image.h"
#include "Node/Node.h"
#include "Process/Process.h"
#include "Screen/Screen.h"
#include "ThreadWorker/ThreadWorker.h"
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <chrono>
#include <cmath>
#include <functional>
#include <iostream>
#include <map>
#include <unistd.h>

using namespace Esperatto;

int main(int argc, char **args) {
	al_init();
	al_init_image_addon();
	al_init_primitives_addon();

	Node root = Node(new char(1));

	root.setPositionInParent(0, 0);
	root.setCenterOfRotation(128, 112);

	Esperatto::Screen d(256, 224, 360);
	Node last = root;
	for (int i = 0; i < 200; i++) {
		Node newNode(new Image("Test_Files/TestSprite2.png"));
		newNode.setPositionInParent(40, 40);
		newNode.setZIndexInParent(0.01);
		newNode.setCenterOfRotation(0, 0);
		last.addChild(newNode);
		last = newNode;
	}

	Process p([&last](double passedTime, ThreadWorker worker) {
		static const double pixelPerSecond = 100;
		last.move(passedTime * pixelPerSecond, 0);
		return;
	});

	ThreadWork w;
	w.second.push(p);
	ThreadWorker worker(&w);
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
			// cam.move(1, 0);
		} else if (j < i / 2) {
			// cam.rotate(3.14159 / (i));
		} else if (j < 3 * i / 4) {
			// cam.zoomIn(0.01);
		} else {
			// cam.zoomIn(-0.01);
		}
	}
	double total = chrono::duration_cast<chrono::duration<double>>(
	                   chrono::high_resolution_clock::now() - before)
	                   .count();

	cout << "Average Framerate: " << i / total << endl;
	return 0;
}