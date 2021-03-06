/*-
 * Copyright (c) 2005 John Bicket
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer,
 *    without modification.
 * 2. Redistributions in binary form must reproduce at minimum a disclaimer
 *    similar to the "NO WARRANTY" disclaimer below ("Disclaimer") and any
 *    redistribution must be conditioned upon including a substantially
 *    similar Disclaimer requirement for further binary redistribution.
 * 3. Neither the names of the above-listed copyright holders nor the names
 *    of any contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * Alternatively, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") version 2 as published by the Free
 * Software Foundation.
 *
 * NO WARRANTY
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF NONINFRINGEMENT, MERCHANTIBILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGES.
 *
 * $Id: sample.h 2394 2007-05-30 01:41:18Z mtaylor $
 */

/*
 * Defintions for the Atheros Wireless LAN controller driver.
 */
#ifndef _DEV_ATH_RATE_SAMPLE_H
#define _DEV_ATH_RATE_SAMPLE_H

/* per-device state */
struct sample_softc {
	struct ath_ratectrl arc; 	/* base state */
	int ath_smoothing_rate; 		/* ewma percentage (out of 100) */
	int ath_sample_rate; 		/* send a different bit-rate 1/X packets */
};
#define ATH_SOFTC_SAMPLE(sc)    ((struct sample_softc *)sc->sc_rc)

struct rate_info {
	int rate;
	int rix;
	int rateCode;
	int shortPreambleRateCode;
};


struct rate_stats {
	unsigned average_tx_time;
	int successive_failures;
	int tries;
	int total_packets;
	int packets_acked;
	unsigned perfect_tx_time; 	/* transmit time for 0 retries */
	int last_tx;
};


/*
 * for now, we track performance for three different packet
 * size buckets
 */
#define NUM_PACKET_SIZE_BINS 3
static int packet_size_bins[NUM_PACKET_SIZE_BINS] = {250, 1600, 3000};

/* per-node state */
struct sample_node {
	int static_rate_ndx;
	int num_rates;

	struct rate_info rates[IEEE80211_RATE_MAXSIZE];

	struct rate_stats stats[NUM_PACKET_SIZE_BINS][IEEE80211_RATE_MAXSIZE];
	int last_sample_ndx[NUM_PACKET_SIZE_BINS];

	int current_sample_ndx[NUM_PACKET_SIZE_BINS];
	int packets_sent[NUM_PACKET_SIZE_BINS];

	int current_rate[NUM_PACKET_SIZE_BINS];
	int packets_since_switch[NUM_PACKET_SIZE_BINS];
	unsigned jiffies_since_switch[NUM_PACKET_SIZE_BINS];

	int packets_since_sample[NUM_PACKET_SIZE_BINS];
	unsigned sample_tt[NUM_PACKET_SIZE_BINS];
};
#define	ATH_NODE_SAMPLE(an)	((struct sample_node *)&an[1])


#ifndef MIN
#define MIN(a,b)        ((a) < (b) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a,b)        ((a) > (b) ? (a) : (b))
#endif

#define WIFI_CW_MIN 31
#define WIFI_CW_MAX 1023

/*
 * Definitions for pulling the rate and trie counts from
 * a 5212 h/w descriptor. These Don't belong here; the
 * driver should record this information so the rate control
 * code doesn't go groveling around in the descriptor bits.
 */
#define	ds_ctl2	ds_hw[0]
#define	ds_ctl3	ds_hw[1]

/* TX ds_ctl3 */
#define	AR_XmitDataTries0	0x000f0000	/* series 0 max attempts */
#define	AR_XmitDataTries0_S	16
#define	AR_XmitDataTries1	0x00f00000	/* series 1 max attempts */
#define	AR_XmitDataTries1_S	20
#define	AR_XmitDataTries2	0x0f000000	/* series 2 max attempts */
#define	AR_XmitDataTries2_S	24
#define	AR_XmitDataTries3	0xf0000000	/* series 3 max attempts */
#define	AR_XmitDataTries3_S	28

/* TX ds_ctl3 */
#define	AR_XmitRate0		0x0000001f	/* series 0 tx rate */
#define	AR_XmitRate0_S		0
#define	AR_XmitRate1		0x000003e0	/* series 1 tx rate */
#define	AR_XmitRate1_S		5
#define	AR_XmitRate2		0x00007c00	/* series 2 tx rate */
#define	AR_XmitRate2_S		10
#define	AR_XmitRate3		0x000f8000	/* series 3 tx rate */
#define	AR_XmitRate3_S		15

#define MS(_v, _f)	(((_v) & (_f)) >> _f##_S)

#endif /* _DEV_ATH_RATE_SAMPLE_H */
