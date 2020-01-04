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

	Esperatto::Display d(256, 224, 360);
	Bitmap b("Test_Files/RedBox.png");
	Bitmap b2(256, 224);

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
	const int i = 500;
	double total = 0;
	d.setFullscreen(true);
	d.setPixelStretch(7.0 / 6.0, 1);
	for (int j = i; j > 0; j--) {
		static map<void *, double> *parentMap = new map<void *, double>();
		Bitmap b3 = Bitmap(256, 224);
		auto before = chrono::high_resolution_clock::now();
		b3.clear_to_color(Color((unsigned char) 0, 0, 0));
		root.foreach ([&](Node n) {
			static auto &map = *parentMap;
			static double z = 0;
			z = n.getZIndexInParent();
			static auto x = n.getParentPointer();
			if (x) {
				z += map.at(x);
			}
			map[(n.getSelfPointer())] =
			    n.getZIndexInParent() + (x ? map[x] : 0);
			static int xPos = 0;
			// b.draw_scaled_bitmap(b2, 0, 0, 256, 224, 0, 0, 256, 224, 0);
			// if (b3.shouldDraw(b, xPos - 128, 0)) {
			// 	b3.draw_rotated_bitmap(b, 128, 112, xPos - 128, 0,
			// 	                       0*(xPos) * (3.1216 / 6), 0);
			// }
			xPos++;
			xPos = (xPos % (2*3072));
			b3.draw_bitmap(b, xPos, xPos , 0);
			// std::cout << "Z: " << z << endl;
		});
		// delete parentMap;
		d.pushFrame(b3);
		total += 1 / chrono::duration_cast<chrono::duration<double>>(
		                 chrono::high_resolution_clock::now() - before)
		                 .count();
	}
	cout << "Average Framerate: " << total / i << endl;
	// std::cout << "Time: "
	//   << 1 / chrono::duration_cast<chrono::duration<double>>(
	//              chrono::high_resolution_clock::now() - before)
	//              .count()
	//           << endl;
	// cout << "List length: " << flatList.size() << endl;

	return 0;
}