// #include "./ThreadPool/ThreadWorker.h"
// #include "./ThreadPool/Process.h"
#include "Display/Display.h"
#include "Node/Node.h"
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

	Node root;

	root.setPositionInParent(1, 1);

	Esperatto::Display d(512, 256, 60);
	Bitmap b("Test_Files/stock_image.png");

	Node last = root;
	for (int i = 0; i < 20; i++) {
		Node newNode;
		newNode.setPositionInParent(0.01, 0.02);
		newNode.setZIndexInParent(0.01);
		last.addChild(newNode);
		last = newNode;
	}

	// auto pos = last.getGlobalPosition();
	// cout << "Lowest node: " << pos.x << ", " << pos.y << endl;

	auto before = chrono::high_resolution_clock::now();
	map<void *, double> *parentMap = new map<void *, double>();
	root.foreach ([&](Node n) {
		static auto &map = *parentMap;
		static double z = 0;
		z = n.getZIndexInParent();
		static auto x = n.getParentPointer();
		if (x) {
			z += map.at(x);
		}
		map[(n.getSelfPointer())] = z;
		static Bitmap b2(100, 100);
		d.drawToBackbuffer(b2);
		// std::cout << "Z: " << z << endl;
	});
	// d.pushFrame(b);
	delete parentMap;
	std::cout << "Time: "
	          << 1 / chrono::duration_cast<chrono::duration<double>>(
	                     chrono::high_resolution_clock::now() - before)
	                     .count()
	          << endl;
	// cout << "List length: " << flatList.size() << endl;

	return 0;
}