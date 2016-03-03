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

#include <statistics/GWStats.h>

GWStats::GWStats()
{
    // TODO Auto-generated constructor stub

}

GWStats::~GWStats()
{
    // TODO Auto-generated destructor stub
}

void BatmanADVMain::updateGwStats(batadv_gw_node *new_gw_node, batadv_gw_node *old)
{

    if (old == NULL)
        selectedGWNone--;
    if (new_gw_node == NULL)
    {
        selectedGWNone++;
        return;
    }
    if (batadv_compare_eth(new_gw_node->orig_node->orig.getMAC(), MACAddress("c0:ff:ee:ba:be:01")))
        selectedGW1++;

    if (batadv_compare_eth(new_gw_node->orig_node->orig.getMAC(), MACAddress("c0:ff:ee:ba:be:02")))
        selectedGW2++;

    if (batadv_compare_eth(new_gw_node->orig_node->orig.getMAC(), MACAddress("c0:ff:ee:ba:be:48")))
        selectedGW3++;

    if (batadv_compare_eth(new_gw_node->orig_node->orig.getMAC(), MACAddress("c0:ff:ee:ba:be:05")))
        selectedGW4++;

    if (batadv_compare_eth(new_gw_node->orig_node->orig.getMAC(), MACAddress("c0:ff:ee:ba:be:04")))
        selectedGW5++;

    if (batadv_compare_eth(new_gw_node->orig_node->orig.getMAC(), MACAddress("c0:ff:ee:ba:be:a7")))
        selectedGW6++;

    if (batadv_compare_eth(new_gw_node->orig_node->orig.getMAC(), MACAddress("c0:ff:ee:ba:be:63")))
        selectedGW7++;

    if (batadv_compare_eth(new_gw_node->orig_node->orig.getMAC(), MACAddress("c0:ff:ee:ba:be:06")))
        selectedGW8++;
//------------
    if (old == NULL)
        return;

    if (batadv_compare_eth(old->orig_node->orig.getMAC(), MACAddress("c0:ff:ee:ba:be:01"))&&selectedGW1>0)
        selectedGW1--;

    if (batadv_compare_eth(old->orig_node->orig.getMAC(), MACAddress("c0:ff:ee:ba:be:02"))&&selectedGW2>0)
        selectedGW2--;

    if (batadv_compare_eth(old->orig_node->orig.getMAC(), MACAddress("c0:ff:ee:ba:be:48"))&&selectedGW3>0)
        selectedGW3--;

    if (batadv_compare_eth(old->orig_node->orig.getMAC(), MACAddress("c0:ff:ee:ba:be:05"))&&selectedGW4>0)
        selectedGW4--;

    if (batadv_compare_eth(old->orig_node->orig.getMAC(), MACAddress("c0:ff:ee:ba:be:04"))&&selectedGW5>0)
        selectedGW5--;

    if (batadv_compare_eth(old->orig_node->orig.getMAC(), MACAddress("c0:ff:ee:ba:be:a7"))&&selectedGW6>0)
        selectedGW6--;

    if (batadv_compare_eth(old->orig_node->orig.getMAC(), MACAddress("c0:ff:ee:ba:be:63"))&&selectedGW7>0)
        selectedGW7--;

    if (batadv_compare_eth(old->orig_node->orig.getMAC(), MACAddress("c0:ff:ee:ba:be:06"))&&selectedGW8>0)
        selectedGW8--;

}
