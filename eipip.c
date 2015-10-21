/*
 * Copyright (C) 2015 Gris Ge
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Gris Ge <cnfourt@gmail.com>
 */

/*
 * Code was based on Linux kernel /net/ipv4/ipip.c
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <net/rtnetlink.h>
#include <net/ip_tunnels.h>
#include <net/xfrm.h>

static int eipip_net_id __read_mostly;
static int __net_init eipip_init_net(struct net *net);
static void __net_exit eipip_exit_net(struct net *net);
static int eipip_rcv(struct sk_buff *skb);
static int eipip_err(struct sk_buff *skb, u32 info);
static void eipip_tunnel_setup(struct net_device *dev);
static int eipip_newlink(struct net *src_net, struct net_device *dev,
			 struct nlattr *tb[], struct nlattr *data[]);
static int eipip_changelink(struct net_device *dev, struct nlattr *tb[],
			    struct nlattr *data[]);



static struct pernet_operations eipip_net_ops = {
	.init = eipip_init_net,
	.exit = eipip_exit_net,
	.id   = &eipip_net_id,
	.size = sizeof(struct ip_tunnel_net),
};

static struct xfrm_tunnel eipip_handler __read_mostly = {
	.handler	=	eipip_rcv,
	.err_handler	=	eipip_err,
	.priority	=	1,
};

static const struct nla_policy eipip_policy[IFLA_IPTUN_MAX + 1] = {
	[IFLA_IPTUN_LINK]		= { .type = NLA_U32 },
	[IFLA_IPTUN_LOCAL]		= { .type = NLA_U32 },
	[IFLA_IPTUN_REMOTE]		= { .type = NLA_U32 },
	[IFLA_IPTUN_TTL]		= { .type = NLA_U8 },
	[IFLA_IPTUN_TOS]		= { .type = NLA_U8 },
	[IFLA_IPTUN_PMTUDISC]		= { .type = NLA_U8 },
	[IFLA_IPTUN_ENCAP_TYPE]		= { .type = NLA_U16 },
	[IFLA_IPTUN_ENCAP_FLAGS]	= { .type = NLA_U16 },
	[IFLA_IPTUN_ENCAP_SPORT]	= { .type = NLA_U16 },
	[IFLA_IPTUN_ENCAP_DPORT]	= { .type = NLA_U16 },
};

static struct rtnl_link_ops eipip_link_ops __read_mostly = {
	.kind		= "eipip",
	.maxtype	= IFLA_IPTUN_MAX,
	.policy		= eipip_policy,
	.priv_size	= sizeof(struct ip_tunnel),
	.setup		= eipip_tunnel_setup,
	.newlink	= eipip_newlink,
	.changelink	= eipip_changelink,
	.dellink	= ip_tunnel_dellink,
	.get_size	= eipip_get_size,
	.fill_info	= eipip_fill_info,
	.get_link_net	= ip_tunnel_get_link_net,
};

static int __init eipip_init(void)
{
	int err = 0;
	pr_info("Loading eipip module\n");
	err = register_pernet_device(&eipip_net_ops);
	if (err < 0)
		return err;
	err = xfrm4_tunnel_register(&eipip_handler, AF_INET);
	if (err < 0) {
		pr_info("%s: can't register tunnel\n", __func__);
		goto xfrm_tunnel_failed;
	}
	err = rtnl_link_register(&eipip_link_ops);
	if (err < 0)
		goto rtnl_link_failed;
	return err;
}

static void __exit eipip_exit(void)
{
	pr_info("Unloading eipip module\n");
}

module_init(eipip_init);
module_exit(eipip_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Encripted personal use IP IP module");
MODULE_AUTHOR("Gris Ge <cnfourt@gmail.com>");
MODULE_ALIAS_RTNL_LINK("eipip");
MODULE_ALIAS_NETDEV("tune0");
MODULE_VERSION("0.1");
