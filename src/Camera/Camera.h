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
			int width = 256;
			int height = 224;
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

		void setCanvasSize(int width, int height);
		int getCanvasWidth();
		int getCanvasHeight();
		void setPosition(double x, double y);
		Coordinates getPosition();
		void setRotation(double radians);
		double getRotation();

		Coordinates getCenterOfRotation();
		void setCenterOfRotation(double x, double y);
	};
} // namespace Esperatto

#endif