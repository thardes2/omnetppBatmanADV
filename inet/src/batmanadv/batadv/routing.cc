//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include <routing.h>
#include <BatmanADVMain.h>
routing::routing()
{
}

routing::~routing()
{
}

/* checks whether the host restarted and is in the protection time.
 * returns:
 *  0 if the packet is to be accepted
 *  1 if the packet is to be ignored.
 */
int BatmanADVMain::batadv_window_protected(long seq_num_diff, simtime_t last_reset)
{
    if (seq_num_diff <= -BATADV_TQ_LOCAL_WINDOW_SIZE || seq_num_diff >= BATADV_EXPECTED_SEQNO_RANGE)
    {
        if (!batadv_has_timed_out(last_reset, BATADV_RESET_PROTECTION_MS))
            return 1;
        last_reset = simTime();
        EV << "Node: " << nodenr << ": old packet received, start protection" << std::endl;
    }
    return 0;
}
/**
 * has_timed_out - compares current time (jiffies) and timestamp + timeout
 * @timestamp:      base value to compare with (in milliseconds)
 * @timeout:        added to base value before comparing (in milliseconds)
 *
 * Returns true if current time is after timestamp + timeout
 */
bool BatmanADVMain::batadv_has_timed_out(simtime_t timestamp, simtime_t timeout)
{
    simtime_t newTime = timestamp + timeout;
    simtime_t currTime = simTime();
    if (currTime.dbl() > newTime.dbl()) //In source code this is "<" -> But this doesn't make sense
        return true;
    else
        return false;
}

/**
 * batadv_update_route - set the router for this originator
 * @bat_priv: the bat priv with all the soft interface information
 * @orig_node: orig node which is to be configured
 * @recv_if: the receive interface for which this route is set
 * @neigh_node: neighbor which should be the next router
 */
void BatmanADVMain::batadv_update_route(batadv_orig_node *orig_node, batadv_hard_iface recv_if, batadv_neigh_node *neigh_node)
{
    batadv_neigh_node *router = NULL;
    if (!orig_node)
        return;
    router = batadv_orig_router_get(orig_node, &recv_if);
    if (router != neigh_node)
        _batadv_update_route(orig_node, recv_if, neigh_node);
    else
        EV << "Node: " << nodenr << ": Router is not equal to neighnode in 'batadv_update_route'" << std::endl;

}

void BatmanADVMain::batadv_update_routeNoOutgoing(batadv_orig_node *orig_node, batadv_neigh_node *neigh_node)
{
    batadv_neigh_node *router = NULL;
    if (!orig_node)
        return;
    router = batadv_orig_router_getNoOutgoing(orig_node);
    if (router != neigh_node)
        _batadv_update_routeNoOutgoing(orig_node, neigh_node);
    else
        EV << "Node: " << nodenr << ": Router is not equal to neighnode in 'batadv_update_route'" << std::endl;

}

/**
 * _batadv_update_route - set the router for this originator
 * @bat_priv: the bat priv with all the soft interface information
 * @orig_node: orig node which is to be configured
 * @recv_if: the receive interface for which this route is set
 * @neigh_node: neighbor which should be the next router
 *
 * This function does not perform any error checks
 */
