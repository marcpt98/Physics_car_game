#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePlayer.h"

ModuleSceneIntro::ModuleSceneIntro(bool start_enabled) : Module(start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	App->audio->PlayMusic("Assets/Music/BestSongEver.ogg");
	
	//Sensors
	Sensor_cube = new Cube(vec3(10, 15, 10), 0);
	Sensor_cube->SetPos(10, 0, -43);
	//ScenePrimitives.PushBack(Sensor_cube);
	Sensor_cube->body.SetAsSensor(true);
	Sensor_cube->name = "sensor1";
	Sensor_cube->body.collision_listeners.PushBack(this);
	Sensor_cube->body.is_sensor = true;

	Rainbow_cube = new Cube(vec3(10, 15, 60), 0);
	Rainbow_cube->SetPos(180, 0, -20);
	Rainbow_cube->body.SetAsSensor(true);
	Rainbow_cube->name = "sensor2";
	Rainbow_cube->body.collision_listeners.PushBack(this);
	Rainbow_cube->body.is_sensor = true;
	//Sphere creation 
	ball1 = new Sphere(1.2, 5);
	ScenePrimitives.PushBack(ball1);
	ball1->SetPos(-165, 1, -36);
	ScenePrimitives[0]->name = "ball1";

	ball1 = new Sphere(1.2, 5);
	ScenePrimitives.PushBack(ball1);
	ball1->SetPos(40, 1, 175);
	ScenePrimitives[1]->name = "ball2";

	ball1 = new Sphere(1.2, 5);
	ScenePrimitives.PushBack(ball1);
	ball1->SetPos(-20, 1, 175);
	ScenePrimitives[2]->name = "ball3";

	ball1 = new Sphere(1.2, 5);
	ScenePrimitives.PushBack(ball1);
	ball1->SetPos(-98, 1, -118);
	ScenePrimitives[3]->name = "ball4";

	ball1 = new Sphere(1.2, 5);
	ScenePrimitives.PushBack(ball1);
	ball1->SetPos(-98, 1, 40);
	ScenePrimitives[4]->name = "ball5";

	

	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));
	camerafollow = true;
	
	
	// Creating map
	CreateMap();
	
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
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		DebugSpawnPrimitive(new Sphere());
	if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
		DebugSpawnPrimitive(new Cube());

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		
		

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
	ScenePrimitives.PushBack(p);
	p->SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);
	p->body.collision_listeners.PushBack(this);
	p->body.Push(-App->camera->Z * 1000.f);
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{

	Plane p(vec3(0, 1, 0));
	p.axis = true;
	//p.Render();

	if (App->debug == true)
		HandleDebugInput();

	for (uint n = 0; n < App->player->CarPrimitives.Count(); n++)
		App->player->CarPrimitives[n]->Update();

	for (uint n = 0; n < ScenePrimitives.Count(); n++)
		ScenePrimitives[n]->Update();


	if (case1 == true && case2 == true && case3 == true && case4 == true && case5 == true)
	{
		finalWall->SetPos(48, 31, -32.5);
	}
	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate(float dt)
{
	for (uint n = 0; n < App->player->CarPrimitives.Count(); n++)
		App->player->CarPrimitives[n]->Render();


	for (uint n = 0; n < ScenePrimitives.Count(); n++)
		ScenePrimitives[n]->Render();

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D * body1, PhysBody3D * body2)
{
	Color color = Color((float)(std::rand() % 255) / 255.f, (float)(std::rand() % 255) / 255.f, (float)(std::rand() % 255) / 255.f);

	//body1->parentPrimitive->color = color;
	//body2->parentPrimitive->color = color;
	if (body1->parentPrimitive->name == "sensor1" && body2->parentPrimitive->name == "ball1") {
		//body2->SetPos(App->player->TruckBody->body.GetPos().x, App->player->TruckBody->body.GetPos().y + 1, App->player->TruckBody->body.GetPos().z);
		//body2->parentPrimitive->name = "none";
		leg1->color = {255,255,255};
		LOG("A spicy meatball");
		case1 = true;
	}
	if (body1->parentPrimitive->name == "sensor1" && body2->parentPrimitive->name == "ball2") {
		leg2->color = { 255,255,255 };
		LOG("A spicy meatball");
		case2 = true;
	}
	if (body1->parentPrimitive->name == "sensor1" && body2->parentPrimitive->name == "ball3") {
		body->color = { 255,255,255 };
		LOG("A spicy meatball");
		case3 = true;
	}
	if (body1->parentPrimitive->name == "sensor1" && body2->parentPrimitive->name == "ball4") {
		arm->color = { 255,255,255 };
		LOG("A spicy meatball");
		case4 = true;
	}
	if (body1->parentPrimitive->name == "sensor1" && body2->parentPrimitive->name == "ball5") {
		head->color = { 255,255,255 };
		LOG("A spicy meatball");
		case5 = true;
	}

	if (body1->parentPrimitive->name == "sensor2" && body2->parentPrimitive->name == "Cart") {
		EndGame = true;
	}
}

void ModuleSceneIntro::CreateMap()
{
	// 1 (first rect)
	App->physics->CreateLineBox(46, 1, -193, 1, vec3(28, 20, 5), 12, 3);
	App->physics->CreateLineBox(60, 1, -185, 7, vec3(6, 0, 10), 12, 3, true);//x y z /number of block u want / block size / distance betwen blocks / direction of the line/ true=random high
	App->physics->CreateLineBox(31, 1, -185, 4, vec3(6, 0, 10), 12, 3, true);

	// 2 (second rect)
	App->physics->CreateLineBox(52, 1, -104, 3, vec3(10, 0, 6), 12, 1, true);
	App->physics->CreateLineBox(29, 1, -140, 3, vec3(10, 0, 6), 12, 1, true);

	// 3 (third rect)
	App->physics->CreateLineBox(-5, 1, -130, 6, vec3(6, 0, 10), 12, 3, true);
	App->physics->CreateLineBox(26, 1, -94, 3, vec3(6, 0, 10), 12, 3, true);

	// 4 (roundabout, interior)
	//App->physics->CreateLineBox(15, 1, -48, 5, vec3(2, 5, 1), 3, 1);
	App->physics->CreateLineBox(15, 1, -48, 2, vec3(2, 5, 1), 2, 1);
	App->physics->CreateLineBox(5, 1, -48, 2, vec3(2, 5, 1), 2, 1);
	App->physics->CreateLineBox(5.5, 1, -44, 1, vec3(1, 5, 7), 2, 1);
	App->physics->CreateLineBox(12.5, 1, -44, 1, vec3(1, 5, 7), 2, 1);
	App->physics->CreateLineBox(9, 1, -40, 1, vec3(8, 5, 1), 2, 1);

	App->physics->CreateDiagonalBox(18, 1, -47, 4, vec3(1, 5, 2), 2, 1, 4);
	App->physics->CreateDiagonalBox(0, 1, -47, 4, vec3(1, 5, 2), 2, 3, 4);
	App->physics->CreateDiagonalBox(18, 1, -24, 3, vec3(1, 5, 2), 2, 0, 4);
	App->physics->CreateDiagonalBox(-4, 1, -32, 3, vec3(1, 5, 2), 2, 1, 4);
	App->physics->CreateLineBox(15, 1, -22, 5, vec3(2, 5, 1), 3, 1);

	// 5 (figure) Has to be done like this in order to change every part color
	leg1 = new Cube({ 3, 7, 3 }, 0);
	leg2 = new Cube({ 3, 7, 3 }, 0);
	body = new Cube({ 10, 7, 5 }, 0);
	arm = new Cube({ 20, 3, 5 }, 0);
	head = new Cube({ 7, 7, 7 }, 0);

	ScenePrimitives.PushBack(leg1);
	ScenePrimitives.PushBack(leg2);
	ScenePrimitives.PushBack(body);
	ScenePrimitives.PushBack(arm);
	ScenePrimitives.PushBack(head);

	leg1->SetPos(12, 1, -36);
	leg2->SetPos(7, 1, -36);
	body->SetPos(9, 8, -36);
	arm->SetPos(9, 13, -36);
	head->SetPos(9, 18, -36);

	ScenePrimitives[6]->name = "leg1";
	ScenePrimitives[7]->name = "leg2";
	ScenePrimitives[8]->name = "body";
	ScenePrimitives[9]->name = "arm";
	ScenePrimitives[10]->name = "head";

	leg1->color = { ((float)(std::rand() % 255) / 255.f), ((float)(std::rand() % 255) / 255.f), ((float)(std::rand() % 255) / 255.f) };
	leg2->color = { ((float)(std::rand() % 255) / 255.f), ((float)(std::rand() % 255) / 255.f), ((float)(std::rand() % 255) / 255.f) };
	body->color = { ((float)(std::rand() % 255) / 255.f), ((float)(std::rand() % 255) / 255.f), ((float)(std::rand() % 255) / 255.f) };
	arm->color = { ((float)(std::rand() % 255) / 255.f), ((float)(std::rand() % 255) / 255.f), ((float)(std::rand() % 255) / 255.f) };
	head->color = { ((float)(std::rand() % 255) / 255.f), ((float)(std::rand() % 255) / 255.f), ((float)(std::rand() % 255) / 255.f) };
	
	//This code can't change colors
	/*App->physics->CreateLineBox(12, 1, -36, 1, vec3(3, 7, 3));
	App->physics->CreateLineBox(7, 1, -36, 1, vec3(3, 7, 3));
	App->physics->CreateLineBox(9, 8, -36, 1, vec3(10, 7, 5));
	App->physics->CreateLineBox(9, 13, -36, 1, vec3(20, 3, 5));
	App->physics->CreateLineBox(9, 18, -36, 1, vec3(7, 7, 7));*/

	// 6 (roundabout, exterior)
	App->physics->CreateDiagonalBox(28, 1, -62, 5, vec3(1, 5, 2), 2, 1, 4);
	App->physics->CreateDiagonalBox(-9, 1, -62, 5, vec3(1, 5, 2), 2, 3, 4);
	App->physics->CreateDiagonalBox(-18, 1, -19, 5, vec3(1, 5, 2), 2, 1, 4);
	App->physics->CreateDiagonalBox(36, 1, -19, 5, vec3(1, 5, 2), 2, 3, 4);

	// FIRST 3 BALLS
	// 7 (first rect)
	App->physics->CreateLineBox(-24, 1, -25, 5, vec3(10, 0, 6), 12, 1, true);
	App->physics->CreateLineBox(-24, 1, -48, 5, vec3(10, 0, 6), 12, 1, true);
	
	// 8 (Left were u pick the spheres)
	App->physics->CreateLineBox(-82, 1, -122, 7, vec3(6, 0, 10), 12, 3, true);
	App->physics->CreateLineBox(-82, 1, -23, 7, vec3(6, 0, 10), 12, 3, true);
	App->physics->CreateLineBox(-87, 1, 59, 3, vec3(10, 0, 6), 12, 1, true);


	// 9 (Right were u pick the spheres)
	App->physics->CreateLineBox(-115, 1, -122, 7, vec3(6, 0, 10), 12, 3, true);
	App->physics->CreateLineBox(-115, 1, -23, 7, vec3(6, 0, 10), 12, 3, true);
	App->physics->CreateLineBox(-87, 1, -132, 3, vec3(10, 0, 6), 12, 1, true);

	// 10 (Forward were u pick the spheres)
	App->physics->CreateLineBox(-125, 1, -48, 5, vec3(10, 0, 6), 12, 1, true);
	App->physics->CreateLineBox(-125, 1, -25, 5, vec3(10, 0, 6), 12, 1, true);
	App->physics->CreateLineBox(-183, 1, -48, 3, vec3(6, 0, 10), 12, 3, true);

	// SECOND 2 BALLS
	// 11 (first rect)
	App->physics->CreateLineBox(24, 1, 4, 3, vec3(6, 0, 10), 12, 3, true);
	App->physics->CreateLineBox(-6, 1, 4, 3, vec3(6, 0, 10), 12, 3, true);
	
	// 12 (Bridge)
	App->physics->CreateRamp(9, 0, 34, 31, vec3(26, 0.3, 1), 1, 3, 0);
	App->physics->CreateLineBox(9, 1, 77, 1, vec3(26, 7, 25), 12, 3);
	App->physics->CreateRamp(9, 1, 90, 35, vec3(26, 7, 1), 1, 3, 1);
	App->physics->CreateLineBox(23, 1, 35, 31, vec3(2, 0, 2), 3, 3, true, true);
	App->physics->CreateLineBox(-5, 1, 35, 31, vec3(2, 0, 2), 3, 3, true, true);

	// 13 (right part)
	App->physics->CreateLineBox(-37, 1, 123, 3, vec3(10, 0, 6), 12, 0, true);
	App->physics->CreateDiagonalBox(-43, 1, 127, 5, vec3(1, 5, 2), 2, 3, 4);
	App->physics->CreateLineBox(-55, 1, 150, 2, vec3(6, 0, 10), 12, 3, true);
	App->physics->CreateDiagonalBox(-52, 1, 169, 5, vec3(1, 5, 2), 2, 1, 4);
	App->physics->CreateLineBox(-37, 1, 188, 8, vec3(10, 0, 6), 12, 0, true);
	App->physics->CreateDiagonalBox(-7, 1, 183, 5, vec3(1, 5, 2), 2, 0, 4);
	App->physics->CreateLineBox(-28, 1, 162, 7, vec3(10, 0, 6), 12, 0, true);

	// 14 (left part)
	App->physics->CreateLineBox(55, 1, 123, 3, vec3(10, 0, 6), 12, 1, true);
	App->physics->CreateDiagonalBox(61, 1, 127, 5, vec3(1, 5, 2), 2, 1, 4);
	App->physics->CreateLineBox(73, 1, 150, 2, vec3(6, 0, 10), 12, 3, true);
	App->physics->CreateDiagonalBox(62, 1, 185, 5, vec3(1, 5, 2), 2, 0, 4);
	App->physics->CreateLineBox(58, 1, 186, 2, vec3(2, 5, 1), 3, 1);
	App->physics->CreateDiagonalBox(15, 1, 167, 5, vec3(1, 5, 2), 2, 1, 4);

	// THIRD 1 BALL
	// 15 (first rect)
	App->physics->CreateLineBox(42, 1, -17, 1, vec3(10, 0, 6), 12, 0, true);
	App->physics->CreateLineBox(42, 1, -48, 1, vec3(10, 0, 6), 12, 0, true);

	// 16 (tunnel)
	App->physics->CreateLineBox(49, 1, -19, 60, vec3(2, 28, 2), 2, 0, false, false, false, true, false);
	App->physics->CreateLineBox(49, 1, -46, 60, vec3(2, 28, 2), 2, 0, false, false, true, false, false);
	App->physics->CreateLineBox(49, 16, -32.5, 60, vec3(2, 2, 29), 2, 0, false, false, false, false, true);
	App->physics->CreateDiagonalBox(169, 1, -38, 14, vec3(2, 12, 2), 2, 0, 1);
	App->physics->CreateDiagonalBox(169, 1, -27, 14, vec3(2, 12, 2), 2, 1, 1);
	App->physics->CreateLineBox(197, 1, -32.5, 1, vec3(2, 12, 39), 2, 0);
	App->physics->CreateLineBox(196, 8, -32.5, 1, vec3(4, 2, 39), 2, 0);
	// Cover last part
	App->physics->CreateLineBox(193, 8, -32.5, 1, vec3(2, 2, 37), 2, 0);
	App->physics->CreateLineBox(191, 8, -32.5, 1, vec3(2, 2, 35), 2, 0);
	App->physics->CreateLineBox(189, 8, -32.5, 1, vec3(2, 2, 33), 2, 0);
	App->physics->CreateLineBox(187, 8, -32.5, 1, vec3(2, 2, 31), 2, 0);
	App->physics->CreateLineBox(185, 8, -32.5, 1, vec3(2, 2, 29), 2, 0);
	App->physics->CreateLineBox(183, 8, -32.5, 1, vec3(2, 2, 27), 2, 0);
	App->physics->CreateLineBox(181, 8, -32.5, 1, vec3(2, 2, 25), 2, 0);
	App->physics->CreateLineBox(179, 8, -32.5, 1, vec3(2, 2, 23), 2, 0);
	App->physics->CreateLineBox(177, 8, -32.5, 1, vec3(2, 2, 21), 2, 0);
	App->physics->CreateLineBox(175, 8, -32.5, 1, vec3(2, 2, 19), 2, 0);
	App->physics->CreateLineBox(173, 8, -32.5, 1, vec3(2, 2, 17), 2, 0);
	App->physics->CreateLineBox(171, 8, -32.5, 1, vec3(2, 2, 15), 2, 0);
	App->physics->CreateLineBox(169, 8, -32.5, 1, vec3(2, 2, 13), 2, 0);

	////////////////////////////////////////////////////////////////////////////////////////////////////////	
	// Floor
	App->physics->CreateFloor(0, 0, 0, vec3(400, 0, 400));

	// DECORATION
	// 17 (first rect)
	App->physics->CreateLineBox(36.5, 0, -164.2, 1, vec3(5, 0.5, 54.3), 0, 0);
	App->physics->CreateLineBox(54.5, 0, -149, 1, vec3(5, 0.5, 83.5), 0, 0);
	App->physics->CreateLamp(39.5, 0, -185, 5, 13, 3);
	App->physics->CreateLamp(51.5, 0, -185, 6, 13, 3);
	App->physics->CreatePerson(37.5, 0, -164.2, 2, 25, 3);
	App->physics->CreatePerson(54.5, 0, -120.2, 1, 25, 3);

	// 18 (second rect)
	App->physics->CreateLineBox(37.5, 0, -109.5, 1, vec3(29, 0.5, 5), 0, 1);
	App->physics->CreateLineBox(18.5, 0, -134.5, 1, vec3(41, 0.5, 5), 0, 1);
	App->physics->CreateLamp(34.5, 0, -131.5, 3, 13, 1);
	App->physics->CreateLamp(49.5, 0, -112.5, 3, 13, 1);

	//19 (third rect)
	App->physics->CreateLineBox(0.5, 0, -98.5, 1, vec3(5, 0.5, 67), 0, 0);
	App->physics->CreateLineBox(20.5, 0, -88.5, 1, vec3(5, 0.5, 47), 0, 0);
	App->physics->CreateLamp(17.5, 0, -110.5, 4, 13, 3);
	App->physics->CreateLamp(3.5, 0, -130.5, 6, 13, 3);
	App->physics->CreatePerson(20.5, 0, -104.2, 2, 25, 3);
	App->physics->CreatePerson(0.5, 0, -85.2, 1, 25, 3);

	// FIRST 3 BALLS
	// 20 (initial rect)
	App->physics->CreateLineBox(-52, 0, -43.5, 1, vec3(66, 0.5, 3), 0, 1);
	App->physics->CreateLineBox(-52, 0, -29.5, 1, vec3(66, 0.5, 3), 0, 1);
	App->physics->CreateLamp(-21, 0, -31.5, 2, 40, 1);
	App->physics->CreateLamp(-40, 0, -41.5, 2, 40, 1);
	App->physics->CreatePerson(-30, 0, -43.5, 2, 40, 1);

	// 21 (left side)
	App->physics->CreateLineBox(-87.5, 0, 12.5, 1, vec3(5, 0.5, 87), 0, 1);
	App->physics->CreateLineBox(-109.5, 0, 14, 1, vec3(5, 0.5, 84), 0, 1);
	App->physics->CreateLineBox(-98.5, 0, 53.5, 1, vec3(17, 0.5, 5), 0, 1);
	App->physics->CreateLamp(-90.5, 0, -29, 7, 13, 3);
	App->physics->CreateLamp(-106.5, 0, -22, 6, 13, 3);
	App->physics->CreateLamp(-103, 0, 50.5, 2, 8, 0);
	App->physics->CreatePerson(-87, 0, -23, 2, 40, 3);
	App->physics->CreatePerson(-109, 0, -15, 3, 30, 3);

	// 22 (right side)
	App->physics->CreateLineBox(-87.5, 0, -85.5, 1, vec3(5, 0.5, 87), 0, 1);
	App->physics->CreateLineBox(-109.5, 0, -87, 1, vec3(5, 0.5, 84), 0, 1);
	App->physics->CreateLineBox(-98.5, 0, -126.5, 1, vec3(17, 0.5, 5), 0, 1);
	App->physics->CreateLamp(-90.5, 0, -44, 7, 13, 2);
	App->physics->CreateLamp(-106.5, 0, -50, 6, 13, 2);
	App->physics->CreateLamp(-103, 0, -123.5, 2, 8, 0);
	App->physics->CreatePerson(-87, 0, -63, 2, 25, 2);
	App->physics->CreatePerson(-109, 0, -55, 3, 25, 2);
	App->physics->CreatePerson(-150, 0, -43.5, 1, 40, 1);

	// 23 (forward side)
	App->physics->CreateLineBox(-143.5, 0, -43.5, 1, vec3(73, 0.5, 3), 0, 1);
	App->physics->CreateLineBox(-143.5, 0, -29.5, 1, vec3(73, 0.5, 3), 0, 1);
	App->physics->CreateLamp(-110, 0, -31.5, 2, 40, 1);
	App->physics->CreateLamp(-125, 0, -41.5, 2, 40, 1);
	App->physics->CreateLineBox(-178.5, 0, -36.5, 1, vec3(3, 0.5, 11), 0, 1);
	App->physics->CreateLamp(-176.5, 0, -36.5, 1, 40, 1);

	// SECOND 2 BALLS
	// 24 (entrance)
	App->physics->CreateLineBox(8, 0, 156.5, 1, vec3(82, 0.5, 5), 0, 1);
	App->physics->CreateLineBox(-25, 0, 128.5, 1, vec3(34, 0.5, 5), 0, 1);
	App->physics->CreateLineBox(43, 0, 128.5, 1, vec3(34, 0.5, 5), 0, 1);
	App->physics->CreateLamp(43, 0, 153.5, 10, 8, 1);
	App->physics->CreateLamp(55, 0, 131.5, 4, 8, 1);
	App->physics->CreateLamp(-13, 0, 131.5, 4, 8, 1);
	App->physics->CreatePerson(-13, 0, 128.5, 2, 65, 0);
	App->physics->CreatePerson(0, 0, 156.5, 4, 7, 0);

	// 25 (forward)
	App->physics->CreateLineBox(-49.5, 0, 156, 1, vec3(5, 0.5, 22), 0, 1);
	App->physics->CreateLineBox(67.5, 0, 156, 1, vec3(5, 0.5, 22), 0, 1);
	App->physics->CreateLamp(-46.5, 0, 147.5, 3, 8, 3);
	App->physics->CreateLamp(64.5, 0, 147.5, 3, 8, 3);
	App->physics->CreatePerson(67, 0, 156.5, 1, 7, 0);
	App->physics->CreatePerson(37, 0, 183.5, 2, 60, 1);

	// 26 (right part)
	App->physics->CreateLineBox(-25, 0, 183.5, 1, vec3(34, 0.5, 3), 0, 1);
	App->physics->CreateLineBox(-18, 0, 166.5, 1, vec3(30, 0.5, 3), 0, 1);
	App->physics->CreateLamp(-13, 0, 181.5, 2, 16, 1);
	App->physics->CreateLamp(-5, 0, 168.5, 2, 16, 1);

	// 27 (left part)
	App->physics->CreateLineBox(38.5, 0, 183.5, 1, vec3(27, 0.5, 3), 0, 1);
	App->physics->CreateLineBox(34, 0, 166.5, 1, vec3(30, 0.5, 3), 0, 1);
	App->physics->CreateLamp(46.5, 0, 181.5, 2, 16, 1);
	App->physics->CreateLamp(38.5, 0, 168.5, 2, 16, 1);


	// Final wall
	finalWall = new Cube({ 2, 28, 25 }, 0);
	ScenePrimitives.PushBack(finalWall);
	finalWall->SetPos(49, 1, -32.5);
	ScenePrimitives[11]->name = "finalWall";
	finalWall->color = { 1,1,1 };
}