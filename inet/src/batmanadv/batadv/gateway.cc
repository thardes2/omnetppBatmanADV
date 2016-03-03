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

#include <gateway.h>
#include <BatmanADVMain.h>

gateway::gateway()
{

}

gateway::~gateway()
{
}

void BatmanADVMain::batadv_gw_select(batadv_gw_node *new_gw_node)
{
    if (new_gw_node)
    {
        updateGwStats(selectedGw, new_gw_node);
        selectedGw = new_gw_node;
        EV << "Node: " << nodenr << ": Switching to a better gateway node!" << std::endl;
    }
    return;
}
/**
 * batadv_gw_dhcp_recipient_get - check if a packet is a DHCP message
 * @skb: the packet to check
 * @header_len: a pointer to the batman-adv header size
 * @chaddr: buffer where the client address will be stored. Valid
 *  only if the function returns BATADV_DHCP_TO_CLIENT
 *
 * Returns:
 * - BATADV_DHCP_NO if the packet is not a dhcp message or if there was an error
 *   while parsing it
 * - BATADV_DHCP_TO_SERVER if this is a message going to the DHCP server
 * - BATADV_DHCP_TO_CLIENT if this is a message going to a DHCP client
 *
 * This function may re-allocate the data buffer of the skb passed as argument.
 */
batadv_dhcp_recipient BatmanADVMain::batadv_gw_dhcp_recipient_get(cMessage *msg)
{
    if (dynamic_cast<Ieee80211DataFrameWithSNAP *>(msg) == NULL)
    {
        delete msg;
        return BATADV_DHCP_NO;
    }
    Ieee80211DataFrameWithSNAP* orig_frame = dynamic_cast<Ieee80211DataFrameWithSNAP *>(msg);
    Ieee80211DataFrameWithSNAP* frame = orig_frame->dup();
    if (frame == NULL)
    {
        delete frame;
        return BATADV_DHCP_NO;
    }
    MACAddress clientMac = frame->getTransmitterAddress();
    /* check for IP header  */
    IPv4Datagram* v4D = dynamic_cast<IPv4Datagram *>(frame->decapsulate());
    if (v4D == NULL)
    {
        delete frame;
        return BATADV_DHCP_NO;
    }

    /* check for udp header  */
    UDPPacket* udp = dynamic_cast<UDPPacket *>(v4D->decapsulate());
    if (udp == NULL)
    {
        delete frame;
        return BATADV_DHCP_NO;
    }

    DHCPMessage *dhcpMessage = dynamic_cast<DHCPMessage *>(udp->decapsulate());
    if (dhcpMessage == NULL)
    {
        delete frame;
        return BATADV_DHCP_NO;
    }

    if (gw_mode == BATADV_GW_MODE_SERVER)
    {
        delete frame;
        return BATADV_DHCP_TO_CLIENT;
    }
    else if (gw_mode == BATADV_GW_MODE_OFF) //Ups -> Forwards this to our selected GW
    {
        delete frame;
        //forwardDHCPRequest(dhcpMessage);
        return BATADV_DHCP_TO_SERVER;
    }
    return BATADV_DHCP_TO_SERVER;
}
/**
 * batadv_gw_reselect - force a gateway reselection
 * @bat_priv: the bat priv with all the soft interface information
 *
 * Set a flag to remind the GW component to perform a new gateway reselection.
 * However this function does not ensure that the current gateway is going to be
 * deselected. The reselection mechanism may elect the same gateway once again.
 *
 * This means that invoking batadv_gw_reselect() does not guarantee a gateway
 * change and therefore a uevent is not necessarily expected.
 */
void BatmanADVMain::batadv_gw_reselect(/*struct batadv_priv *bat_priv*/)
{
    bat_priv_gw.reselect = true;
}