void BatmanADVMain::_batadv_update_route(batadv_orig_node *orig_node, batadv_hard_iface recv_if, batadv_neigh_node *neigh_node)
{
    batadv_orig_ifinfo *orig_ifinfo;
    batadv_neigh_node *curr_router = NULL;
    orig_ifinfo = batadv_orig_ifinfo_get(orig_node, NULL);
    if (!orig_ifinfo)
    {
        delete orig_node;
        return;
    }

    curr_router = orig_ifinfo->router;

    //route deleted - The router is replaced and the old one is not longer needed
    if ((curr_router) && (!neigh_node))
    {
        EV << "Node: " << nodenr << ": Deleting route towards: " << orig_node->orig << std::endl;
        batadv_tt_global_del_orig(orig_node, -1, "Deleted route towards originator");

    } //route added
    else if ((!curr_router) && (neigh_node))
    {
        EV << "Node: " << nodenr << ": Adding route towards: " << orig_node->orig.str() << " (via " << neigh_node->addr.str() << ")\n" << std::endl;
    } // route changed
    else if (neigh_node && curr_router)
    {
        EV << "Node: " << nodenr << ": Changing route towards: " << orig_node->orig << "  (now via " << neigh_node->addr << "  - was via " << curr_router->addr << std::endl;
    }

    //printf("Node: %s --------------------- set new router\n",nodenr.c_str());
    orig_ifinfo->router = neigh_node;
    EV << "Node: " << nodenr << ": Changed router of " << orig_node->orig.getMAC().str() << " orig_ifinfo to " << neigh_node->addr.getMAC().str() << std::endl;

}

void BatmanADVMain::_batadv_update_routeNoOutgoing(batadv_orig_node *orig_node, batadv_neigh_node *neigh_node)
{
    batadv_orig_ifinfo *orig_ifinfo;
    batadv_neigh_node *curr_router = NULL;
    orig_ifinfo = batadv_orig_ifinfo_get(orig_node, NULL);
    if (!orig_ifinfo)
    {
        delete orig_node;
        return;
    }

    curr_router = orig_ifinfo->router;

    //route deleted - The router is replaced and the old one is not longer needed
    if ((curr_router) && (!neigh_node))
    {
        EV << "Node: " << nodenr << ": Deleting route towards: " << orig_node->orig << std::endl;
        batadv_tt_global_del_orig(orig_node, -1, "Deleted route towards originator");

    } //route added
    else if ((!curr_router) && (neigh_node))
    {
        EV << "Node: " << nodenr << ": Adding route towards: " << orig_node->orig.str() << " (via " << neigh_node->addr.str() << ")\n" << std::endl;
    } // route changed
    else if (neigh_node && curr_router)
    {
        EV << "Node: " << nodenr << ": Changing route towards: " << orig_node->orig << "  (now via " << neigh_node->addr << "  - was via " << curr_router->addr << std::endl;
    }

    //printf("Node: %s --------------------- set new router\n",nodenr.c_str());
    orig_ifinfo->router = neigh_node;

    if ((iNodenr == -2 || iNodenr == -3) && batadv_compare_eth(orig_node->orig.getMAC(), MACAddress("12:34:56:78:9A:BA")) == 0)
        printf("Update route - Changed router of %s orig_ifinfo to %s - node %s - time %f\n", orig_node->orig.getMAC().str().c_str(),neigh_node->addr.getMAC().str().c_str(), nodenr.c_str(), simTime().dbl());
    EV << "Node: " << nodenr << ": Changed router of " << orig_node->orig.getMAC().str() << " orig_ifinfo to " << neigh_node->addr.getMAC().str() << std::endl;

}

/**
 * batadv_check_unicast_packet - Check for malformed unicast packets
 * @bat_priv: the bat priv with all the soft interface information
 * @skb: packet to check
 * @hdr_size: size of header to pull
 *
 * Check for short header and bad addresses in given packet. Returns negative
 * value when check fails and 0 otherwise. The negative value depends on the
 * reason: -ENODATA for bad header, -EBADR for broadcast destination or source,
 * and -EREMOTE for non-local (other host) destination.
 */
