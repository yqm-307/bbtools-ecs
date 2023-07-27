#pragma once
#include <map>
#include <vector>
#include "share/aoi/Define.hpp"
#include "util/config/config.hpp"
#include "util/hashmap/Hashmap.hpp"
#include "util/vector/Pos3.hpp"

namespace game::share::aoi
{
static const int AoiHashBucketNum = 16;

/**
 * @brief aoi中实体对象的身份标识
 */
enum AoiEntityFlag 
{
    Watcher     = 1,    // 观察者
    Marker      = 1 << 1,     // 被观察者
};


/**
 * @brief Aoi是基于笛卡尔坐标系. 九宫格实现的
 * 
 */
class Aoi:
    public bbt::templateutil::BaseType<Aoi>
{
    typedef util::hashmap::Hashmap<AoiObjectId, ecs::GameObject::SPtr, AoiHashBucketNum> GameObjMap;    /* 游戏对象hash桶 */
public:
    static RawPtr GetInstance();
private:
    Aoi();
    ~Aoi();

    void Init();
    /**
     * @brief 将 player 放进aoi中的 drop_point 位置
     * 
     * @param player 游戏对象
     * @param drop_point 降落点
     */
    void EnterAoi(ecs::GameObject::SPtr player, util::pos::Index3 drop_point);
    /* 进入地图 */
    void OnEnter(ecs::GameObject::SPtr player);
    void OnLeave(ecs::GameObject::SPtr player);
    void OnMove(ecs::GameObject::SPtr player);
    void OnUpdate(ecs::GameObject::SPtr player);
private:
    bool CheckConfig(const util::config::AoiConfig*);
private:
    /**
     * AOI 相关
     */
    Tower* GetTowerByIndex3(util::pos::Index3 index3);
    util::pos::Index3 GetIndex3ByIndex(int tower_index);
    /* 从aoi中根据id取gameobject对象 */
    ecs::GameObject::SPtr GetObjFromAoiById(AoiObjectId id);
private:
    size_t      m_length;
    int         m_tower_max_x;  // x 轴上灯塔数量
    int         m_tower_max_y;  // y 轴上灯塔数量
    int         m_tower_max_z;  // z 轴上灯塔数量
    GameObjMap  m_gameobj_map;          // AOI中所有游戏对象 hashmap
    std::vector<Tower>      m_towers;   // AOI中所有灯塔
    std::vector<MapSlot>    m_slots;    // AOI中所有Slot
    const util::config::AoiConfig*      m_config;       // AOI配置
};

}