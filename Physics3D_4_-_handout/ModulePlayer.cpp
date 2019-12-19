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

	for (uint n = 0; n < CarPrimitives.Count(); n++)
		delete CarPrimitives[n];

	CarPrimitives.Clear();

	return true;
}

bool ModulePlayer::Start()
{
	App->audio->LoadFx("Assets/Fx/BrumBrum.wav");
	createTruck();
	bool ret = true;
	grip = nullptr;
	
	return ret;
}

update_status ModulePlayer::Update(float dt)
{

	TruckInput(dt);

	CarPrimitives[15]->body.GetBody()->setGravity(btVector3(0, 0, 0));
	CarPrimitives[16]->body.GetBody()->setGravity(btVector3(0, 0, 0));

	return UPDATE_CONTINUE;
}

update_status ModulePlayer::PostUpdate(float dt)
{
	
	return UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(PhysBody3D* body1, PhysBody3D* body2) {

	if (body1->parentPrimitive->name == "Claw" && body2->parentPrimitive->name == "ball1" ) {
		body2->SetPos(TruckBody->body.GetPos().x, TruckBody->body.GetPos().y+1, TruckBody->body.GetPos().z);
	}
	if (body1->parentPrimitive->name == "Claw" && body2->parentPrimitive->name ==  "ball2" ) {
		body2->SetPos(TruckBody->body.GetPos().x, TruckBody->body.GetPos().y + 1, TruckBody->body.GetPos().z);
	}
	if (body1->parentPrimitive->name == "Claw" && body2->parentPrimitive->name == "ball3") {
		body2->SetPos(TruckBody->body.GetPos().x, TruckBody->body.GetPos().y + 1, TruckBody->body.GetPos().z);
	}
	if (body1->parentPrimitive->name == "Claw" && body2->parentPrimitive->name ==  "ball4") {
		body2->SetPos(TruckBody->body.GetPos().x, TruckBody->body.GetPos().y + 1, TruckBody->body.GetPos().z);
	}
	if (body1->parentPrimitive->name == "Claw" && body2->parentPrimitive->name ==  "ball5" ) {
		body2->SetPos(TruckBody->body.GetPos().x, TruckBody->body.GetPos().y + 1, TruckBody->body.GetPos().z);
	}

}

void ModulePlayer::TruckInput(float dt) {

	TruckWheels[0]->enableAngularMotor(true, 0.f, 100.f);
	TruckWheels[1]->enableAngularMotor(true, 0.f, 100.f);
	TruckWheels[2]->enableAngularMotor(true, 0.f, 100.f);
	TruckWheels[3]->enableAngularMotor(true, 0.f, 100.f);
	TruckWheels[4]->enableAngularMotor(true, 0.f, 100.f);
	TruckWheels[5]->enableAngularMotor(true, 0.f, 100.f);
	TruckWheels[6]->enableAngularMotor(true, -2.f, 5);
	Elevator->enableAngularMotor(true, -2.f, 5);


	TruckAxis[0]->setLowerAngLimit(0.f);
	TruckAxis[0]->setUpperAngLimit(0.f);
	TruckAxis[1]->setLowerAngLimit(0.f);
	TruckAxis[1]->setUpperAngLimit(0.f);
	TruckAxis[2]->setLowerAngLimit(0.f);
	TruckAxis[2]->setUpperAngLimit(0.f);

	HingeArm[0]->enableAngularMotor(true, 0.f, 100);
	HingeArm[1]->enableAngularMotor(true, 0.f, 10);
	HingeArm[2]->enableAngularMotor(true, 0.f, 10);


	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		App->audio->PlayFx(1);
		TruckCab->body.Push(vec3{ 0,0,0 });
		TruckWheels[0]->enableAngularMotor(true, -30.f, 100.f);
		TruckWheels[1]->enableAngularMotor(true, -30.f, 100.f);
		TruckWheels[2]->enableAngularMotor(true, -30.f, 100.f);
		TruckWheels[3]->enableAngularMotor(true, -30.f, 100.f);
		TruckWheels[4]->enableAngularMotor(true, -30.f, 100.f);
		TruckWheels[5]->enableAngularMotor(true, -30.f, 100.f);
	}
	
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		App->audio->PlayFx(1);
		TruckCab->body.Push(vec3{ 0,0,0 });

		TruckWheels[0]->enableAngularMotor(true, 20.f, 100.f);
		TruckWheels[1]->enableAngularMotor(true, 20.f, 100.f);
		TruckWheels[2]->enableAngularMotor(true, 20.f, 100.f);
		TruckWheels[3]->enableAngularMotor(true, 20.f, 100.f);
		TruckWheels[4]->enableAngularMotor(true, 20.f, 100.f);
		TruckWheels[5]->enableAngularMotor(true, 20.f, 100.f);



	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		TruckCab->body.Push(vec3{ 0,0,0 });
		
			TruckWheels[0]->enableAngularMotor(true, -20.f, 100.f);
			TruckWheels[1]->enableAngularMotor(true, -5.f, 100.f);
			TruckWheels[2]->enableAngularMotor(true, -20.f, 100.f);
			TruckWheels[4]->enableAngularMotor(true, -20.f, 100.f);
		

		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {

			TruckWheels[0]->enableAngularMotor(true, 20.f, 100.f);
			TruckWheels[1]->enableAngularMotor(true, 5.f, 100.f);
			TruckWheels[2]->enableAngularMotor(true, 20.f, 100.f);
			TruckWheels[3]->enableAngularMotor(true, 5.f, 100.f);
			TruckWheels[4]->enableAngularMotor(true, 20.f, 100.f);
			TruckWheels[5]->enableAngularMotor(true, 5, 100.f);

		}

			TruckAxis[0]->setTargetAngMotorVelocity(0.1);
			TruckAxis[0]->setLowerAngLimit(-0.2f);
			TruckAxis[0]->setUpperAngLimit(0.2f);
			TruckAxis[1]->setLowerAngLimit(-0.2f);
			TruckAxis[1]->setUpperAngLimit(0.2f);
			TruckAxis[2]->setLowerAngLimit(-0.2f);
			TruckAxis[2]->setUpperAngLimit(0.2f);
		
	}

	
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		TruckCab->body.Push(vec3{ 0,0,0 });
		
			TruckWheels[0]->enableAngularMotor(true, -5.f, 100.f);
			TruckWheels[1]->enableAngularMotor(true, -20.f, 100.f);
			TruckWheels[3]->enableAngularMotor(true, -20.f, 100.f);
			TruckWheels[5]->enableAngularMotor(true, -20.f, 100.f);

		

		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
			TruckWheels[0]->enableAngularMotor(true, 0.5f, 100.f);
			TruckWheels[1]->enableAngularMotor(true, 20.f, 100.f);
			TruckWheels[2]->enableAngularMotor(true, 0.5f, 100.f);
			TruckWheels[3]->enableAngularMotor(true, 20.f, 100.f);
			TruckWheels[4]->enableAngularMotor(true, 0.5f, 100.f);
			TruckWheels[5]->enableAngularMotor(true, 20.f, 100.f);

		}
			TruckAxis[0]->setTargetAngMotorVelocity(-0.1);
			TruckAxis[0]->setLowerAngLimit(-0.2f);
			TruckAxis[0]->setUpperAngLimit(0.2f);
			TruckAxis[1]->setLowerAngLimit(-0.2f);
			TruckAxis[1]->setUpperAngLimit(0.2f);
			TruckAxis[2]->setLowerAngLimit(-0.2f);
			TruckAxis[2]->setUpperAngLimit(0.2f);
		
	}
	
	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_REPEAT) {
		TruckCab->body.Push(vec3{ 0,0,0 });
	Elevator->enableAngularMotor(true, 1.f, 5.f);
	TruckWheels[6]->enableMotor(false);


	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
		TruckCab->body.Push(vec3{ 0,0,0 });
		HingeArm[1]->enableAngularMotor(true, 1.2f, 10);
	}

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) {
		TruckCab->body.Push(vec3{ 0,0,0 });
		HingeArm[1]->enableAngularMotor(true, -1.2f, 10);
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		TruckCab->body.Push(vec3{ 0,0,0 });
		HingeArm[0]->enableAngularMotor(true, -1.5f, 100);

	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		TruckCab->body.Push(vec3{ 0,0,0 });
		HingeArm[0]->enableAngularMotor(true, 1.5f, 100);

	}

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {
		TruckCab->body.Push(vec3{ 0,0,0 });
		HingeArm[2]->enableAngularMotor(true, 1.f, 10);

	}

	if (App->input->GetKey(SDL_SCANCODE_RSHIFT) == KEY_REPEAT) {
		TruckCab->body.Push(vec3{ 0,0,0 });
		HingeArm[2]->enableAngularMotor(true, -1.f, 10);

	}

	//We reset all positions froms spheres also from THE	 WALL
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN || App->scene_intro->EndGame == true) {
		setInitialPos();

		App->scene_intro->case1 = false;
		App->scene_intro->case2 = false;
		App->scene_intro->case3 = false;
		App->scene_intro->case4 = false;
		App->scene_intro->case5 = false;
		App->scene_intro->EndGame = false;
		App->scene_intro->case1 = false;
		App->scene_intro->case2 = false;
		App->scene_intro->case3 = false;
		App->scene_intro->case4 = false;
		App->scene_intro->case5 = false;
		App->scene_intro->case1Count = false;
		App->scene_intro->case2Count = false;
		App->scene_intro->case3Count = false;
		App->scene_intro->case4Count = false;
		App->scene_intro->case5Count = false;
		App->scene_intro->showtime = 100000;
		App->scene_intro->ScenePrimitives[0]->SetPos(-165, 1, -36);
		App->scene_intro->ScenePrimitives[1]->SetPos(40, 1, 175);
		App->scene_intro->ScenePrimitives[2]->SetPos(-20, 1, 175);
		App->scene_intro->ScenePrimitives[3]->SetPos(-98, 1, -118);
		App->scene_intro->ScenePrimitives[4]->SetPos(-98, 1, 40);
		App->scene_intro->finalWall->SetPos(49, 1, -32.5);
	
	}

}

