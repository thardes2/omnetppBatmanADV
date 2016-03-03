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

#include <Originator.h>
#include <BatmanADVMain.h>

batadv_neigh_node* BatmanADVMain::batadv_iv_ogm_neigh_new(MACAddress neigh_addr, batadv_orig_node *orig_node, batadv_orig_node *orig_neigh, batadv_hard_iface *if_incoming)
{
    batadv_neigh_node *neigh_node = NULL, *tmp_neigh_node = NULL;
    neigh_node = batadv_neigh_node_new(neigh_addr, orig_node, if_incoming);
    if (!neigh_node)
        return neigh_node;
    neigh_node->orig_node = orig_neigh;
    neigh_node->if_incoming = if_incoming;
    neigh_node->ifNameIncoming = if_incoming->name;
    neigh_node->refcount = 0;
    neigh_node->last_seen = simTime();
    neigh_node->ifinfo_list = std::vector<batadv_neigh_ifinfo *>();
    tmp_neigh_node = batadv_neigh_node_get(orig_node, neigh_addr, if_incoming);
    if (!tmp_neigh_node)
    {
        orig_node->neigh_list.push_back(neigh_node);
        EV << "Node: " << nodenr << ": Creating new neighbor " << neigh_addr.str() << " for orig_node " << orig_node->orig.getMAC().str() << std::endl;
    }
    else
    {
        neigh_node = tmp_neigh_node;
    }

    return neigh_node;
}

/**
 * batadv_neigh_node_get - retrieve a neighbour from the list
 * @orig_node: originator which the neighbour belongs to
 * @hard_iface: the interface where this neighbour is connected to
 * @addr: the address of the neighbour
 *
 * Looks for and possibly returns a neighbour belonging to this originator list
 * which is connected through the provided hard interface.
 * Returns NULL if the neighbour is not found.
 */
batadv_neigh_node* BatmanADVMain::batadv_neigh_node_get(batadv_orig_node *orig_node, MACAddress addr, batadv_hard_iface *hard_iface)
{
    batadv_neigh_node *tmp_neigh_node = NULL;
    MACAddress m = MACAddress(addr);

    for (unsigned int i = 0; i < orig_node->neigh_list.size(); i++)
    {
        tmp_neigh_node = orig_node->neigh_list[i];
        if (tmp_neigh_node->if_incoming->name != hard_iface->name)
            continue;
        if (batadv_compare_eth(tmp_neigh_node->addr.getMAC(), m))
            continue;

    }
    return tmp_neigh_node;
}

/**
 * batadv_neigh_node_new - create and init a new neigh_node object
 * @hard_iface: the interface where the neighbour is connected to
 * @neigh_addr: the mac address of the neighbour interface
 * @orig_node: originator object representing the neighbour
 *
 * Allocates a new neigh_node object and initialises all the generic fields.
 * Returns the new object or NULL on failure.
 */
batadv_neigh_node* BatmanADVMain::batadv_neigh_node_new(MACAddress neigh_addr, batadv_orig_node *orig_node, batadv_hard_iface *if_incoming)
{
    batadv_neigh_node *neigh_node = new batadv_neigh_node();
    neigh_node->orig_node = orig_node;
    neigh_node->addr = ManetAddress(neigh_addr);
    neigh_node->if_incoming = if_incoming;
    neigh_node->ifNameIncoming = if_incoming->name;
    neigh_node->refcount = 0;
    neigh_node->last_seen = simTime();
    neigh_node->ifinfo_list = std::vector<batadv_neigh_ifinfo *>();
    return neigh_node;
}

/**
 * batadv_orig_ifinfo_new - search and possibly create an orig_ifinfo object
 * @orig_node: the orig node to be queried
 * @if_outgoing: the interface for which the ifinfo should be acquired
 *
 * Returns NULL in case of failure or the orig_ifinfo object for the if_outgoing
 * interface otherwise. The object is created and added to the list
 * if it does not exist.
 *
 * The object is returned with refcounter increased by 1.
 */
