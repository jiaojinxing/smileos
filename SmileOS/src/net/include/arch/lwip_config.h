/*********************************************************************************************************
**
** Copyright (c) 2011 - 2012  Jiao JinXing <jiaojinxing1987@gmail.com>
**
** Licensed under the Academic Free License version 2.1
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
**
**--------------------------------------------------------------------------------------------------------
** File name:               lwip_config.h
** Last modified Date:      2012-2-22
** Last Version:            1.0.0
** Descriptions:            lwIP 配置头文件
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-2-22
** Version:                 1.0.0
** Descriptions:            创建文件
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
*********************************************************************************************************/
#ifndef LWIP_CONFIG_H_
#define LWIP_CONFIG_H_

#include <limits.h>

#define __KB                            (1024)
#define __MB                            (1024 * __KB)

/*********************************************************************************************************
*                                           lwIP 设置
*
*  注  意  : 这里仅配置最为基本的 lwIP 选项, 详细配置需要在 lwIP 相关配置文件中完成.
*            如果使用特殊网络, 请在这里加入 PBUF_POOL_BUFSIZE 的配置, 例如: wifi 网络由于底层协议需要较大
*            的头部, 所以需要 PBUF_POOL_BUFSIZE 加大. (使用连续的 POOL 内存可以提高网络驱动效率, 可直接
*            使用 DMA 操作数据包)
*
*            LWIP_CFG_MSG_SIZE
*            LWIP_CFG_NUM_NETBUF
*            LWIP_CFG_NUM_POOLS
*            LWIP_CFG_MEM_SIZE          等配置都与网络协议栈吞吐性能息息相关.
*
*********************************************************************************************************/

/*********************************************************************************************************
**                          lwIP 缓存配置 (以下配置决定 RAM 使用量和 lwIP 效率)
*********************************************************************************************************/

#define LWIP_CFG_MEM_SIZE               (512 * __KB)                    /*  lwIP 内存大小               */
#define LWIP_CFG_MSG_SIZE               512                             /*  lwIP 内部消息队列缓冲长度   */
#define LWIP_CFG_POOL_SIZE              1600                            /*  lwIP POOL 内存块大小        */
                                                                        /*  注意: 必须是字对齐的        */

#define LWIP_CFG_NUM_PBUFS              256                             /*  系统总 pbuf 数量            */
#define LWIP_CFG_NUM_NETBUF             256                             /*  缓冲网络分组 netbuf 数量    */
#define LWIP_CFG_NUM_POOLS              512                             /*  等长缓冲总数量(高速缓冲)    */
                                                                        /*  驱动程序使用                */

/*********************************************************************************************************
**                          lwIP 校验和配置 (建议使用软件生成校验和并检查)
*********************************************************************************************************/

#define LWIP_CFG_GEN_CHECKSUM           1                               /*  是否通过协议栈自己生成校验和*/
                                                                        /*  如果网卡可自动计算发送校验和*/
                                                                        /*  此配置可为 0                */
#define LWIP_CFG_CHECK_CHECKSUM         1                               /*  是否软件检查接收数据包校验和*/
                                                                        /*  如果网卡可自动检查接收校验和*/
                                                                        /*  此配置可为 0                */

/*********************************************************************************************************
**                                          lwIP 组件配置
*********************************************************************************************************/

#define LWIP_CFG_DEBUG                  1                               /*  lwIP 运行在调试模式         */
#define LWIP_CFG_DEBUG_LEVEL            1                               /*  lwIP 调试等级               */

#define LWIP_CFG_PPP                    0                               /*  lwIP ppp 支持               */
#define LWIP_CFG_PPPOE                  0                               /*  lwIP pppoe 支持             */
#define LWIP_CFG_NUM_PPP                2                               /*  lwIP ppp 最大会话数         */

#define LWIP_CFG_TCP_PCB                60                              /*  允许同时的 TCP 连接数       */
#define LWIP_CFG_UDP_PCB                50                              /*  允许同时的 UDP 数量         */
#define LWIP_CFG_RAW_PCB                15                              /*  允许同时的 RAW 数量         */

