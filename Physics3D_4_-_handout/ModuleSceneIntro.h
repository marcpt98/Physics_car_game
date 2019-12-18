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

private:
	void HandleDebugInput();
	void DebugSpawnPrimitive(Primitive* p);

public:
	p2DynArray<Primitive*> ScenePrimitives;
	bool camerafollow = false;
	bool camerafollowhelicopter = false;
	Cube* Sensor_cube;
	PhysBody3D* sensor = nullptr;
	Sphere* ball1;
	bool case1 = false;
	bool case2 = false;
	bool case3 = false;
	bool case4 = false;
	bool case5 = false;
private:
	Cube* leg1;
	Cube* leg2;
	Cube* body;
	Cube* arm;
	Cube* head;
	Cube* finalWall;
};