batadv_orig_ifinfo* BatmanADVMain::batadv_orig_ifinfo_new(batadv_orig_node *orig_node, ManetAddress prevSender, batadv_hard_iface *if_outgoing)
{
    batadv_orig_ifinfo *orig_ifinfo = NULL;
    simtime_t reset_time;
    orig_ifinfo = batadv_orig_ifinfo_get(orig_node, if_outgoing);
    if (orig_ifinfo)
        return orig_ifinfo;
    orig_ifinfo = new batadv_orig_ifinfo();
    reset_time = simTime() - 1;
    reset_time -= BATADV_RESET_PROTECTION_MS;
    orig_ifinfo->batman_seqno_reset = reset_time;
    orig_ifinfo->router = NULL;
    if (if_outgoing != NULL)
    {
        orig_ifinfo->if_outgoing = if_outgoing;
    }
    orig_ifinfo->last_ttl = 0;
    orig_ifinfo->last_real_seqno = 0;
    /*int name = (if_outgoing->name);
     //name = 5;
     orig_ifinfo->ifName = name;*/
    orig_ifinfo->refcount = 0;
    EV << "Node " << nodenr << ": Created new ifinfo for " << orig_node->orig.getMAC().str() << std::endl;
    orig_node->ifinfo_list.push_back(orig_ifinfo);

    return orig_ifinfo;
}
batadv_orig_ifinfo* BatmanADVMain::batadv_orig_ifinfo_newNoOutgoing(batadv_orig_node *orig_node, ManetAddress prevSender)
{
    batadv_orig_ifinfo_new(orig_node, prevSender, NULL);
}
/**
 * batadv_neigh_ifinfo_get - find the ifinfo from an neigh_node
 * @neigh_node: the neigh node to be queried
 * @if_outgoing: the interface for which the ifinfo should be acquired
 *
 * The object is returned with refcounter increased by 1.
 *
 * Returns the requested neigh_ifinfo or NULL if not found
 */
batadv_neigh_ifinfo* BatmanADVMain::batadv_neigh_ifinfo_get(batadv_neigh_node *neigh, batadv_hard_iface *if_outgoing)
{
    batadv_neigh_ifinfo *neigh_ifinfo = NULL;
    for (unsigned int i = 0; i < neigh->ifinfo_list.size(); i++)
    {
        neigh_ifinfo = neigh->ifinfo_list[i];
        if (if_outgoing != NULL)
        {
            if (neigh_ifinfo->ifName != if_outgoing->name)
                continue;
        }
        break;
    }
    if (neigh_ifinfo == NULL)
        EV << "Node: " << nodenr << ": Couldn't find a neigh_ifinfo object for " << neigh->addr.getMAC().str() << std::endl;
    return neigh_ifinfo;
}
batadv_neigh_ifinfo* BatmanADVMain::batadv_neigh_ifinfo_getNoOutgoing(batadv_neigh_node *neigh)
{
    batadv_neigh_ifinfo *neigh_ifinfo = NULL;
    for (unsigned int i = 0; i < neigh->ifinfo_list.size(); i++)
    {
        neigh_ifinfo = neigh->ifinfo_list[i];
        break;
    }
    if (neigh_ifinfo == NULL)
        EV << "Node: " << nodenr << ": Couldn't find a neigh_ifinfo object for " << neigh->addr.getMAC().str() << std::endl;
    return neigh_ifinfo;
}

/**
 * batadv_neigh_ifinfo_new - search and possibly create an neigh_ifinfo object
 * @neigh_node: the neigh node to be queried
 * @if_outgoing: the interface for which the ifinfo should be acquired
 *
 * Returns NULL in case of failure or the neigh_ifinfo object for the
 * if_outgoing interface otherwise. The object is created and added to the list
 * if it does not exist.
 *
 * The object is returned with refcounter increased by 1.
 */