#define LWIP_CFG_DHCP                   0                               /*  是否允许 DHCP 协议          */
#define LWIP_CFG_AUTOIP                 0                               /*  自动分析获取 IP (一般不需要)*/

/*********************************************************************************************************
**                                          lwIP SNMP 设置
*********************************************************************************************************/

#define LWIP_CFG_SNMP                   0                               /*  SNMP 代理                   */

/*********************************************************************************************************
**                                          lwIP IGMP 设置
*********************************************************************************************************/

#define LWIP_CFG_IGMP                   0                               /*  是否使能 IGMP               */
#define LWIP_CFG_IGMP_GROUP             20                              /*  可加入的 IGMP 组最大数量    */

/*********************************************************************************************************
**                                          lwIP 以太网设置
*********************************************************************************************************/

#define LWIP_CFG_ARP_TABLE_SIZE         30                              /*  以太网接口 ARP 表大小       */
#define LWIP_CFG_ARP_TRUST_IP_MAC       0                               /*  接收 IP 包是否更新 ARP      */
                                                                        /*  windows 不信任, 建议为 0    */
#define LWIP_CFG_ETH_PAD_SIZE           2                               /*  以太包头填充对齐数量        */

/*********************************************************************************************************
** Platform specific locking
*********************************************************************************************************/

#define SYS_LIGHTWEIGHT_PROT            1

/*********************************************************************************************************
** Memory options
*********************************************************************************************************/

#define MEM_SIZE                        LWIP_CFG_MEM_SIZE               /*  malloc 堆大小               */
#define MEMP_NUM_PBUF                   LWIP_CFG_NUM_PBUFS              /*  npbufs                      */
#define PBUF_POOL_SIZE                  LWIP_CFG_NUM_POOLS              /*  pool num                    */
#define PBUF_POOL_BUFSIZE               LWIP_CFG_POOL_SIZE              /*  pool block size             */

#if MEM_SIZE >= (32 * __MB)
#define MEMP_NUM_REASSDATA              150                             /*  同时进行重组的 IP 数据包    */
#elif MEM_SIZE >= (1 * __MB)
#define MEMP_NUM_REASSDATA              100
#elif MEM_SIZE >= (512 * __KB)
#define MEMP_NUM_REASSDATA              80
#elif MEM_SIZE >= (256 * __KB)
#define MEMP_NUM_REASSDATA              40
#elif MEM_SIZE >= (128 * __KB)
#define MEMP_NUM_REASSDATA              20
#else
#define MEMP_NUM_REASSDATA              5
#endif                                                                  /*  MEM_SIZE >= ...             */

/*********************************************************************************************************
** ...PCB
*********************************************************************************************************/

#define MEMP_NUM_RAW_PCB                LWIP_CFG_RAW_PCB
#define MEMP_NUM_UDP_PCB                LWIP_CFG_UDP_PCB
#define MEMP_NUM_TCP_PCB                LWIP_CFG_TCP_PCB
#define MEMP_NUM_TCP_PCB_LISTEN         LWIP_CFG_TCP_PCB

#define MEMP_NUM_NETCONN                (LWIP_CFG_RAW_PCB + LWIP_CFG_UDP_PCB + LWIP_CFG_TCP_PCB)

#define MEMP_NUM_TCPIP_MSG_API          (LWIP_CFG_TCP_PCB + LWIP_CFG_UDP_PCB + LWIP_CFG_RAW_PCB)
#define MEMP_NUM_TCPIP_MSG_INPKT        LWIP_CFG_MSG_SIZE               /*  tcp input msgqueue use      */

/*********************************************************************************************************
** check sum
*********************************************************************************************************/

#if LWIP_CFG_GEN_CHECKSUM == 0
#define CHECKSUM_GEN_IP                 0
#define CHECKSUM_GEN_UDP                0
#define CHECKSUM_GEN_TCP                0
#endif                                                                  /*  LWIP_CFG_GEN_CHECKSUM       */

#if LWIP_CFG_CHECK_CHECKSUM == 0
#define CHECKSUM_CHECK_IP               0
#define CHECKSUM_CHECK_UDP              0
#define CHECKSUM_CHECK_TCP              0
#endif                                                                  /*  LWIP_CFG_CHECK_CHECKSUM     */

