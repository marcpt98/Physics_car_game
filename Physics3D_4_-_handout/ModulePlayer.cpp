#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePlayer.h"

ModulePlayer::ModulePlayer(bool start_enabled) : Module(start_enabled)
{

}

ModulePlayer::~ModulePlayer()
{

}

bool ModulePlayer::CleanUp()
{
	LOG("Unloading Player");

	return true;
}

bool ModulePlayer::Start()
{
	bool ret = true;

	//Car body
	car = new Cube(vec3(1, 1, 2));
	//App->scene_intro->primitives.PushBack(car);
	CarPrimitives.PushBack(car);
	car->SetPos(0.f, 2.f, 2.5f);

	//Wheel Axis
	Cube* axis1 = new Cube(vec3(0.2, 0.7, 0.2));
	//App->scene_intro->primitives.PushBack(axis1);
	CarPrimitives.PushBack(axis1);
	axis1->SetPos(0.7f, 2.f, 2.f);

	//Wheel Axis Front
	Cube* axis2 = new Cube(vec3(0.2, 0.7, 0.2));
	//App->scene_intro->primitives.PushBack(axis2);
	CarPrimitives.PushBack(axis2);
	axis2->SetPos(-0.7f, 2.f, 2.f);

	//Wheel Axis
	Cube* axis3 = new Cube(vec3(0.2, 0.7, 0.2));
	//App->scene_intro->primitives.PushBack(axis3);
	CarPrimitives.PushBack(axis3);
	axis3->SetPos(0.7f, 2.f, 3.f);

	//Wheel Axis
	Cube* axis4 = new Cube(vec3(0.2, 0.7, 0.2));
	//App->scene_intro->primitives.PushBack(axis4);
	CarPrimitives.PushBack(axis4);
	axis4->SetPos(-0.7f, 2.f, 3.f);

	btTransform localA;
	btTransform localB;

	localA.setIdentity();
	localB.setIdentity();

	localA.getBasis().setEulerZYX(0, 0, M_PI_2);
	localA.setOrigin(btVector3(0.0, 0.0, -0.5));

	localB.getBasis().setEulerZYX(0, 0, M_PI_2);
	localB.setOrigin(btVector3(-0.6f, 0.8f, 0.5f));


	Axis[0] = App->physics->AddConstraintSlider(**CarPrimitives.At(0), **CarPrimitives.At(1), localA, localB);

	localB.setIdentity();
	localB.getBasis().setEulerZYX(0, 0, M_PI_2);
	localB.setOrigin(btVector3(0.6f, 0.8f, 0.5f));

	Axis[1] = App->physics->AddConstraintSlider(**CarPrimitives.At(0), **CarPrimitives.At(2), localA, localB);

	localB.setIdentity();
	localB.getBasis().setEulerZYX(0, 0, M_PI_2);
	localB.setOrigin(btVector3(-0.6f, 0.8f, -1.5f));

	Axis[2] = App->physics->AddConstraintSlider(**CarPrimitives.At(0), **CarPrimitives.At(3), localA, localB);

	localB.setIdentity();
	localB.getBasis().setEulerZYX(0, 0, M_PI_2);
	localB.setOrigin(btVector3(0.6f, 0.8f, -1.5f));

	Axis[3] = App->physics->AddConstraintSlider(**CarPrimitives.At(0), **CarPrimitives.At(4), localA, localB);



	Cylinder* wheel = new Cylinder(0.4, 0.2, 0.4);
	wheel->SetPos(-1.1f, 1.5f, 2.f);
	//App->scene_intro->primitives.PushBack(wheel);
	CarPrimitives.PushBack(wheel);

	wheel = new Cylinder(0.4, 0.2, 0.4);
	wheel->SetPos(-1.1f, 1.5f, 3.f);
	//App->scene_intro->primitives.PushBack(wheel);
	CarPrimitives.PushBack(wheel);

	wheel = new Cylinder(0.4, 0.2, 0.4);
	wheel->SetPos(1.1f, 1.5f, 2.f);
	//App->scene_intro->primitives.PushBack(wheel);
	CarPrimitives.PushBack(wheel);

	wheel = new Cylinder(0.4, 0.2, 0.4);
	wheel->SetPos(1.1f, 1.5f, 3.f);
	CarPrimitives.PushBack(wheel);
	//App->scene_intro->primitives.PushBack(wheel);

	wheels[0] = App->physics->AddConstraintHinge(**CarPrimitives.At(2), **CarPrimitives.At(5), btVector3{ -0.2,-0.2,-0 }, btVector3{ 0, 0,0 }, btVector3{ 1, 0,0 }, btVector3{ 0,1,0 });
	wheels[1] = App->physics->AddConstraintHinge(**CarPrimitives.At(4), **CarPrimitives.At(6), btVector3{ -0.2,-0.2,-0 }, btVector3{ 0, 0,0 }, btVector3{ 1, 0,0 }, btVector3{ 0,1,0 });
	wheels[2] = App->physics->AddConstraintHinge(**CarPrimitives.At(1), **CarPrimitives.At(7), btVector3{ 0.2,-0.2, -0 }, btVector3{ 0, 0,0 }, btVector3{ -1, 0,0 }, btVector3{ 0,1,0 });
	wheels[3] = App->physics->AddConstraintHinge(**CarPrimitives.At(3), **CarPrimitives.At(8), btVector3{ 0.2,-0.2, -0 }, btVector3{ 0, 0,0 }, btVector3{ -1, 0,0 }, btVector3{ 0,1,0 });

	return ret;
}

