#pragma once

#include "Ocean/Core/Timestep.hpp"

// libs
#include <entt/entt.hpp>

namespace Ocean {

	class Entity;

	// The scene will hold the object list and communicate with the ECS.
	//
	class Scene {
	public:
		Scene();
		~Scene();

		// Creates an Entity with a TagComponent and a TransformComponent.
		// @param name: The name to tag the entity with through TagComponent.
		// @returns A new entity.
		//
		Entity CreateEntity(const std::string& name = std::string());

		void OnUpdate(Timestep ts);
		void OnViewportResize(uint32_t width, uint32_t height);

	private:
		// The object registry.
		//
		entt::registry m_Registry;

		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		friend class Entity;
	};

}	// Ocean