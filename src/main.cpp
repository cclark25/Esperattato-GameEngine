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
	for (int i = 0; i < 2; i++) {
		Node newNode(new Image("Test_Files/TestSprite2.png"));
		newNode.setPositionInParent(40, 40);
		newNode.setZIndexInParent(0.01);
		newNode.setCenterOfRotation(0, 0);
		last.addChild(newNode);
		last = newNode;
	}

	cout << "Here 1\n";
	Process p([&last](double passedTime, ThreadWorker worker) {
		cout << "Function entered.\n";
		last.move(1000*passedTime, 0);
		return;
	});
	cout << "Here 2\n";

	ThreadWork w;
	cout << "Here 3\n";
	w.second.push(p);
	cout << "Here 4\n";
	ThreadWorker worker(&w);
	cout << "Here 5\n";
	worker.take_off_standby();
	cout << "Here 6\n";

	const int i = 256;
	d.setFullscreen(true);
	cout << "Here 7\n";
	d.setPixelStretch(7.0 / 6.0, 1);
	cout << "Here 8\n";
	Camera cam(d);
	cout << "Here 9\n";
	auto before = chrono::high_resolution_clock::now();
	cout << "Here 10\n";
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

	cout << "Average Framerate: " << i / total << endl;
	return 0;
}