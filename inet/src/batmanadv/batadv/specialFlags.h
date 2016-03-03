/*
 * specialFlags.h
 *
 *  Created on: Jul 8, 2015
 *      Author: tobias
 */

#ifndef SPECIALFLAGS_H_
#define SPECIALFLAGS_H_
#define BIT0  0000000000000000000001
#define BIT1  0000000000000000000010
#define BIT2  0000000000000000000100
#define BIT3  0000000000000000001000
#define BIT4  0000000000000000010000
#define BIT5  0000000000000000100000
#define BIT6  0000000000000001000000
#define BIT7  0000000000000010000000
#define BIT8  0000000000000100000000
#define BIT9  0000000000001000000000
#define BIT10 0000000000010000000000
#define BIT11 0000000000100000000000
#define BIT12 0000000001000000000000
#define BIT13 0000000010000000000000
#define BIT14 0000000100000000000000
#define BIT15 0000001000000000000000
/*#define BIT0 0000000000000000000000
#define BIT1 0000000000000000000001
#define BIT2 0000000000000000000010
#define BIT3 0000000000000000000011
#define BIT4 0000000000000000000100
#define BIT5 0000000000000000000101
#define BIT6 0000000000000000000110
#define BIT7 0000000000000000000111
#define BIT8 0000000000000000001000
#define BIT9 0000000000000000001001
#define BIT10 0000000000000000001010
#define BIT11 0000000000000000001011
#define BIT12 0000000000000000001100
#define BIT13 0000000000000000001101
#define BIT14 0000000000000000001110
#define BIT15 0000000000000000001111*/

/**
 * enum batadv_tt_client_flags - TT client specific flags
 * @BATADV_TT_CLIENT_DEL: the client has to be deleted from the table
 * @BATADV_TT_CLIENT_ROAM: the client roamed to/from another node and the new
 *  update telling its new real location has not been received/sent yet
 * @BATADV_TT_CLIENT_WIFI: this client is connected through a wifi interface.
 *  This information is used by the "AP Isolation" feature
 * @BATADV_TT_CLIENT_ISOLA: this client is considered "isolated". This
 *  information is used by the Extended Isolation feature
 * @BATADV_TT_CLIENT_NOPURGE: this client should never be removed from the table
 * @BATADV_TT_CLIENT_NEW: this client has been added to the local table but has
 *  not been announced yet
 * @BATADV_TT_CLIENT_PENDING: this client is marked for removal but it is kept
 *  in the table for one more originator interval for consistency purposes
 * @BATADV_TT_CLIENT_TEMP: this global client has been detected to be part of
 *  the network but no nnode has already announced it
 *
 * Bits from 0 to 7 are called _remote flags_ because they are sent on the wire.
 * Bits from 8 to 15 are called _local flags_ because they are used for local
 * computations only.
 *
 * Bits from 4 to 7 - a subset of remote flags - are ensured to be in sync with
 * the other nodes in the network. To achieve this goal these flags are included
 * in the TT CRC computation.
 */
enum batadv_tt_client_flags
{
    BATADV_TT_CLIENT_DEL = BIT0, BATADV_TT_CLIENT_ROAM = BIT1, BATADV_TT_CLIENT_WIFI = BIT4, BATADV_TT_CLIENT_ISOLA = BIT5, BATADV_TT_CLIENT_NOPURGE = BIT8, BATADV_TT_CLIENT_NEW = BIT9, BATADV_TT_CLIENT_PENDING = BIT10, BATADV_TT_CLIENT_TEMP = BIT11,
};


#endif /* SPECIALFLAGS_H_ */
