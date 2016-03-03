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

#include <multicast.h>

multicast::multicast()
{
    // TODO Auto-generated constructor stub

}

multicast::~multicast()
{
    // TODO Auto-generated destructor stub
}

/**
 * batadv_mcast_forw_mode - check on how to forward a multicast packet
 * @orig: an originator to be set to forward the skb to
 *
 * Returns the forwarding mode as enum batadv_forw_mode and in case of
 * BATADV_FORW_SINGLE set the orig to the single originator the skb
 * should be forwarded to.
 */
enum batadv_forw_mode BatmanADVMain::batadv_mcast_forw_mode(batadv_orig_node **orig)
{
    int ret;
    bool is_unsnoopable = false;
    ret = batadv_mcast_forw_mode_check(&is_unsnoopable);
        if (ret == -ENOMEM)
            return BATADV_FORW_NONE;
        else if (ret < 0)
            return BATADV_FORW_ALL;
}
/**
 * batadv_mcast_forw_mode_check - check for optimized forwarding potential
 * @bat_priv: the bat priv with all the soft interface information
 * @skb: the multicast frame to check
 * @is_unsnoopable: stores whether the destination is snoopable
 *
 * Checks whether the given multicast ethernet frame has the potential to be
 * forwarded with a mode more optimal than classic flooding.
 *
 * If so then returns 0. Otherwise -EINVAL is returned or -ENOMEM if we are out
 * of memory.
 */
 int BatmanADVMain::batadv_mcast_forw_mode_check(bool *is_unsnoopable)
{
     //TODO: IMPLEMENT
     return 0;
}
 /**
  * batadv_mcast_mla_update - update the own MLAs
  *
  * Updates the own multicast listener announcements in the translation
  * table as well as the own, announced multicast tvlv container.
  */
 void BatmanADVMain::batadv_mcast_mla_update()
 {
     //TODO
     /*int ret;

      if (!batadv_mcast_mla_tvlv_update(bat_priv))
      goto update;

      ret = batadv_mcast_mla_softif_get(soft_iface, &mcast_list);
      if (ret < 0)
      goto out;

      update:
      batadv_mcast_mla_tt_retract(bat_priv, &mcast_list);
      batadv_mcast_mla_tt_add(bat_priv, &mcast_list);

      out:
      batadv_mcast_mla_list_free(&mcast_list);*/
 }
