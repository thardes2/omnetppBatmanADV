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

#include <translationTable.h>
#include <BatmanADVMain.h>

translationTable::translationTable()
{

}

translationTable::~translationTable()
{
}

/**
 * batadv_send_tt_request - send a TT Request message to a given node
 * @dst_orig_node: the destination of the message
 * @ttvn: the version number that the source of the message is looking for
 * @tt_vlan: pointer to the first tvlv VLAN object to request
 * @num_vlan: number of tvlv VLAN entries
 * @full_table: ask for the entire translation table if true, while only for the
 *  last TT diff otherwise
 */
bool BatmanADVMain::batadv_send_tt_request(batadv_orig_node *dst_orig_node, int ttvn, batadv_tvlv_tt_vlan_data *tt_vlan, long num_vlan, bool full_table)
{
    bool ret = false;
    //batadv_hard_iface *primary_if = meshvpn; //TODO: MULTIPLE INTERFACES
    batadv_tvlv_tt_data *tvlv_tt_data = NULL;
    batadv_tt_req_node *tt_req_node = NULL;
    //batadv_tvlv_tt_vlan_data *tt_vlan_req;

    /* The new tt_req will be issued only if I'm not waiting for a
     * reply from the same orig_node yet
     */
    tt_req_node = batadv_new_tt_req_node(dst_orig_node);
    if (!tt_req_node)
        return ret;
    tvlv_tt_data = new batadv_tvlv_tt_data();
    tvlv_tt_data->setFlags(BATADV_TT_REQUEST);
    tvlv_tt_data->setTtType(1);
    tvlv_tt_data->setTtvn(ttvn);
    tvlv_tt_data->setNum_vlan(num_vlan);

    //TODO: This is not need right?! -- CRC Shit and no VLAN used --> NO
    /*tt_vlan_req = (batadv_tvlv_tt_vlan_data *) (tvlv_tt_data + 1); //TODO: HÄ?
     for (unsigned int i = 0; i < num_vlan; i++)
     {
     tt_vlan_req->vid = tt_vlan->vid;
     tt_vlan_req->crc = tt_vlan->crc;

     tt_vlan_req++;
     tt_vlan++;
     }*/
    if (full_table)
//tvlv_tt_data->setFlags(tvlv_tt_data->getFlags() | BATADV_TT_FULL_TABLE);
        tvlv_tt_data->setFullTable(true);

    EV << "Node: " << nodenr << ": Sending TT_REQUEST to " << dst_orig_node->orig.getMAC().str() << std::endl;
    StatisticsCollector* stats = getGlobalStatisticsController();
    stats->add_tt_reqSend();

    batadv_tvlv_unicast_send(primary_if, dst_orig_node->orig, /*BATADV_TVLV_TT,*/1, tvlv_tt_data, FORWARD_MY_TT_REQUEST);

    if (ret && tt_req_node)
    {
        batadv_del_tt_req_node(tt_req_node);
    }
    return ret;

}

bool BatmanADVMain::batadv_del_tt_req_node(batadv_tt_req_node *tt_req_node)
{
    batadv_tt_req_node *tmp;
    for (unsigned int i = 0; i < bat_priv_tt.req_list.size(); i++)
    {
        tmp = bat_priv_tt.req_list[i];
        if (batadv_compare_eth(tmp->addr, tt_req_node->addr) == 0)
        {
            bat_priv_tt.req_list.erase(bat_priv_tt.req_list.begin() + i - 1);
            break;
        }
    }
}

/* returns the pointer to the new tt_req_node struct if no request
 * has already been issued for this orig_node, NULL otherwise
 */
batadv_tt_req_node* BatmanADVMain::batadv_new_tt_req_node(batadv_orig_node *orig_node)
{
    batadv_tt_req_node *tt_req_node = NULL;
    for (unsigned int i = 0; i < bat_priv_tt.req_list.size(); i++)
    {
        tt_req_node = bat_priv_tt.req_list[i];
        if (batadv_compare_eth(tt_req_node->addr, orig_node->orig.getMAC()) && batadv_has_timed_out(tt_req_node->issued_at, (simtime_t) (BATADV_TT_REQUEST_TIMEOUT / 1000)))
            return tt_req_node;
    }
    tt_req_node = new batadv_tt_req_node();
    tt_req_node->addr = orig_node->orig.getMAC();
    tt_req_node->issued_at = simTime();
    bat_priv_tt.req_list.push_back(tt_req_node);
    return tt_req_node;
}
/**
 * batadv_tt_local_purge_list - purge inactive tt local entries
 * @bat_priv: the bat priv with all the soft interface information
 * @head: pointer to the list containing the local tt entries
 * @timeout: parameter deciding whether a given tt local entry is considered
 *  inactive or not
 */
void BatmanADVMain::batadv_tt_local_purge_list(batadv_tt_local_entry *tt_local_entry, int timeout)
{
    //TODO: There is some error with the bitstuff
    /*if ((tt_local_entry->common.flags && BATADV_TT_CLIENT_NOPURGE)==BATADV_TT_CLIENT_NOPURGE)
     return;
     // entry already marked for deletion
     if ((tt_local_entry->common.flags && BATADV_TT_CLIENT_PENDING)==BATADV_TT_CLIENT_PENDING)
     return;
     */
    if (!batadv_has_timed_out(tt_local_entry->last_seen, timeout))
        return;
    batadv_tt_local_set_pending(tt_local_entry, BATADV_TT_CLIENT_DEL, "timed out");
}

void BatmanADVMain::batadv_tt_local_set_pending(batadv_tt_local_entry *tt_local_entry, long flags, const char *message)
{
    batadv_tt_local_event(tt_local_entry, flags);

    /* The local client has to be marked as "pending to be removed" but has
     * to be kept in the table in order to send it in a full table
     * response issued before the net ttvn increment (consistency check)
     */
    tt_local_entry->common.flags = tt_local_entry->common.flags | BATADV_TT_CLIENT_PENDING;

    EV << "Node: " << nodenr << ": Local tt entry (" << tt_local_entry->common.addr.str() << ") pending to be removed:" << message << std::endl;
}
/**
 * batadv_tt_len - compute length in bytes of given number of tt changes
 * @changes_num: number of tt changes
 *
 * Returns computed length in bytes.
 */
int BatmanADVMain::batadv_tt_len(int changes_num)
{
    return changes_num * sizeof(batadv_tvlv_tt_change);//TODO: Sizeissue
}
/**
 * batadv_tt_global_del_orig_entry - remove and free an orig_entry
 * @tt_global_entry: the global entry to remove the orig_entry from
 * @orig_entry: the orig entry to remove and free
 *
 * Remove an orig_entry from its list in the given tt_global_entry and
 * free this orig_entry afterwards.
 */
void BatmanADVMain::batadv_tt_global_del_orig(batadv_orig_node *orig_node, long vid, char *message)
{
    batadv_tt_global_entry *tt_global_entry;
    tt_global_entry = checkGlobalTranslationTableForExistence(orig_node->orig);
    if (!tt_global_entry)
        return;
    if (vid >= 0 && tt_global_entry->common.vid != vid)
        return;

    batadv_tt_global_del_orig_node(tt_global_entry, orig_node, message);
    EV << "Node: " << nodenr << ": Deleting global tt entry " << tt_global_entry->common.addr.str() << "(vid: " << vid << "): " << message << std::endl;
    //TODO: Delete it and make the common-entry a pointer
    orig_node->capa_initialized = orig_node->capa_initialized & BATADV_ORIG_CAPA_HAS_TT;
}

/**
 * batadv_is_my_client - check if a client is served by the local node
 * @bat_priv: the bat priv with all the soft interface information
 * @addr: the mac adress of the client to check
 * @vid: VLAN identifier
 *
 * Returns true if the client is served by this node, false otherwise.
 */
bool BatmanADVMain::batadv_is_my_client(MACAddress addr)
{
    batadv_tt_local_entry *tt_local_entry;
    tt_local_entry = checkLocalTranslationTableForExistence(ManetAddress(addr));
    if (tt_local_entry == NULL)
        return false;

    if ((tt_local_entry->common.flags & BATADV_TT_CLIENT_PENDING) || (tt_local_entry->common.flags & BATADV_TT_CLIENT_ROAM))
        return true;
    return false;
}

/**
 * batadv_tt_local_add - add a new client to the local table or update an
 *  existing client
 * @soft_iface: netdev struct of the mesh interface
 * @addr: the mac address of the client to add
 * @vid: VLAN identifier
 * @ifindex: index of the interface where the client is connected to (useful to
 *  identify wireless clients)
 * @mark: the value contained in the skb->mark field of the received packet (if
 *  any)
 *
 * Returns true if the client was successfully added, false otherwise.
 */
