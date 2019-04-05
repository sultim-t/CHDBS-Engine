#pragma once

#include <Engine/Base/Hash.h>
#include <Engine/DataStructures/StaticArray.h>
#include <Engine/Entities/Entity.h>
#include <functional>

#define TOSTRING( x ) #x

// Use this to declare component
#define CLASSDECLARATION( classname )                                                       \
public:                                                                                     \
    static const UINT Type;																	\
    virtual bool IsClassType( const UINT classType ) const override;                 \

// Use this to define child component
#define CLASSDEFINITION( parentclass, childclass )                                          \
const UINT childclass::Type = HASHSTRING( TOSTRING( childclass ) ); \
bool childclass::IsClassType( const UINT classType ) const {                         \
        if ( classType == childclass::Type )                                                \
            return true;                                                                    \
        return parentclass::IsClassType( classType );                                       \
}

class IComponent
{
	friend class EntityFactory;

private:
	// Unique ID
	UINT		id;
	// Is this component active
	bool		isActive;
	
protected:
	// Owner of this component
	Entity		*owner;

public:
	// ID of component type
	static const UINT Type;
	// Check types, each component must override this function
	// to check its own type
	virtual bool IsClassType(const UINT classType) const;

private:
	void SetOwner(Entity *owner);

protected:
	IComponent() {};

public:
	// Virtual destructor
	virtual ~IComponent() {};

	// Is called by entity factory after setting properties
	virtual void Init() = 0;
	// Is called by component system each frame
	virtual void Update() = 0;

	// Set properties 
	virtual void SetProperty(const String &key, const String &value) { }

	// Enable this component
	virtual void Enable();
	// Disable this component
	virtual void Disable();

	// Returns true if component and entity are active
	bool IsActive() const;

	// Get current entity, to which this component is attached
	const Entity &GetOwner() const;
	// Get current entity, to which this component is attached
	Entity &GetOwner();
};