#define LWIP_CHECKSUM_ON_COPY           1                               /*  拷贝数据包同时计算 chksum   */

/*********************************************************************************************************
** SmileOS do not use MEMP_NUM_NETCONN, because SmileOS use another socket interface.
*********************************************************************************************************/

#define IP_FORWARD                      1                               /*  允许 IP 转发                */
#define IP_REASSEMBLY                   1
#define IP_FRAG                         1
#define IP_REASS_MAX_PBUFS              (MEMP_NUM_PBUF / 2)

#define IP_SOF_BROADCAST                1                               /*  Use the SOF_BROADCAST       */
#define IP_SOF_BROADCAST_RECV           1

#define IP_FORWARD_ALLOW_TX_ON_RX_NETIF 1

/*********************************************************************************************************
** IPv6
*********************************************************************************************************/

#define LWIP_IPV6                       1
#define LWIP_IPV6_MLD                   1
#define LWIP_IPV6_FORWARD               1
#define LWIP_ICMP6                      1
#define LWIP_IPV6_FRAG                  1
#define LWIP_IPV6_REASS                 1

#define MEMP_NUM_MLD6_GROUP             16
#define LWIP_ND6_NUM_NEIGHBORS          LWIP_CFG_ARP_TABLE_SIZE
#define LWIP_ND6_NUM_DESTINATIONS       LWIP_CFG_ARP_TABLE_SIZE
#define LWIP_IPV6_DHCP6                 LWIP_CFG_DHCP

/*********************************************************************************************************
** dhcp & autoip
*********************************************************************************************************/

#define LWIP_DHCP                       LWIP_CFG_DHCP                   /*  DHCP                        */
#define LWIP_DHCP_BOOTP_FILE            0                               /*  not include bootp file now  */
#define LWIP_AUTOIP                     LWIP_CFG_AUTOIP

#if (LWIP_DHCP > 0) && (LWIP_AUTOIP > 0)
#define LWIP_DHCP_AUTOIP_COOP           1
#endif                                                                  /*  (LWIP_DHCP > 0)             */
                                                                        /*  (LWIP_AUTOIP > 0)           */
#define MEMP_NUM_SYS_TIMEOUT            (LWIP_CFG_RAW_PCB + LWIP_CFG_UDP_PCB + \
                                         LWIP_CFG_TCP_PCB + 20)

#define MEMP_NUM_NETBUF                 LWIP_CFG_NUM_NETBUF

/*********************************************************************************************************
** ICMP
*********************************************************************************************************/

#define LWIP_ICMP                       1

/*********************************************************************************************************
** RAW
*********************************************************************************************************/

#define LWIP_RAW                        1

/*********************************************************************************************************
** SNMP
*********************************************************************************************************/

#define LWIP_SNMP                       LWIP_CFG_SNMP
#define SNMP_PRIVATE_MIB                1                               /*  support now!                */

#define SNMP_GET_SYSUPTIME(sysuptime)   {                                       \
                                            __netSnmpGetTimestamp(&sysuptime);  \
                                        }
#define SNMP_PRIVATE_MIB_INIT()         __netSnmpPriMibInit()

/*********************************************************************************************************
** IGMP
*********************************************************************************************************/

#define LWIP_IGMP                       LWIP_CFG_IGMP
#define MEMP_NUM_IGMP_GROUP             LWIP_CFG_IGMP_GROUP

/*********************************************************************************************************
** DNS
*********************************************************************************************************/

#define LWIP_DNS                        0

#ifndef MEMP_NUM_NETDB
#define MEMP_NUM_NETDB                  10
#endif                                                                  /*  MEMP_NUM_NETDB              */

#define DNS_MAX_NAME_LENGTH             PATH_MAX

#define DNS_LOCAL_HOSTLIST              1
#define DNS_LOCAL_HOSTLIST_INIT         {{"none-host", 0UL}}

                                                                        /*  范围 IPv4 网络字节序地址    */