int BatmanADVMain::batadv_check_unicast_packet(MACAddress dest, MACAddress src)
{
    /* drop packet if it has not necessary minimum size --> TODO
     /* packet with unicast indication but broadcast recipient */
    if (dest.isBroadcast())
    {
        EV << "Node: " << nodenr << ": batadv_check_unicast_packet dropped b. " << dest.str() << " is a broadcast!" << std::endl;
        return -EBADR;
    }

    /* packet with broadcast sender address */
    if (src.isBroadcast())
    {
        EV << "Node: " << nodenr << ": batadv_check_unicast_packet dropped b. " << src.str() << " is a broadcast!" << std::endl;
        return -EBADR;
    }
    /* not for me */
    if (batadv_is_my_mac(&dest) != 0)
    {
        EV << "Node: " << nodenr << ": batadv_check_unicast_packet dropped b. " << dest.str() << " is not my mac!" << std::endl;
        return -EREMOTE;
    }
    return 0;
}
/**
 * batadv_find_router - find a suitable router for this originator
 * @bat_priv: the bat priv with all the soft interface information
 * @orig_node: the destination node
 * @recv_if: pointer to interface this packet was received on
 *
 * Returns the router which should be used for this orig_node on
 * this interface, or NULL if not available.
 */
batadv_neigh_node* BatmanADVMain::batadv_find_router(/*struct batadv_priv *bat_priv,*/batadv_orig_node *orig_node, batadv_hard_iface *recv_if)
{
    batadv_neigh_node *router = NULL;
    if (!orig_node)
        return NULL;

    router = batadv_orig_router_get(orig_node, recv_if);
    if (!router)
        return router;
    /* only consider bonding for recv_if == BATADV_IF_DEFAULT (first hop)
     * and if activated.
     */
    //TODO: Bonding needed?
    //if (!(recv_if == meshvpn && atomic_read(&bat_priv->bonding)))
    return router;

}
batadv_neigh_node* BatmanADVMain::batadv_find_routerNoOutgoing(/*struct batadv_priv *bat_priv,*/batadv_orig_node *orig_node)
{
    batadv_neigh_node *router = NULL;
    if (!orig_node)
        return NULL;

    router = batadv_orig_router_getNoOutgoing(orig_node);
    if (!router)
        return router;
    /* only consider bonding for recv_if == BATADV_IF_DEFAULT (first hop)
     * and if activated.
     */
    //TODO: Bonding needed?
    //if (!(recv_if == meshvpn && atomic_read(&bat_priv->bonding)))
    return router;

}
int BatmanADVMain::batadv_route_unicast_packet(cPacket *packet, batadv_hard_iface *recv_if)
{
    NET_XMIT ret = NET_RX_DROP;
    NET_XMIT res;
    batadv_orig_node *orig_node = NULL;
    MACAddress src;
    MACAddress dst;
    batadv_unicast_packet* unicast_packet;
    batadv_unicast_tvlv_packet* unicast_tvlv_packet;

    //batadv_unicast_tvlv_packet* unicast_tvlv =(batadv_unicast_tvlv_packet *)packet;
    if (dynamic_cast<batadv_unicast_tvlv_packet *>(packet) != NULL)
    {
        unicast_tvlv_packet = (batadv_unicast_tvlv_packet *) packet;
        src = unicast_tvlv_packet->getSrc();
        dst = unicast_tvlv_packet->getDst();

    }
    else if (dynamic_cast<batadv_unicast_packet *>(packet) != NULL)
    {
        unicast_packet = (batadv_unicast_packet *) packet;
        /*src = eth_frame->getSrc();
         //delete eth_frame;
         dst = unicast_packet->getDest();*/
    }
    if (unicast_tvlv_packet->getTtl() < 2)   //TODO: THIS BREAKS if we just have a unicast
    {
        EV << "Node: " << nodenr << ": Warning - can't forward unicast packet from " << src.str() << " to " << dst.str() << " -> ttl exceeded" << std::endl;
        return -1;
    }
    // get routing information
    orig_node = checkOrigListForExistence(ManetAddress(dst));
    if (!orig_node)
        return -1;
    unicast_tvlv_packet->setTtl(unicast_tvlv_packet->getTtl() - 1);   //TODO: THIS BREAKS if we just have a unicast
    res = batadv_send_skb_to_orig((cPacket*) unicast_tvlv_packet, orig_node, recv_if, ROUTE_UNICAST);   //TODO: THIS BREAKS if we just have a unicast
    if (res == NET_XMIT_SUCCESS) /* skb was transmitted and consumed */
    {
        //Counter stuff is skipped
        ret = NET_RX_SUCCESS;
    }
    else if (res == NET_XMIT_POLICED)
    {
        /* skb was buffered and consumed */
        ret = NET_RX_SUCCESS;
    }
    return ret;
}