bool BatmanADVMain::batadv_tt_local_add(/*struct net_device *soft_iface,*/
MACAddress addr, batadv_hard_iface* device/*,unsigned short vid, int ifindex,*/)
{
    batadv_tt_local_entry *tt_local_local; //global object is also called tt_local, so tt_local_local is the object for this method
    batadv_tt_global_entry *tt_global_global = NULL;
    bool ret = false, roamed_back = false;
    //int remote_flags;

    tt_local_local = checkLocalTranslationTableForExistence(ManetAddress(addr));

    if (!addr.isMulticast())
    {
        //batadv_tt_orig_list_entry* tt_orig_list_entry;
        //tt_orig_list_entry = checkGlobalTranslationTableForExistence(ManetAddress(addr));
        tt_global_global = checkGlobalTranslationTableForExistence(ManetAddress(addr));
        //TODO: THIS IS BROKEN
        //tt_global = tt_orig_list_entry->tt_global_entry;
    }
    if (tt_local_local)        //this client is already known
    {
        tt_local_local->last_seen = simTime();
        if ((tt_local_local->common.flags & BATADV_TT_CLIENT_PENDING) == BATADV_TT_CLIENT_PENDING)        //TODO: CHECK
        {
            EV << "Node: " << nodenr << ": Re-adding pending client " << addr.str() << std::endl;
            /* whatever the reason why the PENDING flag was set,
             * this is a client which was enqueued to be removed in
             * this orig_interval. Since it popped up again, the
             * flag can be reset like it was never enqueued
             */
            tt_local_local->common.flags &= ~BATADV_TT_CLIENT_PENDING;
            goto add_event;
        }

        if ((tt_local_local->common.flags & BATADV_TT_CLIENT_ROAM) == BATADV_TT_CLIENT_ROAM)        //TODO: CHECK
        {
            EV << "Node: " << nodenr << ": Roaming client " << addr.str() << " came back to its original location" << std::endl;
            /* the ROAM flag is set because this client roamed away
             * and the node got a roaming_advertisement message. Now
             * that the client popped up again at its original
             * location such flag can be unset
             */
            tt_local_local->common.flags &= ~BATADV_TT_CLIENT_ROAM;
            roamed_back = true;
        }
        goto check_roaming;
    }
    /* Ignore the client if we cannot send it in a full table response. */
    //TODO: Is this needed here?
    /* table_size = batadv_tt_local_table_transmit_size(bat_priv); // LINE 562
     table_size += batadv_tt_len(1);*/
    tt_local_local = new batadv_tt_local_entry();
    EV << "Node: " << nodenr << ": Creating new local tt entry: " << addr.str() << " ttvn: " << bat_priv_tt.vn << std::endl;
    tt_local_local->common.addr = addr;
    tt_local_local->common.flags = BATADV_TT_CLIENT_NEW;
    tt_local_local->common.vid = vid;

    if (batadv_is_wifi_netdev(device))
        tt_local_local->common.flags = tt_local_local->common.flags bitor BATADV_TT_CLIENT_WIFI;        //BATADV_TT_CLIENT_WIFI;

    tt_local_local->last_seen = simTime();
    tt_local_local->common.added_at = tt_local_local->last_seen;

    /* the batman interface mac and multicast addresses should never be
     * purged
     */
    if (batadv_compare_eth(addr, ownAddress.getMAC()) == 0 || addr.isMulticast())
        tt_local_local->common.flags = tt_local_local->common.flags bitor BATADV_TT_CLIENT_NOPURGE;

    ttLocalAdd(ManetAddress(addr), tt_local_local);

    EV << "Node: " << nodenr << ": Local tt entry added. Size is " << tt_local.size() << std::endl;

    add_event: batadv_tt_local_event(tt_local_local, BATADV_NO_FLAGS); //Creates a new entry for the local tt to be published via ogm stuff

    check_roaming:
    /* Check whether it is a roaming, but don't do anything if the roaming
     * process has already been handled
     */
    if (tt_global_global && !(tt_global_global->common.flags & BATADV_TT_CLIENT_ROAM))
    {
        for (unsigned int i = 0; i < tt_global_global->orig_list.size(); i++) //TODO: UNTESTED
        {
            batadv_tt_orig_list_entry tmp_origNode = tt_global_global->orig_list[i];
            batadv_send_roam_adv(tt_global_global->common.addr, tt_global_global->common.vid, &tmp_origNode);
        }
        if (roamed_back)
        { //633
            tt_global_global = NULL;
        }
        else
        {
            /* The global entry has to be marked as ROAMING and
             * has to be kept for consistency purpose
             */
            tt_global_global->common.flags |= BATADV_TT_CLIENT_ROAM;
            tt_global_global->roam_at = simTime();
        }
    }

    /* store the current remote flags before altering them. This helps
     * understanding is flags are changing or not
     */
    //remote_flags = tt_local_local->common.flags & BATADV_TT_REMOTE_MASK;
    //TODO: MULTIPLE DEVICES
    if (batadv_is_wifi_netdev(device))
        tt_local_local->common.flags |= BATADV_TT_CLIENT_WIFI;
    else
        tt_local_local->common.flags &= ~BATADV_TT_CLIENT_WIFI;

    /* check the mark in the skb: if it's equal to the configured
     * isolation_mark, it means the packet is coming from an isolated
     * non-mesh client
     */
    ///TODO: AP-Isolation is disabled in cat /sys/class/net/bat0/mesh/ap_isolation
    tt_local_local->common.flags &= ~BATADV_TT_CLIENT_ISOLA;

    /*match_mark = (mark & bat_priv->isolation_mark_mask);
     if (bat_priv->isolation_mark_mask && match_mark == bat_priv->isolation_mark)
     tt_local->common.flags |= BATADV_TT_CLIENT_ISOLA;
     else
     tt_local->common.flags &= ~BATADV_TT_CLIENT_ISOLA;*/
    //NOT NEEDED?!
    /* if any "dynamic" flag has been modified, resend an ADD event for this
     * entry so that all the nodes can get the new flags
     */

    //TODO: Whatever
    /* int x =remote_flags ^ (tt_local_local->common.flags & BATADV_TT_REMOTE_MASK);
     if (x)
     batadv_tt_local_event(tt_local_local, BATADV_NO_FLAGS);*/

    ret = true;
    std::stringstream ss;
    ss << "Added node " << addr.str() << " to local tt";
    bubble(ss.str().c_str());

    return ret;
}
/**
 * batadv_tt_local_event - store a local TT event (ADD/DEL)
 * @bat_priv: the bat priv with all the soft interface information
 * @tt_local_entry: the TT entry involved in the event
 * @event_flags: flags to store in the event structure
 */
void BatmanADVMain::batadv_tt_local_event(batadv_tt_local_entry *tt_local_entry, int event_flags)
{
    batadv_tt_change_node *tt_change_node = new batadv_tt_change_node();
    batadv_tt_common_entry *common = &tt_local_entry->common;
    unsigned int lflags = common->flags | event_flags;
    bool del_op_requested, del_op_entry;
    bool event_removed = false;

    tt_change_node->change.setFlags(lflags);
    tt_change_node->change.setAddr(common->addr);
    tt_change_node->change.setVid(common->vid);

    del_op_requested = lflags && BATADV_TT_CLIENT_DEL;

    for (unsigned int i = 0; i < bat_priv_tt.changes_list.size(); i++)
    {
        //batadv_tvlv_tt_change *entry = bat_priv_tt.changes_list[i];
        batadv_tt_change_node* entry = bat_priv_tt.changes_list[i];
        if (batadv_compare_eth(entry->change.getAddr(), common->addr) != 0)
            continue;

        /* DEL+ADD in the same orig interval have no effect and can be
         * removed to avoid silly behaviour on the receiver side. The
         * other way around (ADD+DEL) can happen in case of roaming of
         * a client still in the NEW state. Roaming of NEW clients is
         * now possible due to automatically recognition of "temporary"
         * clients
         */
        del_op_entry = entry->change.getFlags() & BATADV_TT_CLIENT_DEL;

        if (!del_op_requested && del_op_entry)
            goto del;
        if (del_op_requested && !del_op_entry)
            goto del;
        /* this is a second add in the same originator interval. It
         * means that flags have been changed: update them!
         */
        if (!del_op_requested && !del_op_entry)
            entry->change.setFlags(lflags);

        continue;
        del: event_removed = true;
        delete entry;
        goto unlock;
    }
    bat_priv_tt.changes_list.push_back(tt_change_node);
    unlock: if (event_removed)
        bat_priv_tt.local_changes -= 1;
    else
        bat_priv_tt.local_changes += 1;
    return;

}

