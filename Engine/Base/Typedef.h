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
typedef UINT		EntityID;
typedef UINT		MessageID;
typedef UINT		ComponentID;
typedef UINT		ModelID;
typedef UINT		MeshID;
typedef UINT		MaterialID;
typedef UINT		ShaderID;
typedef UINT		TextureID;

// Containers
template <class T, UINT> class StaticArray;
template <class T> class DynamicArray;
template <class T> class LinkedList;
template <class K, class T> class HashTable;
template <class T> class StackManual;
template <class T> class LinkedListManual;

// Math
template <class Type, int Dim> struct Matrix;
template <class Type, int Dim> struct Vector;
struct Quaternion;

typedef Matrix<float, 2> Matrix2;
typedef Matrix<float, 3> Matrix3;
typedef Matrix<float, 4> Matrix4;
typedef Matrix<double, 3> Matrix3Double;

typedef Vector<float, 2> Vector2;
typedef Vector<float, 3> Vector3;
typedef Vector<float, 4> Vector4;

typedef Vector<float, 3> Euler;
#define ROLL  2
#define PITCH 0
#define YAW   1

// Float color RGB
typedef Vector<float, 3> Color;
// Ubyte color RGBA
typedef Vector<UBYTE, 4> Color4;

// Shapes
enum class ShapeType;
class IShape;
class AABB;
class Sphere;

// Misc
class Time;
class ContextWindow;
class Logger;
class ResourceManager;
class String;
typedef char* CString;

// Systems
class ISystem;
class RenderingSystem;
class PhysicsSystem;
// class AudioSystem;
class UISystem;
class ComponentSystem;

// Entities
class Entity;
class Brush;

// Component interface
class IComponent;
// Component creator
typedef IComponent *(*IComponentCreator)(void* xmlElem);

// Components
class CCamera;
class CFreeMovement;
class CModel;
	class Mesh;
	class Material;
	class Shader;
	class Texture;
	class Cubemap;
	class FramebufferTexture;
class CLight;
class CParticleSystem;

class Rigidbody;
class ICollider;
	class AABBCollider;
	class SphereCollider;
	class CapsuleCollider;





