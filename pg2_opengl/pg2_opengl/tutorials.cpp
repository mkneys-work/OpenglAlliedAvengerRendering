#include "pch.h"
#include "tutorials.h"
#include "utils.h"
#include "Rasterizer.h"
#include "mymath.h"

/* create a window and initialize OpenGL context */
int vizualizeAlliedAvanger( const int width, const int height)
{
	Rasterizer rasterizer(640, 480, deg2rad(45.0), Vector3(175, -140, 130), Vector3(0, 0, 35));
	rasterizer.initDevice();
	rasterizer.initFrameBuffer();
	rasterizer.loadScene("../../../data/6887_allied_avenger_gi.obj");
	rasterizer.initMaterials();
	rasterizer.RenderFrame();

	return S_OK;
}