/**
 * batadv_send_roam_adv - send a roaming advertisement message
 * @bat_priv: the bat priv with all the soft interface information
 * @client: mac address of the roaming client
 * @vid: VLAN identifier
 * @orig_node: message destination
 *
 * Send a ROAMING_ADV message to the node which was previously serving this
 * client. This is done to inform the node that from now on all traffic destined
 * for this particular roamed client has to be forwarded to the sender of the
 * roaming message.
 */
void BatmanADVMain::batadv_send_roam_adv(/*batadv_priv *bat_priv,*/MACAddress client, unsigned short vid, batadv_tt_orig_list_entry *orig_node)
{
//TODO: LINE 3036
}
void BatmanADVMain::batadv_tt_global_free(/*struct batadv_priv *bat_priv,*/batadv_tt_global_entry *tt_global, const char *message)
{
    /*batadv_dbg(BATADV_DBG_TT, bat_priv,
     "Deleting global tt entry %pM (vid: %d): %s\n",
     tt_global->common.addr,
     BATADV_PRINT_VID(tt_global->common.vid), message);

     batadv_hash_remove(bat_priv->tt.global_hash, batadv_compare_tt,
     batadv_choose_tt, &tt_global->common);
     batadv_tt_global_entry_free_ref(tt_global);*/
}

/**
 * batadv_send_tt_response - send reply to tt request
 * @bat_priv: the bat priv with all the soft interface information
 * @tt_data: tt data containing the tt request information
 * @req_src: mac address of tt request sender
 * @req_dst: mac address of tt request recipient
 *
 * Returns true if tt request reply was sent, false otherwise.
 */
bool BatmanADVMain::batadv_send_tt_response(batadv_tvlv_tt_data *tt_data, MACAddress *req_src, MACAddress *req_dst)
{
    StatisticsCollector* stats = getGlobalStatisticsController();
    stats->add_tt_reqrcv();

    if (batadv_is_my_mac(req_dst) == 0)
        return batadv_send_my_tt_response(tt_data, req_src);
    return batadv_send_other_tt_response(tt_data, req_src, req_dst);
}
/**
 * batadv_send_my_tt_response - send reply to tt request concerning this node's translation table
 * @bat_priv: the bat priv with all the soft interface information
 * @tt_data: tt data containing the tt request information
 * @req_src: mac address of tt request sender
 *
 * Returns true if tt request reply was sent, false otherwise.
 */
bool BatmanADVMain::batadv_send_my_tt_response(batadv_tvlv_tt_data *tt_data, MACAddress *req_src)
{
    std::vector<batadv_tvlv_tt_change *> tt_change_vector;
    batadv_tvlv_tt_change *tt_change;
    int my_ttvn = bat_priv_tt.vn;
    int req_ttvn = tt_data->getTtvn();
    batadv_orig_node *orig_node;
    bool full_table;
    long tt_len;

    EV << "Node: " << nodenr << ": Received TT_REQUEST from " << req_src->str() << " for ttvn: " << tt_data->getTtvn() << std::endl;

    orig_node = checkOrigListForExistence(ManetAddress(MACAddress(req_src->getInt())));
    if (!orig_node)
        return true;

    //TODO: MULTIPLE INTERFACES
    //primary_if = batadv_primary_if_get_selected(bat_priv);
    // if (!primary_if)
    // goto out;
    // If the full table has been explicitly requested or the gap
    // is too big send the whole local translation table
    //
    //if (tt_data->getFlags() & BATADV_TT_FULL_TABLE || my_ttvn != req_ttvn || bat_priv_tt.last_changeset.size() == 0)
    if (tt_data->getFullTable() || my_ttvn != req_ttvn || bat_priv_tt.last_changeset.size() == 0)
        full_table = true;
    else
        full_table = false;
    /* TT fragmentation not available in BATMAN IV yet, so send as many
     * TT entries fit a single packet as possible only
     */
    if (!full_table)
    {
        tt_len = bat_priv_tt.last_changeset_len;
        batadv_tt_prepare_tvlv_local_data(&tt_data, &tt_change, &tt_len);
        if (!tt_len)    //There are no changes to send
            return true;
        // Copy the last orig_node's OGM buffer

        batadv_tvlv_tt_change* tmp_tt_change;

        std::vector<batadv_tvlv_tt_change> last_change_set2;
        for (unsigned int i = 0; i < bat_priv_tt.last_changeset.size(); i++)
        {
            tmp_tt_change = &(bat_priv_tt.last_changeset[i]);
            tmp_tt_change->setName("tt_change_" + i);
            last_change_set2.push_back(*tmp_tt_change);
            //tt_data->addPar("tt_change_" + i) = (void *) tmp_tt_change;
        }
        tt_data->setTtChanges(last_change_set2);
        //memcpy(tt_change, bat_priv->tt.last_changeset, bat_priv->tt.last_changeset_len);
    }
    else
    {
        req_ttvn = bat_priv_tt.vn;
        /* allocate the tvlv, put the tt_data and all the tt_vlan_data
         * in the initial part
         */
        tt_len = -1;
        batadv_tt_prepare_tvlv_local_data(&tt_data, &tt_change, &tt_len); //TODO: This is not implemented
        if (!tt_len)
            return true;

        // fill the rest of the tvlv with the real TT entries
        //TODO: Experimental
        batadv_tt_tvlv_generate(&tt_local, tt_data);

    }
    tt_data->setFlags(BATADV_TT_RESPONSE);
    tt_data->setTtType(0);
    tt_data->setTtvn(req_ttvn);

    if (full_table)
        //tt_data->setFlags(tt_data->getFlags() | BATADV_TT_FULL_TABLE);
        tt_data->setFullTable(true);
    EV << "Node: " << nodenr << ": Sending TT_RESPONSE to" << orig_node->orig << "  (ttvn: " << req_ttvn << ")" << std::endl;

    //This is useless right?
    //tt_data->addObject(tt_change);

    batadv_tvlv_unicast_send(primary_if, ManetAddress(MACAddress(req_src->getInt())), 1, tt_data, FORWARD_MY_TT_RESPONSE);    //BATADV_TVLV_TT
    return true;
}

/**
 * batadv_tt_tvlv_generate - fill the tvlv buff with the tt entries from the
 *  specified tt hash
 * @bat_priv: the bat priv with all the soft interface information
 * @hash: hash table containing the tt entries
 * @tt_len: expected tvlv tt data buffer length in number of bytes
 * @tvlv_buff: pointer to the buffer to fill with the TT data
 * @valid_cb: function to filter tt change entries
 * @cb_data: data passed to the filter function as argument
 */
void BatmanADVMain::batadv_tt_tvlv_generate(TranslationTableLocal* tt_local, batadv_tvlv_tt_data *tt_data)        //TODO: TTCHANGE --> Should be a list?
{
    //batadv_tt_local_entry* localEntry;
    batadv_tt_local_entry* tmp_localEntry;
    batadv_tvlv_tt_change* tt_change;
    unsigned int i = 0;
    for (TranslationTableLocal::iterator it = tt_local->begin(); it != tt_local->end(); ++it)
    {
        tmp_localEntry = it->second;
        tt_change = new batadv_tvlv_tt_change("tt_change_" + i);
        tt_change->setAddr(tmp_localEntry->common.addr);
        tt_change->setFlags(tmp_localEntry->common.flags);
        tt_change->setVid(tmp_localEntry->common.vid);
        tt_data->addObject(tt_change);
        i++;
    }
}

void BatmanADVMain::batadv_tt_tvlv_generate_global(TranslationTableGlobal global, batadv_tvlv_tt_data* tt_data)
{
    unsigned int i = 0;

    batadv_tvlv_tt_change* tmp_tt_change;
    batadv_tt_global_entry* tmpEntry;
    std::vector<batadv_tvlv_tt_change> last_change_set2;
    for (TranslationTableGlobal::iterator it = global.begin(); it != global.end(); ++it)
    {
        tmpEntry = it->second;
        tmp_tt_change = new batadv_tvlv_tt_change();
        tmp_tt_change->setFlags(tmpEntry->common.flags);
        tmp_tt_change->setAddr(tmpEntry->common.addr);
        tmp_tt_change->setVid(tmpEntry->common.vid);
        tmp_tt_change->setName("tt_change_" + i);
        last_change_set2.push_back(*tmp_tt_change);
        //tt_data->addPar("tt_change_" + i) = (void *) tmp_tt_change;
    }
    tt_data->setTtChanges(last_change_set2);
}

/**
 * batadv_send_other_tt_response - send reply to tt request concerning another
 *  node's translation table
 * @bat_priv: the bat priv with all the soft interface information
 * @tt_data: tt data containing the tt request information
 * @req_src: mac address of tt request sender
 * @req_dst: mac address of tt request recipient
 *
 * Returns true if tt request reply was sent, false otherwise.
 */
