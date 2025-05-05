#pragma once
#include <map>
#include <set>
#include <string>
#include <bbt/ecs/scene/Scene.hpp>
#include <bbt/ecs/filter/TagSet.hpp>

namespace bbt::ecs
{
class Entity final:
    public std::enable_shared_from_this<Entity>,
    public bbt::core::util::MemberBase<EntityId, Entity>
{
    friend class EntityMgr;
    friend class bbt::ecs::Scene;
public:
    explicit        Entity(GameObjectTemplateId gobj_type);
    virtual         ~Entity() = default;


    /**
     * @brief 向实体添加一个组件
     * 
     * @tparam TComponent 组件类型
     * @tparam Args 
     * @param args 构建组件的参数
     * @return bool 成功返回true，失败返回false
     */
    template<class TComponent, typename ...Args>
    bool            AddComponent(Args ...args);


    /**
     * @brief 插入一个组件，如果已经存在则返回false
     * 
     * @param component 
     * @return true 
     * @return false 
     */
    bool            AddComponent(ComponentSPtr component);

    /**
     * @brief 根据组件id获取实体中的一个组件
     * 
     * @param component_name 
     * @return ComponentSPtr 失败返回nullptr
     */
    ComponentSPtr   GetComponent(ComponentTemplateId comp_id) const;

    /**
     * @brief 根据类型获取实体中的一个组件
     * 
     * @tparam TComponent 
     * @return std::shared_ptr<TComponent> 失败返回nullptr 
     */
    template<class TComponent>
    std::shared_ptr<TComponent> GetComponent() const;

    /**
     * @brief 获取实体中的组件数量
     * 
     * @return size_t 
     */
    size_t          GetComponentCount() const;

    /**
     * @brief 删除一个组件
     * 
     * @param component_name 
     * @return ComponentSPtr 返回被删除的组件 
     */
    ComponentSPtr   DelComponent(ComponentTemplateId component_name);
    
    /**
     * @brief 获取实体模板的id
     * 
     * @return GameObjectTemplateId 
     */
    GameObjectTemplateId Type();

    /**
     * @brief 获取实体的id，这个id在进程中是唯一的
     * 
     * @return EntityId 
     */
    EntityId        GetId();

    std::string     GetName() const;
    TagSet&         GetTagSet();

    /**
     * @brief 获取所属的scene
     * 
     * @return SceneSPtr 
     */
    SceneSPtr       GetScene() const;

    /**
     * @brief 获取所属的实体管理器
     * 
     * @return EntityMgrSPtr 
     */
    EntityMgrSPtr   GetEntityMgr() const;

private:
    const GameObjectTemplateId                      m_gobj_type{-1};
    std::unordered_map<ComponentTemplateId, ComponentSPtr>
                                                    m_component_map;
    TagSet                                          m_tag_set;
};


template<class TComponent, typename ...Args>
bool Entity::AddComponent(Args ...args)
{
    auto component_mgr = GetScene()->GetComponentMgr();
    if (!component_mgr)
        return false;

    auto comp = component_mgr->Create<TComponent>(args...);
    return AddComponent(comp);
}

template<class TComponent>
std::shared_ptr<TComponent> Entity::GetComponent() const
{
    return std::dynamic_pointer_cast<TComponent>(GetComponent(BBT_REFLEX_GET_TYPEID(TComponent)));
}

} // namespace bbt::ecs
