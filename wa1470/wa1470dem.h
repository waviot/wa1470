#ifndef WA1471
#ifndef _wa1470DEM_H
#define _wa1470DEM_H

#define DEM_NOISE_TICK  50      //50 ms
#define DEM_NOISE_AVER  10      //10 times

typedef enum
{
	DEM_MINUS90000			= 7,
	DEM_MINUS65000			= 6,
	DEM_MINUS40000			= 5,
	DEM_MINUS15000			= 4,
	DEM_PLUS15000			= 3,
	DEM_PLUS40000			= 2,
	DEM_PLUS65000			= 1,
	DEM_PLUS90000			= 0
}dem_hop_channels_t;

typedef enum {
	DEMOD_CRC = 0,
	DEMOD_1_2 = 1
} DEMODE_TYPE;

#pragma pack(push, 1)
typedef struct {
	uint8_t iter;
	uint8_t	flags;
	uint8_t payload[8];
	uint8_t mic[3];
	uint8_t crc[3];
} dem_protd_st;

typedef struct {
	dem_protd_st packet;
	uint8_t freq;
	uint8_t dummy;
	uint8_t rot;
	uint32_t rssi;
	uint8_t rssi_39_32;
	uint32_t noise;
	uint8_t crc_or_zigzag;
	uint8_t inverted;
	uint8_t i_or_q;
	uint8_t dummy2;
} dem_packet_st;
#pragma pack(pop)

typedef enum
{
	DBPSK_50_PROT_D		= 10,
	DBPSK_400_PROT_D	= 11,
	DBPSK_3200_PROT_D	= 12,
	DBPSK_25600_PROT_D	= 13,
	DBPSK_100H_PROT_D	= 18,
    DBPSK_UNDEFINED         = 100
}dem_bitrate_s;

typedef struct {
	dem_bitrate_s bitrate;
	int16_t rssi;
	uint8_t snr;
    uint32_t freq;
	//uint8_t	num_of_crc;
	//uint8_t	num_of_zigzag;
} dem_packet_info_st;

//----------------------------------------------------------
// DEMODULATOR REGs
//----------------------------------------------------------
#define DEM_RECEIVED_MES_BUF	0
#define DEM_CONTROL				0x20
#define DEM_RX_MODE				0x21
#define DEM_DET_TRESHOLD		0x22
#define DEM_NOSE_START_BIT		0x24
#define DEM_ALPHA_SHIFT			0x25
#define DEM_HOP_LENGTH			0x26
#define DEM_FFT_MSB				0x2C
#define DEM_PREAMBLE_ID			0x28
#define DEM_CRC_POLY			0x2E
#define DEM_HOP_TABLE			0x32
#define DEM_FFT_READ_BUF		0x80
#define DEM_GAIN				0x88

#define DEM_CONTROL_RESET		0x01
#define DEM_CONTROL_FFT_READY	0x40
#define DEM_CONTROL_IRQ_FLAG	0x80


void wa1470dem_init(uint32_t preambula);
void wa1470dem_rx_enable(_Bool en);
void wa1470dem_isr(void);
void wa1470dem_reset(void);
void wa1470dem_set_bitrate(dem_bitrate_s bitrate);
void wa1470dem_set_alpha(uint8_t noise_start_bit, uint8_t shift);
void wa1470dem_set_hop_table(uint8_t* hop);
void wa1470dem_set_hop_len(uint8_t hop_len);
void wa1470dem_set_crc_poly(uint8_t* crc);
void wa1470dem_set_preambule(uint8_t* preambule);
void wa1470dem_set_threshold(uint16_t SOFT_DETECT_THR);
void wa1470dem_set_freq(uint32_t freq);
float wa1470dem_get_rssi();
float wa1470dem_get_noise();
uint8_t wa1470dem_get_noise_calc_duration();
void wa1470dem_get_spectrum(uint8_t size, float* data);
int16_t wa1470dem_get_bitrate_sensitivity(dem_bitrate_s bitrate);

#endif
#endif //#ifndef WA1471