bool BatmanADVMain::batadv_send_other_tt_response(batadv_tvlv_tt_data *tt_data, MACAddress *req_src, MACAddress *req_dst)
{
    bool full_table = false;
    int orig_ttvn, req_ttvn;
    long tt_len;
    batadv_tvlv_tt_change *tt_change;
   // cArray tt_change_array;
    batadv_tvlv_tt_data *tvlv_tt_data = new batadv_tvlv_tt_data();
    batadv_orig_node *req_dst_orig_node;
    batadv_orig_node *res_dst_orig_node = NULL;
    EV << "Node: " << nodenr << ": Received TT_REQUEST from " << req_src->str() << " for ttvn: " << tt_data->getTtvn() << "(" << req_dst->str() << ")" << std::endl;
    // Let's get the orig node of the REAL destination
    req_dst_orig_node = checkOrigListForExistence(ManetAddress(MACAddress(req_dst->getInt())));
    if (!req_dst_orig_node)
        return false;
    res_dst_orig_node = checkOrigListForExistence(ManetAddress(MACAddress(req_src->getInt())));
    if (!res_dst_orig_node)
        return false;

    orig_ttvn = req_dst_orig_node->last_ttvn;
    req_ttvn = tt_data->getTtvn();

    // this node doesn't have the requested data
    if (orig_ttvn != req_ttvn)
        return false;
    // If the full table has been explicitly requested

    if (tt_data->getFullTable() || !(req_dst_orig_node->tt_buff_vector.size() == 0))        //TODO: Check wether size==0 is ok
        full_table = true;
    else
        full_table = false;

    // TT fragmentation hasn't been implemented yet, so send all
    // TT entries together
    //
    if (!full_table)
    {
        tt_len = req_dst_orig_node->tt_buff_len;
        //tt_change_array = req_dst_orig_node->tt_buff;

        batadv_tt_prepare_tvlv_global_data(req_dst_orig_node, &tvlv_tt_data, &tt_change, &tt_len);
    }
    else
    {
        batadv_tt_prepare_tvlv_global_data(req_dst_orig_node, &tvlv_tt_data, &tt_change, &tt_len);
        batadv_tt_tvlv_generate_global(tt_global, tvlv_tt_data);
    }
    //delete tt_change;    //Testing not needed
    tvlv_tt_data->setFlags(BATADV_TT_RESPONSE);
    tvlv_tt_data->setTtType(0);
    tvlv_tt_data->setTtvn(req_ttvn);

    if (full_table)
        tvlv_tt_data->setFullTable(true);
    EV << "Node: " << nodenr << ": Sending TT_RESPONSE " << res_dst_orig_node->orig.getMAC().str() << " for " << req_src->str() << " (ttvn: " << req_ttvn << ")" << std::endl;
    batadv_tvlv_unicast_send(meshvpn, ManetAddress(MACAddress(req_src->getInt())), 1, tvlv_tt_data, GENERATED_TT_RESPONSE);
    return true;
}
/**
 * batadv_tt_prepare_tvlv_local_data - allocate and prepare the TT TVLV for this node
 *
 * @tt_data: uninitialised pointer to the address of the TVLV buffer
 * @tt_change: uninitialised pointer to the address of the area where the TT
 *  changes can be stored
 * @tt_len: pointer to the length to reserve to the tt_change. if -1 this
 *  function reserves the amount of space needed to send the entire local TT
 *  table. In case of success the value is updated with the real amount of
 *  reserved bytes
 *
 * Allocate the needed amount of memory for the entire TT TVLV and write its
 * header made up by one tvlv_tt_data object and a series of tvlv_tt_vlan_data
 * objects, one per active VLAN.
 *
 * Return the size of the allocated buffer or 0 in case of failure.
 */
long BatmanADVMain::batadv_tt_prepare_tvlv_local_data(batadv_tvlv_tt_data **tt_data, batadv_tvlv_tt_change **tt_change, long *t_len)
{
    //Sets a "pointer in a pointer"--> Creates space for the required number of subentries

    long num_vlan = 0/*, num_entries = 0*/, tvlv_len;
    int change_offset = 0;
    //unsigned int *tt_change_ptr;

    change_offset = sizeof(**tt_data);
    change_offset += 0;//num_vlan * sizeof(batadv_tvlv_tt_vlan_data*);
    tvlv_len = *t_len;
    tvlv_len += change_offset;
    (*tt_data)->setFlags(BATADV_NO_FLAGS);
    (*tt_data)->setTtvn(bat_priv_tt.vn);
    (*tt_data)->setNum_vlan(0);

    //Set the new pointer accordingly to the memory address where the changes need to be stored later on
    /* tt_change_ptr = (unsigned int*) *tt_data + change_offset;
     *tt_change = (batadv_tvlv_tt_change *) tt_change_ptr;*/

    return tvlv_len;
}
long BatmanADVMain::batadv_tt_prepare_tvlv_global_data(batadv_orig_node *orig_node, batadv_tvlv_tt_data **tt_data, batadv_tvlv_tt_change **tt_change, long *tt_len)
{
    return 0;
}
/**
 * batadv_tt_update_orig - update global translation table with new tt information received via ogms
 * @bat_priv: the bat priv with all the soft interface information
 * @orig: the orig_node of the ogm
 * @tt_vlan: pointer to the first tvlv VLAN entry
 * @tt_num_vlan: number of tvlv VLAN entries
 * @tt_change: pointer to the first entry in the TT buffer
 * @tt_num_changes: number of tt changes inside the tt buffer
 * @ttvn: translation table version number of this changeset
 * @tt_crc: crc32 checksum of orig node's translation table
 */
void BatmanADVMain::batadv_tt_update_orig(batadv_orig_node *orig_node, batadv_tvlv_tt_vlan_data *tt_buff, long tt_num_vlan, std::vector<batadv_tvlv_tt_change> tt_changeList, long tt_num_changes, int ttvn)    //batadv_tvlv_tt_change *tt_change
{
    delete tt_buff;
    int orig_ttvn = orig_node->last_ttvn;
    bool full_table = true;
    bool has_tt_init;
    has_tt_init = orig_node->capa_initialized && BATADV_ORIG_CAPA_HAS_TT;    //& or &&
    // orig table not initialised AND first diff is in the OGM OR the ttvn
    // increased by one -> we can apply the attached changes
    if ((!has_tt_init && ttvn == 1) || ttvn - orig_ttvn == 1)
    {
        /* the OGM could not contain the changes due to their size or
         * because they have already been sent BATADV_TT_OGM_APPEND_MAX
         * times.
         * In this case send a tt request
         */
        if (!tt_num_changes)
        {
            full_table = false;
            goto request_table;
        }
        //cArray paras = tt_data->getParList();
        batadv_tt_update_changes(orig_node, ttvn, tt_changeList);

        /* Even if we received the precomputed crc with the OGM, we
         * prefer to recompute it to spot any possible inconsistency
         * in the global table
         */
        //batadv_tt_global_update_crc(bat_priv, orig_node);//TODO: We can believe without checking this?
    }
    else
    {
        /* if we missed more than one change or our tables are not
         * in sync anymore -> request fresh tt data
         */
        if (!has_tt_init || ttvn != orig_ttvn /*|| !batadv_tt_global_check_crc(orig_node, tt_vlan, tt_num_vlan)*/)
        {
            request_table:    //Done if there is no tt data attached --> this might be because of size problems -> OGM gets tooo big
            EV << "Node: " << nodenr << ": TT inconsistency for " << orig_node->orig << "Need to retrieve the correct information (ttvn: " << ttvn << " last_ttvn: " << orig_ttvn << "num_changes: " << tt_num_changes << std::endl;
            batadv_send_tt_request(orig_node, ttvn, 0, 0, full_table);
            return;
        }
    }

}
//void BatmanADVMain::batadv_tt_update_changes(batadv_orig_node *orig_node, int ttvn, cArray tt_change_param)
void BatmanADVMain::batadv_tt_update_changes(batadv_orig_node *orig_node, int ttvn, std::vector<batadv_tvlv_tt_change> tt_changeList)

{
    _batadv_tt_update_changes(orig_node, tt_changeList, ttvn);

    batadv_tt_save_orig_buffer(orig_node, tt_changeList);

    orig_node->last_ttvn = ttvn;

}