int BatmanADVMain::batadv_recv_unicast_packet(batadv_hard_iface *recv_if, cMessage* msg)
{
    frame = (EtherFrame*) msg;
    EtherFrame* f = getCurrentFrame();
    batadv_unicast_packet* unicast_packet = (batadv_unicast_packet*) f->decapsulate();
    batadv_orig_node *orig_node = NULL;
    int check = 0;
    bool is4addr;
    is4addr = unicast_packet->getPacket_type() == BATADV_UNICAST_4ADDR;
    if (is4addr)
    {

    }
    /* function returns -EREMOTE for promiscuous packets */
    MACAddress dest = unicast_packet->getDest();
    check = batadv_check_unicast_packet(unicast_packet->getDest(), f->getSrc());
    //delete f->decapsulate();
    delete f;
    /* packet for me */
    if (batadv_is_my_mac(&unicast_packet->getDest()) == 0)
    {
        if (is4addr)
        {

        }
        batadv_interface_rx(msg, recv_if, orig_node);
        return NET_RX_SUCCESS;
    }
    if (check < 0)
        return NET_RX_DROP;
    if (!batadv_check_unicast_ttvn(recv_if, unicast_packet))
        return NET_RX_DROP;

    return batadv_route_unicast_packet(unicast_packet, recv_if);
}
/*void BatmanADVMain::batadv_interface_rx(batadv_hard_iface *recv_if, batadv_orig_node *orig_node)
 {

 }*/
int BatmanADVMain::batadv_check_unicast_ttvn(batadv_hard_iface *recv_if, batadv_unicast_packet* unicast_packet)
{
    batadv_orig_node *orig_node;
    int curr_ttvn, old_ttvn;
    unsigned short vid;
    EtherFrame* f = getCurrentFrame();
    delete f->decapsulate();
    bool is_old_ttvn;

    /* check if the destination client was served by this node and it is now
     * roaming. In this case, it means that the node has got a ROAM_ADV
     * message and that it knows the new destination in the mesh to re-route
     * the packet to
     */
    /*if (batadv_tt_local_client_is_roaming(&(f->getDest()), 0))//vid = 0
     {
     if (batadv_reroute_unicast_packet(bat_priv, unicast_packet, f->getDest(),0))// vid = 0
     EV << "Node: " << nodenr << ": Rerouting unicast packet to " << unicast_packet->getDest().str() << " (dst=%pM)" << std::endl;*/
    /* at this point the mesh destination should have been
     * substituted with the originator address found in the global
     * table. If not, let the packet go untouched anyway because
     * there is nothing the node can do
     */
    /*   return 1;

     }*/

    /* retrieve the TTVN known by this node for the packet destination. This
     * value is used later to check if the node which sent (or re-routed
     * last time) the packet had an updated information or not
     */
    curr_ttvn = bat_priv_tt.vn;
    if (!batadv_is_my_mac(&(unicast_packet->getDest())))
    {
        orig_node = checkOrigListForExistence(ManetAddress(unicast_packet->getDest()));
        /* if it is not possible to find the orig_node representing the
         * destination, the packet can immediately be dropped as it will
         * not be possible to deliver it
         */
        if (!orig_node)
            return 0;

        curr_ttvn = orig_node->last_ttvn;
    }

    /* check if the TTVN contained in the packet is fresher than what the
     * node knows
     */
    if (unicast_packet->getTtvn() < curr_ttvn)
        is_old_ttvn = false;

    else
        is_old_ttvn = true;
//is_old_ttvn = batadv_seq_before(unicast_packet->ttvn, curr_ttvn);
    if (!is_old_ttvn)
        return 1;
    old_ttvn = unicast_packet->getTtvn();
    /* the packet was forged based on outdated network information. Its
     * destination can possibly be updated and forwarded towards the new
     * target host
     */

    if (batadv_reroute_unicast_packet(unicast_packet, &(f->getDest()), 0))    //vid = 0
    {
        EV << "Node: " << nodenr << ": Rerouting unicast packet to " << unicast_packet->getDest() << " (dst=%pM): TTVN mismatch old_ttvn=" << old_ttvn << "new_ttvn=" << curr_ttvn << std::endl;
        delete f;
        return 1;
    }

    /* the packet has not been re-routed: either the destination is
     * currently served by this node or there is no destination at all and
     * it is possible to drop the packet
     */
    if (!batadv_is_my_client(f->getDest()))
    {
        delete f;
        return 0;
    }
    unicast_packet->setDest(ownAddress.getMAC());
    unicast_packet->setTtvn(curr_ttvn);
    delete f;
    return 0;
}
/**
 * batadv_reroute_unicast_packet - update the unicast header for re-routing
 * @bat_priv: the bat priv with all the soft interface information
 * @unicast_packet: the unicast header to be updated
 * @dst_addr: the payload destination
 * @vid: VLAN identifier
 *
 * Search the translation table for dst_addr and update the unicast header with
 * the new corresponding information (originator address where the destination
 * client currently is and its known TTVN)
 *
 * Returns true if the packet header has been updated, false otherwise
 */