batadv_neigh_ifinfo* BatmanADVMain::batadv_neigh_ifinfo_new(batadv_neigh_node *neigh, batadv_hard_iface *if_outgoing)
{
    batadv_neigh_ifinfo *neigh_ifinfo = NULL;
    neigh_ifinfo = batadv_neigh_ifinfo_get(neigh, if_outgoing);
    if (neigh_ifinfo)
    {
        EV << "Node: " << nodenr << ": There was already an neigh_ifinfo object for " << neigh->addr.getMAC().str() << std::endl;
        return neigh_ifinfo;
    }
    neigh_ifinfo = new batadv_neigh_ifinfo();
    neigh_ifinfo->last_ttl = 0;
    neigh_ifinfo->bat_iv = batadv_neigh_ifinfo_bat_iv();
    neigh_ifinfo->bat_iv.real_packet_count = 0;
    neigh_ifinfo->bat_iv.tq_avg = 0;
    neigh_ifinfo->bat_iv.tq_index = 0;
    if (if_outgoing != NULL)
    {
        neigh_ifinfo->if_outgoing = if_outgoing;
        neigh_ifinfo->ifName = if_outgoing->name;
    }
    neigh_ifinfo->refcount = 0;

    neigh->ifinfo_list.push_back(neigh_ifinfo);
    EV << "Node: " << nodenr << ": Created and stored a neigh_ifinfo object for " << neigh->addr.getMAC().str() << std::endl;
    return neigh_ifinfo;
}
batadv_neigh_ifinfo* BatmanADVMain::batadv_neigh_ifinfo_newNoOutgoing(batadv_neigh_node *neigh)
{
    return batadv_neigh_ifinfo_new(neigh, NULL);
}

/**
 * batadv_orig_ifinfo_get - find the ifinfo from an orig_node
 * @orig_node: the orig node to be queried
 * @if_outgoing: the interface for which the ifinfo should be acquired
 *
 * Returns the requested orig_ifinfo or NULL if not found.
 *
 * The object is returned with refcounter increased by 1.
 */
batadv_orig_ifinfo* BatmanADVMain::batadv_orig_ifinfo_get(batadv_orig_node *orig_node, batadv_hard_iface * if_outgoing)
{
    batadv_orig_ifinfo *tmp = NULL, *orig_ifinfo = NULL;

    for (unsigned int list_pos = 0; list_pos < orig_node->ifinfo_list.size(); list_pos++)
    {
        tmp = orig_node->ifinfo_list.at(list_pos);

        if (if_outgoing != NULL)
        {
            if (tmp->ifName != if_outgoing->name)
                continue;
        }
        orig_ifinfo = tmp;
        break;
    }

    return orig_ifinfo;
}

/**
 * batadv_orig_router_get - router to the originator
 * @orig_node: the orig node for the router
 * @if_outgoing: the interface where the payload packet has been received or
 *  the OGM should be sent to
 *
 * Returns the neighbor which should be router for this orig_node/iface.
 * The object is returned with refcounter increased by 1.
 */
batadv_neigh_node* BatmanADVMain::batadv_orig_router_get(batadv_orig_node *orig_node, batadv_hard_iface *if_outgoing)
{
    batadv_orig_ifinfo *orig_ifinfo = NULL;
    batadv_neigh_node *router = NULL;
    for (unsigned int list_pos = 0; list_pos < orig_node->ifinfo_list.size(); list_pos++)
    {
        orig_ifinfo = orig_node->ifinfo_list[list_pos]; //.at(list_pos);
        if (orig_ifinfo->if_outgoing->name != if_outgoing->name)
            continue;
        router = orig_ifinfo->router;
    }
    return router;
}
batadv_neigh_node* BatmanADVMain::batadv_orig_router_getNoOutgoing(batadv_orig_node *orig_node)
{
    batadv_orig_ifinfo *orig_ifinfo = NULL;
    batadv_neigh_node *router = NULL;
    for (unsigned int list_pos = 0; list_pos < orig_node->ifinfo_list.size(); list_pos++)
    {
        orig_ifinfo = orig_node->ifinfo_list[list_pos]; //.at(list_pos);
        router = orig_ifinfo->router;
    }
    return router;
}
/**
 * batadv_orig_node_new - creates a new orig_node
 * @addr: the mac address of the originator
 *
 * Creates a new originator object and initialise all the generic fields.
 * The new object is not added to the originator list.
 * Returns the newly created object or NULL on failure.
 */
