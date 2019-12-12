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
	void TruckInput(float dt);

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2) override;

	// Map walls
	bool CleanUp();

private:

	Cube* TruckCab;

	btHingeConstraint* wheels[4];
	btHingeConstraint* TruckWheels[6];
	btSliderConstraint* Axis[4];
	btSliderConstraint* TruckAxis[3];

public:
	Cube* car;
	p2DynArray<Primitive*> CarPrimitives;
};
