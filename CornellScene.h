#pragma once

#include "GenericScene.h"

class CornellScene : public GenericScene
{
public:
	CornellScene() = delete;
	CornellScene(Window*);
	~CornellScene();

	void update(GLfloat timedelta, GLfloat timeElapsed) override;
	void drawScene() override;
	void handleEvent(WindowEvent& ev, GLfloat timedelta) override;

private:
	int voxelGridSize;
	Texture3D* voxelGrid;
	int cycleMode;

};

