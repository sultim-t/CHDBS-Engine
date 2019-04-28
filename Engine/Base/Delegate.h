#pragma once

class IDelegate
{
public:
	virtual void Invoke() const = 0;
};

class DelegateStatic : public IDelegate
{
public:
	typedef void(*StaticFunction)();

private:
	StaticFunction function;

public:
	DelegateStatic(StaticFunction staticFunction)
		: function(staticFunction) {}

	void Invoke() const override
	{
		function();
	}
};


template <class T>
class Delegate : public IDelegate
{
public:
	typedef void(T::*TFunction)();

private:
	TFunction	function;
	T			*object;

public:
	Delegate(T *object, TFunction objectFunction) 
		: object(object), function(objectFunction) { }

	void Invoke() const override
	{
		(object->*function)();
	}
};
