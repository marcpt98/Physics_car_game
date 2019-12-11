#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(bool start_enabled) : Module(start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));



	//Car body
	car = new Cube(vec3(1,1,2));
	primitives.PushBack(car);
	car->SetPos(0.f, 2.f, 2.5f);

	//Wheel Axis
	Cube* axis1 = new Cube(vec3(0.2, 0.7, 0.2));
	primitives.PushBack(axis1);
	axis1->SetPos(0.7f, 2.f, 2.f);

	//Wheel Axis
	Cube* axis2 = new Cube(vec3(0.2, 0.7, 0.2));
	primitives.PushBack(axis2);
	axis2->SetPos(-0.7f, 2.f, 2.f);

	//Wheel Axis
	Cube* axis3 = new Cube(vec3(0.2, 0.7, 0.2));
	primitives.PushBack(axis3);
	axis3->SetPos(0.7f, 2.f, 3.f);

	//Wheel Axis
	Cube* axis4 = new Cube(vec3(0.2, 0.7, 0.2));
	primitives.PushBack(axis4);
	axis4->SetPos(-0.7f, 2.f, 3.f);

	
	
	btTransform localA;
	btTransform localB;

	localA.setIdentity();
	localB.setIdentity();

	localA.getBasis().setEulerZYX(0, 0, M_PI_2);
	localA.setOrigin(btVector3(0.0, 0.0, -0.5));

	localB.getBasis().setEulerZYX(0, 0, M_PI_2);
	localB.setOrigin(btVector3(-0.6f, 0.8f, 0.5f));


	Axis[0] = App->physics->AddConstraintSlider(**primitives.At(0), **primitives.At(1), localA, localB);

	localB.setIdentity();
	localB.getBasis().setEulerZYX(0, 0, M_PI_2);
	localB.setOrigin(btVector3(0.6f, 0.8f, 0.5f));
	
	Axis[1] = App->physics->AddConstraintSlider(**primitives.At(0), **primitives.At(2), localA, localB);

	localB.setIdentity();
	localB.getBasis().setEulerZYX(0, 0, M_PI_2);
	localB.setOrigin(btVector3(-0.6f, 0.8f, -1.5f));

	Axis[2] = App->physics->AddConstraintSlider(**primitives.At(0), **primitives.At(3), localA, localB);

	localB.setIdentity();
	localB.getBasis().setEulerZYX(0, 0, M_PI_2);
	localB.setOrigin(btVector3(0.6f, 0.8f, -1.5f));

	Axis[3] = App->physics->AddConstraintSlider(**primitives.At(0), **primitives.At(4), localA, localB);


	
	Cylinder* wheel = new Cylinder(0.4, 0.2, 0.4);
	wheel->SetPos(-1.1f, 1.5f, 2.f);
	primitives.PushBack(wheel);
	wheel->body.GetBody()->setFriction(1);
	wheel = new Cylinder(0.4, 0.2, 0.4);
	wheel->SetPos(-1.1f, 1.5f, 3.f);
	wheel->body.GetBody()->setFriction(1);
	primitives.PushBack(wheel);
	wheel = new Cylinder(0.4, 0.2, 0.4);
	wheel->SetPos(1.1f, 1.5f, 2.f);
	wheel->body.GetBody()->setFriction(1);
	primitives.PushBack(wheel);
	wheel = new Cylinder(0.4, 0.2, 0.4);
	wheel->SetPos(1.1f, 1.5f, 3.f);
	wheel->body.GetBody()->setFriction(1);
	primitives.PushBack(wheel);

	wheels[0] = App->physics->AddConstraintHinge(**primitives.At(2), **primitives.At(5), btVector3{ -0.2,-0.2,-0}, btVector3{ 0, 0,0 }, btVector3{ 1, 0,0 }, btVector3{ 0,1,0 });
	wheels[1] = App->physics->AddConstraintHinge(**primitives.At(4), **primitives.At(6), btVector3{ -0.2,-0.2,-0}, btVector3{ 0, 0,0 }, btVector3{ 1, 0,0 }, btVector3{ 0,1,0 });
	wheels[2] = App->physics->AddConstraintHinge(**primitives.At(1), **primitives.At(7), btVector3{ 0.2,-0.2, -0 }, btVector3{ 0, 0,0 }, btVector3{ -1, 0,0 }, btVector3{ 0,1,0 });
	wheels[3] = App->physics->AddConstraintHinge(**primitives.At(3), **primitives.At(8), btVector3{ 0.2,-0.2, -0 }, btVector3{ 0, 0,0 }, btVector3{ -1, 0,0 }, btVector3{ 0,1,0 });
	

	/*
//	btTransform trans4 = axis3->body.GetBody()->getWorldTransform();
	App->physics->AddConstraintSlider(**primitives.At(0), **primitives.At(3), frameInA, frameInB3);

	//btTransform trans5 = axis4->body.GetBody()->getWorldTransform();
	App->physics->AddConstraintSlider(**primitives.At(0), **primitives.At(4), frameInA, frameInB4);
	*/
	//Sphere code
	/*
	Sphere* s = new Sphere(0.5);
	primitives.PushBack(s);
	s->SetPos(0.f, 2.5f, 2.f);
	*/
	/*
	const int SnakeLength = 7;
	const float StartingSize = 0.5f;
	const float SizeIncrement = 0.2f;
	const float BallDistance = 0.3f;

	float XPos = 0.f;
	float Size = StartingSize;
	Sphere* aux = nullptr;
	for (int n = 0; n < SnakeLength; n++)
	{
		Sphere* s = new Sphere(Size);
		primitives.PushBack(s);
		s->SetPos(XPos, 10.f, 2.5f);
		
		//TODO 2: Link all the spheres with your P2P constraints
		if (primitives.Count() > 1)
			App->physics->AddConstraintHinge(**primitives.At(n - 1), **primitives.At(n), btVector3{ (-s->GetRadius() - BallDistance),0,0 }, btVector3{ (aux->GetRadius() + BallDistance),0,0 }, btVector3{ 0,0,1 }, btVector3{ 0,0,1 });
			//App->physics->AddConstraintP2P(**primitives.At(n-1), **primitives.At(n), btVector3{ (-s->GetRadius()- BallDistance),0,0 }, btVector3{ (aux->GetRadius()+BallDistance),0,0 });

		aux = s;
		XPos += Size + Size + SizeIncrement + BallDistance;
		Size += SizeIncrement;
	}*/
	/*
	for (int n = 0; n < SnakeLength; n++)
	{
		Sphere* s = new Sphere(Size);
		primitives.PushBack(s);
		s->SetPos(XPos, 10.f, 2.5f);

		if (primitives.Count() > 1)
			App->physics->AddConstraintHinge(**primitives.At(n - 1), **primitives.At(n), btVector3{ (-s->GetRadius() - BallDistance),0,0 }, btVector3{ (aux->GetRadius() + BallDistance),0,0 }, btVector3{ 0,0,0 }, btVector3{ 0,0,0 });

		aux = s;
		XPos += Size + Size + SizeIncrement + BallDistance;
		Size += SizeIncrement;
	}
	*/
	//TODO 4: Link some other spheres with your Hinge constraint

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

