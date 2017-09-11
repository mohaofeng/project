#ifndef __PIC_H
#define __PIC_H 

const unsigned char gImage_logo[] = {

0x00,0x00,0x00,0x00,0x0D,0x00,
0x00,0x00,0x00,0x80,0x3F,0x00,
0x00,0x00,0x00,0xC0,0x61,0x00,
0x00,0x00,0x00,0x40,0xC0,0x00,
0x00,0x00,0x00,0x60,0x80,0x01,
0x00,0x00,0x00,0x40,0x80,0x01,
0x00,0x00,0x00,0x60,0x80,0x00,
0x00,0x00,0x00,0x60,0xC0,0x00,
0x00,0x00,0x00,0x60,0xC0,0x00,
0x00,0x00,0x00,0x20,0x80,0x00,
0x00,0x00,0x80,0x60,0x80,0x00,
0x00,0x00,0x24,0x35,0xC0,0x00,
0x00,0xC0,0x01,0x40,0xC1,0x00,
0x00,0x70,0xFC,0x3F,0x74,0x00,
0x00,0x98,0xFF,0xFF,0x39,0x00,
0x00,0xC6,0xFF,0xFF,0x07,0x00,
0x00,0xF1,0xFF,0xFF,0x4F,0x00,
0x80,0xF8,0xFF,0xFF,0x3F,0x00,
0x40,0xFE,0xFF,0xFF,0x7F,0x00,
0x20,0xFE,0xFF,0xFF,0xFF,0x02,
0xB0,0xFF,0xFF,0xFF,0xFF,0x01,
0x90,0xFF,0xFF,0xFF,0xFF,0x05,
0xC8,0xFF,0xFF,0xFF,0xFF,0x0B,
0xE8,0xBF,0xFF,0xFF,0xFD,0x17,
0xE4,0xDF,0x3F,0xF8,0xFD,0x07,
0xF4,0xDF,0x0F,0xE0,0xFB,0x2F,
0xF2,0xDF,0x87,0xEF,0xF3,0x1F,
0xF2,0xDF,0xCF,0xDF,0xF7,0x4F,
0xFA,0xFF,0xC7,0xD3,0xF7,0x1F,
0xF9,0xFF,0xCF,0xD3,0xE7,0xBF,
0xFD,0xFF,0xCF,0xF7,0xAF,0x1F,
0xF9,0xFF,0x9F,0xFF,0x6F,0x3F,
0xFC,0xFF,0x7F,0xF0,0x6F,0xBF,
0xFD,0xFF,0xFF,0xFF,0x6F,0x3E,
0xFC,0xFF,0xFF,0xFF,0xEF,0x7E,
0xFD,0xFF,0xFF,0xFF,0xEF,0xBE,
0xFC,0xFF,0xFF,0xFF,0xEF,0x3E,
0xFC,0xFF,0xFF,0xFF,0xEF,0x7E,
0xFD,0xFF,0xFF,0xFF,0xEF,0xBE,
0xFC,0xFF,0xFF,0xFF,0xEF,0x3E,
0xFC,0xFF,0xFF,0xFF,0xEF,0xBE,
0xFA,0xFF,0xFF,0xFF,0x6F,0x3E,
0xF8,0xFF,0xFF,0xFF,0x6F,0x5F,
0xF8,0xFF,0xFF,0xFF,0xAF,0x1F,
0xF4,0xFF,0x7F,0xF0,0xDF,0x5F,
0xE0,0xFF,0x3F,0xE0,0xF7,0x1F,
0xE8,0xFF,0x8F,0xC7,0xF7,0x07,
0xF0,0xFF,0x8F,0x9F,0xFB,0x0F,
0xC0,0xFF,0x8F,0xFF,0xFB,0x13,
0xA0,0xFF,0x8F,0xC7,0xFF,0x03,
0x40,0xBF,0x1F,0xE3,0xFF,0x0D,
0x00,0xBE,0x7F,0xFE,0xFF,0x00,
0x80,0x3E,0xFF,0xFF,0x7F,0x02,
0x00,0xF9,0xFF,0xFF,0x3F,0x01,
0x00,0xE4,0xFF,0xFF,0xCF,0x03,
0x00,0xC8,0xFF,0xFF,0x07,0x06,
0x00,0x10,0xFF,0xFF,0x11,0x06,
0x00,0x40,0xF8,0x5F,0x04,0x06,
0x00,0x00,0x05,0x80,0x03,0x02,
0x00,0x00,0x50,0x2D,0x01,0x03,
0x00,0x00,0x00,0x00,0x03,0x06,
0x00,0x00,0x00,0x00,0x03,0x0E,
0x00,0x00,0x00,0x00,0x03,0x06,
0x00,0x00,0x00,0x00,0x02,0x0E,
0x00,0x00,0x00,0x00,0x06,0x07,
0x00,0x00,0x00,0x00,0xDC,0x0D,
0x00,0x00,0x00,0x00,0xEC,0x0D,
0x00,0x00,0x00,0x00,0x2C,0x0C,
0x00,0x00,0x00,0x00,0x38,0x04,
0x00,0x00,0xB0,0x2F,0x20,0x06,
0x00,0x00,0xFF,0xFF,0x20,0x04,
0x00,0xC0,0x4F,0xF2,0x33,0x06,
0x00,0xF0,0x01,0x80,0x1F,0x02,
0x00,0x38,0x00,0x1C,0x3C,0x03,
0x00,0x1E,0x00,0xE3,0x78,0x01,
0x00,0x07,0x80,0x00,0xC1,0x01,
0x80,0x03,0x80,0x00,0xC1,0x01,
0x80,0x01,0x84,0x78,0x81,0x03,
0xE0,0x00,0x84,0xFC,0x01,0x03,
0xE0,0x00,0x84,0xFC,0x00,0x0E,
0x30,0x00,0x88,0xFC,0x04,0x0C,
0x38,0x00,0x00,0x7B,0x08,0x18,
0x18,0x00,0x00,0x2C,0x08,0x18,
0x08,0x00,0x00,0x00,0x10,0x30,
0x1C,0x00,0x00,0x00,0x10,0x20,
0x0C,0x00,0x00,0x00,0x10,0x60,
0x06,0x00,0x00,0x00,0x70,0x60,
0x06,0x00,0x00,0x00,0xF0,0x60,
0x06,0x00,0x00,0x00,0xF0,0x41,
0x06,0x00,0x00,0x00,0xF0,0xC1,
0x06,0x00,0x00,0x00,0xF0,0xC1,
0x07,0x00,0x00,0x00,0xF8,0x43,
0x02,0x00,0x00,0x00,0xF0,0xC1,
0x03,0x00,0x00,0x00,0xF0,0xC0,
0x06,0x00,0x00,0x7C,0xFC,0xC0,
0x03,0x00,0x00,0x82,0x2C,0xC0,
0x06,0x00,0x80,0x90,0x00,0xC0,
0x06,0x00,0x40,0xB8,0x00,0xC0,
0x06,0x00,0x40,0x7E,0x01,0xC0,
0x06,0x00,0x44,0xFE,0x00,0x60,
0x0E,0x00,0x44,0xFE,0x00,0x60,
0x0C,0x00,0xC4,0xBC,0x00,0x60,
0x0C,0x00,0x88,0x63,0x00,0x30,
0x18,0x00,0x80,0x3F,0x00,0x30,
0x18,0x00,0x00,0x00,0x00,0x10,
0x38,0x00,0x00,0x00,0x00,0x18,
0x30,0x00,0x00,0x00,0x00,0x0C,
0x60,0x00,0x00,0x00,0x00,0x0E,
0xE0,0x00,0x00,0x00,0x00,0x0F,
0xC0,0x01,0x00,0x00,0x00,0x03,
0x80,0x03,0x00,0x00,0x80,0x01,
0x00,0x07,0x00,0x00,0xC0,0x00,
0x00,0x1E,0x00,0x00,0xF0,0x00,
0x00,0x38,0x00,0x00,0x3C,0x00,
0x00,0xF0,0x00,0x00,0x1F,0x00,
0x00,0xE0,0x0F,0xA0,0x1B,0x00,
0x00,0x00,0xFF,0xFF,0x31,0x00,
0x00,0x00,0xF8,0xDF,0x60,0x00,
0x00,0x00,0x00,0x80,0xE1,0x03,
0x00,0x00,0x00,0x00,0xC1,0x06,
0x00,0x00,0x00,0x00,0x03,0x0C,
0x00,0x00,0x00,0x00,0x03,0x04,
0x00,0x00,0x00,0x00,0x02,0x0C,
0x00,0x00,0x00,0x00,0x06,0x18,
0x00,0x00,0x00,0x00,0x04,0x30,
0x00,0x00,0x00,0x00,0x0C,0x10,
0x00,0x00,0x00,0x00,0x08,0x30,
0x00,0x00,0x00,0x00,0x38,0x20,
0x00,0x00,0x00,0x00,0x30,0x18,
0x00,0x00,0x00,0x00,0xE0,0x1A,
0x00,0x00,0x00,0x00,0x80,0x0F};/*"C:\Users\mo\Desktop\f7b39d76ccfc501c0ea390a888ea5a8e.bmp",0*/


