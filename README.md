# RingCache
循环缓存1.0
用于缓存多条字节流
##功能概述
目前仅支持缓冲本身256长度的缓存使用，字节流数量可通过RING_MSG_NUM来配置。
缓存使用之前必须要先调用RingMsg_init初始化。
溢出后覆盖最先进入缓存的字节流（新的覆盖旧的）。
