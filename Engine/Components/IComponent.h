#pragma once

#include <Engine/Base/Hash.h>
#include <Engine/DataStructures/StaticArray.h>
#include <Engine/Entities/Entity.h>
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

private:
	void SetOwner(Entity *owner)
	{
		this->owner = owner;
	}

protected:
	IComponent() {};

public:
	virtual ~IComponent() {};

	virtual void Init() = 0;
	virtual void Update() = 0;

	virtual void SetProperty(const String &key, const String &value) { }

	virtual void Enable();
	virtual void Disable();

	// Returns true if component and entity are active
	bool IsActive() const;
	const Entity &GetOwner() const;
};