batadv_gw_node* BatmanADVMain::batadv_gw_get_best_gw_node(/*struct batadv_priv *bat_priv*/)
{
    double gw_divisor;
    long max_gw_factor = 0, tmp_gw_factor = 0;
    int max_tq = 0;
    int tq_avg;
    batadv_neigh_node *router;
    batadv_neigh_ifinfo *router_ifinfo;
    batadv_orig_node *orig_node;
    batadv_gw_node *gw_node, *curr_gw = NULL;
    gw_divisor = BATADV_TQ_LOCAL_WINDOW_SIZE * BATADV_TQ_LOCAL_WINDOW_SIZE;
    gw_divisor *= 64;
    for (unsigned int i = 0; i < gwList.size(); i++)
    {
        gw_node = gwList[i];
        if (gw_node->deleted)
            continue;

        orig_node = gw_node->orig_node;
        router = batadv_orig_router_get(orig_node,BATADV_IF_DEFAULT);
        if (!router)
            continue;

        router_ifinfo = batadv_neigh_ifinfo_get(router,BATADV_IF_DEFAULT);
        if (!router_ifinfo)
            continue; //return selectedGw;
        tq_avg = router_ifinfo->bat_iv.tq_avg;
        switch (gw_sel_class)
        {
            case fastConnection: /* fast connection */
                tmp_gw_factor = tq_avg * tq_avg;
                tmp_gw_factor *= gw_node->bandwidth_down;
                tmp_gw_factor *= 100 * 100;
                tmp_gw_factor /= gw_divisor;
                if ((tmp_gw_factor > max_gw_factor) || ((tmp_gw_factor == max_gw_factor) && (tq_avg > max_tq)))
                {
                    curr_gw = gw_node;
                }
                break;
            default:
                /* 2:  stable connection (use best statistic)
                 * 3:  fast-switch (use best statistic but change as
                 *     soon as a better gateway appears)
                 * XX: late-switch (use best statistic but change as
                 *     soon as a better gateway appears which has
                 *     $routing_class more tq points)
                 */
                if (tq_avg > max_tq)
                {
                    curr_gw = gw_node;
                }
                break;
        }
        if (tq_avg > max_tq)
            max_tq = tq_avg;

        if (tmp_gw_factor > max_gw_factor)
            max_gw_factor = tmp_gw_factor;
    }
    return curr_gw;
}
void BatmanADVMain::batadv_gw_selection(/*struct batadv_priv *bat_priv*/)
{
    batadv_gw_node *next_gw = NULL;
    batadv_neigh_node *router = NULL;
    batadv_neigh_ifinfo *router_ifinfo = NULL;
    if (gw_mode != BATADV_GW_MODE_CLIENT)
    {
        EV << "Node: " << nodenr << ": We are already a gateway, so we don't have to select one" << std::endl;
        return;
    }

    bat_priv_gw.reselect = false;

    next_gw = batadv_gw_get_best_gw_node();
    if (selectedGw == next_gw)
        return;
    if (next_gw == NULL || selectedGw == next_gw)
    {
        EV << "Node: " << nodenr << ": Current gateway is still the best one -> " << selectedGw->orig_node->orig.getMAC().str() << std::endl;
        return;
    }
    if (next_gw)
    {

        router = batadv_orig_router_get(next_gw->orig_node,BATADV_IF_DEFAULT);
        if (!router)
        {
            selectedGw->reselect = true;
            return;
        }
        router_ifinfo = batadv_neigh_ifinfo_get(router,BATADV_IF_DEFAULT);
        if (!router_ifinfo)
        {
            selectedGw->reselect = true;
            return;
        }
        if ((selectedGw) && (!next_gw))
        {
            EV << "Node: " << nodenr << ": Removing selected gateway - no gateway in range" << std::endl;
            //TODO: We should do something here
            /*  batadv_throw_uevent(bat_priv, BATADV_UEV_GW, BATADV_UEV_ADD,
             gw_addr);*/
        }
        else if ((!selectedGw) && (next_gw))
        {
            EV << "Node: " << nodenr << ": Adding route to gateway " << next_gw->orig_node->orig.getMAC().str() << "(bandwidth:" << (next_gw->bandwidth_down / 10) << "." << (fmod(next_gw->bandwidth_down, 10.0)) << "/" << next_gw->bandwidth_up / 10 << "." << (fmod(next_gw->bandwidth_up, 10.0)) << "MBit, tq: " << router_ifinfo->bat_iv.tq_avg << std::endl;
            //TODO: We should do something here
            /*batadv_throw_uevent(bat_priv, BATADV_UEV_GW, BATADV_UEV_ADD,
             gw_addr);*/
        }
    }
    if ((selectedGw) && (!next_gw))
    {
        EV << "Node: " << nodenr << ": No gateway in range" << std::endl;
        return;
    }
    batadv_gw_select(next_gw);

}
/**
 * batadv_gw_node_add - add gateway node to list of available gateways
 * @bat_priv: the bat priv with all the soft interface information
 * @orig_node: originator announcing gateway capabilities
 * @gateway: announced bandwidth information
 */
