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

	p2DynArray<Primitive*> primitives;

private:
	void HandleDebugInput();
	void DebugSpawnPrimitive(Primitive* p);

	Cube* car;
	btHingeConstraint* wheels[4];
	btSliderConstraint* Axis[4];
};