const unsigned char gImageEye[]=
{
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x03,0xE0,0xFF,0x0F,0x00,0x00,0x00,0x00,0xF8,0x1F,0xF8,0xFF,0x7F,0x00,0x00,0x00,0x00,0x7E,0x7E,0xFC,0xFF,
0xFF,0x01,0x00,0x00,0x00,0x07,0xE0,0xFD,0xFF,0xFF,0x07,0x00,0x00,0x80,0x03,0x80,0xFF,0xFF,0xFF,0x0F,0x00,0x00,0xC0,0x00,0x00,0xFF,0xFF,0xFF,0x1F,0x00,0x00,0xE0,
0x00,0x00,0xFE,0x01,0xFF,0x3F,0x00,0x00,0x60,0x00,0x00,0x0C,0x00,0xF0,0x7F,0x00,0x00,0x30,0x00,0x00,0x0C,0x00,0x80,0xFF,0x00,0x00,0x38,0x00,0x00,0x18,0x00,0x00,
0xFE,0x00,0x00,0x18,0x00,0x00,0x18,0x00,0x00,0xFC,0x01,0x00,0x18,0x00,0x00,0x18,0x00,0x00,0xF0,0x01,0x00,0x18,0x00,0x00,0x30,0x00,0x00,0xE0,0x01,0x00,0x0C,0x00,
0x00,0x30,0x00,0x00,0xC0,0x01,0x00,0x0C,0x00,0x00,0x30,0x00,0x00,0x80,0x01,0x00,0x0C,0x00,0x00,0x30,0x00,0x00,0x80,0x01,0x00,0x0C,0x00,0x00,0x30,0x00,0x00,0x00,
0x03,0x00,0x0C,0x00,0x00,0x30,0x00,0x00,0x00,0x06,0x00,0x0C,0x00,0x00,0x30,0x00,0x00,0x00,0x06,0x00,0x18,0x00,0x00,0x10,0x00,0x00,0x00,0x0C,0x00,0x18,0x00,0x00,
0x18,0x00,0x00,0x00,0x0C,0x00,0x18,0x00,0x00,0x18,0x00,0x00,0x00,0x18,0x00,0x38,0x00,0x00,0x1C,0x00,0x00,0x00,0x18,0xC0,0x30,0x00,0x00,0x0C,0x00,0x00,0x00,0x18,
0xC0,0x60,0x00,0x00,0x06,0x00,0x00,0x00,0x10,0xE0,0xE0,0x00,0x00,0x37,0x00,0x00,0x00,0x30,0xE0,0xC0,0x01,0x00,0xFB,0x00,0x00,0x00,0x30,0xF0,0xC0,0x03,0xC0,0xFF,
0x03,0x00,0x00,0x30,0xF0,0xC0,0x0F,0xF0,0xFF,0x07,0x00,0x00,0x20,0xF0,0xE0,0xFE,0x7F,0xFF,0x0F,0x00,0x00,0x60,0xF8,0xE0,0xFB,0xDF,0xFF,0x1F,0x00,0x00,0x20,0xF8,
0xE0,0xCF,0xE7,0xFF,0x3F,0x00,0x00,0xE0,0xFF,0xE0,0x03,0xFC,0xFF,0x3F,0x00,0x00,0xE0,0xFF,0x60,0xFE,0xF7,0xFF,0x3F,0x00,0x00,0xE0,0x7F,0xE0,0xFF,0xDF,0xFF,0x3F,
0x00,0x00,0xE0,0x7F,0xE0,0x03,0xFC,0xFF,0x3F,0x00,0x00,0x60,0x7F,0xE0,0x00,0xF0,0xFF,0x1F,0x00,0x00,0x60,0x38,0x30,0x00,0xC0,0xFF,0x0F,0x00,0x00,0xE0,0x7F,0x18,
0x00,0x80,0xFD,0x07,0x00,0x00,0xE0,0x7F,0x0C,0x00,0x00,0xFF,0x01,0x00,0x00,0xF0,0x7F,0x0E,0x00,0x00,0xFB,0x00,0x00,0x00,0xF0,0xFF,0x06,0x00,0x00,0x06,0x00,0x00,
0x00,0xF0,0xFF,0x06,0x00,0x00,0x06,0x00,0x00,0x00,0x10,0xFC,0x03,0x00,0x00,0x0C,0x00,0x00,0x00,0x18,0xF8,0x03,0x00,0x00,0x0C,0x00,0x00,0x00,0x18,0xF8,0x03,0x00,
0x00,0x0C,0x00,0x00,0x00,0x18,0x78,0x03,0x00,0x00,0x0C,0x00,0x00,0x00,0x0C,0x70,0x03,0x00,0x00,0x08,0x00,0x00,0x00,0x0C,0x70,0x03,0x00,0x00,0x18,0x00,0x00,0x00,
0x06,0x70,0x03,0x00,0x00,0x08,0x00,0x00,0x00,0x06,0xF0,0x03,0x00,0x00,0x0C,0x00,0x00,0x00,0x03,0xE0,0x03,0x00,0x00,0x0C,0x00,0x00,0x80,0x01,0x40,0x03,0x00,0x00,
0x0C,0x00,0x00,0xC0,0x01,0x00,0x03,0x00,0x00,0x0C,0x00,0x00,0xE0,0x00,0x00,0x03,0x00,0x00,0x06,0x00,0x00,0x70,0x00,0x00,0x06,0x00,0x00,0x06,0x00,0x00,0x38,0x00,
0x00,0x0E,0x00,0x00,0x07,0x00,0x00,0x1C,0x00,0x00,0x0C,0x00,0x00,0x03,0x00,0x00,0x0F,0x00,0x00,0x18,0x00,0x80,0x07,0x00,0xC0,0x0F,0x00,0x00,0x38,0x00,0xC0,0x7F,
0x00,0xFC,0x0F,0x00,0x00,0x70,0x00,0xE0,0xFE,0xFF,0xFF,0x07,0x00,0x00,0xE0,0x01,0x78,0xFC,0xFF,0xFF,0x03,0x00,0x00,0x80,0x1F,0x1F,0xFC,0xFF,0xFF,0x01,0x00,0x00,
0x00,0xFE,0x07,0xF0,0xFF,0x7F,0x00,0x00,0x00,0x00,0xF0,0x01,0xC0,0xFF,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"C:\Users\mo\Desktop\表情图\未标题-172x67a 拷贝.bmp",0*/
};






const unsigned char gOneEye[] ={
0x00,0x00,0xE0,0x07,0xF0,0x0F,0xF8,0x1F,0xFC,0x3F,0xFC,0x3F,0xFE,0x7F,0xFE,0x7F,0xFE,0x7F,0xFE,0x7F,0xFC,0x3F,0xFC,0x3F,0xF8,0x1F,0xF0,0x0F,0xE0,0x07,0x00,0x00,/*"C:\Users\mo\Desktop\表情图\eye16x16 拷贝.bmp",0*/
};














#endif