void BatmanADVMain::batadv_gw_node_add(/*struct batadv_priv *bat_priv,*/batadv_orig_node *orig_node, batadv_tvlv_gateway_data *gateway)
{
    batadv_gw_node *gw_node;
    if (gateway->getBandwidth_down() == 0 || gateway->getBandwidth_up() == 0)
    {
        EV << "Node: " << nodenr << ": Announced Bandwith of entry " << orig_node->orig.getMAC().str() << " is 0, so it is not useful for us" << std::endl;
        delete gateway; //Testing
        return;
    }
    gw_node = new batadv_gw_node();
    gw_node->orig_node = orig_node;
    gw_node->bandwidth_down = gateway->getBandwidth_down();
    gw_node->bandwidth_up = gateway->getBandwidth_up();
    gw_node->reselect = false;
    gw_node->deleted = 0;
    gwList.push_back(gw_node);
    EV << "Node: " << nodenr << ": Found new Gateway " << gw_node->orig_node->orig.getMAC().str() << "(bandwidth:" << (gw_node->bandwidth_down / 10) << "." << (fmod(gw_node->bandwidth_down, 10.0)) << "/" << gw_node->bandwidth_up / 10 << "." << (fmod(gw_node->bandwidth_up, 10.0)) << "MBit" << std::endl;

}

/**
 * batadv_gw_node_get - retrieve gateway node from list of available gateways
 * @orig_node: originator announcing gateway capabilities
 *
 * Returns gateway node if found or NULL otherwise.
 */
batadv_gw_node* BatmanADVMain::batadv_gw_node_get(/*struct batadv_priv *bat_priv,*/batadv_orig_node *orig_node)
{
    batadv_gw_node *gw_node_tmp, *gw_node = NULL;

    for (unsigned int i = 0; i < gwList.size(); i++)
    {
        gw_node_tmp = gwList[i];
        if (gw_node_tmp->orig_node != orig_node)
            continue;
        if (gw_node_tmp->deleted)
            continue;
        gw_node = gw_node_tmp;
        break;
    }
    return gw_node;
}
/**
 * batadv_gw_node_update - update list of available gateways with changed
 *  bandwidth information
 * @bat_priv: the bat priv with all the soft interface information
 * @orig_node: originator announcing gateway capabilities
 * @gateway: announced bandwidth information
 */
void BatmanADVMain::batadv_gw_node_update(batadv_orig_node *orig_node, batadv_tvlv_gateway_data *gateway)
{
    batadv_gw_node *gw_node;

    gw_node = batadv_gw_node_get(orig_node);
    if (!gw_node)
    {
        batadv_gw_node_add(orig_node, gateway);
        //delete gateway;//Testing
        gateway = NULL;
        return;
    }
    if ((gw_node->bandwidth_down == gateway->getBandwidth_down()) && (gw_node->bandwidth_up == gateway->getBandwidth_up()))
        return;

    EV << "Node: " << nodenr << ": Gateway bandwidth of originator" << orig_node->orig.getMAC().str() << "changed from " << gw_node->bandwidth_down / 10 << "." << fmod(gw_node->bandwidth_down, 10.0) << "/" << gw_node->bandwidth_up / 10 << "." << fmod(gw_node->bandwidth_up, 10.0) << "MBit to " << (gateway->getBandwidth_down() / 10) << "." << fmod(gateway->getBandwidth_down(), 10) << "/" << gateway->getBandwidth_up() / 10 << "." << fmod(gateway->getBandwidth_up(), 10.0) << "MBit" << std::endl;

    gw_node->bandwidth_down = gateway->getBandwidth_down();
    gw_node->bandwidth_up = gateway->getBandwidth_up();
    gw_node->deleted = 0;
    if (gateway->getBandwidth_down() == 0)
    {
        delete gateway; //Testing
        gw_node->deleted = 1; //Maybe this has to be a simetime() value
        EV << "Node: " << nodenr << ": Gateway " << orig_node->orig.getMAC().str() << " removed from gateway list" << std::endl;
        /* Note: We don't need a NULL check here, since curr_gw never
         * gets dereferenced.
         */

        if (gw_node == selectedGw)
            batadv_gw_reselect();
    }
    gateway = NULL;
    return;
}

