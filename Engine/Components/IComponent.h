#pragma once

#include <Engine/Base/Hash.h>
#include <Engine/DataStructures/StaticArray.h>
#include <functional>

#define TOSTRING( x ) #x

// Use this to declare component
#define CLASSDECLARATION( classname )                                                       \
public:                                                                                     \
    static const ComponentID Type;                                                          \
    virtual bool IsClassType( const ComponentID classType ) const override;                 \

// Use this to define child component
#define CLASSDEFINITION( parentclass, childclass )                                          \
const ComponentID childclass::Type = HASHSTRING( TOSTRING( childclass ) ); \
bool childclass::IsClassType( const ComponentID classType ) const {                         \
        if ( classType == childclass::Type )                                                \
            return true;                                                                    \
        return parentclass::IsClassType( classType );                                       \
}                                                                                           \

#define CLASSDEFINITION( parentclass, childclass )                                          \
const ComponentID childclass::Type = HASHSTRING( TOSTRING( childclass ) );                  \
bool childclass::IsClassType( const ComponentID classType ) const {                         \
        if ( classType == childclass::Type )                                                \
            return true;                                                                    \
        return parentclass::IsClassType( classType );                                       \
}                                             

class IComponent
{
	friend class EntityFactory;

protected:
	Entity *owner;
	ComponentID id;
	bool isActive;

public:
	static const ComponentID Type;
	virtual bool IsClassType(const ComponentID classType) const
	{
		return classType == Type;
	}

// temp public
public:
	void SetOwner(Entity *owner)
	{
		this->owner = owner;
	}

	IComponent() {};

public:
	virtual void Init() = 0;
	virtual void Update() = 0;

	virtual void SetProperty(const String &key, const String &value) { }
	// const virtual String[] GetProperties() const { }

	virtual void Enable()
	{
		isActive = true;
	};

	virtual void Disable()
	{
		isActive = false;
	};

	bool IsActive() const
	{
		return isActive;
	}

	const Entity &GetOwner() const
	{
		return *owner;
	}
};