void BatmanADVMain::batadv_tt_save_orig_buffer(batadv_orig_node *orig_node, std::vector<batadv_tvlv_tt_change> tt_changeList)
{
    if (tt_changeList.size() > 0)
    {
        orig_node->tt_buff_vector = tt_changeList;
        orig_node->tt_buff_len = tt_changeList.size();
    }
}
//void BatmanADVMain::_batadv_tt_update_changes(batadv_orig_node *orig_node, cArray tt_change_param, int ttvn)
void BatmanADVMain::_batadv_tt_update_changes(batadv_orig_node *orig_node, std::vector<batadv_tvlv_tt_change> tt_changeList, int ttvn)
{
    bool roam;
    //int index = 0;
    //for (int i = 0; i < tt_change_param.size(); i++)
    for(unsigned int i =0; i < tt_changeList.size(); i++)
    {
        /*cMsgPar* msgPar = ((cMsgPar*) tt_change_param.remove(i));
        batadv_tvlv_tt_change* tt_change = (batadv_tvlv_tt_change *) msgPar->pointerValue();*/
        batadv_tvlv_tt_change tt_change =tt_changeList[i];
        //if (tt_change->getFlags() & BATADV_TT_CLIENT_DEL)
        if ((tt_change.getFlags() & BATADV_TT_CLIENT_DEL) == BATADV_TT_CLIENT_DEL)
        {
            roam = (tt_change.getFlags() & BATADV_TT_CLIENT_ROAM) == BATADV_TT_CLIENT_ROAM;
            batadv_tt_global_del(orig_node, &(tt_change.getAddr()), tt_change.getVid(), "tt removed by changes", roam);
        }
        else
        {
            bool globalAdd = batadv_tt_global_add(orig_node, &(tt_change.getAddr()), tt_change.getVid(), tt_change.getFlags(), ttvn);

            if (!globalAdd)
                /* In case of problem while storing a
                 * global_entry, we stop the updating
                 * procedure without committing the
                 * ttvn change. This will avoid to send
                 * corrupted data on tt_request
                 */
                return;
        }
    }

    orig_node->capa_initialized = /*orig_node->capa_initialized|*/BATADV_ORIG_CAPA_HAS_TT;    //TODO: Do it flagable
}

/**
 * batadv_tt_global_del - remove a client from the global table
 * @bat_priv: the bat priv with all the soft interface information
 * @orig_node: an originator serving this client
 * @addr: the mac address of the client
 * @vid: VLAN identifier
 * @message: a message explaining the reason for deleting the client to print
 *  for debugging purpose
 * @roaming: true if the deletion has been triggered by a roaming event
 */
void BatmanADVMain::batadv_tt_global_del(batadv_orig_node *orig_node, MACAddress *addr, unsigned short vid, const char *message, bool roaming)
{
    batadv_tt_global_entry *tt_global_entry;
    batadv_tt_local_entry *local_entry = NULL;
    //batadv_tt_orig_list_entry *tt_global_entry_list;
    //tt_global_entry_list = checkGlobalTranslationTableForExistence(ManetAddress(*addr));
    tt_global_entry = checkGlobalTranslationTableForExistence(ManetAddress(*addr));
    if (!roaming)
    {
        if (tt_global_entry)
        {
            TranslationTableGlobal::iterator it = tt_global.find(ManetAddress(*addr)); //Check whether this node is already part of the known hosts
            tt_global.erase(it); //TODO: Check this
            StatisticsCollector* stats = getGlobalStatisticsController();
            stats->addGlobalClientCountDel();
        }
        return;
    }
    /* if we are deleting a global entry due to a roam
     * event, there are two possibilities:
     * 1) the client roamed from node A to node B => if there
     *    is only one originator left for this client, we mark
     *    it with BATADV_TT_CLIENT_ROAM, we start a timer and we
     *    wait for node B to claim it. In case of timeout
     *    the entry is purged.
     *
     *    If there are other originators left, we directly delete
     *    the originator.
     * 2) the client roamed to us => we can directly delete
     *    the global entry, since it is useless now.
     */
    local_entry = checkLocalTranslationTableForExistence(ManetAddress(tt_global_entry->common.addr));
    if (local_entry)
    {/* local entry exists, case 2: client roamed to us. */

        batadv_tt_global_del_orig_list(tt_global_entry);
    }
    else
    { /* no local entry exists, case 1: check for roaming */
        batadv_tt_global_del_roaming(tt_global_entry, orig_node, message);
    }
    return;
}

/* If the client is to be deleted, we check if it is the last origantor entry
 * within tt_global entry. If yes, we set the BATADV_TT_CLIENT_ROAM flag and the
 * timer, otherwise we simply remove the originator scheduled for deletion.
 */
void BatmanADVMain::batadv_tt_global_del_roaming(batadv_tt_global_entry *tt_global_entry, batadv_orig_node *orig_node, const char *message)
{
    /* bool last_entry = true;
     tt_global_entry->orig_list
     batadv_tt_orig_list_entry *orig_entry;
     for (unsigned int i = 0; i < tt_global_entry->orig_list.size(); i++)
     {
     orig_entry = tt_global_entry->orig_list[i];
     if (orig_entry->orig_node != orig_node)
     {
     last_entry = false;
     break;
     }
     }
     if (last_entry) {
     // its the last one, mark for roaming.
     tt_global_entry->common.flags |= BATADV_TT_CLIENT_ROAM;
     tt_global_entry->roam_at = simTime();
     } else{
     // there is another entry, we can simply delete this one and can still use the other one.

     batadv_tt_global_del_orig_node(tt_global_entry, orig_node,
     message);
     }*/

}
/**
 * batadv_tt_global_del_orig_node - remove orig_node from a global tt entry
 * @bat_priv: the bat priv with all the soft interface information
 * @tt_global_entry: the global entry to remove the orig_node from
 * @orig_node: the originator announcing the client
 * @message: message to append to the log on deletion
 *
 * Remove the given orig_node and its according orig_entry from the given
 * global tt entry.
 */
void BatmanADVMain::batadv_tt_global_del_orig_node(batadv_tt_global_entry *tt_global_entry, batadv_orig_node *orig_node, const char *message)
{
    batadv_tt_orig_list_entry *orig_entry;

    for (unsigned int i = 0; i < tt_global_entry->orig_list.size(); i++)
    {
        orig_entry = &tt_global_entry->orig_list[i];
        if (orig_entry->orig_node == orig_node)
        { //TODO: Does this really work with a simple ==
            EV << "Node: " << nodenr << ": Deleting " << orig_entry->orig_node->orig.getMAC().str() << " from global tt entry " << tt_global_entry->common.addr.str() << std::endl;
            //batadv_tt_global_del_orig_entry(tt_global_entry,orig_entry);
            tt_global_entry->orig_list.erase(tt_global_entry->orig_list.begin() + i - 1);
        }
    }
}

/**
 * batadv_tt_global_del_orig_entry - remove and free an orig_entry
 * @tt_global_entry: the global entry to remove the orig_entry from
 * @orig_entry: the orig entry to remove and free
 *
 * Remove an orig_entry from its list in the given tt_global_entry and
 * free this orig_entry afterwards.
 */
void BatmanADVMain::batadv_tt_global_del_orig_entry(batadv_tt_global_entry *tt_global_entry, batadv_tt_orig_list_entry *orig_entry)
{
    /*batadv_tt_orig_list_entry *tmp;
     for (unsigned int i = 0; i < tt_global_entry->orig_list.size(); i++)
     {
     tmp = &tt_global_entry->orig_list[i];
     //TODO
     }*/

}

/**
 * batadv_tt_tvlv_unicast_handler_v1 - process incoming (unicast) tt tvlv
 *  container
 * @bat_priv: the bat priv with all the soft interface information
 * @src: mac address of tt tvlv sender
 * @dst: mac address of tt tvlv recipient
 * @tvlv_value: tvlv buffer containing the tt data
 * @tvlv_value_len: tvlv buffer length
 *
 * Returns NET_RX_DROP if the tt tvlv is to be re-routed, NET_RX_SUCCESS
 * otherwise.
 */
