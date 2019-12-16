#include "Globals.h"
#include "Application.h"
#include "ModulePhysics3D.h"
#include "PhysBody3D.h"
#include "Primitive.h"
#include "Globals.h"

#include "glut/glut.h"

#ifdef _DEBUG
	#pragma comment (lib, "Bullet/libx86/BulletDynamics_debug.lib")
	#pragma comment (lib, "Bullet/libx86/BulletCollision_debug.lib")
	#pragma comment (lib, "Bullet/libx86/LinearMath_debug.lib")
#else
	#pragma comment (lib, "Bullet/libx86/BulletDynamics.lib")
	#pragma comment (lib, "Bullet/libx86/BulletCollision.lib")
	#pragma comment (lib, "Bullet/libx86/LinearMath.lib")
#endif

ModulePhysics3D::ModulePhysics3D(bool start_enabled) : Module(start_enabled), world(nullptr)
{
	collision_conf = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collision_conf);
	broad_phase = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	debug_draw = new DebugDrawer();
}

// Destructor
ModulePhysics3D::~ModulePhysics3D()
{
	delete debug_draw;
	delete solver;
	delete broad_phase;
	delete dispatcher;
	delete collision_conf;
}

// Render not available yet----------------------------------
bool ModulePhysics3D::Init()
{
	LOG("Creating 3D Physics simulation");
	bool ret = true;

	return ret;
}

