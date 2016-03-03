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

#include <batmanadv/batadv/bitarray.h>
#include "BatmanADVMain.h"

bitarray::bitarray()
{
}

bitarray::~bitarray()
{
}
/*
 * Returns 1 if the corresponding bit in the given seq_bits indicates true
 * and curr_seqno is within range of last_seqno. Otherwise returns 0.
 */
int BatmanADVMain::batadv_test_bit( /*long seq_bits,*/long last_seqno, long curr_seqno)
{
    /*long diff;

    diff = last_seqno - curr_seqno;
    if (diff < 0 || diff >= BATADV_TQ_LOCAL_WINDOW_SIZE)
     return 0;*/
    //TODO
    bool dupStat = batadv_sliding_window_is_dup(curr_seqno);
    if (dupStat)
        return 1;
    else
        return 0;
    //return 1; // test_bit(diff, seq_bits) != 0;
}
/* turn corresponding bit on, so we can remember that we got the packet */
void BatmanADVMain::batadv_set_bit(unsigned long *seq_bits, long n)
{
    /* if too old, just drop it */
    if (n < 0 || n >= BATADV_TQ_LOCAL_WINDOW_SIZE)
        return;

    //TODO: set_bit NOT THERE
    //set_bit(n, seq_bits); /* turn the position on */
}
/* receive and process one packet within the sequence number window.
 *
 * returns:
 *  1 if the window was moved (either new or very old)
 *  0 if the window was not moved/shifted.
 */
int BatmanADVMain::batadv_bit_get_packet(unsigned long *seq_bits, long seq_num_diff, int set_mark)
{
    /* sequence number is slightly older. We already got a sequence number
     * higher than this one, so we just mark it.
     */
    if (seq_num_diff <= 0 && seq_num_diff > -BATADV_TQ_LOCAL_WINDOW_SIZE)
    {
        if (set_mark)
            batadv_set_bit(seq_bits, -seq_num_diff);
        return 0;
    }
}
