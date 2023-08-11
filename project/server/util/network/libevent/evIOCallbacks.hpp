#pragma once
#include "util/network/IOCallbacks.hpp"
#include "util/network/libevent/evConnection.hpp"

namespace game::util::network
{

struct evIOCallbacks
{
    ev::evConnectionSPtr m_conn_ptr;
    errcode::ErrCode err;
};



template<>
inline void OnAccept<ev::evConnection>(ConnectionSPtr new_conn, const errcode::ErrCode& err)
{
    printf("OnAccept 对于 evConnection 的专用化!\n");
    auto ev_conn = std::static_pointer_cast<ev::evConnection>(new_conn);
}

template<>
inline void OnConnect<ev::evConnection>(ConnectionSPtr new_conn, const errcode::ErrCode& err)
{
    printf("OnConnect 对于 evConnection 的专用化!\n");
}

/* 因为本身 OnRecv 函数带状态，所以只能用全局的void(void*)来包裹 */

void OnConnectCallback(int sockfd, short events, void* args);

void OnAcceptCallback(int sockfd, short events, void* args);

void OnRecvCallback(int sockfd, short events, void* args);

}