update_status ModulePlayer::Update(float dt)
{
	wheels[0]->enableMotor(false);
	wheels[1]->enableMotor(false);
	wheels[2]->enableMotor(false);
	wheels[3]->enableMotor(false);

	Axis[1]->setPoweredAngMotor(false);
	Axis[0]->setPoweredAngMotor(false);
	Axis[1]->setLowerAngLimit(0);
	Axis[1]->setUpperAngLimit(0);
	Axis[0]->setLowerAngLimit(0);
	Axis[0]->setUpperAngLimit(0);
	Axis[2]->setLowerAngLimit(0.f);
	Axis[2]->setUpperAngLimit(0.f);
	Axis[3]->setLowerAngLimit(0.f);
	Axis[3]->setUpperAngLimit(0.f);


	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) {
		//al reves para las de alante
		wheels[0]->enableAngularMotor(true, 50.f, 1000.f);
		//	wheels[1]->enableAngularMotor(true, 50.f, 100.f);
		wheels[2]->enableAngularMotor(true, -50.f, 1000.f);
		//	wheels[3]->enableAngularMotor(true, -50.f, 100.f);
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
		wheels[0]->enableAngularMotor(true, -50.f, 1000.f);
		//	wheels[1]->enableAngularMotor(true, -50.f, 100.f);
		wheels[2]->enableAngularMotor(true, 50.f, 1000.f);
		//	wheels[3]->enableAngularMotor(true, 50.f, 100.f);
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {

		Axis[1]->setPoweredAngMotor(true);
		Axis[1]->setLowerAngLimit(-0.7F);
		Axis[1]->setUpperAngLimit(0.7F);
		Axis[1]->setTargetAngMotorVelocity(1);
		Axis[1]->setMaxAngMotorForce(10);

		Axis[0]->setPoweredAngMotor(true);
		Axis[0]->setLowerAngLimit(-0.7F);
		Axis[0]->setUpperAngLimit(0.7F);
		Axis[0]->setTargetAngMotorVelocity(1);
		Axis[0]->setMaxAngMotorForce(10);

		Axis[2]->setLowerAngLimit(-0.7F);
		Axis[2]->setUpperAngLimit(0.7F);
		Axis[3]->setLowerAngLimit(-0.7F);
		Axis[3]->setUpperAngLimit(0.7F);

		wheels[2]->enableAngularMotor(true, -20.f, 1000.f);

		wheels[0]->enableAngularMotor(true, 20.f, 1000.f);
		wheels[1]->enableAngularMotor(true, 20.f, 1000.f);

		//wheels[1]->enableMotor(false);
		//wheels[3]->enableMotor(false);

		//Axis[2]->setPoweredAngMotor(true);
		//Axis[2]->setLowerAngLimit(-0.5F);
		//Axis[2]->setUpperAngLimit(0.5F);
		//Axis[2]->setTargetAngMotorVelocity(-1);
		//Axis[2]->setMaxAngMotorForce(10); 
		//								  
		//Axis[3]->setPoweredAngMotor(true);
		//Axis[3]->setLowerAngLimit(-0.5F); 
		//Axis[3]->setUpperAngLimit(0.5F);  
		//Axis[3]->setTargetAngMotorVelocity(-1);
		//Axis[3]->setMaxAngMotorForce(10);
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		Axis[1]->setPoweredAngMotor(true);
		Axis[1]->setLowerAngLimit(-0.7F);
		Axis[1]->setUpperAngLimit(0.7F);
		Axis[1]->setTargetAngMotorVelocity(-1);
		Axis[1]->setMaxAngMotorForce(10);

		Axis[0]->setPoweredAngMotor(true);
		Axis[0]->setLowerAngLimit(-0.7F);
		Axis[0]->setUpperAngLimit(0.7F);
		Axis[0]->setTargetAngMotorVelocity(-1);
		Axis[0]->setMaxAngMotorForce(10);

		Axis[2]->setLowerAngLimit(-0.7F);
		Axis[2]->setUpperAngLimit(0.7F);
		Axis[3]->setLowerAngLimit(-0.7F);
		Axis[3]->setUpperAngLimit(0.7F);


		wheels[0]->enableAngularMotor(true, 20.f, 1000.f);

		wheels[2]->enableAngularMotor(true, -20.f, 1000.f);
		wheels[3]->enableAngularMotor(true, -20.f, 1000.f);



		//wheels[3]->enableMotor(false);
		/*Axis[2]->setPoweredAngMotor(true);
		Axis[2]->setLowerAngLimit(-0.5F);
		Axis[2]->setUpperAngLimit(0.5F);
		Axis[2]->setTargetAngMotorVelocity(1);
		Axis[2]->setMaxAngMotorForce(10);

		Axis[3]->setPoweredAngMotor(true);
		Axis[3]->setLowerAngLimit(-0.5F);
		Axis[3]->setUpperAngLimit(0.5F);
		Axis[3]->setTargetAngMotorVelocity(1);
		Axis[3]->setMaxAngMotorForce(10);*/
	}
	return UPDATE_CONTINUE;
}

update_status ModulePlayer::PostUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(PhysBody3D* body1, PhysBody3D* body2) {

}