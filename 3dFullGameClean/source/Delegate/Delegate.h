#pragma once
#include <tuple>
#include "AActor.h"

#define DECLARE_DELEGATE(DelegateName, ...)\
	using DelegateName = Delegate<__VA_ARGS__>

template <typename... Args>
class Delegate
{
private:
	std::vector<AActor*> Observers;
	std::vector<void(AActor::*)(Args...)> ObserversFunctions;
	std::vector<std::tuple<Args...>> ObserversFunctionsArgs;

	static void RunObsFuncWithArgs(AActor* Observer, void(AActor::* Func)(Args...), Args... args);

public:
	void Broadcast();

	template <typename Type>
	void AddUObject(AActor* Observer, void(Type::* Func)(Args...), Args... args);
};

template <typename... Args>
void Delegate<Args...>::RunObsFuncWithArgs(AActor* Observer, void(AActor::* Func)(Args...), Args... args)
{
	(Observer->*Func)(args...);
}

template <typename... Args>
void Delegate<Args...>::Broadcast()
{
	for (size_t i = 0; i < Observers.size(); ++i)
	{
		if constexpr (sizeof...(Args) == 0)
			(Observers[i]->*ObserversFunctions[i])();
		else
		{
			std::tuple<AActor*, void(AActor::*)(Args...)> ObsAndObsFuncs(Observers[i], ObserversFunctions[i]);

			std::apply(Delegate::RunObsFuncWithArgs, std::tuple_cat(ObsAndObsFuncs, ObserversFunctionsArgs[i]));
		}
	}
}

template <typename... Args>
template <typename Type>
void Delegate<Args...>::AddUObject(AActor* Observer, void(Type::* Func)(Args...), Args... args)
{
	Observers.push_back(Observer);
	ObserversFunctions.push_back(reinterpret_cast<void(AActor::*)(Args...)>(Func));

	if constexpr (sizeof...(Args) != 0)
		ObserversFunctionsArgs.push_back(std::tuple<Args...>(args...));
}