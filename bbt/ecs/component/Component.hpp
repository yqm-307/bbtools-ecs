#pragma once
#include <string>
#include <bbt/ecs/Define.hpp>
#include "./ComponentMgr.hpp"

namespace bbt::ecs
{

class Component:
    public bbt::core::util::MemberBase<ComponentId, Component>,
    public bbt::core::reflex::ReflexDynTypeInfo<Component>
{
    friend class Entity;
    friend class ComponentMgr;
public:
    explicit Component();

    // interface
    //--------------------------------------------
    virtual                     ~Component() = 0;
    virtual bbt::core::reflex::TypeId Reflex_GetTypeId() = 0;
    virtual const char*         Reflex_GetTypeName() = 0;

    /**
     * @brief 当被添加到实体时调用
     */
    virtual void                OnEntity(ecs::EntitySPtr parent) {}

    /**
     * @brief 当被从实体中移除时调用
     */
    virtual void                OnUnEntity(ecs::EntitySPtr parent) {}
    //--------------------------------------------

    ComponentTemplateId         GetTemplateId();
    ComponentId                 GetId() const;
    EntitySPtr                  GetParentObject() const;
    SceneSPtr                   GetScene() const;
    ComponentMgrSPtr            GetComponentMgr() const;
private:
    virtual void                OnAddComponent(ecs::EntitySPtr);
    virtual void                OnDelComponent(ecs::EntitySPtr);
private:
    /**
     * 父对象的弱引用
     */
    EntityWKPtr         m_parent_gameobject;
};

}