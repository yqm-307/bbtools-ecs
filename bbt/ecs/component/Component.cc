#include "bbt/ecs/component/Component.hpp"

namespace bbt::ecs
{


Component::Component()
{
}

Component::~Component()
{
}

ComponentTemplateId Component::GetTemplateId()
{
    return Reflex_GetTypeId();
}

ComponentId Component::GetId() const
{
    return GetMemberId();
}

void Component::OnAddComponent(ecs::EntitySPtr parent)
{
    m_parent_gameobject = parent;
    OnEntity(parent);
}

void Component::OnDelComponent(ecs::EntitySPtr parent)
{
    auto parent_sptr = m_parent_gameobject.lock();
    OnUnEntity(parent_sptr);
    AssertWithInfo(parent_sptr == parent, "this a wrong! please check object life cycle!");
    m_parent_gameobject.reset(); // 释放parent所有权
}

EntitySPtr Component::GetParentObject() const
{
    return m_parent_gameobject.lock();
}

SceneSPtr Component::GetScene() const
{
    auto component_mgr = GetComponentMgr();
    if (component_mgr == nullptr)
        return nullptr;
    
    return component_mgr->GetScene();
}

ComponentMgrSPtr Component::GetComponentMgr() const
{
    return std::static_pointer_cast<ComponentMgr>(GetManager());
}


}



