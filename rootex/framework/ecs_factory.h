#pragma once

#include "common/common.h"
#include "core/resource_files/text_resource_file.h"

#include "entity.h"
#include "component.h"

/// Function pointer to a function that constructs a component, taking in a set of component data.
typedef Component* (*ComponentCreator)(const JSON::json& componentDescription);
/// Collection of a component, its name, and a function that constructs that component.
typedef Vector<Tuple<ComponentID, String, ComponentCreator>> ComponentDatabase;

#define REGISTER_COMPONENT(ComponentType) ECSFactory::RegisterComponent<ComponentType>(#ComponentType)

class ECSFactory
{
	static inline ComponentDatabase s_ComponentCreators;

public:
	static void RegisterAPI(sol::table& rootex);

	static void Initialize();

	static bool AddComponent(Entity* entity, Ptr<Component>& component);
	static Ptr<Component> CreateComponent(const String& componentName, const JSON::json& componentData);
	static Ptr<Component> CreateDefaultComponent(const String& componentName);

	static Ptr<Entity> CreateEntity(Scene* scene, const JSON::json& entityJSON);
	static Ptr<Entity> CreateEntityFromFile(Scene* scene, TextResourceFile* textResourceFile);
	static Ptr<Entity> CreateEmptyEntity(Scene* scene);
	static Ptr<Entity> CreateRootEntity(Scene* scene);
	static Ptr<Entity> CopyEntity(Scene* scene, Entity& entity);
	
	static const ComponentDatabase& GetComponentDatabase() { return s_ComponentCreators; }
	template <class T>
	static void RegisterComponent(const String& name);
};

template <class T>
inline void ECSFactory::RegisterComponent(const String& name)
{
	s_ComponentCreators.push_back({ T::s_ID, name, T::Create });
}