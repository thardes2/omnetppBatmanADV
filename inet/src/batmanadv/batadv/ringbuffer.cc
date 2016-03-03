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

#include <ringbuffer.h>
#include <BatmanADVMain.h>
ringbuffer::ringbuffer()
{
}

ringbuffer::~ringbuffer()
{
}

/**
 * batadv_ring_buffer_set - update the ring buffer with the given value
 * @lq_recv: pointer to the ring buffer
 * @lq_index: index to store the value at
 * @value: value to store in the ring buffer
 */
void BatmanADVMain::batadv_ring_buffer_set(int lq_recv[], int *lq_index, int value)
{
    lq_recv[*lq_index] = value;
    *lq_index = (*lq_index + 1) % TQ_GLOBAL_WINDOW_SIZE;
}
/**
 * batadv_ring_buffer_set_vector - update the ring buffer with the given value
 * @lq_recv: pointer to the ring buffer
 * @lq_index: index to store the value at
 * @value: value to store in the ring buffer
 */
void BatmanADVMain::batadv_ring_buffer_set_vector(std::vector<int> *buffer,  int value)
{
    buffer->push_back(value);
}

/**
 * batadv_ring_buffer_set - compute the average of all non-zero values stored
 * in the given ring buffer
 * @lq_recv: pointer to the ring buffer
 *
 * Returns computed average value.
 */
int BatmanADVMain::batadv_ring_buffer_avg(int lq_recv[])
{
    int *ptr;
    long count = 0, i = 0, sum = 0;

    ptr = lq_recv;

    while (i < TQ_GLOBAL_WINDOW_SIZE) {
        if (*ptr != 0) {
            count++;
            sum += *ptr;
        }
        i++;
        ptr++;
    }

    if (count == 0)
        return 0;

    return (int)(sum / count);
}
