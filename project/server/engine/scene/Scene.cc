#include "engine/scene/Scene.hpp"
#include "engine/ecs/gameobject/GameObjectMgr.hpp"
#include <algorithm>

namespace engine::scene
{

Scene::Scene()
{
}

Scene::~Scene()
{
}

// FIXME 尚未完成的逻辑
void Scene::Update()
{
    for(auto obj_ptr : m_root_gobjs)
    {
        obj_ptr->Update();
    }
    OnUpdate();
}

bool Scene::MountGameObject(engine::ecs::GameObjectSPtr gameobj)
{
    auto it = std::find_if(m_root_gobjs.begin(), m_root_gobjs.end(), 
        [gameobj](const engine::ecs::GameObjectSPtr value){
            return value->GetId() == gameobj->GetId();
    });
    if(it != m_root_gobjs.end())
        return false;

    m_root_gobjs.push_back(gameobj);
    return true;
}

Scene::Result Scene::UnMountGameObject(engine::ecs::GameObjectSPtr gameobj)
{
    auto obj_id = gameobj->GetId();
    return UnMountGameObject(obj_id);
}

Scene::Result Scene::UnMountGameObject(engine::ecs::GameObjectId id)
{
    auto it = std::find_if(m_root_gobjs.begin(), m_root_gobjs.end(),
        [id](const engine::ecs::GameObjectSPtr value){
            return value->GetId() == id;
        }
    );

    if(it == m_root_gobjs.end())
        return {nullptr, false};
    
    m_root_gobjs.erase(it);
    return {*it, true};
}


size_t Scene::GetChildNum()
{
    return m_root_gobjs.size();
}

}