batadv_orig_node* BatmanADVMain::batadv_orig_node_new(MACAddress addr)
{
    EV << "Node: " << nodenr << ": Creating a new originator with mac " << addr.str() << std::endl;
    batadv_orig_node* orig_node = new batadv_orig_node();

    orig_node->bat_iv.bcast_own = 0;
    orig_node->bat_iv.bcast_own_sum = 0;
    orig_node->last_seen = simTime();
    orig_node->last_ttvn = 0;
    orig_node->last_bcast_seqno = 0;
    orig_node->orig = ManetAddress(addr);
    orig_node->tt_buff_len = 0;
    simtime_t reset_time = simTime() - 1 - BATADV_RESET_PROTECTION_MS;
    orig_node->bcast_seqno_reset = reset_time;

    orig_node->capa_initialized = 0;
    orig_node->ifinfo = NULL;
    orig_node->tt_buff_vector = std::vector<batadv_tvlv_tt_change>();
    orig_node->last_bcast_seqno = 0;
    orig_node->last_seen = simTime();
    orig_node->last_ttvn = 0;
    orig_node->refcount = 0;
    orig_node->tt_buff_len = 0;
    orig_node->ifinfo_list = std::vector<batadv_orig_ifinfo *>();
    orig_node->neigh_list = std::vector<batadv_neigh_node *>();
    orig_node->capabilities = 0;
    orig_node->tt_buff_len = 0;
    return orig_node;
}
/**
 * batadv_purge_orig_node - purges obsolete information from an orig_node
 * @bat_priv: the bat priv with all the soft interface information
 * @orig_node: orig node which is to be checked
 *
 * This function checks if the orig_node or substructures of it have become
 * obsolete, and purges this information if that's the case.
 *
 * Returns true if the orig_node is to be removed, false otherwise.
 */
bool BatmanADVMain::batadv_purge_orig_node(batadv_orig_node* orig_node)
{
    batadv_neigh_node *best_neigh_node = NULL;
    bool changed_ifinfo, changed_neigh;
    if (batadv_has_timed_out(orig_node->last_seen, /* 2 * */BATADV_PURGE_TIMEOUT))
    {
        EV << "Node: " << nodenr << ": Originator timeout: originator " << orig_node->orig.getMAC().str() << ", last_seen " << orig_node->last_seen << ", simetime: " << simTime() << std::endl;
        return true;
    }
    changed_ifinfo = batadv_purge_orig_ifinfo(orig_node);    //TODO: CHECK THIS
    changed_neigh = batadv_purge_orig_neighbors(orig_node);    //TODO: CHECK THIS
    if (!changed_ifinfo && !changed_neigh)
        return false;
    /* first for NULL ... */
    best_neigh_node = batadv_find_best_neighbor(orig_node, BATADV_IF_DEFAULT);
    batadv_update_route(orig_node, BATADV_IF_DEFAULTNP, best_neigh_node);

//TODO: MULTIPLE INTERFACES

    return false;

}

/**
 * batadv_purge_orig_neighbors - purges neighbors from originator
 * @bat_priv: the bat priv with all the soft interface information
 * @orig_node: orig node which is to be checked
 *
 * Returns true if any neighbor was purged, false otherwise
 */