#define DNS_LOCAL_HOSTLIST_IS_DYNAMIC   1
#define DNS_LOOKUP_LOCAL_EXTERN(x)      __inetHostTableGetItem(x)

/*********************************************************************************************************
** transmit layer (注意: 如果 TCP_WND 大于网卡接收缓冲, 可能造成批量传输时, 网卡芯片缓冲溢出.
**                       如果出现这种情况, 请在这里自行配置 TCP_WND 大小. 并确保大于 2 * TCP_MSS)
*********************************************************************************************************/

#define LWIP_UDP                        1
#define LWIP_UDPLITE                    1
#define LWIP_NETBUF_RECVINFO            1

#define LWIP_TCP                        1
#define TCP_LISTEN_BACKLOG              1
#define LWIP_TCP_TIMESTAMPS             1

#define TCP_QUEUE_OOSEQ                 1

#ifndef TCP_MSS
#define TCP_MSS                         1460                            /*  usually                     */
#endif                                                                  /*  TCP_MSS                     */

#define TCP_CALCULATE_EFF_SEND_MSS      1                               /*  use effective send MSS      */

#if MEM_SIZE >= (8 * __MB)
#define TCP_WND                         ((64 * __KB) - 1)               /*  MAX WINDOW                  */
#elif MEM_SIZE >= (4 * __MB)
#define TCP_WND                         (32  * __KB)
#elif MEM_SIZE >= (1 * __MB)
#define TCP_WND                         (16  * __KB)
#elif MEM_SIZE >= (512 * __KB)
#define TCP_WND                         ( 8  * __KB)
/*********************************************************************************************************
** MEM_SIZE < 128 KB  SMALL TCP_MSS XXX
*********************************************************************************************************/
#elif MEM_SIZE >= (64 * __KB)
#define TCP_WND                         ( 4  * TCP_MSS)
#else
#undef  TCP_MSS
#define TCP_MSS                         536                             /*  small mss                   */
#define TCP_WND                         ( 4  * TCP_MSS)
#endif                                                                  /*  MEM_SIZE >= ...             */

#if TCP_WND < (4  * TCP_MSS)
#undef  TCP_WND
#define TCP_WND                         ( 4  * TCP_MSS)                 /*  must bigger than 4 * TCP_MSS*/
#endif                                                                  /*  TCP_WND < (4  * TCP_MSS)    */

#define TCP_SND_BUF                     ((64 * __KB) - 1)               /*  tcp snd buf size (max size) */
#define MEMP_NUM_TCP_SEG                (8 * TCP_SND_QUEUELEN)

#define LWIP_TCP_KEEPALIVE              1
#define LWIP_SO_SNDTIMEO                1
#define LWIP_SO_RCVTIMEO                1
#define LWIP_SO_RCVBUF                  1

#define SO_REUSE                        1                               /*  enable SO_REUSEADDR         */
#define SO_REUSE_RXTOALL                1

#define LWIP_FIONREAD_LINUXMODE         1                               /*  linux FIONREAD compatibility*/

/*********************************************************************************************************
** Network Interfaces options
*********************************************************************************************************/

#define LWIP_NETIF_HOSTNAME             1                               /*  netif have nostname member  */
#define LWIP_NETIF_API                  1                               /*  support netif api           */
#define LWIP_NETIF_STATUS_CALLBACK      1                               /*  interface status change     */
#define LWIP_NETIF_REMOVE_CALLBACK      1                               /*  interface remove hook       */
#define LWIP_NETIF_LINK_CALLBACK        1                               /*  link status change          */
#define LWIP_NETIF_HWADDRHINT           1                               /*  XXX                         */
#define LWIP_NETIF_LOOPBACK             1

/*********************************************************************************************************
** eth net options
*********************************************************************************************************/

#define LWIP_ARP                        1
#define ARP_QUEUEING                    1
#define ARP_TABLE_SIZE                  LWIP_CFG_ARP_TABLE_SIZE
#define ETHARP_TRUST_IP_MAC             LWIP_CFG_ARP_TRUST_IP_MAC
#define ETHARP_SUPPORT_VLAN             1                               /*  IEEE 802.1q VLAN            */
#define ETH_PAD_SIZE                    LWIP_CFG_ETH_PAD_SIZE
#define ETHARP_SUPPORT_STATIC_ENTRIES   1