int BatmanADVMain::batadv_tt_tvlv_unicast_handler_v1(/*struct batadv_priv *bat_priv,*/
MACAddress *src, MACAddress *dst, batadv_tvlv_tt_data *tt_data/*,uint16_t tvlv_value_len*/)
{
    bool ret;
    EV << "Node: " << nodenr << ": batadv_tt_tvlv_unicast_handler_v1 reached!" << std::endl;
    char tt_flag;
    //switch (tt_data->getFlags() & BATADV_TT_DATA_TYPE_MASK)
    switch (tt_data->getTtType())
    {
        //case BATADV_TT_REQUEST: //1
        case 1: //BATADV_TT_REQUEST
            /* If this node cannot provide a TT response the tt_request is
             * forwarded
             */
            ret = batadv_send_tt_response(tt_data, src, dst);
            if (!ret)
            {
                //if (tt_data->getFlags() & BATADV_TT_FULL_TABLE)
                if (tt_data->getFullTable())
                    tt_flag = 'F';
                else
                    tt_flag = '.';
                EV << "Node: " << nodenr << ": Routing TT_REQUEST to" << dst->str() << "[" << tt_flag << "]" << std::endl;

                /* tvlv API will re-route the packet */
                return NET_RX_DROP;
            }
            break;
            //case BATADV_TT_RESPONSE:
        case 0: //BATADV_TT_RESPONSE
            if (batadv_is_my_mac(dst) == 0)
            {
                batadv_handle_tt_response(tt_data, src);        //tt_num_entries
                return NET_RX_SUCCESS;
            }

            //if (tt_data->getFlags() & BATADV_TT_FULL_TABLE)
            if (tt_data->getFullTable())
                tt_flag = 'F';
            else
                tt_flag = '.';
            EV << "Node: " << nodenr << ": Routing TT_REQUEST to" << dst->str() << "[" << tt_flag << "]" << std::endl;

            /* tvlv API will re-route the packet */
            return NET_RX_DROP;
            break;
        default:
            EV << "Nodenr" << nodenr << " unknown case in batadv_tt_tvlv_unicast_handler_v1" << std::endl;
    }
    return NET_RX_SUCCESS;
}
bool BatmanADVMain::batadv_tt_add_temporary_global_entry(batadv_orig_node *orig_node, MACAddress *addr, unsigned int vid)
{

    if (!batadv_tt_global_add(orig_node, addr, vid, BATADV_TT_CLIENT_TEMP, orig_node->last_ttvn))
        return true;
    EV << "Node: " << nodenr << ": Added temporary global client (addr: " << addr->str() << ", vid: " << vid << ", orig: " << orig_node->orig.getMAC().str() << std::endl;
    return false;
}

/**
 * batadv_tt_global_add - add a new TT global entry or update an existing one
 * @bat_priv: the bat priv with all the soft interface information
 * @orig_node: the originator announcing the client
 * @tt_addr: the mac address of the non-mesh client
 * @vid: VLAN identifier
 * @flags: TT flags that have to be set for this non-mesh client
 * @ttvn: the tt version number ever announcing this non-mesh client
 *
 * Add a new TT global entry for the given originator. If the entry already
 * exists add a new reference to the given originator (a global entry can have
 * references to multiple originators) and adjust the flags attribute to reflect
 * the function argument.
 * If a TT local entry exists for this non-mesh client remove it.
 *
 * The caller must hold orig_node refcount.
 *
 * Return true if the new entry has been added, false otherwise
 */
bool BatmanADVMain::batadv_tt_global_add(batadv_orig_node *orig_node, MACAddress *tt_addr, int vid, int flags, int ttvn)
{
    batadv_tt_global_entry *tt_global_entry;
    batadv_tt_local_entry *tt_local_entry;
    bool ret = false;

    //batadv_tt_common_entry *common;
    long local_flags;

    /* ignore global entries from backbone nodes */
    if (batadv_bla_is_backbone_gw_orig(/*bat_priv, */orig_node->orig, vid))
        return true;

    ManetAddress tt_addr_manet = ManetAddress(MACAddress(tt_addr->getInt()));

    //batadv_tt_orig_list_entry* orig_list_entry;

    //orig_list_entry = checkGlobalTranslationTableForExistence(tt_addr_manet);
    tt_global_entry = checkGlobalTranslationTableForExistence(tt_addr_manet);        //Search the non-mesh-client in the global table
    tt_local_entry = checkLocalTranslationTableForExistence(tt_addr_manet);        //Search the non-mesh-client in the local table

    /* if the node already has a local client for this entry, it has to wait
     * for a roaming advertisement instead of manually messing up the global
     * table
     */
    if ((flags & BATADV_TT_CLIENT_TEMP) && tt_local_entry && !(tt_local_entry->common.flags & BATADV_TT_CLIENT_NEW))
        return ret;

    if (!tt_global_entry)
    {
        tt_global_entry = new batadv_tt_global_entry();

        //common = &tt_global_entry->common;
        tt_global_entry->common.addr = *tt_addr;
        tt_global_entry->common.vid = vid;
        tt_global_entry->common.flags = flags;
        tt_global_entry->roam_at = 0;
        /* node must store current time in case of roaming. This is
         * needed to purge this entry out on timeout (if nobody claims
         * it)
         */

        if (flags & BATADV_TT_CLIENT_ROAM)
            tt_global_entry->roam_at = simTime();
        tt_global_entry->common.added_at = simTime();

        tt_global_entry->orig_list_count = 0;

        //goto out_remove;
    }
    else
    {
        //common = &tt_global_entry->common;
        /* If there is already a global entry, we can use this one for
         * our processing.
         * But if we are trying to add a temporary client then here are
         * two options at this point:
         * 1) the global client is not a temporary client: the global
         *    client has to be left as it is, temporary information
         *    should never override any already known client state
         * 2) the global client is a temporary client: purge the
         *    originator list and add the new one orig_entry
         */
        if (flags & BATADV_TT_CLIENT_TEMP)
        {
            if (!(tt_global_entry->common.flags & BATADV_TT_CLIENT_TEMP))
                return ret;
            if (checkGlobalTranslationTableForExistence(orig_node->orig))  //TODO: There is some other mechanism in the real code
                goto out_remove;
            batadv_tt_global_del_orig_list(tt_global_entry);
            goto add_orig_entry;
        }
        /* if the client was temporary added before receiving the first
         * OGM announcing it, we have to clear the TEMP flag
         */
        tt_global_entry->common.flags &= ~BATADV_TT_CLIENT_TEMP;
        /* the change can carry possible "attribute" flags like the
         * TT_CLIENT_WIFI, therefore they have to be copied in the
         * client entry
         */
        tt_global_entry->common.flags |= flags;
        /* If there is the BATADV_TT_CLIENT_ROAM flag set, there is only
         * one originator left in the list and we previously received a
         * delete + roaming change for this originator.
         *
         * We should first delete the old originator before adding the
         * new one.
         */
        if (tt_global_entry->common.flags & BATADV_TT_CLIENT_ROAM)
        {
            //  batadv_tt_global_del_orig_list(tt_global_entry);
            tt_global_entry->common.flags &= ~BATADV_TT_CLIENT_ROAM;
            tt_global_entry->roam_at = 0;
        }
    }
    add_orig_entry:
    /* add the new orig_entry (if needed) or update it */
    batadv_tt_global_orig_entry_add(tt_global_entry, orig_node, ttvn);
    EV << "Node: " << nodenr << ": Creating new global tt entry: " << tt_global_entry->common.addr.str() << "(vid:" << tt_global_entry->common.vid << ". via " << orig_node->orig.getMAC().str() << std::endl;
    ret = true;
    out_remove:
    /* Do not remove multicast addresses from the local hash on
     * global additions
     */
    if (tt_addr->isMulticast())
        return ret;
    /* remove address from local hash if present */
    local_flags = batadv_tt_local_remove(tt_addr, vid, "global tt received", flags & BATADV_TT_CLIENT_ROAM);
    //local_flags = batadv_tt_local_remove(bat_priv, tt_addr, vid, "global tt received", flags & BATADV_TT_CLIENT_ROAM);
    tt_global_entry->common.flags |= local_flags & BATADV_TT_CLIENT_WIFI;

    tt_global.insert(std::pair<ManetAddress, batadv_tt_global_entry*>(ManetAddress(MACAddress(tt_addr->getInt())), tt_global_entry));
    StatisticsCollector* stats = getGlobalStatisticsController();
    stats->addGlobalClientCountAdd(meshId);
    std::stringstream ss;
    ss << "Added node " << tt_addr->str() << " to global tt";
    bubble(ss.str().c_str());
    if (!(flags & BATADV_TT_CLIENT_ROAM))
        /* this is a normal global add. Therefore the client is not in a
         * roaming state anymore.
         */
        tt_global_entry->common.flags &= ~BATADV_TT_CLIENT_ROAM;
    return ret;
}

batadv_tt_orig_list_entry* BatmanADVMain::batadv_tt_global_orig_entry_find(batadv_tt_global_entry *entry, batadv_orig_node *orig_node)
{
    batadv_tt_orig_list_entry *tmp_orig_entry, *orig_entry = NULL;
    for (unsigned int i = 0; i < entry->orig_list.size(); i++)
    {
        tmp_orig_entry = &entry->orig_list[i];
        if (tmp_orig_entry->orig_node != orig_node)
            continue;
        orig_entry = tmp_orig_entry;
        break;
    }
    return orig_entry;
}

void BatmanADVMain::batadv_tt_global_orig_entry_add(batadv_tt_global_entry *tt_global_entry, batadv_orig_node *orig_node, int ttvn)
{
    batadv_tt_orig_list_entry *orig_entry;

    // orig_entry = checkGlobalTranslationTableForExistence(orig_node->orig);
    orig_entry = batadv_tt_global_orig_entry_find(tt_global_entry, orig_node);
    if (orig_entry)
    {
        /* refresh the ttvn: the current value could be a bogus one that
         * was added during a "temporary client detection"
         */
        orig_entry->ttvn = ttvn;
        return;
    }
    orig_entry = new batadv_tt_orig_list_entry();
    orig_entry->orig_node = orig_node;
    orig_entry->ttvn = ttvn;

    tt_global_entry->orig_list.push_back(*orig_entry);
//tt_global->orig_list.push_back(orig_entry);
    return;

}