void BatmanADVMain::batadv_gw_node_delete(batadv_orig_node *orig_node)
{
    batadv_tvlv_gateway_data gateway;
    gateway.setBandwidth_down(0);
    gateway.setBandwidth_up(0);
    batadv_gw_node_update(orig_node, &gateway);
    EV << "Node: " << nodenr << ": Setting bandwith to 0 - Gateway: " << orig_node->orig.getMAC().str() << std::endl;
}
void BatmanADVMain::batadv_gw_node_purge()
{
    batadv_gw_node *gw_node;
    for (unsigned int i = 0; i < gwList.size(); i++)
    {
        gw_node = gwList[i];
        if (((gw_node->deleted == 0)))
            continue;

        /* if (curr_gw == gw_node)
         do_reselect = 1;

         hlist_del_rcu(&gw_node->list);*/

        delete gw_node;
        gwList.erase(gwList.begin() + i - 1); //TODO: Check this
        EV << "Node: " << nodenr << ": Erased a gateway node: " << gw_node->orig_node->orig.getMAC().str() << std::endl;

    }

}
/**
 * batadv_gw_tvlv_ogm_handler_v1 - process incoming gateway tvlv container
 * @bat_priv: the bat priv with all the soft interface information
 * @orig: the orig_node of the ogm
 * @flags: flags indicating the tvlv state (see batadv_tvlv_handler_flags)
 * @tvlv_value: tvlv buffer containing the gateway data
 * @tvlv_value_len: tvlv buffer length
 */
void BatmanADVMain::batadv_gw_tvlv_ogm_handler_v1(batadv_orig_node *orig, int flags, batadv_tvlv_gateway_data *tvlv_value)
{
    batadv_tvlv_gateway_data gateway, *gateway_ptr;
    /*if ((flags & BATADV_TVLV_HANDLER_OGM_CIFNOTFND))
     {
     gateway.setBandwidth_down(0);
     gateway.setBandwidth_up(0);
     }
     else
     {*/
    gateway_ptr = tvlv_value;
    gateway.setBandwidth_down(gateway_ptr->getBandwidth_down());
    gateway.setBandwidth_up(gateway_ptr->getBandwidth_up());
    //delete gateway_ptr;//Testing
    if ((gateway.getBandwidth_down() == 0) || (gateway.getBandwidth_up() == 0))
    {
        gateway.setBandwidth_down(0);
        gateway.setBandwidth_up(0);
    }
    //}
    batadv_gw_node_update(/*bat_priv, */orig, &gateway);

    /* restart gateway selection if fast or late switching was enabled */
    if (((gateway.getBandwidth_down() != 0) && gw_mode == BATADV_GW_MODE_CLIENT) && (gw_sel_class > 2))
    {
        batadv_gw_check_selection(orig);
    }
}
void BatmanADVMain::batadv_gw_check_selection(/*struct batadv_priv *bat_priv,*/batadv_orig_node *orig_node)
{
    batadv_neigh_ifinfo *router_orig_tq = NULL;
    batadv_neigh_ifinfo *router_gw_tq = NULL;
    batadv_orig_node *curr_gw_orig = NULL;
    if (selectedGw != NULL)
        curr_gw_orig = selectedGw->orig_node;
    int gw_tq_avg, orig_tq_avg;
    batadv_neigh_node *router_gw = NULL, *router_orig = NULL;
    if (curr_gw_orig == NULL)
        goto reselect;

    router_gw = batadv_orig_router_get(curr_gw_orig,BATADV_IF_DEFAULT);
    if (!router_gw)
        goto reselect;

    router_gw_tq = batadv_neigh_ifinfo_get(router_gw,BATADV_IF_DEFAULT);
    if (!router_gw_tq)
        goto reselect;

    /* this node already is the gateway */
    if (curr_gw_orig == orig_node)
        return;

    router_orig = batadv_orig_router_get(orig_node,BATADV_IF_DEFAULT);
    if (!router_orig)
        return;

    router_orig_tq = batadv_neigh_ifinfo_get(router_orig,BATADV_IF_DEFAULT);
    if (!router_orig_tq)
        return;

    gw_tq_avg = router_gw_tq->bat_iv.tq_avg;
    orig_tq_avg = router_orig_tq->bat_iv.tq_avg;
    /* the TQ value has to be better */
    if (orig_tq_avg < gw_tq_avg)
        return;
    /* if the routing class is greater than 3 the value tells us how much
     * greater the TQ value of the new gateway must be
     */
    if ((gw_sel_class > 3) && (orig_tq_avg - gw_tq_avg < gw_sel_class))
        return;

    reselect: batadv_gw_reselect();
    return;
}

