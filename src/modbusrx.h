#include <PubSubClient.h>
#include <SoftwareSerial.h>


float _t15 = 15000000 / 115200; // 1T * 1.5 = T1.5
float _t35 = 35000000 / 115200;

uint8_t convert(char *src);

const byte _auchCRCHi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
    0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
    0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
    0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
    0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40};

/* Table of CRC values for low order byte */
const byte _auchCRCLo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
    0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
    0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
    0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
    0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
    0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
    0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
    0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
    0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
    0x40};

word calcCrc(byte address, byte *pduFrame, byte pduLen)
{
    byte CRCHi = 0xFF, CRCLo = 0x0FF, Index;

    Index = CRCHi ^ address;
    CRCHi = CRCLo ^ _auchCRCHi[Index];
    CRCLo = _auchCRCLo[Index];

    while (pduLen--)
    {
        Index = CRCHi ^ *pduFrame++;
        CRCHi = CRCLo ^ _auchCRCHi[Index];
        CRCLo = _auchCRCLo[Index];
    }

    return (CRCHi << 8) | CRCLo;
}

void sendsignal(SoftwareSerial *sfserial, char *msg)
{
    byte slaveid;
    byte controlstr[10] = {};
    char delimiter[] = ",";
    Serial.println("xx:" + String(msg));
    char *command = strtok(msg, delimiter);
    int index = 0;
    while (command != NULL)
    {
        if (index == 0)
        {
            slaveid = strtol(command, NULL, 10);
        }
        else
        {
            controlstr[index - 1] = strtol(command, NULL, 10);
        }
        command = strtok(NULL, delimiter);
        index++;
    }
    
    word crc = calcCrc(slaveid, controlstr, index - 1);
    sfserial->write(slaveid);
    int size = sfserial->write(&controlstr[0], index - 1);
    sfserial->write(crc >> 8);
    sfserial->write(crc & 0xFF);
    sfserial->flush();
    Serial.println("xxx" + String(slaveid));
    Serial.println("xxx" + String(size));
    Serial.println("xxx1: " + String(crc >> 8));
    Serial.println("xxx2: " + String(crc & 0xFF));
    delayMicroseconds(_t35);
}

String *byteToHexStr(unsigned char byte_arr[], int arr_len)
{
    String *hexstr = new String();
    for (int i = 0; i < arr_len; i++)
    {
        char hex1;
        char hex2;
        int value = byte_arr[i];
        int v1 = value / 16;
        int v2 = value % 16;

        if (v1 >= 0 && v1 <= 9)
            hex1 = (char)(48 + v1);
        else
            hex1 = (char)(55 + v1);

        if (v2 >= 0 && v2 <= 9)
            hex2 = (char)(48 + v2);
        else
            hex2 = (char)(55 + v2);

        *hexstr = *hexstr + hex1 + hex2;
    }
    return hexstr;
}

String *byteToHexStr(char byte_arr[], int arr_len)
{
    String *hexstr = new String();
    for (int i = 0; i < arr_len; i++)
    {
        char hex1;
        char hex2;
        int value = byte_arr[i];
        int v1 = value / 16;
        int v2 = value % 16;

        if (v1 >= 0 && v1 <= 9)
            hex1 = (char)(48 + v1);
        else
            hex1 = (char)(55 + v1);

        if (v2 >= 0 && v2 <= 9)
            hex2 = (char)(48 + v2);
        else
            hex2 = (char)(55 + v2);

        *hexstr = *hexstr + hex1 + hex2;
    }
    return hexstr;
}

uint8_t convert(char *src)
{
    uint8_t ret = 0, d;
    while (*src)
    {
        if (*src >= '0' && *src <= '9')
            d = *src - '0';
        else if (*src >= 'a' && *src <= 'f')
            d = *src - 'a' + 10;
        else if (*src >= 'A' && *src <= 'Z')
            d = *src - 'A' + 10;

        ret = ret * 16 + d;
        src++;
    }
    return ret;
}
void modbustask(SoftwareSerial *sfserial, WiFiClient *TCPClient)
{
    int _len = 0;
    char *_frame = {};
    while ((*sfserial).available() > _len)
    {
        _len = (*sfserial).available();
        delayMicroseconds(_t15);
    }

    if (_len == 0)
        return;

    byte i;
    _frame = (char *)malloc(_len);
    for (i = 0; i < _len; i++)
        _frame[i] = (*sfserial).read();
     for(byte cln = 0; cln < 4; cln++) {   
        if(TCPClient[cln])                     
          TCPClient[cln].write(_frame, _len); //send the buffer to TCP port:8880 
      }
    if (_len > 0)
    {
        String callbackstr = *byteToHexStr(_frame, _len);
        Serial.print("callback:");
        Serial.println(callbackstr);
    }
    free(_frame);
    _len = 0;
}