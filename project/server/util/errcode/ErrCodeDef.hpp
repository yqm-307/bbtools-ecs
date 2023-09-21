#pragma once
#include <cstring>

namespace util::errcode
{



enum ErrType
{
    Nothing = 1,
    NetWorkErr = 100,   // 网络err
};

namespace network { 
enum err
{
    Default                         = 0,
    //--------- network recv ---------//     

    //--------- network recv ---------//     
    Recv_Success                    = 2000, // 读取成功
    Recv_Eof                        = 2001, // 对端连接已关闭
    Recv_Connect_Refused            = 2002, // 连接被拒绝
    Recv_TryAgain                   = 2003, // 当前繁忙，重试
    Recv_Other_Err                  = 2004, // 其他错误，查看errno

};

} // 网络err



}// namespace end