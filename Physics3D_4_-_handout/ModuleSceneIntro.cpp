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
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		DebugSpawnPrimitive(new Sphere());
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		DebugSpawnPrimitive(new Cube());
	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
		DebugSpawnPrimitive(new Cylinder());
	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
		for (uint n = 0; n < ScenePrimitives.Count(); n++)
			ScenePrimitives[n]->SetPos((float)(std::rand() % 40 - 20), 10.f, (float)(std::rand() % 40 - 20));
	if (App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN)
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
	/*// First rect
	App->physics->CreateLineBox(5, 1, 0, 15, vec3(1, 2, 1), 2, 3);
	App->physics->CreateLineBox(-5, 1, 0, 10, vec3(1, 2, 1), 2, 3);

	// First diagonal
	App->physics->CreateDiagonalBox(5, 1, 30, 15, vec3(1, 2, 1), 1.5f, 3, 1);
	App->physics->CreateDiagonalBox(-5, 1, 20, 9, vec3(1, 2, 1), 1.5f, 3, 1);

	// Second rect
	App->physics->CreateLineBox(-26, 1, 44, 5, vec3(1, 2, 1), 2, 0);
	App->physics->CreateLineBox(-21, 1, 28, 2, vec3(1, 2, 1), 2, 0);

	// Second diagonal
	App->physics->CreateDiagonalBox(-28, 1, 44, 10, vec3(1, 2, 1), 1.5f, 2, 1);
	App->physics->CreateDiagonalBox(-23, 1, 28, 5, vec3(1, 2, 1), 1.5f, 2, 1);


	//App->physics->CreateCurveBox(10, 1.f, 15.f, 7, vec3(0.5f, 1.f, 0.5f), 0.5f, 0, 0.5f);*/

	// 1
	App->physics->CreateLineBox(26, 20, -195, 1, vec3(28, 20, 5), 12, 3);
	App->physics->CreateLineBox(40, 1, -185, 6, vec3(6, 0, 10), 12, 3, true);
	App->physics->CreateLineBox(11, 1, -185, 6, vec3(6, 0, 10), 12, 3, true);

	// 2
	App->physics->CreateLineBox(0, 1, 5, 1, vec3(1, 1, 1), 12, 3);
	App->physics->CreateLineBox(0.5, 1, 5, 1, vec3(3, 1, 1), 12, 3);
}