/* deletes the orig list of a tt_global_entry */
void BatmanADVMain::batadv_tt_global_del_orig_list(batadv_tt_global_entry *tt_global_entry)
{

}

/**
 * batadv_bla_is_backbone_gw_orig
 * @bat_priv: the bat priv with all the soft interface information
 * @orig: originator mac address
 * @vid: VLAN identifier
 *
 * Check if the originator is a gateway for the VLAN identified by vid.
 *
 * Returns true if orig is a backbone for this vid, false otherwise.
 */
bool BatmanADVMain::batadv_bla_is_backbone_gw_orig(/*struct batadv_priv *bat_priv,*/ManetAddress orig, unsigned short vid)
{
//TODO: Do we have a real backbone according to batman? This is always simple non mesh wifi stuff
    return false;
}
/**
 * batadv_handle_tt_response - process incoming tt reply
 * @bat_priv: the bat priv with all the soft interface information
 * @tt_data: tt data containing the tt request information
 * @resp_src: mac address of tt reply sender
 * @num_entries: number of tt change entries appended to the tt data
 */
void BatmanADVMain::batadv_handle_tt_response(batadv_tvlv_tt_data *tt_data, MACAddress *resp_src)
{

    batadv_orig_node *orig_node = NULL;
    EV << "Node: " << nodenr << ": Received TT_RESPONSE from " << resp_src->str() << " for ttvn " << tt_data->getTtvn() << std::endl;

    orig_node = checkOrigListForExistence(ManetAddress(MACAddress(resp_src->getInt())));
    if (!orig_node)
        return;
    //cArray paras = tt_data->getParList();
    std::vector<batadv_tvlv_tt_change> tt_changeList = tt_data->getTtChanges();

    //if (tt_data->getFlags() & BATADV_TT_FULL_TABLE)
    if (tt_data->getFullTable())
    {
        //Should be also the array here
        batadv_tt_fill_gtable(tt_changeList, tt_data->getTtvn(), resp_src);
    }
    else
    {
        //batadv_tt_update_changes(orig_node, tt_data->getTtvn(), tt_change);
        batadv_tt_update_changes(orig_node, tt_data->getTtvn(), tt_changeList);
    }
//CRC for this orig_node and store it
//batadv_tt_global_update_crc(bat_priv, orig_node); //TODO: SKIPPED

    /**
     * Clear the list of unanswered requests
     */
//    std::vector<int> indices;
    /*for (std::vector<batadv_tt_req_node *>::iterator it = bat_priv_tt.req_list.begin(); it != bat_priv_tt.req_list.end(); ++it)
     {
     node = *it;
     if (!(batadv_compare_eth(node->addr, MACAddress(resp_src->getInt())) == 0))
     continue;
     bat_priv_tt.req_list.erase(it);
     }*/
    bat_priv_tt.req_list.clear();
}
void BatmanADVMain::batadv_tt_fill_gtable(std::vector<batadv_tvlv_tt_change> tt_changeList, int ttvn, MACAddress *resp_src)
{
    batadv_orig_node *orig_node;
    orig_node = checkOrigListForExistence(ManetAddress(*resp_src));

    /* Purge old entries as we got a complete new table */
    batadv_tt_global_del_orig(orig_node, -1, "Received full table");
    _batadv_tt_update_changes(orig_node, tt_changeList, ttvn);
    orig_node->tt_buff_len = 0;
    orig_node->tt_buff_vector.clear();
    orig_node->last_ttvn = ttvn;
}
/**
 * batadv_tt_local_client_is_roaming - tells whether the client is roaming
 * @bat_priv: the bat priv with all the soft interface information
 * @addr: the mac address of the local client to query
 * @vid: VLAN identifier
 *
 * Returns true if the local client is known to be roaming (it is not served by
 * this node anymore) or not. If yes, the client is still present in the table
 * to keep the latter consistent with the node TTVN
 */
bool BatmanADVMain::batadv_tt_local_client_is_roaming(/*struct batadv_priv *bat_priv,*/MACAddress *addr, unsigned short vid)
{
    return false;
}

/**
 * batadv_tt_local_commit_changes - commit all pending local tt changes which
 *  have been queued in the time since the last commit
 */
void BatmanADVMain::batadv_tt_local_commit_changes()
{
    batadv_tt_local_commit_changes_nolock();
}
/**
 * batadv_tt_local_commit_changes_nolock - commit all pending local tt changes
 *  which have been queued in the time since the last commit
 * @bat_priv: the bat priv with all the soft interface information
 *
 * Caller must hold tt->commit_lock.
 */
void BatmanADVMain::batadv_tt_local_commit_changes_nolock()
{
    /* Update multicast addresses in local translation table */
    batadv_mcast_mla_update();
    //TODO: bat_priv_tt.local_change should be set to 0 after this shit here
    if (bat_priv_tt.local_changes < 1)
    {
        if (!(bat_priv_tt.ogm_append_cnt - 1 != 0))    //batadv_atomic_dec_not_zero in real code
            batadv_tt_tvlv_container_update();
        return;
    }

    batadv_tt_local_set_flags(BATADV_TT_CLIENT_NEW, false, true);

    batadv_tt_local_purge_pending_clients();

    /* Increment the TTVN only once per OGM interval */
    bat_priv_tt.vn = bat_priv_tt.vn + 1;
    EV << "Node: " << nodenr << ": Local changes committed, updating to ttvn " << bat_priv_tt.vn << std::endl;

    /* reset the sending counter */
    bat_priv_tt.ogm_append_cnt = BATADV_TT_OGM_APPEND_MAX;
    batadv_tt_tvlv_container_update();
}
void BatmanADVMain::batadv_tt_purge()
{
    /** statistics and debugging **/
    batadv_tt_local_print_text();
    batadv_tt_global_print_text();

    batadv_tt_local_purge(BATADV_TT_LOCAL_TIMEOUT);
    batadv_tt_global_purge();
}

void BatmanADVMain::batadv_tt_global_purge()
{
    batadv_tt_global_entry *tt_global_entry = NULL;
    char *msg = NULL;
    typedef TranslationTableGlobal::iterator globalTTMapit;
    for (globalTTMapit iterator = tt_global.begin(); iterator != tt_global.end(); iterator++)
    {
        tt_global_entry = iterator->second;
        if (!batadv_tt_global_to_purge(tt_global_entry, &msg))
            continue;

        StatisticsCollector* stats = getGlobalStatisticsController();
        stats->addGlobalClientCountDel();
        tt_global.erase(iterator++);    //TODO: TEST
        EV << "Node: " << nodenr << ": Deleting global tt entry " << tt_global_entry->common.addr << ", " << msg << std::endl;
    }
}
bool BatmanADVMain::batadv_tt_global_to_purge(batadv_tt_global_entry *tt_global_entry, char **msg)
{
    bool purge = false;
    if ((tt_global_entry->common.flags & BATADV_TT_CLIENT_ROAM) && batadv_has_timed_out(tt_global_entry->roam_at, BATADV_TT_CLIENT_ROAM_TIMEOUT))
    {
        purge = true;
        *msg = "Roaming timeout";
    }
    if ((tt_global_entry->common.flags & BATADV_TT_CLIENT_TEMP) && batadv_has_timed_out(tt_global_entry->common.added_at, BATADV_TT_CLIENT_TEMP_TIMEOUT))
    {
        purge = true;
        *msg = "Temporary client timeout";
    }
    return purge;
}
/**
 * batadv_tt_local_purge - purge inactive tt local entries
 * @timeout: parameter deciding whether a given tt local entry is considered
 *  inactive or not
 */
void BatmanADVMain::batadv_tt_local_purge(long timeout)
{

    batadv_tt_local_entry *tt_local_local = NULL;
    typedef TranslationTableLocal::iterator localTTMapit;
    for (localTTMapit iterator = tt_local.begin(); iterator != tt_local.end(); iterator++)
    {
        tt_local_local = iterator->second;
        batadv_tt_local_purge_list(tt_local_local, timeout);
    }
}

