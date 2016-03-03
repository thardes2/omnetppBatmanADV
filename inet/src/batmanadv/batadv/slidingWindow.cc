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

#include <slidingWindow.h>
#include "BatmanADVMain.h"

slidingWindow::slidingWindow()
{
}

slidingWindow::~slidingWindow()
{
}

int BatmanADVMain::batadv_iv_window_CheckSize()
{
    EV << "Node: " << nodenr << ": Size of sliding window is now " << ogmWindow.size() << std::endl;
    return ogmWindow.size();
    return 0;
}
void BatmanADVMain::batadv_iv_window_shift_left(long n)
{

}
int BatmanADVMain::batadv_iv_window_packetCount(MACAddress mac)
{
    //if (batadv_iv_window_CheckSize() < BATADV_TQ_LOCAL_WINDOW_SIZE)//There's is still same space in the sliding window
    int packetCount = 0;
    for (unsigned int i = 0; i < ogmWindow.size(); i++)
    {
        slidingWindowEntry tmp = ogmWindow[i];
        MACAddress macTbl = tmp.addr;
        if (batadv_compare_eth(macTbl,mac)==0)
            packetCount++;
    }
    EV << "Node: " << nodenr << ": Sliding window packet count for -> " << mac.str() << " is " << packetCount << std::endl;

    return packetCount;
}

batadv_window_status BatmanADVMain::batadv_iv_window_add_entry(MACAddress mac, long seqno, int tq)
{
    slidingWindowEntry tmpEntry = slidingWindowEntry();
    tmpEntry.addr = mac;
    tmpEntry.seqno = seqno;
    tmpEntry.tq = tq;

    if (batadv_iv_window_CheckSize() < BATADV_TQ_LOCAL_WINDOW_SIZE) //There's is still same space in the sliding window
    {
        ogmWindow.push_back(tmpEntry);
        EV << "Node: " << nodenr << ": Added a new item to the sliding window -> " << mac.str() << " seqno: " << seqno<<". Size of sliding window is now " << ogmWindow.size() << std::endl;

        return seqnoNew;
    }
    else //Shift window and remove the first entry
    {
        EV << "Node: " << nodenr << ": Window size exceeded! Remove the first item" << std::endl;
        ogmWindow.erase(ogmWindow.begin());
        ogmWindow.push_back(tmpEntry);
        EV << "Node: " << nodenr << ": Added a new item to the sliding window -> " << mac.str() << " seqno: " << seqno<< ". Size of sliding window is now" << ogmWindow.size() << std::endl;
        return seqnoNew;
    }
    return seqnoNew;
}

void BatmanADVMain::batadv_iv_window_iterate()
{
    EV << "Node: " << nodenr << ": Iterate window!: START" << std::endl;
    for (unsigned int i = 0; i < ogmWindow.size(); i++)
    {
        slidingWindowEntry tmp = ogmWindow[i];
        EV << "Node: " << nodenr << ": MAC: " << tmp.addr.str() << " - Seqno: " << tmp.seqno << std::endl;
    }
    EV << "Node: " << nodenr << ": Iterate window!: DONE " << std::endl;
}

void BatmanADVMain::batadv_iv_window_create()
{
//ogmWindow = new slidingWindowObject();
}
/**
 * batadv_sliding_window_avg - compute the average of all non-zero values stored
 * in the given ring buffer
 *
 * Returns computed average value.
 */
int BatmanADVMain::batadv_sliding_window_avg()
{
    unsigned int i = 0, sum = 0;
    if (ogmWindow.size())
        return 0;
    while (i < ogmWindow.size())
    {
        sum += ogmWindow[i].tq;
        i++;
    }
    return (int) sum / ogmWindow.size();
}

void BatmanADVMain::batadv_sliding_window_set_tq(MACAddress addr, int tq_value)
{
    unsigned int i = 0;
    if (ogmWindow.size())
        return;
    while (i < ogmWindow.size())
    {
        if (ogmWindow[i].addr == addr)
        {
            ogmWindow[i].tq = tq_value;
            break;
        }
    }
}

bool BatmanADVMain::batadv_sliding_window_is_dup(long diff)
{
    slidingWindowEntry e;
   //batadv_iv_window_iterate();
    for (unsigned int i = 0; i < ogmWindow.size(); i++)
    {
        e = ogmWindow[i];
        if (e.seqno == diff)
            return true;
    }
    return false;
}