void ModuleSceneIntro::HandleDebugInput()
{
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		DebugSpawnPrimitive(new Sphere());
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		DebugSpawnPrimitive(new Cube());
	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
		DebugSpawnPrimitive(new Cylinder());
	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
		for (uint n = 0; n < primitives.Count(); n++)
			primitives[n]->SetPos((float)(std::rand() % 40 - 20), 10.f, (float)(std::rand() % 40 - 20));
	if (App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN)
		for (uint n = 0; n < primitives.Count(); n++)
			primitives[n]->body.Push(vec3((float)(std::rand() % 500) - 250, 500, (float)(std::rand() % 500) - 250));

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		//TODO: NEW CODE
		//A snippet of new code that may be useful for you. Nothing to do here really


		//Get a vector indicating the direction from the camera viewpoint to the "mouse"
		const vec2 mousePos(((float)App->input->GetMouseX() / (float)App->window->Width()) * 2.f - 1.f,
			-((float)App->input->GetMouseY() / (float)App->window->Height()) * 2.f + 1.f);
		const vec4 rayEye = inverse(App->renderer3D->ProjectionMatrix) * vec4(mousePos.x, mousePos.y, -1.f, 1.f);
		const vec4 rayWorld(inverse(App->camera->GetViewMatrix()) * vec4(rayEye.x, rayEye.y, -1.f, 0.f));

		vec3 Dir(rayWorld.x, rayWorld.y, rayWorld.z);
		//Cast a ray from the camera, in the "mouse" direction
		PhysBody3D* body = App->physics->RayCast(App->camera->Position, Dir);
		if (body)
		{
			//Change the color of the clicked primitive
			body->parentPrimitive->color = Color((float)(std::rand() % 255) / 255.f, (float)(std::rand() % 255) / 255.f, (float)(std::rand() % 255) / 255.f);
		}
	}
}

void ModuleSceneIntro::DebugSpawnPrimitive(Primitive * p)
{
	primitives.PushBack(p);
	p->SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);
	p->body.collision_listeners.PushBack(this);
	p->body.Push(-App->camera->Z * 1000.f);
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(vec3(0, 1, 0));
	p.axis = true;
	p.Render();
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
	
	
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT){
		wheels[0]->enableAngularMotor(true, 50.f, 1000.f);
		wheels[1]->enableAngularMotor(true, 50.f, 100.f);
		wheels[2]->enableAngularMotor(true, -50.f, 1000.f);
		wheels[3]->enableAngularMotor(true, -50.f, 100.f);
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
			wheels[0]->enableAngularMotor(true, -50.f, 1000.f);
		wheels[1]->enableAngularMotor(true, -50.f, 100.f);
			wheels[2]->enableAngularMotor(true, 50.f, 1000.f);
	  wheels[3]->enableAngularMotor(true, 50.f, 100.f);
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

		wheels[1]->enableMotor(false);
		wheels[3]->enableMotor(false);

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
		wheels[1]->enableMotor(false);
		wheels[3]->enableMotor(false);
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

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) {
		for (uint n = 0; n < primitives.Count(); n++)
			primitives[n]->SetPos(0,0,0);
	}
	

	if (App->debug == true)
		HandleDebugInput();

	for (uint n = 0; n < primitives.Count(); n++)
		primitives[n]->Update();

	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate(float dt)
{
	for (uint n = 0; n < primitives.Count(); n++)
	{
		primitives[n]->Render();
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D * body1, PhysBody3D * body2)
{
	Color color = Color((float)(std::rand() % 255) / 255.f, (float)(std::rand() % 255) / 255.f, (float)(std::rand() % 255) / 255.f);

	body1->parentPrimitive->color = color;
	body2->parentPrimitive->color = color;

}