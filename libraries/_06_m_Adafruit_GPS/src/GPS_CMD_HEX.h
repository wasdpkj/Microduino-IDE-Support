#ifndef __GPS_CMD_HEX_H__
#define __GPS_CMD_HEX_H__

#define HEX_SET_NMEA_UPDATE_1HZ {0xB5, 0x62, 0x06, 0x8A, 0x0A, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x00, 0x21, 0x30, 0xE8, 0x03, 0xD9, 0xCE}
#define HEX_SET_NMEA_UPDATE_2HZ {0xB5, 0x62, 0x06, 0x8A, 0x0A, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x00, 0x21, 0x30, 0xF4, 0x01, 0xE3, 0xE4}
#define HEX_SET_NMEA_UPDATE_5HZ {0xB5, 0x62, 0x06, 0x8A, 0x0A, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x00, 0x21, 0x30, 0xC8, 0x00, 0xB6, 0x8B}
#define HEX_SET_NMEA_UPDATE_10HZ {0xB5, 0x62, 0x06, 0x8A, 0x0A, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x00, 0x21, 0x30, 0x64, 0x00, 0x52, 0xC3}
#define HEX_SET_NMEA_UPDATE_15HZ {0xB5, 0x62, 0x06, 0x8A, 0x0A, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x00, 0x21, 0x30, 0x42, 0x00, 0x30, 0x7F}
#define HEX_SET_NMEA_UPDATE_20HZ {0xB5, 0x62, 0x06, 0x8A, 0x0A, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x00, 0x21, 0x30, 0x32, 0x00, 0x20, 0x5F}
#define HEX_SET_NMEA_UPDATE_25HZ {0xB5, 0x62, 0x06, 0x8A, 0x0A, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x00, 0x21, 0x30, 0x28, 0x00, 0x16, 0x4B} 

#define HEX_SET_NMEA_GPS_ENABLE         {0xB5, 0x62, 0x06, 0x8A, 0x09, 0x00, 0x01, 0x01, 0x00, 0x00, 0x1F, 0x00, 0x31, 0x10, 0x01, 0xFC, 0x89}
#define HEX_SET_NMEA_GPS_DISABLE        {0xB5, 0x62, 0x06, 0x8A, 0x09, 0x00, 0x01, 0x01, 0x00, 0x00, 0x1F, 0x00, 0x31, 0x10, 0x00, 0xFB, 0x88}
	    
#define HEX_SET_NMEA_SBAS_ENABLE        {0xB5, 0x62, 0x06, 0x8A, 0x09, 0x00, 0x01, 0x01, 0x00, 0x00, 0x20, 0x00, 0x31, 0x10, 0x01, 0xFD, 0x8E}
#define HEX_SET_NMEA_SBAS_DISABLE       {0xB5, 0x62, 0x06, 0x8A, 0x09, 0x00, 0x01, 0x01, 0x00, 0x00, 0x20, 0x00, 0x31, 0x10, 0x00, 0xFC, 0x8D}
	    
#define HEX_SET_NMEA_GALILEO_ENABLE     {0xB5, 0x62, 0x06, 0x8A, 0x09, 0x00, 0x01, 0x01, 0x00, 0x00, 0x21, 0x00, 0x31, 0x10, 0x01, 0xFE, 0x93}
#define HEX_SET_NMEA_GALILEO_DISABLE    {0xB5, 0x62, 0x06, 0x8A, 0x09, 0x00, 0x01, 0x01, 0x00, 0x00, 0x21, 0x00, 0x31, 0x10, 0x00, 0xFD, 0x92}
	    
#define HEX_SET_NMEA_BEIDOU_ENABLE      {0xB5, 0x62, 0x06, 0x8A, 0x09, 0x00, 0x01, 0x01, 0x00, 0x00, 0x22, 0x00, 0x31, 0x10, 0x01, 0xFF, 0x98}
#define HEX_SET_NMEA_BEIDOU_DISABLE     {0xB5, 0x62, 0x06, 0x8A, 0x09, 0x00, 0x01, 0x01, 0x00, 0x00, 0x22, 0x00, 0x31, 0x10, 0x00, 0xFE, 0x97}
	    
#define HEX_SET_NMEA_QZSS_ENABLE        {0xB5, 0x62, 0x06, 0x8A, 0x09, 0x00, 0x01, 0x01, 0x00, 0x00, 0x24, 0x00, 0x31, 0x10, 0x01, 0x01, 0xA2}
#define HEX_SET_NMEA_QZSS_DISABLE       {0xB5, 0x62, 0x06, 0x8A, 0x09, 0x00, 0x01, 0x01, 0x00, 0x00, 0x24, 0x00, 0x31, 0x10, 0x00, 0x00, 0xA1}
	    
#define HEX_SET_NMEA_GLONASS_ENABLE     {0xB5, 0x62, 0x06, 0x8A, 0x09, 0x00, 0x01, 0x01, 0x00, 0x00, 0x25, 0x00, 0x31, 0x10, 0x01, 0x02, 0xA7}
#define HEX_SET_NMEA_GLONASS_DISABLE    {0xB5, 0x62, 0x06, 0x8A, 0x09, 0x00, 0x01, 0x01, 0x00, 0x00, 0x25, 0x00, 0x31, 0x10, 0x00, 0x01, 0xA6}

#define HEX_SET_NMEA_SIGNAL_GPS_GAL_BDS_GLO     {0xB5, 0x62, 0x06, 0x8A, 0x22, 0x00, 0x01, 0x01, 0x00, 0x00, 0x1F, 0x00, 0x31, 0x10, 0x01, 0x20, 0x00, 0x31, 0x10, 0x00, 0x21, 0x00, 0x31, 0x10, 0x01, 0x22, 0x00, 0x31, 0x10, 0x01, 0x24, 0x00, 0x31, 0x10, 0x00, 0x25, 0x00, 0x31, 0x10, 0x01, 0x09, 0xC9}

#define HEX_SET_MSG_NMEA_ENABLE     {0xB5 , 0x62 , 0x06 , 0x8A , 0x09 , 0x00 , 0x01 , 0x01 , 0x00 , 0x00 , 0x02 , 0x00 , 0x74 , 0x10 , 0x01 , 0x22 , 0xC1}
#define HEX_SET_MSG_NMEA_DISABLE    {0xB5 , 0x62 , 0x06 , 0x8A , 0x09 , 0x00 , 0x01 , 0x01 , 0x00 , 0x00 , 0x02 , 0x00 , 0x74 , 0x10 , 0x00 , 0x21 , 0xC0}

#define HEX_SET_MSG_UBX_ENABLE     {0xB5, 0x62 , 0x06 , 0x8A , 0x09 , 0x00 , 0x01 , 0x01 , 0x00 , 0x00 , 0x01 , 0x00 , 0x74 , 0x10 , 0x01 , 0x21 , 0xBC}
#define HEX_SET_MSG_UBX_DISABLE    {0xB5, 0x62 , 0x06 , 0x8A , 0x09 , 0x00 , 0x01 , 0x01 , 0x00 , 0x00 , 0x01 , 0x00 , 0x74 , 0x10 , 0x00 , 0x20 , 0xBB}



#endif
