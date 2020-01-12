#ifndef ESPERATTO_CAMERA
#define ESPERATTO_CAMERA
#include "../Node/Node.h"
#include "../Screen/Screen.h"

namespace Esperatto {
	class Camera {
	  private:
		struct cameraData {
			double xPosition = 0;
			double yPosition = 0;
			double xCenterOfRotation = 0;
			double yCenterOfRotation = 0;
			double rotationRadians = 0;
			double zoom = 1;
			int width = 256;
			int height = 224;
			bool drawAnchors = false;
			unsigned int referenceCount = 0;
			Bitmap canvas = al_create_bitmap(width, height);
			Screen *screen;
		} * data;

	  public:
		Camera(const Screen &screen);
		Camera(const Camera &source);
		~Camera();

		void drawToScreen(Node rootNode);

		void move(double x, double y);
		void rotate(double radians);
		void zoomIn(double zoom);

		void setCanvasSize(int width, int height);
		int getCanvasWidth();
		int getCanvasHeight();
		void setPosition(double x, double y);
		Coordinates getPosition();
		void setRotation(double radians);
		double getRotation();
		double getZoom();
		void setZoom(double zoom);
		void toggleAnchor();
		void toggleAnchor(bool enable);

		Coordinates getCenterOfRotation();
		void setCenterOfRotation(double x, double y);

		bool shouldDraw(Transform &t, Bitmap bitmap,
		                        float destination_x, float destination_y);
	};
} // namespace Esperatto

#endif