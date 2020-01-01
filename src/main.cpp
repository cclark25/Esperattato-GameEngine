// #include "./ThreadPool/ThreadWorker.h"
// #include "./ThreadPool/Process.h"
#include "Display/Display.h"
#include "Node/Node.h"
#include <allegro5/allegro_image.h>
#include <cmath>
#include <iostream>
#include <unistd.h>

using namespace Esperatto;

int main(int argc, char **args) {
	al_init();
	al_init_image_addon();

	Node root;

	root.setPositionInParent(1, 1);

	Node last = root;

	for(int i = 0; i < 1000; i++){
		Node newNode;
		newNode.setPositionInParent(0.01,0.02);
		last.addChild(newNode);
		last = newNode;
	}

	// auto pos = last.getGlobalPosition();
	// cout << "Lowest node: " << pos.x << ", " << pos.y << endl;
	
	auto flatList = root.makeNodeSet();
	cout << "List length: " << flatList.size() << endl;

	return 0;
}