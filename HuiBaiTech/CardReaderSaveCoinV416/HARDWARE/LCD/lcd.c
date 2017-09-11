#include "lcd.h"
#include "stdlib.h"
#include "font.h" 
#include "usart.h"	 
#include "delay.h"	 
#include "st7565r.h"
//#include "pic.h"
#include "common.h"
#include "string.h"	 
#include "wt588d.h"	 



volatile uint8_t LCDRam[140][70];
//static uint8_t LCDRam[8][128];


void LCD_12864DrawPoint(u16 x,u16 y,u16 colr)
{
	uint8_t pos,bx,temp=0;
	if((x>=lcddev.width)||(y>lcddev.height))return;//������Χ��.
	
	pos=y/8;
	bx=y%8;
	temp=(0x01<<bx);
	if(colr){  
		LCDRam[x][pos] |= temp;
	}
	else{
		LCDRam[x][pos] &= ~temp;	
	}    

}

// void LCD_12864DrawPoint(uint8_t x,uint8_t y,uint8_t colr)
// {
// 	uint8_t bit,bx,bitpos=0;
// 	if((x>=lcddev.width)||(y>=lcddev.height))return;//������Χ��.
// 	
// 	bit=y/8;
// 	bx=y%8;
// 	bitpos=(0x80>>bx);
// 	if(colr){  
// 		LCDRam[y][bit] |= bitpos;
// 	}
// 	else{
// 		LCDRam[y][bit] &= ~bitpos;	
// 	}    
// }
//��ʼ��lcd
//�ó�ʼ���������Գ�ʼ������ILI93XXҺ��,�������������ǻ���ILI9320��!!!
//�������ͺŵ�����оƬ��û�в���! 
void LCD_Init(void)
{ 
  lcddev.height = 65;
  lcddev.width= 132;
	Init_ST7565r();
	LCD_12864_Clear(0x00);
	//LCD_DrawLine(0,12,127,12,DIS_NORMAL);		
}  




void LCD_12864_ClearRec(uint8_t dat)  
{  
	 uint8_t i,j;  
	 for(i=0;i<132;i++)
	 {   
			for(j=0;j<8;j++)
			{      
				LCDRam[i][j]=dat;
			}
	 }
}


#if 0
//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
void LCD_12864_Clear(uint8_t dat)  
{  
	uint8_t i,j;  
	for(i=0;i<lcddev.height;i++)
  {   
		for(j=0;j<lcddev.width;j++)
    {      
			LCD_12864DrawPoint(i,j,0);
    }
  }
	LCDRefresh_Gram();//������ʾ
}
#else

void LCD_12864_Clear(uint8_t dat)  
{  
// 	uint8_t i,j;  
// 	for(i=0;i<132;i++)
//     {   
// 			for(j=0;j<8;j++)
// 			{      
// 				LCDRam[i][j]=dat;
// 			}
//     }
// //	LCDRefresh_Gram();//������ʾ
//�����Դ浽LCD		 
	uint8_t i,n;		    
	for(i=8;i>5;i--)
	{  
			//Write_ST7565R_COM(SET_START_lINE|0);
	    Write_ST7565R_COM(SET_YADDR+i);	 //SET PAGE 0---7
			Write_ST7565R_COM(SET_XADDH|0);	  //SET CLUMN MSB
	    Write_ST7565R_COM(SET_XADDL|0);	 //SET COLUMN LSB
			for(n=0;n<132;n++)
			{
				Write_ST7565R_DISPLAY_DATA_serial_4SPI(0);
			}
	}   
	
	for(i=0;i<5;i++)
	{  
			//Write_ST7565R_COM(SET_START_lINE|0);
	    Write_ST7565R_COM(SET_YADDR+i);	 //SET PAGE 0---7
			Write_ST7565R_COM(SET_XADDH|0);	  //SET CLUMN MSB
	    Write_ST7565R_COM(SET_XADDL|0);	 //SET COLUMN LSB
			for(n=0;n<128;n++)
			{
				Write_ST7565R_DISPLAY_DATA_serial_4SPI(0);
			}
	}   
}
#endif

// void clearwkdat(void)
// {
// 	u8 i,j;
// 	for(i=0;i<128;i++)
// 		for(j=13;j<64;j++)
// 			LCD_12864DrawPoint(i,j,0);
// }

