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
	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		DebugSpawnPrimitive(new Cylinder());
	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		for (uint n = 0; n < ScenePrimitives.Count(); n++)
			ScenePrimitives[n]->SetPos((float)(std::rand() % 40 - 20), 10.f, (float)(std::rand() % 40 - 20));
	if (App->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
		for (uint n = 0; n < ScenePrimitives.Count(); n++)
			ScenePrimitives[n]->body.Push(vec3((float)(std::rand() % 500) - 250, 500, (float)(std::rand() % 500) - 250));

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
	p.Render();

	if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN) {
		for (uint n = 0; n < ScenePrimitives.Count(); n++)
			ScenePrimitives[n]->SetPos(0,0,0);
	}
	

	if (App->debug == true)
		HandleDebugInput();

	for (uint n = 0; n < App->player->CarPrimitives.Count(); n++)
		App->player->CarPrimitives[n]->Update();

	for (uint n = 0; n < ScenePrimitives.Count(); n++)
		ScenePrimitives[n]->Update();

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

	body1->parentPrimitive->color = color;
	body2->parentPrimitive->color = color;

}

void ModuleSceneIntro::CreateMap()
{
	// 1 (first rect)
	App->physics->CreateLineBox(46, 1, -193, 1, vec3(28, 20, 5), 12, 3);
	App->physics->CreateLineBox(60, 1, -185, 7, vec3(6, 0, 10), 12, 3, true);//x y z /number of block u want / block size / distance betwen blocks / direction of the line/ true=random high
	App->physics->CreateLineBox(31, 1, -185, 4, vec3(6, 0, 10), 12, 3, true);

	// 2 (second rect)
	App->physics->CreateLineBox(52, 1, -104, 3, vec3(10, 0, 6), 13, 1, true);
	App->physics->CreateLineBox(29, 1, -140, 3, vec3(10, 0, 6), 13, 1, true);

	// 3 (third rect)
	App->physics->CreateLineBox(-5, 1, -130, 6, vec3(6, 0, 10), 12, 3, true);
	App->physics->CreateLineBox(24, 1, -94, 3, vec3(6, 0, 10), 12, 3, true);

	// 4 (roundabout, interior)
	App->physics->CreateLineBox(15, 1, -48, 5, vec3(2, 5, 1), 3, 1);
	App->physics->CreateDiagonalBox(18, 1, -47, 4, vec3(1, 5, 2), 2, 1, 4);
	App->physics->CreateDiagonalBox(0, 1, -47, 4, vec3(1, 5, 2), 2, 3, 4);
	App->physics->CreateDiagonalBox(18, 1, -24, 3, vec3(1, 5, 2), 2, 0, 4);
	App->physics->CreateDiagonalBox(-4, 1, -32, 3, vec3(1, 5, 2), 2, 1, 4);
	App->physics->CreateLineBox(15, 1, -22, 5, vec3(2, 5, 1), 3, 1);

	// 5 (figure)
	App->physics->CreateLineBox(12, 1, -36, 1, vec3(3, 7, 3), 3, 1);
	App->physics->CreateLineBox(7, 1, -36, 1, vec3(3, 7, 3), 3, 1);
	App->physics->CreateLineBox(9, 8, -36, 1, vec3(10, 7, 5), 3, 1);
	App->physics->CreateLineBox(9, 13, -36, 1, vec3(20, 3, 5), 3, 1);
	App->physics->CreateLineBox(9, 18, -36, 1, vec3(7, 7, 7), 3, 1);

	// 6 (roundabout, exterior)
	App->physics->CreateDiagonalBox(28, 1, -62, 5, vec3(1, 5, 2), 2, 1, 4);
	App->physics->CreateDiagonalBox(-9, 1, -62, 5, vec3(1, 5, 2), 2, 3, 4);
	App->physics->CreateDiagonalBox(-18, 1, -19, 5, vec3(1, 5, 2), 2, 1, 4);
	App->physics->CreateDiagonalBox(36, 1, -19, 5, vec3(1, 5, 2), 2, 3, 4);

	// FIRST 3 BALLS
	// 7 (first rect)
	App->physics->CreateLineBox(-24, 1, -25, 5, vec3(10, 0, 6), 13, 1, true);
	App->physics->CreateLineBox(-24, 1, -48, 5, vec3(10, 0, 6), 13, 1, true);
	
	// 8 (Left were u pick the spheres)
	App->physics->CreateLineBox(-85, 1, -122, 7, vec3(6, 0, 10), 12, 3, true);
	App->physics->CreateLineBox(-85, 1, -23, 7, vec3(6, 0, 10), 12, 3, true);
	App->physics->CreateLineBox(-87, 1, 59, 3, vec3(10, 0, 6), 12, 1, true);


	// 9 (Right were u pick the spheres)
	App->physics->CreateLineBox(-115, 1, -122, 7, vec3(6, 0, 10), 12, 3, true);
	App->physics->CreateLineBox(-115, 1, -23, 7, vec3(6, 0, 10), 12, 3, true);
	App->physics->CreateLineBox(-87, 1, -132, 3, vec3(10, 0, 6), 12, 1, true);

	// 10 (Forward were u pick the spheres)
	App->physics->CreateLineBox(-125, 1, -50, 5, vec3(10, 0, 6), 12, 1, true);
	App->physics->CreateLineBox(-125, 1, -25, 5, vec3(10, 0, 6), 12, 1, true);
	App->physics->CreateLineBox(-183, 1, -48, 3, vec3(6, 0, 10), 12, 3, true);

	// SECOND 2 BALLS
	// 11 (first rect)
	App->physics->CreateLineBox(24, 1, 4, 3, vec3(6, 0, 10), 12, 3, true);
	App->physics->CreateLineBox(-6, 1, 4, 3, vec3(6, 0, 10), 12, 3, true);
	
	// 12 (Bridge)
	App->physics->CreateRamp(9, 1, 34, 28, vec3(26, 0.3, 1), 1.2, 3, 0);
	App->physics->CreateLineBox(9, 1, 80, 1, vec3(26, 9, 25), 12, 3);
	App->physics->CreateRamp(9, 1, 93.5, 28, vec3(26, 9, 1), 1.2, 3, 1);
	App->physics->CreateLineBox(24, 1, 35, 31, vec3(2, 0, 2), 3, 3, true, true);
	App->physics->CreateLineBox(-6, 1, 35, 31, vec3(2, 0, 2), 3, 3, true, true);

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
	App->physics->CreateLineBox(42, 1, -17, 1, vec3(10, 0, 6), 13, 0, true);
	App->physics->CreateLineBox(42, 1, -48, 1, vec3(10, 0, 6), 13, 0, true);
	
	
	
	
	// X (City edificies that don't create track)
	App->physics->CreateLineBox(-75, 1, -132, 7, vec3(6, 0, 10), 12, 3, true);
	App->physics->CreateLineBox(-65, 1, -132, 7, vec3(6, 0, 10), 12, 3, true);
	App->physics->CreateLineBox(-55, 1, -132, 7, vec3(6, 0, 10), 12, 3, true);
	App->physics->CreateLineBox(-45, 1, -132, 7, vec3(6, 0, 10), 12, 3, true);
	App->physics->CreateLineBox(-35, 1, -132, 7, vec3(6, 0, 10), 12, 3, true);
	App->physics->CreateLineBox(-25, 1, -132, 7, vec3(6, 0, 10), 12, 3, true);
	App->physics->CreateLineBox(-15, 1, -132, 7, vec3(6, 0, 10), 12, 3, true);

	
	
}