bool BatmanADVMain::batadv_reroute_unicast_packet(batadv_unicast_packet *unicast_packet, MACAddress *dst_addr, unsigned short vid)
{
    batadv_orig_node *orig_node = NULL;
    batadv_hard_iface *primary_if = NULL;
    bool ret = false;
    MACAddress orig_addr;
    int orig_ttvn;

    if (batadv_is_my_client(*dst_addr))
    {
        orig_addr = ownAddress.getMAC();
        orig_ttvn = bat_priv_tt.vn;
    }
    else
    {
        //orig_node = batadv_transtable_search(NULL, dst_addr, vid);
        if (!orig_node)
            return ret;

        if (batadv_compare_eth(orig_node->orig.getMAC(), unicast_packet->getDest()))
            return ret;

        orig_addr = orig_node->orig.getMAC();
        orig_ttvn = orig_node->last_ttvn;
    }
    /* update the packet header */
    unicast_packet->setDest(orig_addr);
    unicast_packet->setTtvn(orig_ttvn);

    ret = true;
    return ret;
}

int BatmanADVMain::batadv_recv_bcast_packet(cMessage *msg, batadv_hard_iface *recv_if)
{
    int ret = NET_RX_DROP;
    long seq_diff;
    unsigned long seqno;
    batadv_orig_node *orig_node = NULL;
    batadv_bcast_packet* bcast_packet = NULL;
    if (!dynamic_cast<Ieee80211DataFrameWithSNAP *>(msg) != NULL)
        return ret;
    Ieee80211DataFrameWithSNAP* frame = dynamic_cast<Ieee80211DataFrameWithSNAP *>(msg);
    MACAddress senderMac = frame->getTransmitterAddress();
    MACAddress rcvMac = frame->getReceiverAddress();
    if (!rcvMac.isBroadcast())
        return ret;
    if (senderMac.isBroadcast())    //Broadcast sender
        return ret;

    /* ignore broadcasts sent by myself */
    /* if (batadv_is_my_mac(&ownAddress.getMAC(), senderMac) == 0)
     goto out;*/
    if (frame == NULL)
        return ret;

    bcast_packet = check_and_cast<batadv_bcast_packet *>(frame->decapsulate());
    seqno = bcast_packet->getSeqno();
//ignore broadcasts originated by myself
    if (batadv_is_my_mac(&(bcast_packet->getOrig())) == 0)
        return ret;

    if (bcast_packet->getTtl() < 2)
        return ret;
    orig_node = checkOrigListForExistence(ManetAddress(bcast_packet->getOrig()));
    if (!orig_node)
        return ret;

    /* check whether the packet is a duplicate */
    /*if (batadv_test_bit(orig_node->bcast_bits, orig_node->last_bcast_seqno,
     seqno))*/

    seq_diff = seqno - orig_node->last_bcast_seqno;
    /* check whether the packet is old and the host just restarted. */
    if (batadv_window_protected(seq_diff, orig_node->bcast_seqno_reset))
        return ret;

    /* mark broadcast in flood history, update window position
     * if required.
     */
//TODO
    /*if (batadv_bit_get_packet( orig_node->bcast_bits, seq_diff, 1))
     orig_node->last_bcast_seqno = seqno;*/

    /* rebroadcast packet */
//TODO!!!
    /* broadcast for me */
//batadv_interface_rx(recv_if->soft_iface, skb, recv_if, hdr_size,         orig_node);
    ret = NET_RX_SUCCESS;

    return ret;
}
/**
 * batadv_recv_unicast_tvlv - receive and process unicast tvlv packets
 * @skb: unicast tvlv packet to process
 * @recv_if: pointer to interface this packet was received on
 * @dst_addr: the payload destination
 *
 * Returns NET_RX_SUCCESS if the packet has been consumed or NET_RX_DROP
 * otherwise.
 */
