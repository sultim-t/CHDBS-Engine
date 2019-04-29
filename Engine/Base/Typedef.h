#pragma once

// Base
//typedef bool				BOOL;

typedef char				BYTE;
typedef unsigned char		UBYTE;
typedef short				SHORT;
typedef unsigned short		USHORT;

//typedef int				INT;
typedef unsigned int		UINT;
typedef unsigned long 		LONG;
typedef unsigned long		ULONG;
typedef unsigned long long 	LLONG;
typedef unsigned long long	ULLONG;

// typedef float			FLOAT;
// typedef double			DOUBLE;

typedef char				INT8;
typedef unsigned char		UINT8;
typedef short				INT16;
typedef unsigned short		UINT16;
typedef int					INT32;
typedef unsigned int		UINT32;
typedef unsigned long long	INT64;
typedef unsigned long long	UINT64;

// IDs
typedef UINT				EntityID;
typedef UINT				ModelID;
typedef UINT				MeshID;
typedef UINT				MaterialID;
typedef UINT				ShaderID;
typedef UINT				TextureID;

// Containers
template <class T, UINT> class		Array;
template <class T> class			StaticArray;
template <class T> class			StaticStack;
template <class T> class			DynamicArray;
template <class T> class			LinkedList;
template <class K, class T> class	HashTable;

// Math
template <class Type, int Dim> struct Matrix;
template <class Type, int Dim> struct Vector;
struct Quaternion;

typedef Matrix<float, 2>	Matrix2;
typedef Matrix<float, 3>	Matrix3;
typedef Matrix<float, 4>	Matrix4;
typedef Matrix<double, 3>	Matrix3Double;

typedef Vector<float, 2>	Vector2;
typedef Vector<float, 3>	Vector3;
typedef Vector<float, 4>	Vector4;

typedef Vector<int, 2>		VectorI2;
typedef Vector<int, 3>		VectorI3;
typedef Vector<int, 4>		VectorI4;

typedef Vector<float, 3>	Euler;
#define ROLL  0		// around z axis
#define PITCH 1		// around x axis
#define YAW   2		// around y axis

// Float color RGB
typedef Vector<float, 3>	Color3F;
// Ubyte color RGBA
typedef Vector<float, 4>	Color4F;
// Ubyte color RGB
typedef Vector<UBYTE, 4>	Color3;
// Ubyte color RGBA
typedef Vector<UBYTE, 4>	Color4;

struct			Vertex;
struct			Vertex5;

// Shapes
enum class		ShapeType;
class			AABB;
class			Sphere;
class			Frustum;
class			Triangle;
class			Plane;
class			Ray;

// Misc
class			Time;
class			ContextWindow;
class			Logger;
class			String;
typedef char*	CString;
class			DebugDrawer;
class			StringTuple;

// Events
class								Event;
template <class D> class			EventParam;
class								IDelegate;
template <class D> class			IDelegateParam;
class								DelegateStatic;
template <class T> class			Delegate;
template <class T, class D> class	DelegateParam;

// Resources
class		ResourceManager;
class		MeshResource;
class		TextureResource;
class		ComponentResource;
class		EntityResource;
class		MeshColliderResource;
class		ModelResource;
class		ShaderResource;
class		VertexAnimatedResource;

// Systems
class		ISystem;
class		RenderingSystem;
class		PhysicsSystem;
class		UISystem;
class		ComponentSystem;
// class	AudioSystem;

// Entities
class		Entity;
class		Brush;

// Scene management
class		SceneManager;
class		Scene;

// Component interface
class		IComponent;
// Component creator
typedef		IComponent *(*IComponentCreator)(const ComponentResource &componentResource);

// Components
class CCamera;
	class ICamera;
class CFreeMovement;
class CModel;
	class CVertexAnimated;
	class ModelHierarchy;
	class Mesh;

	class Skeleton;
	class Bone;
	struct VertexWeight;

	class Animation;
	class AnimationNode;
	template <class T> struct			AnimationKey;
	typedef AnimationKey<Vector3>		AKeyPosition;
	typedef AnimationKey<Quaternion>	AKeyRotation;
	typedef AnimationKey<Vector3>		AKeyScale;

	class Material;
	class Shader;
	class Texture;
	class Cubemap;
	class FramebufferTexture;
class CLight;
class CParticleSystem;

class Rigidbody;
class ICollider;
	struct RaycastInfo;
	enum class CollisionType;
	struct CollisionInfo;
	struct BroadCollisionInfo;

	class AABBCollider;
	class SphereCollider;
	class MeshCollider;
	// class CapsuleCollider;

	// Components
	class CBoxCollider;
	class CSphereCollider;
	class CMeshCollider;



