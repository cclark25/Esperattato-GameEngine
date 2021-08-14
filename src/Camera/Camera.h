#ifndef ESPERATTO_CAMERA
#define ESPERATTO_CAMERA
#include "../Node/Node.h"
#include "../Screen/Screen.h"
#include <memory>
#include <thread>

namespace Esperatto {
	class Camera : public Node {
	  private:
		struct cameraData {
			double zoom = 1;
			int width = 256;
			int height = 224;
			bool drawAnchors = false;
			unsigned int referenceCount = 0;
			Bitmap canvas;
			Screen *screen;
			ALLEGRO_COLOR canvasColor = al_map_rgb(128, 128, 128);
		};
		std::shared_ptr<cameraData> data;

	  public:
		Camera(const Screen &screen);
		Camera(const Camera &source);
		~Camera();

		void drawToScreen(Node rootNode);

		void zoomIn(double zoom);

		void setCanvasSize(int width, int height);
		void setCanvasColor(ALLEGRO_COLOR color);
		int getCanvasWidth();
		int getCanvasHeight();
		double getZoom();
		void setZoom(double zoom);
		void toggleAnchor();
		void toggleAnchor(bool enable);

		bool shouldDraw(Transform &t, Bitmap bitmap,
		                        float destination_x, float destination_y);
	};
} // namespace Esperatto

#endif