int BatmanADVMain::batadv_recv_unicast_tvlv(cMessage *msg, batadv_hard_iface *recv_if)
{
    EtherFrame *eth_frame = dynamic_cast<EtherFrame*>(msg);
    batadv_unicast_tvlv_packet *unicast_tvlv_packet;

    int ret = NET_RX_DROP;
    if (batadv_check_unicast_packet(eth_frame->getDest(), eth_frame->getSrc()) < 0)
    {
        EV << "Node: " << nodenr << ": Unicast tvlv package got dropped because batadv_check_unicast_packet failed" << std::endl;
        delete eth_frame;
        return NET_RX_DROP;
    }
    unicast_tvlv_packet = dynamic_cast<batadv_unicast_tvlv_packet*>(eth_frame->decapsulate());
    delete eth_frame;
    cPacket* innerpacket = unicast_tvlv_packet->decapsulate();
    if (innerpacket != NULL)
    {
        if (dynamic_cast<batadv_tvlv_tt_data *>(innerpacket) != NULL)
        {
            batadv_tvlv_tt_data *tvlv_value = (batadv_tvlv_tt_data*) innerpacket;
            ret = batadv_tt_tvlv_unicast_handler_v1(&(unicast_tvlv_packet->getSrc()), &(unicast_tvlv_packet->getDst()), tvlv_value);
            if (ret != NET_RX_SUCCESS)
            {
                unicast_tvlv_packet->encapsulate(tvlv_value);
                /*batadv_unicast_packet *unicast = new batadv_unicast_packet();
                 unicast->setPacket_type(BATADV_UNICAST);
                 unicast->setVersion(BATADV_COMPAT_VERSION);
                 unicast->setTtl(BATADV_TTL);
                 unicast->setDest(unicast_tvlv_packet->getDst());
                 unicast->setTtvn(tvlv_value->getTtvn());
                 unicast->encapsulate(unicast_tvlv_packet);
                 eth_frame->encapsulate(unicast);*/
                ret = batadv_route_unicast_packet(unicast_tvlv_packet, recv_if);
                if (ret < 0)
                {
                    delete unicast_tvlv_packet;
                    delete innerpacket;
                }
            }
        }

    }
    return ret;
}
