#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "glmath.h"

class Primitive;
class PhysBody3D;
class btHingeConstraint;
class btSliderConstraint;
class Cube;
class Sphere;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2) override;

	void CreateMap();

	bool CleanUp();
	void restartScene();
	void HandleDebugInput();
	void DebugSpawnPrimitive(Primitive* p);
	void CheckHUDandWall();

public:
	p2DynArray<Primitive*> ScenePrimitives;

	bool camerafollow = false;
	bool camerafollowhelicopter = false;
	bool camerafollowarm = false;
	
	PhysBody3D* sensor = nullptr;
	Sphere* ball1;
	Sphere* ball2;
	Sphere* ball3;
	Sphere* ball4;
	Sphere* ball5;

	Cube* Sensor_cube;
	Cube* Rainbow_cube;
	Cube* parking1;
	Cube* parking2;
	Cube* parking3;
	Cube* finalWall;

	bool case1 = false;
	bool case2 = false;
	bool case3 = false;
	bool case4 = false;
	bool case5 = false;
	bool case1Count = false;
	bool case2Count = false;
	bool case3Count = false;
	bool case4Count = false;
	bool case5Count = false;
	bool EndGame = false;

	int showtime;
	int ballCount;

private:
	Cube* leg1;
	Cube* leg2;
	Cube* body;
	Cube* arm;
	Cube* head;
};