/*********************************************************************************************************
** loop interface
*********************************************************************************************************/

#define LWIP_HAVE_LOOPIF                1                               /*  127.0.0.1                   */

/*********************************************************************************************************
** slip interface
*********************************************************************************************************/

#define LWIP_HAVE_SLIPIF                0

/*********************************************************************************************************
** inet thread
*********************************************************************************************************/

#define TCPIP_THREAD_NAME               "lwIP"
#define TCPIP_THREAD_STACKSIZE          (4 * __KB)
#define TCPIP_THREAD_PRIO               (50)

#define NETIF_THREAD_NAME               "netif"
#define NETIF_THREAD_STACKSIZE          (4 * __KB)
#define NETIF_THREAD_PRIO               (100)


#define TCPIP_MBOX_SIZE                 LWIP_CFG_MSG_SIZE

/*********************************************************************************************************
** Socket options
*********************************************************************************************************/

#define ERRNO                                                           /*  include errno               */
#define LWIP_SOCKET                     1
#define LWIP_TIMEVAL_PRIVATE            0                               /*  SmileOS has already defined */
#define LWIP_COMPAT_SOCKETS             0                               /*  some function conflict      */
#define LWIP_POSIX_SOCKETS_IO_NAMES     0                               /*  do not have this!!!         */

#define LWIP_DNS_API_HOSTENT_STORAGE    1                               /*  have bsd DNS                */
#define LWIP_DNS_API_DECLARE_STRUCTS    1                               /*  use lwIP DNS struct         */

/*********************************************************************************************************
** Statistics options
*********************************************************************************************************/

#define LWIP_STATS                      1
#define LWIP_STATS_DISPLAY              1
#define LWIP_STATS_LARGE                1

/*********************************************************************************************************
** PPP options
*********************************************************************************************************/

#define PPP_SUPPORT                     LWIP_CFG_PPP
#define PPPOE_SUPPORT                   LWIP_CFG_PPPOE

#if PPP_SUPPORT > 0 || PPPOE_SUPPORT > 0
#define MEMP_NUM_PPPOE_INTERFACES       LWIP_CFG_NUM_PPP
#define NUM_PPP                         LWIP_CFG_NUM_PPP

#define PAP_SUPPORT                     1
#define CHAP_SUPPORT                    1
#define MD5_SUPPORT                     1
#define VJ_SUPPORT                      1

#define LCP_ECHOINTERVAL                10                              /*  check lcp link per 10seconds*/
#endif                                                                  /*  PPP_SUPPORT > 0 ||          */
                                                                        /*  PPPOE_SUPPORT > 0           */

/*********************************************************************************************************
** SmileOS 需要使用 lwIP core lock 模式, 来支持全双工 socket
*********************************************************************************************************/

#define LWIP_TCPIP_CORE_LOCKING         1

/*********************************************************************************************************
** Debugging options (TCP UDP IP debug & only can print message)
*********************************************************************************************************/

#define LWIP_DBG_TYPES_ON               LWIP_DBG_ON

#if LWIP_CFG_DEBUG > 0
#define LWIP_DEBUG
#define LWIP_DBG_MIN_LEVEL              LWIP_DBG_LEVEL_OFF              /*  允许全部主动打印信息        */
#define TCP_DEBUG                       (LWIP_DBG_OFF | 0)              /*  仅允许 TCP UDP IP debug     */
#define UDP_DEBUG                       (LWIP_DBG_OFF | 0)
#define IP_DEBUG                        (LWIP_DBG_OFF | 0)
#define SOCKETS_DEBUG                   (LWIP_DBG_OFF | 0)
#define NETIF_DEBUG                     (LWIP_DBG_ON  | 0)
#else
#define LWIP_DBG_MIN_LEVEL              LWIP_DBG_LEVEL_SEVERE           /*  不允许主动打印信息          */
#endif                                                                  /*  LWIP_CFG_DEBUG > 0          */

#endif                                                                  /*  LWIP_CONFIG_H_              */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