bool BatmanADVMain::batadv_purge_orig_neighbors(batadv_orig_node* orig_node)
{
    batadv_neigh_node *neigh_node = NULL;
    bool neigh_purged = false;
    simtime_t last_seen;
    batadv_hard_iface *if_incoming;

    for (unsigned int i = 0; i < orig_node->neigh_list.size(); i++)
    {
        neigh_node = orig_node->neigh_list[i];
        last_seen = neigh_node->last_seen;
        if_incoming = neigh_node->if_incoming;
        if ((batadv_has_timed_out(last_seen, BATADV_PURGE_TIMEOUT)) || (if_incoming->if_status == BATADV_IF_INACTIVE) || (if_incoming->if_status == BATADV_IF_NOT_IN_USE) || (if_incoming->if_status == BATADV_IF_TO_BE_REMOVED))
        {
            if ((if_incoming->if_status == BATADV_IF_INACTIVE) || (if_incoming->if_status == BATADV_IF_NOT_IN_USE) || (if_incoming->if_status == BATADV_IF_TO_BE_REMOVED))
                EV << "Node: " << nodenr << ": neighbor purge: originator " << orig_node->orig.getMAC().str() << ", neighbor: " << neigh_node->addr.getMAC().str() << std::endl;

            else
                EV << "Node: " << nodenr << ": neighbor timeout: originator " << orig_node->orig.getMAC().str() << ", neighbor: " << neigh_node->addr.getMAC().str() << ", last_seen: " << last_seen << std::endl;
            neigh_purged = true;
            orig_node->neigh_list.erase(orig_node->neigh_list.begin() + i);
            delete neigh_node;
            neigh_node = NULL;
        }
        else
        {
            /* only necessary if not the whole neighbor is to be
             * deleted, but some interface has been removed.
             */
            batadv_purge_neigh_ifinfo(/*bat_priv, */neigh_node);
        }
    }
    return neigh_purged;
}

/**
 * batadv_purge_neigh_ifinfo - purge obsolete ifinfo entries from neighbor
 * @bat_priv: the bat priv with all the soft interface information
 * @neigh: orig node which is to be checked
 */
void BatmanADVMain::batadv_purge_neigh_ifinfo(batadv_neigh_node *neigh)
{
    batadv_neigh_ifinfo *neigh_ifinfo = NULL;
    batadv_hard_iface *if_outgoing = NULL;
    for (unsigned int i = 0; i < neigh->ifinfo_list.size(); i++)
    {
        neigh_ifinfo = neigh->ifinfo_list[i];
        if_outgoing = neigh_ifinfo->if_outgoing;
        /* always keep the default interface */
        if (if_outgoing == BATADV_IF_DEFAULT)
            continue;
        /* don't purge if the interface is not (going) down */
        if ((if_outgoing->if_status != BATADV_IF_INACTIVE) && (if_outgoing->if_status != BATADV_IF_NOT_IN_USE) && (if_outgoing->if_status != BATADV_IF_TO_BE_REMOVED))
            continue;

        EV << "Node: " << nodenr << ": neighbor/ifinfo purge: neighbor " << neigh->addr.getMAC().str() << std::endl;
        //delete neigh_ifinfo;    //TESTING
        neigh->ifinfo_list.erase(neigh->ifinfo_list.begin() + i);
        delete neigh_ifinfo;
        neigh_ifinfo = NULL;
    }
    return;
}

void BatmanADVMain::_batadv_purge_orig( /*batadv_priv *bat_priv*/)
{
    /*   batadv_orig_node *orig_node;
     typedef nodeList::iterator origMapit;
     int ogmPurged = 0;
     for (origMapit iterator = origMap.begin(); iterator != origMap.end(); iterator++)
     {
     orig_node = iterator->second;
     if (batadv_purge_orig_node(orig_node))
     {
     batadv_gw_node_delete(orig_node);    //If the originator gets removed, the gateway is also removed
     origMap.erase(iterator);
     ogmPurged++;
     EV << "Node: " << nodenr << ": Purging originator " << orig_node->orig.getMAC().str() << std::endl;
     //delete orig_node;    //Testing
     continue;
     }
     //batadv_frag_purge_orig(orig_node, batadv_frag_check_entry);//TODO: Not needed?
     }    //TODO: Runtime

     purgedOrigs.record(ogmPurged);

     batadv_gw_node_purge();
     batadv_gw_selection();*/

}
void BatmanADVMain::batadv_purge_orig()
{
    _batadv_purge_orig();

}
/**
 * batadv_purge_orig_ifinfo - purge obsolete ifinfo entries from originator
 * @bat_priv: the bat priv with all the soft interface information
 * @orig_node: orig node which is to be checked
 *
 * Returns true if any ifinfo entry was purged, false otherwise.
 */
