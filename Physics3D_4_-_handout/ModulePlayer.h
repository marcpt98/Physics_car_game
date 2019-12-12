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


class ModulePlayer : public Module
{
public:
	ModulePlayer(bool start_enabled = true);
	~ModulePlayer();

	bool Start();
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2) override;

	// Map walls
	bool CleanUp();

private:


	Cube* car;
	btHingeConstraint* wheels[4];
	btSliderConstraint* Axis[4];

public:
	p2DynArray<Primitive*> CarPrimitives;
};
