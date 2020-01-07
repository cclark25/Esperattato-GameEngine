// #include "./ThreadPool/ThreadWorker.h"
// #include "./ThreadPool/Process.h"
#include "Screen/Screen.h"
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

	Esperatto::Screen d(256, 224, 360);
	Bitmap b = al_load_bitmap("Test_Files/RedBox.png");

	Node last = root;
	for (int i = 0; i < 50; i++) {
		Node newNode;
		newNode.setPositionInParent(0.01, 0.02);
		newNode.setZIndexInParent(0.01);
		last.addChild(newNode);
		last = newNode;
	}

	// auto pos = last.getGlobalPosition();
	// cout << "Lowest node: " << pos.x << ", " << pos.y << endl;
	const int i = 50000;
	double total = 0;
	d.setFullscreen(true);
	d.setPixelStretch(7.0 / 6.0, 1);
	for (int j = i; j > 0; j--) {
		static map<void *, double> *parentMap = new map<void *, double>();
		Bitmap b3 = al_create_bitmap(256, 224);
		auto before = chrono::high_resolution_clock::now();
		al_set_target_bitmap(b3);
		al_clear_to_color(al_map_rgb(0,0,0));
		// root.foreach ([&](Node n) {
		// 	static auto &map = *parentMap;
		// 	static double z = 0;
		// 	z = n.getZIndexInParent();
		// 	static auto x = n.getParentPointer();
		// 	if (x) {
		// 		z += map.at(x);
		// 	}
		// 	map[(n.getSelfPointer())] =
		// 	    n.getZIndexInParent() + (x ? map[x] : 0);
		// 	static int xPos = 0;
		// 	// b.draw_scaled_bitmap(b2, 0, 0, 256, 224, 0, 0, 256, 224, 0);
		// 	// if (b3.shouldDraw(b, xPos - 128, 0)) {
		// 	// 	b3.draw_rotated_bitmap(b, 128, 112, xPos - 128, 0,
		// 	// 	                       0*(xPos) * (3.1216 / 6), 0);
		// 	// }
		// 	xPos++;
		// 	xPos = (xPos % (2*3072));
		// 	b3.draw_bitmap(b, xPos, xPos , 0);
		// 	// std::cout << "Z: " << z << endl;
		// });
		for (SovereignNode node : root.makeNodeSet()) {
			static int xPos = 0;
			xPos++;
			xPos = (xPos % (2 * 3072));
			// al_use_transform(&node.transformation);
			// al_set_target_bitmap(b3);
			al_draw_bitmap(b, xPos, xPos, 0);
		}
		// delete parentMap;
		total += 1 / chrono::duration_cast<chrono::duration<double>>(
		                 chrono::high_resolution_clock::now() - before)
		                 .count();
		d.pushFrame(b3);
		al_destroy_bitmap(b3);
	}
	cout << "Average Framerate: " << total / i << endl;
	cout << b << endl;
	// std::cout << "Time: "
	//   << 1 / chrono::duration_cast<chrono::duration<double>>(
	//              chrono::high_resolution_clock::now() - before)
	//              .count()
	//           << endl;
	// cout << "List length: " << flatList.size() << endl;
	return 0;
}