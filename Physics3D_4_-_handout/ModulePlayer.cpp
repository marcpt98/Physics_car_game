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
	grip = nullptr;
	//Car body
	car = new Cube(vec3(1, 1, 2));
	CarPrimitives.PushBack(car);
	car->SetPos(0.f, 2.f, 2.5f);

	//Wheel Axis
	Cube* axis1 = new Cube(vec3(0.2, 0.7, 0.2));
	CarPrimitives.PushBack(axis1);
	axis1->SetPos(0.7f, 2.f, 2.f);

	//Wheel Axis Front
	Cube* axis2 = new Cube(vec3(0.2, 0.7, 0.2));
	CarPrimitives.PushBack(axis2);
	axis2->SetPos(-0.7f, 2.f, 2.f);

	//Wheel Axis
	Cube* axis3 = new Cube(vec3(0.2, 0.7, 0.2));
	CarPrimitives.PushBack(axis3);
	axis3->SetPos(0.7f, 2.f, 3.f);

	//Wheel Axis
	Cube* axis4 = new Cube(vec3(0.2, 0.7, 0.2));
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
	Axis[0]->setLowerLinLimit(0.1f);
	Axis[0]->setUpperLinLimit(0.2f);
	Axis[0]->setLowerAngLimit(-0.2f);
	Axis[0]->setUpperAngLimit(0.2f);

	localB.setIdentity();
	localB.getBasis().setEulerZYX(0, 0, M_PI_2);
	localB.setOrigin(btVector3(0.6f, 0.8f, 0.5f));

	Axis[1] = App->physics->AddConstraintSlider(**CarPrimitives.At(0), **CarPrimitives.At(2), localA, localB);
	Axis[1]->setLowerLinLimit(0.1f);
	Axis[1]->setUpperLinLimit(0.2f);
	Axis[1]->setLowerAngLimit(-0.2f);
	Axis[1]->setUpperAngLimit(0.2f);

	localB.setIdentity();
	localB.getBasis().setEulerZYX(0, 0, M_PI_2);
	localB.setOrigin(btVector3(-0.6f, 0.8f, -1.5f));

	Axis[2] = App->physics->AddConstraintSlider(**CarPrimitives.At(0), **CarPrimitives.At(3), localA, localB);
	Axis[2]->setLowerLinLimit(0.1f);
	Axis[2]->setUpperLinLimit(0.2f);
	Axis[2]->setLowerAngLimit(-0.2f);
	Axis[2]->setUpperAngLimit(0.2f);

	localB.setIdentity();
	localB.getBasis().setEulerZYX(0, 0, M_PI_2);
	localB.setOrigin(btVector3(0.6f, 0.8f, -1.5f));

	Axis[3] = App->physics->AddConstraintSlider(**CarPrimitives.At(0), **CarPrimitives.At(4), localA, localB);
	Axis[3]->setLowerLinLimit(0.1f);
	Axis[3]->setUpperLinLimit(0.2f);
	Axis[3]->setLowerAngLimit(-0.2f);
	Axis[3]->setUpperAngLimit(0.2f);

	


	Cylinder* wheel = new Cylinder(0.4, 0.2, 0.4);
	wheel->SetPos(-1.1f, 1.5f, 2.f);
	CarPrimitives.PushBack(wheel);

	wheel = new Cylinder(0.4, 0.2, 0.4);
	wheel->SetPos(-1.1f, 1.5f, 3.f);
	CarPrimitives.PushBack(wheel);

	wheel = new Cylinder(0.4, 0.2, 0.4);
	wheel->SetPos(1.1f, 1.5f, 2.f);
	CarPrimitives.PushBack(wheel);

	wheel = new Cylinder(0.4, 0.2, 0.4);
	wheel->SetPos(1.1f, 1.5f, 3.f);
	CarPrimitives.PushBack(wheel);

	wheels[0] = App->physics->AddConstraintHinge(**CarPrimitives.At(2), **CarPrimitives.At(5), btVector3{ -0.2,-0.2,-0 }, btVector3{ 0, 0,0 }, btVector3{ 1, 0,0 }, btVector3{ 0,1,0 });
	wheels[1] = App->physics->AddConstraintHinge(**CarPrimitives.At(4), **CarPrimitives.At(6), btVector3{ -0.2,-0.2,-0 }, btVector3{ 0, 0,0 }, btVector3{ 1, 0,0 }, btVector3{ 0,1,0 });
	wheels[2] = App->physics->AddConstraintHinge(**CarPrimitives.At(1), **CarPrimitives.At(7), btVector3{ 0.2,-0.2, -0 }, btVector3{ 0, 0,0 }, btVector3{ -1, 0,0 }, btVector3{ 0,1,0 });
	wheels[3] = App->physics->AddConstraintHinge(**CarPrimitives.At(3), **CarPrimitives.At(8), btVector3{ 0.2,-0.2, -0 }, btVector3{ 0, 0,0 }, btVector3{ -1, 0,0 }, btVector3{ 0,1,0 });

	
	float truckposx = 5.f, truckposy = 5.f, truckposz = -5.f;
	TruckCab = new Cube(vec3(2, 2, 2));
	TruckCab->color.Set(100,100,100);
	CarPrimitives.PushBack(TruckCab);
	TruckCab->SetPos(truckposx, truckposy, truckposz);

	temp = new Cube(vec3(2.4, 0.5, 5));
	CarPrimitives.PushBack(temp);
	temp->SetPos(truckposx, truckposy, truckposz -3.5f);

	temp = new Cube(vec3(2.5f, 0.25, 0.25));
	CarPrimitives.PushBack(temp);
	temp->SetPos(truckposx, truckposy-1, truckposz);

	temp = new Cube(vec3(2.5f, 0.25, 0.25));
	CarPrimitives.PushBack(temp);
	temp->SetPos(truckposx, truckposy-1, truckposz - 5.5f);

	temp = new Cube(vec3(2.5f, 0.25, 0.25));
	CarPrimitives.PushBack(temp);
	temp->SetPos(truckposx, truckposy-1, truckposz - 2.2f);



	temp2 = new Cylinder(0.4, 0.2, 0.4);
	temp2->SetPos(truckposx+3, truckposy - 1, truckposz);
	temp2->color.Set(225, 0, 0);
	temp2->body.GetBody()->setFriction(5);
	CarPrimitives.PushBack(temp2);

	temp2 = new Cylinder(0.4, 0.2, 0.4);
	temp2->SetPos(truckposx-3, truckposy - 1, truckposz);
	temp2->color.Set(0, 0, 225);
	temp2->body.GetBody()->setFriction(5);
	CarPrimitives.PushBack(temp2);

	temp2 = new Cylinder(0.4, 0.2, 0.4);
	temp2->SetPos(truckposx+3, truckposy - 1, truckposz - 5.5f);
	temp2->color.Set(225, 0, 0);
	temp2->body.GetBody()->setFriction(5);
	CarPrimitives.PushBack(temp2);

	temp2 = new Cylinder(0.4, 0.2, 0.4);
	temp2->SetPos(truckposx-3, truckposy - 1, truckposz - 5.5f);
	temp2->color.Set(0, 0, 225);
	temp2->body.GetBody()->setFriction(5);
	CarPrimitives.PushBack(temp2);

	temp2 = new Cylinder(0.4, 0.2, 0.4);
	temp2->SetPos(truckposx + 3, truckposy - 1, truckposz - 2.2f);
	temp2->color.Set(225, 0, 0);
	temp2->body.GetBody()->setFriction(5);
	CarPrimitives.PushBack(temp2);

	temp2 = new Cylinder(0.4, 0.2, 0.4);
	temp2->SetPos(truckposx - 3, truckposy - 1, truckposz - 2.2f);
	temp2->color.Set(0, 0, 225);
	temp2->body.GetBody()->setFriction(5);
	CarPrimitives.PushBack(temp2);

	temp = new Cube(vec3(2.4, 0.25, 4.5));
	CarPrimitives.PushBack(temp);
	temp->SetPos(truckposx, truckposy + 1, truckposz - 3.5f);

	temp = new Cube(vec3(0.25, 1, 4.5));
	CarPrimitives.PushBack(temp);
	temp->SetPos(truckposx-2, truckposy + 1, truckposz - 3.5f);

	temp = new Cube(vec3(0.25, 1, 4.5));
	CarPrimitives.PushBack(temp);
	
	temp->color.Set(225, 0, 0);
	temp->SetPos(truckposx+2, truckposy + 1, truckposz - 3.5f);

	
	
	App->physics->AddConstraintHinge(**CarPrimitives.At(9), **CarPrimitives.At(10), btVector3{ 0,-0.7 ,-1.2 }, btVector3{ 0,0, 2.6 }, btVector3{ 0, 1, 0 }, btVector3{ 0, 1, 0 });

	/*btTransform localA;
	btTransform localB;*/

	localA.setOrigin(btVector3(0.0, 0.0, 0));
	localB.setOrigin(btVector3(0.f, 1.6f, 0.f));
	TruckAxis[0] = App->physics->AddConstraintSlider(**CarPrimitives.At(9), **CarPrimitives.At(11), localA, localB);
	
	localB.setOrigin(btVector3(0.f, 0.7f, -0.5f));
	TruckAxis[1] =	App->physics->AddConstraintSlider(**CarPrimitives.At(10), **CarPrimitives.At(12), localA, localB);

	localB.setOrigin(btVector3(0.f, 0.7f, 1.6f));
	TruckAxis[2] = App->physics->AddConstraintSlider(**CarPrimitives.At(10), **CarPrimitives.At(13), localA, localB);

	TruckWheels[0] = App->physics->AddConstraintHinge(**CarPrimitives.At(11), **CarPrimitives.At(14), btVector3{  1.7f,-0.,-0 }, btVector3{ 0, 0,0 }, btVector3{ 1, 0,0 }, btVector3{ 0,1,0 });
	TruckWheels[1] = App->physics->AddConstraintHinge(**CarPrimitives.At(11), **CarPrimitives.At(15), btVector3{ -1.7f,-0.,-0 }, btVector3{ 0, 0,0 }, btVector3{ 1, 0,0 }, btVector3{ 0,1,0 });
	TruckWheels[2] = App->physics->AddConstraintHinge(**CarPrimitives.At(12), **CarPrimitives.At(16), btVector3{  1.7f, 0, -0 }, btVector3{ 0, 0,0 }, btVector3{ 1, 0,0 }, btVector3{ 0,1,0 });
	TruckWheels[3] = App->physics->AddConstraintHinge(**CarPrimitives.At(12), **CarPrimitives.At(17), btVector3{ -1.7f, 0, -0 }, btVector3{ 0, 0,0 }, btVector3{ 1, 0,0 }, btVector3{ 0,1,0 });
	TruckWheels[4] = App->physics->AddConstraintHinge(**CarPrimitives.At(13), **CarPrimitives.At(18), btVector3{  1.7f, 0, -0 }, btVector3{ 0, 0,0 }, btVector3{ 1, 0,0 }, btVector3{ 0,1,0 });
	TruckWheels[5] = App->physics->AddConstraintHinge(**CarPrimitives.At(13), **CarPrimitives.At(19), btVector3{ -1.7f, 0, -0 }, btVector3{ 0, 0,0 }, btVector3{ 1, 0,0 }, btVector3{ 0,1,0 });

	Elevator = App->physics->AddConstraintHinge(**CarPrimitives.At(10), **CarPrimitives.At(20), btVector3{ 0, -0 , -2. }, btVector3{0 ,-0.5 , -2. }, btVector3{ 1, 0, 0 }, btVector3{ 1, 0, 0 });
	App->physics->AddConstraintP2P(**CarPrimitives.At(20), **CarPrimitives.At(22), btVector3{ 1.1, 0., -2}, btVector3{ -0.25, -0.5 , -2 });
	App->physics->AddConstraintP2P(**CarPrimitives.At(20), **CarPrimitives.At(21), btVector3{ -1.1, 0., -2 }, btVector3{ 0.25,-0.5 , -2 });
	App->physics->AddConstraintP2P(**CarPrimitives.At(20), **CarPrimitives.At(22), btVector3{ 1.1, 0., 2 }, btVector3{ -0.25, -0.5 , 2 });
	App->physics->AddConstraintP2P(**CarPrimitives.At(20), **CarPrimitives.At(21), btVector3{ -1.1, 0., 2 }, btVector3{ 0.25, -0.5 , 2 });

	App->physics->AddConstraintP2P(**CarPrimitives.At(20), **CarPrimitives.At(22), btVector3{ 1.1, 0.5, -2 }, btVector3{ -0.25,-0.0 , -2 });
	App->physics->AddConstraintP2P(**CarPrimitives.At(20), **CarPrimitives.At(21), btVector3{ -1.1,0.5, -2 }, btVector3{ 0.25, -0.0 , -2 });
	App->physics->AddConstraintP2P(**CarPrimitives.At(20), **CarPrimitives.At(22), btVector3{ 1.1, 0.5, 2 }, btVector3{ -0.25, -0.0 , 2 });
	App->physics->AddConstraintP2P(**CarPrimitives.At(20), **CarPrimitives.At(21), btVector3{ -1.1,0.5, 2 }, btVector3{ 0.25,  -0.0 , 2 });
	

	temp2 = new Cylinder(0.4, 0.4, 0.4);
	temp2->SetPos(truckposx, truckposy + 3, truckposz);
	CarPrimitives.PushBack(temp2);

	temp = new Cube(vec3(0.4, 2, 0.2));
	CarPrimitives.PushBack(temp);
	temp->SetPos(truckposx, truckposy +3.3, truckposz);

	temp = new Cube(vec3(0.3, 2.5, 0.1));
	CarPrimitives.PushBack(temp);
	temp->SetPos(truckposx + 1, truckposy + 3.3, truckposz);

	temp = new Cube(vec3(0.3, 0.3, 0.1), 0.01);
	CarPrimitives.PushBack(temp);
	temp->SetPos(truckposx + 1.4, truckposy + 3.3, truckposz);
	CarPrimitives[26]->name = "Claw";
	CarPrimitives[26]->body.collision_listeners.PushBack(this);

	temp3 = new Sphere(1, 0.01);
	CarPrimitives.PushBack(temp3);
	temp3->SetPos(truckposx + 5, truckposy , truckposz);
	CarPrimitives[27]->name = "Ball";

	HingeArm[0] = App->physics->AddConstraintHinge(**CarPrimitives.At(9), **CarPrimitives.At(23), btVector3{ 0, 1.2f, 0 }, btVector3{ 0, 0 , 0 }, btVector3{ 0, 1, 0 }, btVector3{ 0, 1 , 0 });
	HingeArm[1] = App->physics->AddConstraintHinge(**CarPrimitives.At(23), **CarPrimitives.At(24), btVector3{ 0, 0.4, 0 }, btVector3{ 0, -1 , 0 }, btVector3{ 0, 0, 1 }, btVector3{ 0, 0 , 1 });
	//HingeArm[1]->setLimit(0.5, 0, 100);
	HingeArm[2] = App->physics->AddConstraintHinge(**CarPrimitives.At(24), **CarPrimitives.At(25), btVector3{ 0, 0.8, 0.15 }, btVector3{ 0, -1.25 , 0 }, btVector3{ 0, 0, 1 }, btVector3{ 0, 0 , 1 });

	App->physics->AddConstraintP2P(**CarPrimitives.At(25), **CarPrimitives.At(26), btVector3{ 0, 1.3, 0 }, btVector3{ 0, 0 , 0 });

	//btCollisionObject::
	
	cangrip = false;

	return ret;
}