/* Purge out all the tt local entries marked with BATADV_TT_CLIENT_PENDING */
void BatmanADVMain::batadv_tt_local_purge_pending_clients()
{
    batadv_tt_local_entry *tt_local_local;
    batadv_tt_common_entry tt_common;

    typedef TranslationTableLocal::iterator localTTMapit;
    for (localTTMapit iterator = tt_local.begin(); iterator != tt_local.end(); iterator++)    //Probably this will break if more than 1 client gets purged
    {
        tt_local_local = iterator->second;
        tt_common = tt_local_local->common;
        if (!((tt_common.flags & BATADV_TT_CLIENT_PENDING) == BATADV_TT_CLIENT_PENDING))    //TODO Check if this works!
        {
            continue;
        }
        EV << "Node: " << nodenr << ": Deleting local tt entry (" << tt_common.addr.str() << " vid: " << tt_common.vid << "): pending\n" << std::endl;
        tt_local.erase(iterator->first);

        StatisticsCollector* stats = getGlobalStatisticsController();
        stats->addLocalClientCountDel();
    }
}

long BatmanADVMain::batadv_tt_local_remove(MACAddress *addr, int vid, char* message, bool roaming)
{
    batadv_tt_local_entry *tt_local_entry = NULL;
    long lFlags, curr_flags = BATADV_NO_FLAGS;

    checkLocalTranslationTableForExistence(ManetAddress(*addr));
    if (tt_local_entry == NULL)
        return curr_flags;

    curr_flags = tt_local_entry->common.flags;

    lFlags = BATADV_TT_CLIENT_DEL;
    /* if this global entry addition is due to a roaming, the node has to
     * mark the local entry as "roamed" in order to correctly reroute
     * packets later
     */
    if (roaming)
    {
        lFlags = lFlags | BATADV_TT_CLIENT_ROAM;

        /* mark the local client as ROAMed */
        tt_local_entry->common.flags |= BATADV_TT_CLIENT_ROAM;
    }

    if (!(tt_local_entry->common.flags & BATADV_TT_CLIENT_NEW))
    {
        batadv_tt_local_set_pending(tt_local_entry, lFlags, message);
        return curr_flags;
    }
    /* if this client has been added right now, it is possible to
     * immediately purge it
     */
    batadv_tt_local_event(tt_local_entry, BATADV_TT_CLIENT_DEL);
    typedef TranslationTableLocal::iterator localTTMapit;
    for (localTTMapit iterator = tt_local.begin(); iterator != tt_local.end();)
    {
        if (batadv_compare_eth(tt_local_entry->common.addr, iterator->second->common.addr))
        {
            tt_local.erase(iterator->first);
            StatisticsCollector* stats = getGlobalStatisticsController();
            stats->addLocalClientCountDel();
        }
        else
            iterator++;
    }
    return curr_flags;
}
/**
 * batadv_tt_local_set_flags - set or unset the specified flags on the local
 *  table and possibly count them in the TT size
 * @bat_priv: the bat priv with all the soft interface information
 * @flags: the flag to switch
 * @enable: whether to set or unset the flag
 * @count: whether to increase the TT size by the number of changed entries
 */
void BatmanADVMain::batadv_tt_local_set_flags(long flags, bool enable, bool count)
{
    batadv_tt_local_entry *localEntry = NULL;
    batadv_tt_common_entry tt_common_entry;
    typedef TranslationTableLocal::iterator localTTMapit;
    for (localTTMapit iterator = tt_local.begin(); iterator != tt_local.end(); iterator++)
    {
        localEntry = iterator->second;
        tt_common_entry = localEntry->common;
        if (enable)
        {
            if ((tt_common_entry.flags & flags) == flags)
                continue;
            tt_common_entry.flags = tt_common_entry.flags | flags;
        }
        else
        {
            if (!(tt_common_entry.flags & flags))
                continue;
            tt_common_entry.flags = tt_common_entry.flags & flags;
        }
        if (!count)
            continue;
    }
}
/**
 * batadv_tt_tvlv_container_update - update the translation table tvlv container
 *  after local tt changes have been committed
 */
void BatmanADVMain::batadv_tt_tvlv_container_update()
{
    batadv_tvlv_tt_data *tt_data = new batadv_tvlv_tt_data();    //TODO: that "new" thing should be done in batadv_tt_prepare_tvlv_local_data

    batadv_tvlv_tt_change *tt_change = new batadv_tvlv_tt_change();
    std::vector<batadv_tvlv_tt_change> tt_change_vector;
    int tt_diff_entries_num = 0, tt_diff_entries_count = 0;
    long tvlv_len = 0, tt_diff_len = 0;

    tt_diff_entries_num = bat_priv_tt.local_changes;

    tt_diff_len = batadv_tt_len(tt_diff_entries_num);
    if (tt_diff_len > mtusize)
        tt_diff_len = 0;

    tvlv_len = batadv_tt_prepare_tvlv_local_data(&tt_data, &tt_change, &tt_diff_len);
    delete tt_change;    //testing -> Not needed
    if (tvlv_len == 0)
    {
        bat_priv_tvlv.ogm_tt = NULL;
        return;
    }

    tt_data->setFlags(BATADV_TT_OGM_DIFF);
    bat_priv_tt.local_changes = 0;

    batadv_tvlv_tt_change* tmp_tt_change;
    batadv_tt_change_node* change_node;
    std::vector<batadv_tvlv_tt_change> last_change_set;
    std::vector<batadv_tvlv_tt_change> last_change_set2;
    for (unsigned int i = 0; i < bat_priv_tt.changes_list.size(); i++)
    {
        change_node = bat_priv_tt.changes_list[i];
        tmp_tt_change = &(change_node->change);
        if (tt_diff_entries_count < tt_diff_entries_num)
        {
            tmp_tt_change->setName("tt_change_" + tt_diff_entries_count);
            last_change_set.push_back(*tmp_tt_change);
            //last_change_set2.push_back(tmp_tt_change);
           // tt_data->addPar("tt_change_" + tt_diff_entries_count) = (void *) tmp_tt_change;
            tt_diff_entries_count++;
        }
        tt_data->setTtChanges(last_change_set);
        delete change_node;//Testing
        bat_priv_tt.changes_list.erase(bat_priv_tt.changes_list.begin() + i);    //Experimental
        i--;
    }

    bat_priv_tt.last_changeset_len = 0;
    bat_priv_tt.last_changeset.clear();

    /* check whether this new OGM has no changes due to size problems */
    if (tt_diff_entries_count > 0)
    {
        bat_priv_tt.last_changeset = last_change_set;
    }
    if (tt_diff_entries_count > 0)
        bat_priv_tt.last_changeset_len = bat_priv_tt.last_changeset.size();

    bat_priv_tvlv.ogm_tt = tt_data;
}
/**
 * batadv_transtable_best_orig - Get best originator list entry from tt entry
 * @tt_global_entry: global translation table entry to be analyzed
 *
 * Returns best originator list entry or NULL on errors.
 */
batadv_tt_orig_list_entry* BatmanADVMain::batadv_transtable_best_orig(batadv_tt_global_entry *tt_global_entry)
{
    batadv_neigh_node *router;    //, *best_router = NULL;
    batadv_tt_orig_list_entry orig_entry, *best_entry = NULL;
    for (unsigned int i = 0; i < tt_global_entry->orig_list.size(); i++)
    {
        orig_entry = tt_global_entry->orig_list[i];
        router = batadv_orig_router_get(orig_entry.orig_node,BATADV_IF_DEFAULT);
        if (!router)
            continue;
        best_entry = &orig_entry;
        //best_router = router;
    }
    return best_entry;
}

/**
 * batadv_transtable_search - get the mesh destination for a given client
 * @src: mac address of the source client
 * @addr: mac address of the destination client
 * @vid: VLAN identifier
 *
 * Returns a pointer to the originator that was selected as destination in the
 * mesh for contacting the client 'addr', NULL otherwise.
 * In case of multiple originators serving the same client, the function returns
 * the best one (best in terms of metric towards the destination node).
 *
 * If the two clients are AP isolated the function returns NULL.
 */
batadv_orig_node * BatmanADVMain::batadv_transtable_search(MACAddress *src, MACAddress *addr, int vid)
{
    batadv_tt_local_entry *tt_local_entry = NULL;
    batadv_tt_global_entry *tt_global_entry = NULL;
    batadv_orig_node *orig_node = NULL;
    batadv_tt_orig_list_entry *best_entry;

    tt_local_entry = checkLocalTranslationTableForExistence(ManetAddress(*src));
    if (!tt_local_entry || ((tt_local_entry->common.flags & BATADV_TT_CLIENT_PENDING) == BATADV_TT_CLIENT_PENDING))
    {
        return orig_node;
    }
    tt_global_entry = checkGlobalTranslationTableForExistence(ManetAddress(*src));
    if (!tt_global_entry)
    {
        return orig_node;
    }
    best_entry = batadv_transtable_best_orig(tt_global_entry);

    /* found anything? */
    if (best_entry)
        orig_node = best_entry->orig_node;

    return orig_node;
}

