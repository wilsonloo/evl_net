#ifndef EVL_NET_EVL_NET_HPP_
#define EVL_NET_EVL_NET_HPP_

#include "config.h"

//////////////////////////////////////////////////////////////////////////
// library name
#define RTQ_LIBRARY_NAME "evl_net"

//////////////////////////////////////////////////////////////////////////
// static or dynamic
#ifdef EVL_NET_BUILD_DLL
	#define EVL_LIB_RTL_OPT "dynamic-rtl"
#else
	#define EVL_LIB_RTL_OPT "static-rtl"
#endif

//////////////////////////////////////////////////////////////////////////
// platform toolset
#if _MSC_VER == 1700
#define PLARFORM_TOOLSET "v110"
#elif _MSC_VER == 1800
#define PLARFORM_TOOLSET "v120"
#else
#error unsuported playerform toolset
#endif

//////////////////////////////////////////////////////////////////////////
// set EVL_LIB_DEBUG_OPT
#ifdef _DEBUG
#  define EVL_LIB_DEBUG_OPT "debug"
#else
#  define EVL_LIB_DEBUG_OPT "release"
#endif

#ifdef _PF_WINDOWS_

#define RTQ_LIBRARY_NAME_FULLNAME RTQ_LIBRARY_NAME "_" EVL_LIB_RTL_OPT "_" PLARFORM_TOOLSET "_" EVL_LIB_DEBUG_OPT

#pragma comment(lib, RTQ_LIBRARY_NAME_FULLNAME".lib")

#else
#error not implemented yet for this unknown platform.
#endif

#undef RTQ_LIBRARY_NAME
#undef EVL_LIB_RTL_OPT
#undef EVL_LIB_DEBUG_OPT
#undef RTQ_LIBRARY_NAME_FULLNAME

#include "evl_net_def.h"
#include "tcp_client.h"
#include "tcp_server.h"
#include "tcp_session.h"
#include "detail/net_mgr.h"

#endif // EVL_NET_EVL_NET_HPP_