/**
 * batadv_gw_out_of_range - check if the dhcp request destination is the best gw
 * @bat_priv: the bat priv with all the soft interface information
 * @skb: the outgoing packet
 *
 * Check if the skb is a DHCP request and if it is sent to the current best GW
 * server. Due to topology changes it may be the case that the GW server
 * previously selected is not the best one anymore.
 *
 * Returns true if the packet destination is unicast and it is not the best gw,
 * false otherwise.
 *
 * This call might reallocate skb data.
 * Must be invoked only when the DHCP packet is going TO a DHCP SERVER.
 */
bool BatmanADVMain::batadv_gw_out_of_range(Ieee80211DataFrameWithSNAP* frame)
{
    bool out_of_range = false;
    batadv_neigh_node *neigh_curr = NULL, *neigh_old = NULL;
    batadv_gw_node *gw_node = NULL;
    batadv_orig_node *orig_dst_node = NULL;
    batadv_neigh_ifinfo *curr_ifinfo, *old_ifinfo;
    unsigned int curr_tq_avg;

    orig_dst_node = batadv_transtable_search(&(frame->getTransmitterAddress()), &(frame->getReceiverAddress()), 0);

    if (!orig_dst_node)
        return out_of_range;

    gw_node = batadv_gw_node_get(orig_dst_node);
    if (!gw_node->bandwidth_down == 0)
        return orig_dst_node;
    switch (gw_mode)
    {
        case BATADV_GW_MODE_SERVER:
            /* If we are a GW then we are our best GW. We can artificially
             * set the tq towards ourself as the maximum value
             */
            curr_tq_avg = TQ_MAX;
            break;
        case BATADV_GW_MODE_CLIENT:
            /* packet is going to our gateway */
            if (selectedGw == NULL)
                return out_of_range;
            if (selectedGw->orig_node == orig_dst_node)
                return out_of_range;

            /* If the dhcp packet has been sent to a different gw,
             * we have to evaluate whether the old gw is still
             * reliable enough
             */
            neigh_curr = batadv_find_router(selectedGw->orig_node, NULL);
            if (!neigh_curr)
                return out_of_range;

            curr_ifinfo = batadv_neigh_ifinfo_get(neigh_curr,BATADV_IF_DEFAULT);
            if (!curr_ifinfo)
                return out_of_range;

            curr_tq_avg = curr_ifinfo->bat_iv.tq_avg;
            break;
        case BATADV_GW_MODE_OFF:
            return out_of_range;
            break;
        default:
            break;
    }
    neigh_old = batadv_find_router(orig_dst_node, NULL);
    if (!neigh_old)
        return out_of_range;
    old_ifinfo = batadv_neigh_ifinfo_get(neigh_old,BATADV_IF_DEFAULT);
    if (!old_ifinfo)
        return out_of_range;

    if ((curr_tq_avg - old_ifinfo->bat_iv.tq_avg) > BATADV_GW_THRESHOLD)
        out_of_range = true;

    return out_of_range;
}

batadv_orig_node* BatmanADVMain::batadv_gw_get_selected_orig()
{

    batadv_orig_node *orig_node = NULL;
    if (selectedGw == NULL)
        return orig_node;

    orig_node = selectedGw->orig_node;
    return orig_node;
}
