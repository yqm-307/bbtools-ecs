#include <typeinfo>
#include <bbt/ecs/component/Component.hpp>
#include <bbt/ecs/entity/Entity.hpp>
namespace bbt::ecs
{



Entity::Entity(GameObjectTemplateId gobj_type)
    :m_gobj_type(gobj_type)
{
    AssertWithInfo( gobj_type >= 0, "game object type error");
}

ComponentSPtr Entity::GetComponent(ComponentTemplateId tid) const
{
    if(tid < 0)
        return nullptr;
    auto it = m_component_map.find(tid);
    if(it == m_component_map.end())
        return nullptr;
        
    return it->second;
}

size_t Entity::GetComponentCount() const
{
    return m_component_map.size();
}

ComponentSPtr Entity::DelComponent(ComponentTemplateId tid)
{
    if(tid < 0)
        return nullptr;
    auto it = m_component_map.find(tid);
    if(it == m_component_map.end())
        return nullptr;
    
    m_component_map.erase(it);
    it->second->OnDelComponent(shared_from_this());
    if (it->second == nullptr) {
        return nullptr;
    }

    m_tag_set.DelTag(it->second->Reflex_GetTypeId());
    return it->second;
}


bool Entity::AddComponent(ComponentSPtr component)
{
    if(component == nullptr)
        return false;

    ComponentTemplateId tid = component->GetTemplateId();
    auto it = m_component_map.insert(std::make_pair(tid, component));
    if (!it.second) {
        return false;
    }

    component->OnAddComponent(shared_from_this());

    // 处理tag
    return m_tag_set.AddTag(Tag{component->Reflex_GetTypeId()});
}

int Entity::Type()
{
    return m_gobj_type;
}

EntityId Entity::GetId()
{
    return GetMemberId();
}

std::string Entity::GetName() const
{
    return GetEntityMgr()->GetName(m_gobj_type);
}

TagSet& Entity::GetTagSet()
{
    return m_tag_set;
}

SceneSPtr Entity::GetScene() const
{
    return GetEntityMgr()->GetScene();
}

EntityMgrSPtr Entity::GetEntityMgr() const
{
    return std::static_pointer_cast<EntityMgr>(GetManager());
}

} // namespace bbt::ecs