//����
//x1,y1:�������
//x2,y2:�յ�����  
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u8 mod)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		LCD_12864DrawPoint(uRow,uCol,mod);//���� 
		
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}    
//������	  
//(x1,y1),(x2,y2):���εĶԽ�����
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u8 mod)
{
	LCD_DrawLine(x1,y1,x2,y1,mod);
	LCD_DrawLine(x1,y1,x1,y2,mod);
	LCD_DrawLine(x1,y2,x2,y2,mod);
	LCD_DrawLine(x2,y1,x2,y2,mod);
}
//��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
//r    :�뾶
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r,u8 mod)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //�ж��¸���λ�õı�־
	while(a<=b)
	{
		LCD_12864DrawPoint(x0+a,y0-b,mod);             //5
 		LCD_12864DrawPoint(x0+b,y0-a,mod);             //0           
		LCD_12864DrawPoint(x0+b,y0+a,mod);             //4               
		LCD_12864DrawPoint(x0+a,y0+b,mod);             //6 
		LCD_12864DrawPoint(x0-a,y0+b,mod);             //1       
 		LCD_12864DrawPoint(x0-b,y0+a,mod);             
		LCD_12864DrawPoint(x0-a,y0-b,mod);             //2             
  		LCD_12864DrawPoint(x0-b,y0-a,mod);             //7     	         
		a++;
		//ʹ��Bresenham�㷨��Բ     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 									  
//��ָ��λ����ʾһ���ַ�
//x,y:��ʼ����
//num:Ҫ��ʾ���ַ�:" "--->"~"
//size:�����С 12/16/24
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  							  
    u8 temp,t1,t;
	u16 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���	
 	num=num-' ';//�õ�ƫ�ƺ��ֵ��ASCII�ֿ��Ǵӿո�ʼȡģ������-' '���Ƕ�Ӧ�ַ����ֿ⣩
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=asc2_1206[num][t]; 	 	//����1206����
		else if(size==16)temp=asc2_1608[num][t];	//����1608����
		else if(size==24)temp=asc2_2412[num][t];	//����2412����
		else if(size==32)temp=asc2_3216[num][t];	//����2412����
		
		else return;								//û�е��ֿ�
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)
					LCD_12864DrawPoint(x,y,mode);
			else
					LCD_12864DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if(y>=lcddev.height)return;		//��������
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=lcddev.width)return;	//��������
				break;
			}
		}  	 
	}  	    	   	 	  
}   
//m^n����
//����ֵ:m^n�η�.
u32 LCD_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//��ʾ����,��λΪ0,����ʾ
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//color:��ɫ 
//num:��ֵ(0~4294967295);	 
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
} 
//??��?��y��?,?????a0,?��2???��?
//x,y :?e��?��?����	 
//len :��y��?��???��y
//size:��?��?�䨮D?
//color:??��? 
//num:��y?��(0~4294967295);	 
void LCD_ShowNumAndBk(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mod)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,mod);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mod); 
	}
} 

//��ʾ����,��λΪ0,������ʾ
//x,y:�������
//num:��ֵ(0~999999999);	 
//len:����(��Ҫ��ʾ��λ��)
//size:�����С
//mode:
//[7]:0,�����;1,���0.
//[6:1]:����
//[0]:0,�ǵ�����ʾ;1,������ʾ.
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode)
{  
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
				else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
 				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
	}
} 
//��ʾ�ַ���
//x,y:�������
//width,height:�����С  
//size:�����С
//*p:�ַ�����ʼ��ַ		  
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
{         
	u8 x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//�˳�
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}