void ModulePlayer::setInitialPos() {

	for (uint n = 0; n < App->player->CarPrimitives.Count(); n++){
		CarPrimitives[n]->SetPos(primitivesPos[n].x, primitivesPos[n].y, primitivesPos[n].z);
		CarPrimitives[n]->body.SetRotation({ 0,0,0,1 });
	}
}

void ModulePlayer::createTruck() {
	Cube* temp;
	Cylinder* temp2;


	float truckposx = 45.f, truckposy = 2.f, truckposz = -175.f;
	TruckCab = new Cube(vec3(2, 2, 2), 10);
	TruckCab->color = { 0.75, 0.7, 0 };
	CarPrimitives.PushBack(TruckCab);
	primitivesPos[0] = { truckposx, truckposy, truckposz };
	TruckCab->SetPos(truckposx, truckposy, truckposz);
	TruckCab->name = "Cart";
	TruckCab->body.collision_listeners.PushBack(this);

	Plataform = new Cube(vec3(2.4, 0.5, 5), 10);
	CarPrimitives.PushBack(Plataform);
	Plataform->SetPos(truckposx, truckposy, truckposz - 3.5f);
	primitivesPos[1] = { truckposx, truckposy, truckposz - 3.5f };
	Plataform->color.Set(0.3, 0.3, 0.3);

	temp = new Cube(vec3(2.5f, 0.25, 0.25), 10);
	CarPrimitives.PushBack(temp);
	temp->SetPos(truckposx, truckposy - 1, truckposz);
	primitivesPos[2] = { truckposx, truckposy - 1, truckposz };
	temp->color.Set(0.5, 0.5, 0.5);

	temp = new Cube(vec3(2.5f, 0.25, 0.25), 10);
	CarPrimitives.PushBack(temp);
	temp->SetPos(truckposx, truckposy - 1, truckposz - 5.5f);
	primitivesPos[3] = { truckposx, truckposy - 1, truckposz - 5.5f };
	temp->color.Set(0.5, 0.5, 0.5);

	temp = new Cube(vec3(2.5f, 0.25, 0.25), 10);
	CarPrimitives.PushBack(temp);
	temp->SetPos(truckposx, truckposy - 1, truckposz - 2.2f);
	primitivesPos[4] = { truckposx, truckposy - 1, truckposz - 2.2f };
	temp->color.Set(0.5, 0.5, 0.5);


	temp2 = new Cylinder(0.4, 0.2, 0.4);
	temp2->SetPos(truckposx + 3, truckposy - 1, truckposz);
	primitivesPos[5] = { truckposx + 3, truckposy - 1, truckposz };
	temp2->color.Set(0.1, 0.1, 0.1);
	temp2->body.GetBody()->setFriction(5);
	CarPrimitives.PushBack(temp2);

	temp2 = new Cylinder(0.4, 0.2, 0.4);
	temp2->SetPos(truckposx - 3, truckposy - 1, truckposz);
	primitivesPos[6] = { truckposx - 3, truckposy - 1, truckposz };
	temp2->color.Set(0.1, 0.1, 0.1);
	temp2->body.GetBody()->setFriction(5);
	CarPrimitives.PushBack(temp2);

	temp2 = new Cylinder(0.4, 0.2, 0.4);
	temp2->SetPos(truckposx + 3, truckposy - 1, truckposz - 5.5f);
	primitivesPos[7] = { truckposx + 3, truckposy - 1, truckposz - 5.5f };
	temp2->color.Set(0.1, 0.1, 0.1);
	temp2->body.GetBody()->setFriction(5);
	CarPrimitives.PushBack(temp2);

	temp2 = new Cylinder(0.4, 0.2, 0.4);
	temp2->SetPos(truckposx - 3, truckposy - 1, truckposz - 5.5f);
	primitivesPos[8] = { truckposx - 3, truckposy - 1, truckposz - 5.5f };
	temp2->color.Set(0.1, 0.1, 0.1);
	temp2->body.GetBody()->setFriction(5);
	CarPrimitives.PushBack(temp2);

	temp2 = new Cylinder(0.4, 0.2, 0.4);
	temp2->SetPos(truckposx + 3, truckposy - 1, truckposz - 2.2f);
	primitivesPos[9] = { truckposx + 3, truckposy - 1, truckposz - 2.2f };
	temp2->color.Set(0.1, 0.1, 0.1);
	temp2->body.GetBody()->setFriction(5);
	CarPrimitives.PushBack(temp2);

	temp2 = new Cylinder(0.4, 0.2, 0.4);
	temp2->SetPos(truckposx - 3, truckposy - 1, truckposz - 2.2f);
	primitivesPos[10] = { truckposx - 3, truckposy - 1, truckposz - 2.2f };
	temp2->color.Set(0.1, 0.1, 0.1);
	temp2->body.GetBody()->setFriction(5);
	CarPrimitives.PushBack(temp2);

	TruckBody = temp = new Cube(vec3(2.4, 0.25, 4.5));
	CarPrimitives.PushBack(temp);
	temp->SetPos(truckposx, truckposy + 1, truckposz - 3.5f);
	primitivesPos[11] = { truckposx, truckposy + 1, truckposz - 3.5f };
	temp->color.Set(0.75, 0.7, 0);


	temp = new Cube(vec3(0.25, 1, 4.5));
	CarPrimitives.PushBack(temp);
	temp->SetPos(truckposx - 2, truckposy + 1, truckposz - 3.5f);
	primitivesPos[12] = { truckposx - 2, truckposy + 1, truckposz - 3.5f };
	temp->color.Set(0, 0, 0.2);

	temp = new Cube(vec3(0.25, 1, 4.5));
	CarPrimitives.PushBack(temp);
		temp->color.Set(0, 0, 0.2);
	temp->SetPos(truckposx + 2, truckposy + 1, truckposz - 3.5f);
	primitivesPos[13] = { truckposx + 2, truckposy + 1, truckposz - 3.5f };



	App->physics->AddConstraintHinge(**CarPrimitives.At(0), **CarPrimitives.At(1), btVector3{ 0,-0.7 ,-1.2 }, btVector3{ 0,0, 2.6 }, btVector3{ 0, 1, 0 }, btVector3{ 0, 1, 0 });

	btTransform localA;
	btTransform localB;


	localA.setIdentity();
	localB.setIdentity();

	localA.setOrigin(btVector3(0.0, 0.0, 0));
	localB.setOrigin(btVector3(0.f, 1.6f, 0.f));
	TruckAxis[0] = App->physics->AddConstraintSlider(**CarPrimitives.At(0), **CarPrimitives.At(2), localA, localB);

	localB.setOrigin(btVector3(0.f, 0.7f, -0.5f));
	TruckAxis[1] = App->physics->AddConstraintSlider(**CarPrimitives.At(1), **CarPrimitives.At(3), localA, localB);

	localB.setOrigin(btVector3(0.f, 0.7f, 1.6f));
	TruckAxis[2] = App->physics->AddConstraintSlider(**CarPrimitives.At(1), **CarPrimitives.At(4), localA, localB);

	TruckWheels[0] = App->physics->AddConstraintHinge(**CarPrimitives.At(2), **CarPrimitives.At(5), btVector3{ 1.7f,-0.,-0 }, btVector3{ 0, 0,0 }, btVector3{ 1, 0,0 }, btVector3{ 0,1,0 });
	TruckWheels[1] = App->physics->AddConstraintHinge(**CarPrimitives.At(2), **CarPrimitives.At(6), btVector3{ -1.7f,-0.,-0 }, btVector3{ 0, 0,0 }, btVector3{ 1, 0,0 }, btVector3{ 0,1,0 });
	TruckWheels[2] = App->physics->AddConstraintHinge(**CarPrimitives.At(3), **CarPrimitives.At(7), btVector3{ 1.7f, 0, -0 }, btVector3{ 0, 0,0 }, btVector3{ 1, 0,0 }, btVector3{ 0,1,0 });
	TruckWheels[3] = App->physics->AddConstraintHinge(**CarPrimitives.At(3), **CarPrimitives.At(8), btVector3{ -1.7f, 0, -0 }, btVector3{ 0, 0,0 }, btVector3{ 1, 0,0 }, btVector3{ 0,1,0 });
	TruckWheels[4] = App->physics->AddConstraintHinge(**CarPrimitives.At(4), **CarPrimitives.At(9), btVector3{ 1.7f, 0, -0 }, btVector3{ 0, 0,0 }, btVector3{ 1, 0,0 }, btVector3{ 0,1,0 });
	TruckWheels[5] = App->physics->AddConstraintHinge(**CarPrimitives.At(4), **CarPrimitives.At(10), btVector3{ -1.7f, 0, -0 }, btVector3{ 0, 0,0 }, btVector3{ 1, 0,0 }, btVector3{ 0,1,0 });

	Elevator = App->physics->AddConstraintHinge(**CarPrimitives.At(1), **CarPrimitives.At(11), btVector3{ 0, -0 , -2.5 }, btVector3{ 0 ,-0.5 , -2. }, btVector3{ 1, 0, 0 }, btVector3{ 1, 0, 0 });
	App->physics->AddConstraintP2P(**CarPrimitives.At(11), **CarPrimitives.At(13), btVector3{ 1.1, 0., -2 }, btVector3{ -0.25, -0.5 , -2 });
	App->physics->AddConstraintP2P(**CarPrimitives.At(11), **CarPrimitives.At(12), btVector3{ -1.1, 0., -2 }, btVector3{ 0.25,-0.5 , -2 });
	App->physics->AddConstraintP2P(**CarPrimitives.At(11), **CarPrimitives.At(13), btVector3{ 1.1, 0., 2 }, btVector3{ -0.25, -0.5 , 2 });
	App->physics->AddConstraintP2P(**CarPrimitives.At(11), **CarPrimitives.At(12), btVector3{ -1.1, 0., 2 }, btVector3{ 0.25, -0.5 , 2 });

	App->physics->AddConstraintP2P(**CarPrimitives.At(11), **CarPrimitives.At(13), btVector3{ 1.1, 0.5, -2 }, btVector3{ -0.25,-0.0 , -2 });
	App->physics->AddConstraintP2P(**CarPrimitives.At(11), **CarPrimitives.At(12), btVector3{ -1.1,0.5, -2 }, btVector3{ 0.25, -0.0 , -2 });
	App->physics->AddConstraintP2P(**CarPrimitives.At(11), **CarPrimitives.At(13), btVector3{ 1.1, 0.5, 2 }, btVector3{ -0.25, -0.0 , 2 });
	App->physics->AddConstraintP2P(**CarPrimitives.At(11), **CarPrimitives.At(12), btVector3{ -1.1,0.5, 2 }, btVector3{ 0.25,  -0.0 , 2 });


	temp2 = new Cylinder(0.4, 0.4, 0.4);
	temp2->SetPos(truckposx, truckposy + 3, truckposz);
	CarPrimitives.PushBack(temp2);
	primitivesPos[14] = { truckposx, truckposy + 3, truckposz };
	temp2->color.Set(0, 0, ((std::rand() % 255)) / 255.f);

	temp = new Cube(vec3(0.4, 2, 0.2));
	CarPrimitives.PushBack(temp);
	temp->SetPos(truckposx, truckposy + 4, truckposz);
	primitivesPos[15] = { truckposx, truckposy + 4, truckposz };
	temp->color.Set(0, 0, ((std::rand() % 255)) / 255.f);


	temp = new Cube(vec3(0.3, 2.5, 0.1));
	CarPrimitives.PushBack(temp);
	temp->SetPos(truckposx + 1, truckposy + 3, truckposz);
	primitivesPos[16] = { truckposx + 1, truckposy + 3, truckposz };
	temp->color.Set(0, 0, ((std::rand() % 255)) / 255.f);


	Claw = new Cube(vec3(0.3, 0.6, 0.3), 0.01);
	CarPrimitives.PushBack(Claw);
	Claw->SetPos(truckposx + 1, truckposy + 5, truckposz);
	CarPrimitives[17]->name = "Claw";
	CarPrimitives[17]->body.collision_listeners.PushBack(this);
	primitivesPos[17] = { truckposx + 1, truckposy + 5, truckposz };
	temp->color.Set(0, 0, ((std::rand() % 255)) / 255.f);



	temp = new Cube(vec3(3, 1.2, 0.25));
	CarPrimitives.PushBack(temp);
	temp->SetPos(truckposx, truckposy, truckposz - 8);
	primitivesPos[18] = { truckposx , truckposy , truckposz - 8 };
	temp->color.Set(0, 0, ((std::rand() % 255)) / 255.f);

	HingeArm[0] = App->physics->AddConstraintHinge(**CarPrimitives.At(0), **CarPrimitives.At(14), btVector3{ 0, 1.2f, 0 }, btVector3{ 0, 0 , 0 }, btVector3{ 0, 1, 0 }, btVector3{ 0, 1 , 0 });
	HingeArm[1] = App->physics->AddConstraintHinge(**CarPrimitives.At(14), **CarPrimitives.At(15), btVector3{ 0, 0.4, 0 }, btVector3{ 0, -1 , 0 }, btVector3{ 0, 0, 1 }, btVector3{ 0, 0 , 1 });
	HingeArm[2] = App->physics->AddConstraintHinge(**CarPrimitives.At(15), **CarPrimitives.At(16), btVector3{ 0, 0.8, 0.15 }, btVector3{ 0, -1.25 , 0 }, btVector3{ 0, 0, 1 }, btVector3{ 0, 0 , 1 });
	App->physics->AddConstraintP2P(**CarPrimitives.At(16), **CarPrimitives.At(17), btVector3{ 0, 1.3, 0 }, btVector3{ 0, -0.3 , 0 });

	TruckWheels[6] = App->physics->AddConstraintHinge(**CarPrimitives.At(11), **CarPrimitives.At(18), btVector3{ 0, 0.2, -2.2 }, btVector3{ 0, -0.7, 0.125 }, btVector3{ 1, 0,0 }, btVector3{ 1,0,0 });


}