#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2List.h"
#include "Primitive.h"

#include "Bullet/include/btBulletDynamicsCommon.h"

// Recommended scale is 1.0f == 1 meter, no less than 0.2 objects
#define GRAVITY btVector3(0.0f, -10.0f, 0.0f) 

class DebugDrawer;
class  PhysBody3D;

class ModulePhysics3D : public Module
{
public:
	ModulePhysics3D(bool start_enabled = true);
	~ModulePhysics3D();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt) override;
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp();

	void AddBodyToWorld(btRigidBody* body);
	void RemoveBodyFromWorld(btRigidBody* body);

	PhysBody3D* RayCast(const vec3& Origin, const vec3& Direction, vec3& HitPoint = vec3());

	//TODO 1: Implement the code to add a Point to Point constraint ( btPoint2PointConstraint )
	//void AddConstraintP2P(const Primitive& bodyA, const Primitive& bodyB, ...);
	
	btPoint2PointConstraint* AddConstraintP2P(const Primitive& bodyA, const Primitive& bodyB, btVector3& pivotInA, btVector3& pivotInB);

	//TODO 3: Implement the code to add a Hinge constraint ( btHingeConstraint )

	btHingeConstraint* AddConstraintHinge(const Primitive & bodyA, const Primitive & bodyB, btVector3& pivotInA, btVector3& pivotInB, btVector3& axisInA, btVector3& axisInB);
	btSliderConstraint* AddConstraintSlider(const Primitive& bodyA, const Primitive& bodyB, btTransform& frameinA, btTransform& frameinB, bool linearreference = true);

	// Create map walls
	void CreateLineBox(float posx, float posy, float posz, int lenght, vec3& size, float interval, int direction, bool randh = false);
	void CreateDiagonalBox(float posx, float posy, float posz, int lenght, vec3& size, float interval, int direction, float diagonal, bool randh = false);
	void CreateCurveBox(float posx, float posy, float posz, int lenght, vec3& size, float interval, int direction, float rad);
	void CreateRamp(float posx, float posy, float posz, int lenght, vec3& size, float interval, int direction);

private:
	btDefaultCollisionConfiguration*	collision_conf;
	btCollisionDispatcher*				dispatcher;
	btBroadphaseInterface*				broad_phase;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld*			world;
	DebugDrawer*						debug_draw;
	p2List<btPoint2PointConstraint*>	P2PConstraints;
	p2List<btHingeConstraint*>			HingeConstraints;
	p2List<btSliderConstraint*>			SliderConstraints;

	// Change color of map walls
	int color = 0;
};

class DebugDrawer : public btIDebugDraw
{
public:
	DebugDrawer() : line()
	{}

	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
	void reportErrorWarning(const char* warningString);
	void draw3dText(const btVector3& location, const char* textString);
	void setDebugMode(int debugMode);
	int	 getDebugMode() const;

	DebugDrawModes mode;
	Line line;
	Primitive point;
	
};