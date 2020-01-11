// #include "./ThreadPool/ThreadWorker.h"
// #include "./ThreadPool/Process.h"
#include "Camera/Camera.h"
#include "Image/Image.h"
#include "Node/Node.h"
#include "Screen/Screen.h"
#include <allegro5/allegro_image.h>
#include <chrono>
#include <cmath>
#include <iostream>
#include <map>
#include <unistd.h>

using namespace Esperatto;

int main(int argc, char **args) {
	al_init();
	al_init_image_addon();

	Node root = Node(new char(1));

	root.setPositionInParent(0, 0);
	root.setCenterOfRotation(128, 112);

	Esperatto::Screen d(256, 224, 360);
	Node last = root;
	for (int i = 0; i < 50; i++) {
		Node newNode(new Image("Test_Files/stock_image.png"));
		// newNode.setPositionInParent(0.01, 0.02);
		newNode.setZIndexInParent(0.01);
		newNode.setCenterOfRotation(128, 112);
		last.addChild(newNode);
		last = newNode;
	}

	const int i = 256;
	d.setFullscreen(true);
	d.setPixelStretch(7.0 / 6.0, 1);
	Camera cam(d);
	auto before = chrono::high_resolution_clock::now();
	for (int j = i; j > 0; j--) {
		cam.drawToScreen(root);
		// last.move(-2, 0);
		cam.move(1, 0);
	}
	double total = chrono::duration_cast<chrono::duration<double>>(
	                   chrono::high_resolution_clock::now() - before)
	                   .count();

	cout << "Average Framerate: " << i / total << endl;
	return 0;
}