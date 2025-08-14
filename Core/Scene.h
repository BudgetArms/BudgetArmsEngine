#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Managers/SceneManager.h"
#include "Core/GameObject.h"


namespace bae
{
	class GameObject;
	class Scene final
	{
	public:
		~Scene();

		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;


		void Update();
		void FixedUpdate();
		void LateUpdate();
		void Render() const;
		void RenderGUI();

		void Add(std::shared_ptr<GameObject> object);
		void Remove(std::shared_ptr<GameObject> object);
		void RemoveAll();

		std::vector<std::shared_ptr<GameObject>> GetObjects() { return m_Objects; };


		bool m_bIsEnabled{ true };


	private:
		friend Scene& SceneManager::CreateScene(const std::string& name);

		explicit Scene(const std::string& name);


		std::string m_Name;
		std::vector<std::shared_ptr<GameObject>> m_Objects{};

		static unsigned int m_IdCounter;


	};
}