bool BatmanADVMain::batadv_purge_orig_ifinfo(batadv_orig_node *orig_node)
{
    return false;
    batadv_orig_ifinfo *orig_ifinfo = NULL;
    bool ifinfo_purged = false;
    batadv_hard_iface *if_outgoing = NULL;
    for (unsigned int i = 0; i < orig_node->ifinfo_list.size(); i++)
    {
        orig_ifinfo = orig_node->ifinfo_list[i];
        //if_outgoing = orig_ifinfo->if_outgoing;

        /* always keep the default interface */
        if (if_outgoing == BATADV_IF_DEFAULT)
            continue;
        /* don't purge if the interface is not (going) down */
        if ((if_outgoing->if_status != BATADV_IF_INACTIVE) && (if_outgoing->if_status != BATADV_IF_NOT_IN_USE) && (if_outgoing->if_status != BATADV_IF_TO_BE_REMOVED))
            continue;
        EV << "Node: " << nodenr << ": router/ifinfo purge: originator " << orig_node->orig.getMAC().str() << std::endl;

        ifinfo_purged = true;
        delete orig_ifinfo;    //Testing
        orig_node->ifinfo_list.erase(orig_node->ifinfo_list.begin() + i);
    }
    return ifinfo_purged;
}
/**
 * batadv_find_best_neighbor - finds the best neighbor after purging
 * @bat_priv: the bat priv with all the soft interface information
 * @orig_node: orig node which is to be checked
 * @if_outgoing: the interface for which the metric should be compared
 *
 * Returns the current best neighbor, with refcount increased.
 */
batadv_neigh_node* BatmanADVMain::batadv_find_best_neighbor(batadv_orig_node *orig_node, batadv_hard_iface *if_outgoing)
{
    batadv_neigh_node *best = NULL, *neigh = NULL;
    for (unsigned int i = 0; i < orig_node->neigh_list.size(); i++)
    {
        neigh = orig_node->neigh_list[i];
        if (best && batadv_iv_ogm_neigh_cmp(neigh, meshvpn, best, meshvpn))    //TODO: Multiple interfaces
            continue;
        best = neigh;
    }
    return neigh;
}

/**
 * batadv_iv_ogm_neigh_cmp - compare the metrics of two neighbors
 * @neigh1: the first neighbor object of the comparison
 * @if_outgoing1: outgoing interface for the first neighbor
 * @neigh2: the second neighbor object of the comparison
 * @if_outgoing2: outgoing interface for the second neighbor
 *
 * Returns a value less, equal to or greater than 0 if the metric via neigh1 is
 * lower, the same as or higher than the metric via neigh2
 */
int BatmanADVMain::batadv_iv_ogm_neigh_cmp(batadv_neigh_node *neigh1, batadv_hard_iface *if_outgoing1, batadv_neigh_node *neigh2, batadv_hard_iface *if_outgoing2)
{
    batadv_neigh_ifinfo *neigh1_ifinfo = NULL, *neigh2_ifinfo = NULL;
    int tq1, tq2;
    int diff;
    neigh1_ifinfo = batadv_neigh_ifinfo_get(neigh1, if_outgoing1);
    neigh2_ifinfo = batadv_neigh_ifinfo_get(neigh2, if_outgoing2);
    if (!neigh1_ifinfo || !neigh2_ifinfo)
    {
        diff = 0;
        return diff;
    }
    tq1 = neigh1_ifinfo->bat_iv.tq_avg;
    tq2 = neigh2_ifinfo->bat_iv.tq_avg;
    diff = tq1 - tq2;
    return diff;
}