update_status ModulePlayer::Update(float dt)
{
	
	TruckInput(dt);
	CarInput(dt);


	CarPrimitives[24]->body.GetBody()->setGravity(btVector3(0, 0, 0));
	CarPrimitives[25]->body.GetBody()->setGravity(btVector3(0, 0, 0));

	return UPDATE_CONTINUE;
}

update_status ModulePlayer::PostUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(PhysBody3D* body1, PhysBody3D* body2) {
	if (body1->parentPrimitive->name == "Claw" && body2->parentPrimitive->name == "Ball" && cangrip) {
		body2->SetPos(TruckCab->body.GetPos().x, TruckCab->body.GetPos().y+2, TruckCab->body.GetPos().z-2);
	}
	
}

void ModulePlayer::TruckInput(float dt) {

	TruckWheels[0]->enableMotor(false);
	TruckWheels[1]->enableMotor(false);
	TruckWheels[2]->enableMotor(false);
	TruckWheels[3]->enableMotor(false);
	TruckWheels[4]->enableMotor(false);
	TruckWheels[5]->enableMotor(false);
	Elevator->enableMotor(false);

	Axis[0]->setPoweredAngMotor(false);

	TruckAxis[0]->setLowerAngLimit(0.f);
	TruckAxis[0]->setUpperAngLimit(0.f);
	TruckAxis[1]->setLowerAngLimit(0.f);
	TruckAxis[1]->setUpperAngLimit(0.f);
	TruckAxis[2]->setLowerAngLimit(0.f);
	TruckAxis[2]->setUpperAngLimit(0.f);

	HingeArm[0]->enableAngularMotor(true, 0.f, 100);
	HingeArm[1]->enableAngularMotor(true, 0.f, 10);
	HingeArm[2]->enableAngularMotor(true, 0.f, 10);


	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) {
		TruckCab->body.Push(vec3{ 0,0,0 });
		TruckWheels[0]->enableAngularMotor(true, -20.f, 100.f);
		TruckWheels[1]->enableAngularMotor(true, -20.f, 100.f);
		TruckWheels[2]->enableAngularMotor(true, -20.f, 100.f);
		TruckWheels[3]->enableAngularMotor(true, -20.f, 100.f);
		TruckWheels[4]->enableAngularMotor(true, -20.f, 100.f);
		TruckWheels[5]->enableAngularMotor(true, -20.f, 100.f);



	}
	
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		TruckCab->body.Push(vec3{ 0,0,0 });
		TruckWheels[0]->enableAngularMotor(true, -20.f, 100.f);
		TruckWheels[1]->enableAngularMotor(true, -5.f, 100.f);
		TruckWheels[2]->enableAngularMotor(true, -20.f, 100.f);
		TruckWheels[4]->enableAngularMotor(true, -20.f, 100.f);

		TruckAxis[0]->setPoweredAngMotor(true);
		TruckAxis[0]->setTargetAngMotorVelocity(0.1);
	//	TruckAxis[0]->setMaxAngMotorForce(10);
		TruckAxis[0]->setLowerAngLimit(-0.2f);
		TruckAxis[0]->setUpperAngLimit( 0.2f);
		TruckAxis[1]->setLowerAngLimit(-0.2f);
		TruckAxis[1]->setUpperAngLimit( 0.2f);
		TruckAxis[2]->setLowerAngLimit(-0.2f);
		TruckAxis[2]->setUpperAngLimit( 0.2f);
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		TruckCab->body.Push(vec3{ 0,0,0 });
		TruckWheels[0]->enableAngularMotor(true, -5.f, 100.f);
		TruckWheels[1]->enableAngularMotor(true, -20.f, 100.f);
		TruckWheels[3]->enableAngularMotor(true, -20.f, 100.f);
		TruckWheels[5]->enableAngularMotor(true, -20.f, 100.f);

		TruckAxis[0]->setPoweredAngMotor(true);
		TruckAxis[0]->setTargetAngMotorVelocity(-0.1);
		//TruckAxis[0]->setMaxAngMotorForce(-10);
		TruckAxis[0]->setLowerAngLimit(-0.2f);
		TruckAxis[0]->setUpperAngLimit( 0.2f);
		TruckAxis[1]->setLowerAngLimit(-0.2f);
		TruckAxis[1]->setUpperAngLimit( 0.2f);
		TruckAxis[2]->setLowerAngLimit(-0.2f);
		TruckAxis[2]->setUpperAngLimit( 0.2f);
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
		TruckCab->body.Push(vec3{ 0,0,0 });
		TruckWheels[0]->enableAngularMotor(true, 20.f, 100.f);
		TruckWheels[1]->enableAngularMotor(true, 20.f, 100.f);
		TruckWheels[2]->enableAngularMotor(true, 20.f, 100.f);
		TruckWheels[3]->enableAngularMotor(true, 20.f, 100.f);
		TruckWheels[4]->enableAngularMotor(true, 20.f, 100.f);
		TruckWheels[5]->enableAngularMotor(true, 20.f, 100.f);


	}
	if (App->input->GetKey(SDL_SCANCODE_0) == KEY_REPEAT) {
		TruckCab->body.Push(vec3{ 0,0,0 });
	Elevator->enableAngularMotor(true, 1.f, 100.f);

	}
	if (App->input->GetKey(SDL_SCANCODE_9) == KEY_REPEAT) {
		TruckCab->body.Push(vec3{ 0,0,0 });
		HingeArm[1]->enableAngularMotor(true, 1.2f, 10);
		HingeArm[2]->enableAngularMotor(true, 1.f, 10);
	}

	if (App->input->GetKey(SDL_SCANCODE_8) == KEY_REPEAT) {
		TruckCab->body.Push(vec3{ 0,0,0 });
		HingeArm[1]->enableAngularMotor(true, -1.2f, 10);
		HingeArm[2]->enableAngularMotor(true, -1.f, 10);
	}

	if (App->input->GetKey(SDL_SCANCODE_7) == KEY_REPEAT) {
		TruckCab->body.Push(vec3{ 0,0,0 });
		HingeArm[0]->enableAngularMotor(true, 1.5f, 100);

	}

	if (App->input->GetKey(SDL_SCANCODE_6) == KEY_REPEAT) {
		TruckCab->body.Push(vec3{ 0,0,0 });
		HingeArm[2]->enableAngularMotor(true, 1.f, 10);

	}

	if (App->input->GetKey(SDL_SCANCODE_5) == KEY_REPEAT) {
		TruckCab->body.Push(vec3{ 0,0,0 });
		HingeArm[2]->enableAngularMotor(true, -1.f, 10);

	}
	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN) {
		if (cangrip)
		{
			cangrip = false;

		}
		else { cangrip = true; 
		}
	}

}

void ModulePlayer::CarInput(float dt) 
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


	if (App->input->GetKey(SDL_SCANCODE_I) == KEY_REPEAT) {
		car->body.Push(vec3{ 0,0,0 });
		wheels[0]->enableAngularMotor(true, 50.f, 1000.f);
		//	wheels[1]->enableAngularMotor(true, 50.f, 100.f);
		wheels[2]->enableAngularMotor(true, -50.f, 1000.f);

	}
	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_REPEAT) {
		car->body.Push(vec3{ 0,0,0 });
		wheels[0]->enableAngularMotor(true, -50.f, 1000.f);
		//	wheels[1]->enableAngularMotor(true, 50.f, 100.f);
		wheels[2]->enableAngularMotor(true, 50.f, 1000.f);
	}

	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT) {
		car->body.Push(vec3{ 0,0,0 });
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
	if (App->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT) {
		car->body.Push(vec3{ 0,0,0 });
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


}