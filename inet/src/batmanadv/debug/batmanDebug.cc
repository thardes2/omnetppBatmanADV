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

#include <batmanDebug.h>

batmanDebug::batmanDebug()
{
}

batmanDebug::~batmanDebug()
{
}

void BatmanADVMain::debug_showOrigList()
{
    bool print = true;
    typedef nodeList::iterator origMapit;
    if (print)
    {
        std::string filename = "origList_export"; //_"<< mac <<".txt";
        std::ofstream out;
        out.open(filename.c_str(), std::ios_base::app);
        out << "\n\n" << simTime() << " Node: " << nodenr << ": Entry from MAC " << ownAddress.getMAC().str() << "\n";
        for (origMapit iterator = origMap.begin(); iterator != origMap.end(); iterator++)
        {
            EV << "Node: " << nodenr << ": orig: " << iterator->second->orig.getMAC().str() << std::endl;
            out << "Node: " << nodenr << ": orig: " << iterator->second->orig.getMAC().str() << "\n";
        }
        if (!selectedGw)
        {
            EV << "Node: " << nodenr << ": selected gw: NONE" << std::endl;
            out << "Node: " << nodenr << ": selected gw: NONE\n";
        }
        else
        {
            batadv_neigh_node* bNeigh = batadv_find_best_neighbor(selectedGw->orig_node, wlan0);
            if (bNeigh == NULL)
            {
                EV << "Node: " << nodenr << ": selected gw: " << selectedGw->orig_node->orig.getMAC().str() << " via NULL" << std::endl;
                out << "Node: " << nodenr << ": selected gw: " << selectedGw->orig_node->orig.getMAC().str() << " via NULL" << "\n";
            }
            else
            {
                EV << "Node: " << nodenr << ": selected gw: " << selectedGw->orig_node->orig.getMAC().str() << " via " << bNeigh->addr.getMAC().str() << std::endl;
                out << "Node: " << nodenr << ": selected gw: " << selectedGw->orig_node->orig.getMAC().str() << " via " << bNeigh->addr.getMAC().str() << "\n";
            }
        }
        out << "Node: " << nodenr << ": Registered Originators: " << origMap.size() << "\n";
        out << "Node: " << nodenr << ": Registered GWs: " << gwList.size() << "\n";
        for (unsigned int i = 0; i < gwList.size(); i++)
        {
            batadv_gw_node* gw_node = gwList[i];
            out << "Node: " << nodenr << ": gwNode: " << gw_node->orig_node->orig.getMAC().str() << "\n";
        }
        out.close();
    }
}
void BatmanADVMain::debug_local_tt()
{

}

void BatmanADVMain::debug_global_tt()
{
    bool print = false;
       typedef nodeList::iterator origMapit;
       EV << "Node: " << nodenr << ": -" << ownAddress.getMAC().str() << "- Showing originator list" << std::endl;
       if (print)
       {

           std::string filename = "origList_export"; //_"<< mac <<".txt";
           std::ofstream out;
           out.open(filename.c_str(), std::ios_base::app);
           out << "\n\n" << simTime() << " Node: " << nodenr << ": Entry from MAC " << ownAddress.getMAC().str() << "\n";
           for (origMapit iterator = origMap.begin(); iterator != origMap.end(); iterator++)
           {
               EV << "Node: " << nodenr << ": orig: " << iterator->second->orig.getMAC().str() << std::endl;
               out << "Node: " << nodenr << ": orig: " << iterator->second->orig.getMAC().str() << "\n";
           }
           if (!selectedGw)
           {
               EV << "Node: " << nodenr << ": selected gw: NONE" << std::endl;
               out << "Node: " << nodenr << ": selected gw: NONE\n";
           }
           else
           {
               batadv_neigh_node* bNeigh = batadv_find_best_neighbor(selectedGw->orig_node, wlan0);
               if (bNeigh == NULL)
               {
                   EV << "Node: " << nodenr << ": selected gw: " << selectedGw->orig_node->orig.getMAC().str() << " via NULL" << std::endl;
                   out << "Node: " << nodenr << ": selected gw: " << selectedGw->orig_node->orig.getMAC().str() << " via NULL" << "\n";
               }
               else
               {
                   EV << "Node: " << nodenr << ": selected gw: " << selectedGw->orig_node->orig.getMAC().str() << " via " << bNeigh->addr.getMAC().str() << std::endl;
                   out << "Node: " << nodenr << ": selected gw: " << selectedGw->orig_node->orig.getMAC().str() << " via " << bNeigh->addr.getMAC().str() << "\n";
               }

           }
           out << "Node: " << nodenr << ": Registered Originators: " << origMap.size() << "\n";
           out << "Node: " << nodenr << ": Registered GWs: " << gwList.size() << "\n";
           for (unsigned int i = 0; i < gwList.size(); i++)
           {
               batadv_gw_node* gw_node = gwList[i];
               out << "Node: " << nodenr << ": gwNode: " << gw_node->orig_node->orig.getMAC().str() << "\n";
           }
           out.close();
       }
}

/**
 * batadv_iv_ogm_orig_print - print the originator table
 * @seq: debugfs table seq_file struct
 * @if_outgoing: the outgoing interface for which this should be printed
 */
void BatmanADVMain::batadv_iv_ogm_orig_print(batadv_hard_iface *if_outgoing)
{

}
/**
 * batadv_iv_ogm_orig_print_neigh - print neighbors for the originator table
 * @orig_node: the orig_node for which the neighbors are printed
 * @if_outgoing: outgoing interface for these entries
 * @seq: debugfs table seq_file struct
 *
 * Must be called while holding an rcu lock.
 */
void BatmanADVMain::batadv_iv_ogm_orig_print_neigh(batadv_orig_node *orig_node, batadv_hard_iface *if_outgoing, std::ofstream& out)
{

}

void BatmanADVMain::batadv_tt_local_print_text()
{

}

void BatmanADVMain::batadv_tt_global_print_text()
{

}
/**
 * batadv_tt_global_print_entry - print all orig nodes who announce the address
 *  for this global entry

 * @tt_global_entry: global translation table entry to be printed
 * @out: seq_file struct
 *
 */
void BatmanADVMain::batadv_tt_global_print_entry(batadv_tt_global_entry *tt_global_entry, std::ofstream& out)
{

}
