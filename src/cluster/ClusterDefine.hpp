#pragma once
#include <string>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <cluster/ClusterDefine.hpp>
#include <util/other/Uuid.hpp>
#include <bbt/core/buffer/Buffer.hpp>
#include <bbt/core/clock/Clock.hpp>
#include <bbt/network/Define.hpp>


#define BBGENGINE_HEARTBEAT_TIMEOUT_MS 3000
#define BBGENGINE_CLUSTER_CONN_FREE_TIMEOUT 10000

namespace cluster
{

class NodeMgr;


enum NodeState
{
    NODESTATE_UNREGISTER = 0,
    NODESTATE_ONLINE,
    NODESTATE_OFFLINE,
    NODESTATE_INVALID,
};

}