//��ʾ�ַ���
//x,y:�������
//width,height:�����С  
//size:�����С
//*p:�ַ�����ʼ��ַ		  
void LCD_ShowStringAndBk(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p,u8 mod)
{         
    u8 x0=x;
    width+=x;
    height+=y;
    while((*p<='~')&&(*p>=' '))//?D??��?2?��?��?������?��?!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//��?3?
        LCD_ShowChar(x,y,*p,size,mod);
        x+=size/2;
        p++;
    }  
}


 void LCD_Showhz(uint8_t x,uint8_t y,uint8_t hro,uint8_t size,uint8_t mode)
 {      			    
	uint8_t temp=0;
	uint8_t t=0;
	uint8_t t1=0;
	uint8_t t2=0;
	uint8_t y0=y;
	if(size == 16)
		t2 = 2;
	if(size == 32)
		t2 = 4;
	for(t=0;t<(size*t2);t++)
	{
 		if(size == 16)
		{
			temp=LCD_hz_1616[hro][t];		 //�̡¨�?1608��?��? 
		}		
		else
		{
			temp=LCD_hz_3232[hro][t];		 //�̡¨�?1608��?��?
		}
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x01)
				LCD_12864DrawPoint(x,y,mode);
			else 
				LCD_12864DrawPoint(x,y,!mode);
			temp>>=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
//				break;
			}
		}  	 
	}          
}

void LCDShowHZList(uint8_t x,uint8_t y,uint8_t hro,uint8_t size,uint8_t mode,uint8_t num)
{
	uint8_t i;
	
	for(i=0;i<num;i++)
	{
		LCD_Showhz(x,y,hro,size,mode);
		if(x>100)
		
		{
			x = 0;
			y =y +(size+2);
		}
		
		x = x+(size+2);
		hro++;
	}
}



void Display_pic(uint8_t x,uint8_t y,uint8_t with,uint8_t hight,uint8_t* addr,uint8_t mode)
{      		
	 uint16_t i,j;
//	 uint8_t y0=y;
	 uint8_t x0=x;
	 uint8_t temp;	
	 uint8_t with0;
	 with0 = with/8;
	 for(j=0;j<with0*hight;j++)
	 {   
			temp=addr[j];		                    
		 for(i=0;i<8;i++)
		 {
				if(temp&0x01)
					LCD_12864DrawPoint(x,y,mode);
				else 
					LCD_12864DrawPoint(x,y,!mode);
				temp>>=1;
				x++;
				if((x-x0)== with)
				{
					x=x0;
					y++;
					break;
				}
			}  	 
		}
}

#if 1
//�����Դ浽LCD		 
void LCDRefresh_Gram(void)
{
	uint8_t i,n;		    
	for(i=8;i>=4;i--)
	{  
			//Write_ST7565R_COM(SET_START_lINE|0);
		
			if(Config.disdir)
			{
				Write_ST7565R_COM(FUNC_SET_Y|MIRROR_Y);	 
				Write_ST7565R_COM(FUNC_SET_X);	 
			}
			else
			{
				Write_ST7565R_COM(FUNC_SET_Y & (~MIRROR_Y));	 
				Write_ST7565R_COM(FUNC_SET_X |MIRROR_X);	 				
			}
		
		
	    Write_ST7565R_COM(SET_YADDR+i);	 //SET PAGE 0---7
			Write_ST7565R_COM(SET_XADDH|0);	  //SET CLUMN MSB
	    Write_ST7565R_COM(SET_XADDL|0);	 //SET COLUMN LSB
			for(n=0;n<128;n++)
			{
				Write_ST7565R_DISPLAY_DATA_serial_4SPI(LCDRam[n][i]);
			}
	}   
	
	for(i=0;i<4;i++)
	{  
			//Write_ST7565R_COM(SET_START_lINE|0);
	    Write_ST7565R_COM(SET_YADDR+i);	 //SET PAGE 0---7
			Write_ST7565R_COM(SET_XADDH|0);	  //SET CLUMN MSB
	    Write_ST7565R_COM(SET_XADDL|0);	 //SET COLUMN LSB
			for(n=0;n<128;n++)
			{
				Write_ST7565R_DISPLAY_DATA_serial_4SPI(LCDRam[n][i]);
			}
	}   
}


#else

//�����Դ浽LCD		 
void LCDRefresh_Gram(void)
{
	uint8_t i,n;		    
	for(i=0;i<8;i--)
	{  
			//Write_ST7565R_COM(SET_START_lINE|0);
	    Write_ST7565R_COM(SET_YADDR+i);	 //SET PAGE 0---7
			Write_ST7565R_COM(SET_XADDH|0);	  //SET CLUMN MSB
	    Write_ST7565R_COM(SET_XADDL|0);	 //SET COLUMN LSB
			for(n=0;n<128;n++)
			{
				Write_ST7565R_DISPLAY_DATA_serial_4SPI(LCDRam[n][i]);
			}
	}   
}

#endif























