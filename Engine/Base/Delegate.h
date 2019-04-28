#pragma once

class IDelegate
{
public:
	virtual void Invoke() const = 0;
};

template <class D>
class IDelegateParam
{
public:
	virtual void Invoke(const D *data) const = 0;
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

template <class T, class D>
class DelegateParam : public IDelegateParam<D>
{
public:
	typedef void(T::*TFunction)(const D *data);

private:
	TFunction	function;
	T			*object;

public:
	DelegateParam(T *object, TFunction objectFunction)
		: object(object), function(objectFunction) { }

	void Invoke(const D *data) const override
	{
		(object->*function)(data);
	}
};