#ifdef _PF_LINUX_

#define EVL_NET_DECL   

#else // _PF_LINUX_

/* export if this is our own source, otherwise import: */
#ifdef EVL_NET_LIBRARY_SOURCE
# define EVL_NET_DECL __declspec(dllexport)
#else
# define EVL_NET_DECL __declspec(dllimport)
#endif  /* EVL_NET_LIBRARY_SOURCE */



#endif // _PF_LINUX_
