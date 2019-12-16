#ifndef __PhysBody3D_H__
#define __PhysBody3D_H__

#include "p2DynArray.h"
#include "glmath.h"

class btRigidBody;
class btCollisionShape;
struct btDefaultMotionState;
class Module;
class btRaycastVehicle;

class vec3;
class Sphere;
class Cube;
class Cylinder;
class Plane;

class Primitive;

class PhysBody3D
{
public:
	PhysBody3D();
	~PhysBody3D();

	void SetBody(Sphere* primitive, float mass);
	void SetBody(Cube* primitive, float mass);
	void SetBody(Cylinder* primitive, float mass);
	bool HasBody() const;
	btRigidBody* GetBody() const;

	void GetTransform(float* matrix) const;
	void SetTransform(const float* matrix) const;
	void SetPos(float x, float y, float z);
	const vec3 GetPos() const;
	vec3 GetForwardVector() const;

	void SetSpeed(vec3 speed);
	void Push(vec3 force);
	void Stop();

private:
	void SetBody(btCollisionShape* shape, Primitive* parent, float mass);

	btRigidBody* body;
	btCollisionShape* colShape;
	btDefaultMotionState* motionState;
public:
	Primitive* parentPrimitive;
	p2DynArray<Module*> collision_listeners;
	btRaycastVehicle* vehicle;
	
};

#endif // __PhysBody3D_H__