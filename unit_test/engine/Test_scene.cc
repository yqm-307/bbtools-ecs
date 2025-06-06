#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <bbt/ecs/scene/Scene.hpp>
#include <bbt/ecs/system/System.hpp>
#include <bbt/ecs/component/Component.hpp>
#include <bbt/ecs/filter/EntityFilter.hpp>

class PosComp:
    public bbt::ecs::Component
{
public:
    PosComp() {};
    virtual ~PosComp() = default;
    BBT_REFLEX_DYN_TYPEINFO_METHOD(PosComp);

    int x{0};
    int y{0};
};

class CommandComp:
    public bbt::ecs::Component
{
public:
    CommandComp() {};
    virtual ~CommandComp() = default;
    BBT_REFLEX_DYN_TYPEINFO_METHOD(CommandComp);

    int cmd{0};
};

class VelocityComp:
    public bbt::ecs::Component
{
public:
    VelocityComp() {};
    virtual ~VelocityComp() = default;
    BBT_REFLEX_DYN_TYPEINFO_METHOD(VelocityComp);

    int x{1};
    int y{1};
};


class MoveSystem:
    public bbt::ecs::System
{
public:
    MoveSystem() = default;
    ~MoveSystem() = default;

    void OnInitFilter(std::shared_ptr<bbt::ecs::EntityFilter> filter) override
    {
        filter->AddTag<PosComp>().AddTag<VelocityComp>();
    }

    void OnUpdate() override
    {
        for (auto& go_wkptr : m_gameobjects)
        {
            if (go_wkptr.expired())
                continue;
            auto go_sptr = go_wkptr.lock();
            auto pos = go_sptr->GetComponent<PosComp>();
            auto vel = go_sptr->GetComponent<VelocityComp>();
            pos->x += vel->x;
            pos->y += vel->y;
        }
    }
};

#define DefineTestComponent(name) \
class name: \
    public bbt::ecs::Component \
{ \
public: \
    name() {}; \
    virtual ~name() = default; \
    BBT_REFLEX_DYN_TYPEINFO_METHOD(name); \
};


DefineTestComponent(TestComp1);
DefineTestComponent(TestComp2);
DefineTestComponent(TestComp3);
DefineTestComponent(TestComp4);
DefineTestComponent(TestComp5);

std::map<bbt::ecs::EntityId, int> g_gameobject_count_map;

class FilterTestSystem:
    public bbt::ecs::System
{
public:
    FilterTestSystem() = default;
    ~FilterTestSystem() = default;

    void OnInitFilter(std::shared_ptr<bbt::ecs::EntityFilter> filter) override
    {
        filter->AddTag<TestComp2>().AddTag<TestComp3>().AddTag<TestComp4>();
    }
    void OnUpdate() override
    {
        for (auto& go_wkptr : m_gameobjects)
        {
            if (go_wkptr.expired())
                continue;
            auto go_sptr = go_wkptr.lock();
            auto test2 = go_sptr->GetComponent<TestComp2>();
            auto test3 = go_sptr->GetComponent<TestComp3>();
            auto test4 = go_sptr->GetComponent<TestComp4>();

            BOOST_TEST_MESSAGE( "component_count=" << go_sptr->GetComponentCount() );
            BOOST_CHECK_EQUAL(g_gameobject_count_map[go_sptr->GetId()], go_sptr->GetComponentCount());
            BOOST_CHECK_NE(test2, nullptr);
            BOOST_CHECK_NE(test3, nullptr);
            BOOST_CHECK_NE(test4, nullptr);
        }
    }
};

BOOST_AUTO_TEST_SUITE(SceneTest)

// 单独场景 Update
BOOST_AUTO_TEST_CASE(t_scene_alone_update)
{
    auto scene = std::make_shared<bbt::ecs::Scene>();
    scene->Init();
    scene->RegistComponent<PosComp>("PosComp");
    scene->RegistComponent<VelocityComp>("VelocityComp");

    auto move_system = std::make_shared<MoveSystem>();
    scene->RegistSystem(move_system, bbt::ecs::EntityFilter());
    

    auto go = scene->GetEntityMgr()->AddEntity(1,
        {   std::make_shared<PosComp>(),
            std::make_shared<VelocityComp>()});

    BOOST_ASSERT(go != nullptr);

    scene->Update();
    
    auto pos = go->GetComponent<PosComp>();
    BOOST_CHECK_EQUAL(pos->x, 1);
    BOOST_CHECK_EQUAL(pos->y, 1);

}

BOOST_AUTO_TEST_CASE(t_entity_filter_test)
{
    auto scene = std::make_shared<bbt::ecs::Scene>();
    scene->Init();
    scene->RegistComponent<TestComp1>("TestComp1");
    scene->RegistComponent<TestComp2>("TestComp2");
    scene->RegistComponent<TestComp3>("TestComp3");
    scene->RegistComponent<TestComp4>("TestComp4");
    scene->RegistComponent<TestComp5>("TestComp5");

    auto filter_system = std::make_shared<FilterTestSystem>();
    scene->RegistSystem(filter_system, bbt::ecs::EntityFilter());
    
    auto go_1 = scene->GetEntityMgr()->AddEntity(1,
        {   std::make_shared<TestComp1>() });

    {
        BOOST_ASSERT(go_1 != nullptr);
        g_gameobject_count_map[go_1->GetId()] = go_1->GetComponentCount();
    }

    auto go_12 = scene->GetEntityMgr()->AddEntity(1,
        {   std::make_shared<TestComp1>(),
            std::make_shared<TestComp2>() });
    {
        BOOST_ASSERT(go_12 != nullptr);
        g_gameobject_count_map[go_12->GetId()] = go_12->GetComponentCount();
    }

    auto go_123 = scene->GetEntityMgr()->AddEntity(1,
        {   std::make_shared<TestComp1>(),
            std::make_shared<TestComp2>(),
            std::make_shared<TestComp3>() });
    {
        BOOST_ASSERT(go_123 != nullptr);
        g_gameobject_count_map[go_123->GetId()] = go_123->GetComponentCount();
    }

    auto go_1234 = scene->GetEntityMgr()->AddEntity(1,
        {   std::make_shared<TestComp1>(),
            std::make_shared<TestComp2>(),
            std::make_shared<TestComp3>(),
            std::make_shared<TestComp4>() });
    {
        BOOST_ASSERT(go_1234 != nullptr);
        g_gameobject_count_map[go_1234->GetId()] = go_1234->GetComponentCount();
    }

    auto go_12345 = scene->GetEntityMgr()->AddEntity(1,
        {   std::make_shared<TestComp1>(),
            std::make_shared<TestComp2>(),
            std::make_shared<TestComp3>(),
            std::make_shared<TestComp4>(),
            std::make_shared<TestComp5>() });
    {
        BOOST_ASSERT(go_12345 != nullptr);
        g_gameobject_count_map[go_12345->GetId()] = go_12345->GetComponentCount();
    }

    auto go_234 = scene->GetEntityMgr()->AddEntity(1,
        {   std::make_shared<TestComp2>(),
            std::make_shared<TestComp3>(),
            std::make_shared<TestComp4>() });
    {
        BOOST_ASSERT(go_234 != nullptr);
        g_gameobject_count_map[go_234->GetId()] = go_234->GetComponentCount();
    }

    scene->Update();

}

BOOST_AUTO_TEST_SUITE_END()