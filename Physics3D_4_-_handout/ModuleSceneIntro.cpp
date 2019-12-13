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
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		if (camerafollow ==false) 
		{
			camerafollow = true;
		}
		else if (camerafollow == true) 
		{
			camerafollow = false;
		}
	}

	if (camerafollow == true) 
	{
		/*
		const vec3 pa = App->player->temp->body.GetPos();
		const vec3 camera_offset(0, 6, -10);
		const vec3 r = pa + camera_offset;
		App->camera->Look(r, pa, false);

		vec3 d;
		//App->camera->Look(r, pa, false);
		d.Set(App->player->temp2->body.GetPos().x, App->player->temp2->body.GetPos().y + 5, App->player->temp2->body.GetPos().z);
		App->camera->Position = d;
		App->camera->LookAt(App->player->TruckCab->body.GetPos());
		*/
		const vec3 pa = App->player->TruckCab->body.GetPos();
		const vec3 f = App->player->temp->body.GetForwardVector();
		vec3 d; 
		d.Set(pa.x + (f.x * -11), pa.y + (f.y + 5), pa.z + (f.z * -11));
		App->camera->Look(d,pa);

	}

	//const vec3 f = car->body.GetForwardVector();
	//vec3 d; d.Set(pa.x + (f.x * -8), pa.y + (f.y + 5), pa.z + (f.z * -8));
	//App->camera->Look(d, pa);

	//const vec3 p = vehicle->GetPos();
	//const vec3 camera_offset(0, 6, -10);
	//const vec3 r = p + camera_offset;
	//App->camera->Look(r, p, false);
	//const vec3 f = vehicle->GetForwardVector();
	//vec3 d; d.Set(p.x + (f.x * -8), p.y + (f.y + 5), p.z + (f.z * -8));
	//App->camera->Look(d, p);
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
	// First rect
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


	//App->physics->CreateCurveBox(10, 1.f, 15.f, 7, vec3(0.5f, 1.f, 0.5f), 0.5f, 0, 0.5f);
}