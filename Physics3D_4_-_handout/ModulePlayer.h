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
	
	void createTruck();
	void OnCollision(PhysBody3D* body1, PhysBody3D* body2) override;
	// Map walls
	bool CleanUp();

private:


	btHingeConstraint* wheels[4];
	btHingeConstraint* TruckWheels[7];
	btHingeConstraint* Elevator;
	btSliderConstraint* Axis[4];
	btSliderConstraint* TruckAxis[3];
	btSliderConstraint* SliderArm[3];
	btHingeConstraint* HingeArm[4];
	btPoint2PointConstraint* grip;
	bool cangrip = false;
	void setInitialPos();

public:

	//Cube* Sensor_cube;
	Cube* TruckCab;
	Cube* Plataform;
	Cube* TruckBody;
	Cube* car;
	Cube* Claw;

	p2DynArray<Primitive*> CarPrimitives;
	vec3 primitivesPos[19];
};