// ---------------------------------------------------------
bool ModulePhysics3D::Start()
{
	LOG("Creating Physics environment");

	world = new btDiscreteDynamicsWorld(dispatcher, broad_phase, solver, collision_conf);
	world->setDebugDrawer(debug_draw);
	world->setGravity(GRAVITY);

	// Big rectangle as ground
	{
		btCollisionShape* colShape = new btBoxShape(btVector3(200.0f, 2.0f, 200.0f));

		mat4x4 glMatrix = IdentityMatrix;
		glMatrix.translate(0.f, -2.f, 0.f);
		btTransform startTransform;
		startTransform.setFromOpenGLMatrix(&glMatrix);

		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0f, myMotionState, colShape);

		btRigidBody* body = new btRigidBody(rbInfo);
		world->addRigidBody(body);
	}

	return true;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::PreUpdate(float dt)
{
	world->stepSimulation(dt, 15);

	for (int n = 0; n < world->getDispatcher()->getNumManifolds(); n++)
	{
		btPersistentManifold* manifold = world->getDispatcher()->getManifoldByIndexInternal(n);
		if (manifold->getNumContacts() > 0)
		{
			PhysBody3D* body1 = (PhysBody3D*)manifold->getBody0()->getUserPointer();
			PhysBody3D* body2 = (PhysBody3D*)manifold->getBody1()->getUserPointer();

			if (body1 != nullptr && body2 != nullptr)
			{
				for (uint n = 0; n < body1->collision_listeners.Count(); n++)
				{
					body1->collision_listeners[n]->OnCollision(body1, body2);
				}

				for (uint n = 0; n < body2->collision_listeners.Count(); n++)
				{
					body2->collision_listeners[n]->OnCollision(body2, body1);
				}
			}
		}
	}

	return UPDATE_CONTINUE;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::Update(float dt)
{
	if (App->debug == true)
	{
		glDisable(GL_LIGHTING);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		world->debugDrawWorld();
		glEnable(GL_LIGHTING);
	}

	return UPDATE_CONTINUE;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModulePhysics3D::CleanUp()
{
	LOG("Destroying 3D Physics simulation");

	// Remove from the world all collision bodies
	for(int i = world->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = world->getCollisionObjectArray()[i];
		world->removeCollisionObject(obj);
	}
	
	for (p2List_item<btPoint2PointConstraint*>*iterator = P2PConstraints.getFirst(); iterator; iterator = iterator->next) {
		world->removeConstraint(iterator->data);
		delete iterator->data;
	}

	for (p2List_item<btHingeConstraint*>*iterator = HingeConstraints.getFirst(); iterator; iterator = iterator->next) {
		world->removeConstraint(iterator->data);
		delete iterator->data;
	}
	
	delete world;

	return true;
}

PhysBody3D * ModulePhysics3D::RayCast(const vec3 & Origin, const vec3 & Direction, vec3& HitPoint)
{
	//TODO: NEW CODE
	//A snippet of new code that may be useful for you. Nothing to do here really
	vec3 Dir = normalize(Direction);

	btVector3 Start = btVector3(Origin.x, Origin.y, Origin.z);
	btVector3 End = btVector3(Origin.x + Dir.x * 1000.f, Origin.y + Dir.y * 1000.f, Origin.z + Dir.z * 1000.f);

	btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);

	// Perform raycast
	world->rayTest(Start, End, RayCallback);
	if (RayCallback.hasHit()) {

		HitPoint = vec3(RayCallback.m_hitPointWorld.x(), RayCallback.m_hitPointWorld.y(), RayCallback.m_hitPointWorld.z());
		return (PhysBody3D*)RayCallback.m_collisionObject->getUserPointer();
	}
	return nullptr;
}

void ModulePhysics3D::AddBodyToWorld(btRigidBody * body)
{
	world->addRigidBody(body);
}

void ModulePhysics3D::RemoveBodyFromWorld(btRigidBody * body)
{
	world->removeRigidBody(body);
}


btPoint2PointConstraint* ModulePhysics3D::AddConstraintP2P(const Primitive& bodyA, const Primitive& bodyB, btVector3& pivotInA, btVector3& pivotInB) {
	btPoint2PointConstraint* constraint = new btPoint2PointConstraint(*bodyA.body.GetBody(), *bodyB.body.GetBody(), pivotInA, pivotInB);
	world->addConstraint(constraint);
	P2PConstraints.add(constraint);
	return constraint;
}

btHingeConstraint* ModulePhysics3D::AddConstraintHinge(const Primitive& bodyA, const Primitive& bodyB, btVector3& pivotInA, btVector3& pivotInB, btVector3& axisInA, btVector3& axisInB) {
	btHingeConstraint* constraint = new btHingeConstraint(*bodyA.body.GetBody(), *bodyB.body.GetBody(), pivotInA, pivotInB, axisInA, axisInB);
	world->addConstraint(constraint);
	HingeConstraints.add(constraint);
	return constraint;
}

btSliderConstraint* ModulePhysics3D::AddConstraintSlider(const Primitive& bodyA, const Primitive& bodyB, btTransform& frameinA, btTransform& frameinB, bool linearreference) {
	btSliderConstraint* constraint = new btSliderConstraint(*bodyA.body.GetBody(), *bodyB.body.GetBody(),frameinA, frameinB, linearreference);
	constraint->setLowerLinLimit(0.f);
	constraint->setUpperLinLimit(0.4f);
	constraint->setLowerAngLimit(-0.2f);
	constraint->setUpperAngLimit(0.2f);
	
	world->addConstraint(constraint);
	SliderConstraints.add(constraint);

	return constraint;
}

void ModulePhysics3D::CreateLineBox(float posx, float posy, float posz, int lenght, vec3& size, float interval, int direction, bool randh)
{
	for (int n = 0; n < lenght; n++)
	{
		if (randh == true)
		{
			size.y = (float)(std::rand() % 30 + 10);
		}

		Cube* s = new Cube(size, 900000);

		s->color = { ((float)(std::rand() % 255) / 255.f), ((float)(std::rand() % 255) / 255.f), ((float)(std::rand() % 255) / 255.f) };

		App->scene_intro->ScenePrimitives.PushBack(s);
		s->SetPos(posx, posy, posz);

		// Right
		if (direction == 0)
		{
			posx += interval;
		}

		// Left
		else if (direction == 1)
		{
			posx -= interval;
		}

		// Forward
		else if (direction == 2)
		{
			posz -= interval;
		}

		// Backward
		else if (direction == 3)
		{
			posz += interval;
		}
	}
}

void ModulePhysics3D::CreateDiagonalBox(float posx, float posy, float posz, int lenght, vec3& size, float interval, int direction, float diagonal, bool randh)
{
	for (int n = 0; n < lenght; n++)
	{
		Cube* s = new Cube(size, 900000);

		s->color = { ((float)(std::rand() % 255) / 255.f), ((float)(std::rand() % 255) / 255.f), ((float)(std::rand() % 255) / 255.f) };
		App->scene_intro->ScenePrimitives.PushBack(s);
		s->SetPos(posx, posy, posz);

		// Diagonal top right
		if (direction == 0)
		{
			posx += interval;
			posz -= diagonal;
		}

		// Diagonal bottom right

		if (direction == 1)
		{
			posx += interval;
			posz += diagonal;
		}

		// Diagonal top left
		if (direction == 2)
		{
			posx -= interval;
			posz -= diagonal;
		}

		// Diagonal bottom left
		if (direction == 3)
		{
			posx -= interval;
			posz += diagonal;
		}
	}
}

void ModulePhysics3D::CreateCurveBox(float posx, float posy, float posz, int lenght, vec3& size, float interval, int direction, float rad)
{
	for (int n = 0; n < lenght; n++)
	{
		Cube* s = new Cube(size);
		App->scene_intro->ScenePrimitives.PushBack(s);
		s->SetPos(posx, posy, posz);

		if (direction == 0)
		{
			posx += interval;
			//posx = sqrt(pow(rad, 2) - pow(posx, 2));
			posz += sqrt(pow(posx, 2)-pow(rad, 2));
			posz = posz;
		}
	}
}

void ModulePhysics3D::CreateRamp(float posx, float posy, float posz, int lenght, vec3& size, float interval, int direction)
{
	for (int n = 0; n < lenght; n++)
	{
		Cube* s = new Cube(size, 900000);

		s->color = { ((float)(std::rand() % 255) / 255.f), ((float)(std::rand() % 255) / 255.f), ((float)(std::rand() % 255) / 255.f) };
		App->scene_intro->ScenePrimitives.PushBack(s);
		s->SetPos(posx, posy, posz);

		size.y += 0.3;

		// Right
		if (direction == 0)
		{
			posx += interval;
		}

		// Left
		else if (direction == 1)
		{
			posx -= interval;
		}

		// Forward
		else if (direction == 2)
		{
			posz -= interval;
		}

		// Backward
		else if (direction == 3)
		{
			posz += interval;
		}
	}
}

// =============================================
void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	line.origin.Set(from.getX(), from.getY(), from.getZ());
	line.destination.Set(to.getX(), to.getY(), to.getZ());
	line.color.Set(color.getX(), color.getY(), color.getZ());
	line.Render();
}

void DebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
	point.transform.translate(PointOnB.getX(), PointOnB.getY(), PointOnB.getZ());
	point.color.Set(color.getX(), color.getY(), color.getZ());
	point.Render();
}

void DebugDrawer::reportErrorWarning(const char* warningString)
{
	LOG("Bullet warning: %s", warningString);
}

void DebugDrawer::draw3dText(const btVector3& location, const char* textString)
{
	LOG("Bullet draw text: %s", textString);
}

void DebugDrawer::setDebugMode(int debugMode)
{
	mode = (DebugDrawModes) debugMode;
}

int	 DebugDrawer::getDebugMode() const
{
	return mode;
}