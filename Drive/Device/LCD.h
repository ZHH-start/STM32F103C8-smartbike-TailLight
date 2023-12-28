#ifndef __LCD_H
#define __LCD_H

#include "stm32f10x.h"

#define USE_HORIZONTAL 0 // ���ú�������������ʾ 0��1Ϊ���� 2��3Ϊ����

#define LCD_W          240
#define LCD_H          240

//-----------------LCD�˿ڶ���----------------

#define LCD_SCLK_Clr() GPIO_ResetBits(GPIOA, GPIO_Pin_0) // SCL=SCLK
#define LCD_SCLK_Set() GPIO_SetBits(GPIOA, GPIO_Pin_0)

#define LCD_MOSI_Clr() GPIO_ResetBits(GPIOA, GPIO_Pin_1) // SDA=MOSI
#define LCD_MOSI_Set() GPIO_SetBits(GPIOA, GPIO_Pin_1)

#define LCD_RES_Clr()  GPIO_ResetBits(GPIOA, GPIO_Pin_2) // RES
#define LCD_RES_Set()  GPIO_SetBits(GPIOA, GPIO_Pin_2)

#define LCD_DC_Clr()   GPIO_ResetBits(GPIOA, GPIO_Pin_3) // DC
#define LCD_DC_Set()   GPIO_SetBits(GPIOA, GPIO_Pin_3)

#define LCD_CS_Clr()   GPIO_ResetBits(GPIOA, GPIO_Pin_4) // CS
#define LCD_CS_Set()   GPIO_SetBits(GPIOA, GPIO_Pin_4)

#define LCD_BLK_Clr()  GPIO_ResetBits(GPIOA, GPIO_Pin_5) // BLK
#define LCD_BLK_Set()  GPIO_SetBits(GPIOA, GPIO_Pin_5)

void LCD_GPIO_Init(void);                             // ��ʼ��GPIO
void LCD_Writ_Bus(u8 dat);                            // ģ��SPIʱ��
void LCD_WR_DATA8(u8 dat);                            // д��һ���ֽ�
void LCD_WR_DATA(u16 dat);                            // д�������ֽ�
void LCD_WR_REG(u8 dat);                              // д��һ��ָ��
void LCD_Address_Set(u16 x1, u16 y1, u16 x2, u16 y2); // �������꺯��
void LCD_Init(void);                                  // LCD��ʼ��

void LCD_Fill(u16 xsta, u16 ysta, u16 xend, u16 yend, u16 color);  // ָ�����������ɫ
void LCD_DrawPoint(u16 x, u16 y, u16 color);                       // ��ָ��λ�û�һ����
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);      // ��ָ��λ�û�һ����
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 color); // ��ָ��λ�û�һ������
void Draw_Circle(u16 x0, u16 y0, u8 r, u16 color);                 // ��ָ��λ�û�һ��Բ

void LCD_ShowChinese(u16 x, u16 y, u8 *s, u16 fc, u16 bc, u8 sizey, u8 mode);      // ��ʾ���ִ�
void LCD_ShowChinese12x12(u16 x, u16 y, u8 *s, u16 fc, u16 bc, u8 sizey, u8 mode); // ��ʾ����12x12����
void LCD_ShowChinese16x16(u16 x, u16 y, u8 *s, u16 fc, u16 bc, u8 sizey, u8 mode); // ��ʾ����16x16����
void LCD_ShowChinese24x24(u16 x, u16 y, u8 *s, u16 fc, u16 bc, u8 sizey, u8 mode); // ��ʾ����24x24����
void LCD_ShowChinese32x32(u16 x, u16 y, u8 *s, u16 fc, u16 bc, u8 sizey, u8 mode); // ��ʾ����32x32����

void LCD_ShowChar(u16 x, u16 y, u8 num, u16 fc, u16 bc, u8 sizey, u8 mode);        // ��ʾһ���ַ�
void LCD_ShowString(u16 x, u16 y, const u8 *p, u16 fc, u16 bc, u8 sizey, u8 mode); // ��ʾ�ַ���
u32 mypow(u8 m, u8 n);                                                             // ����
void LCD_ShowIntNum(u16 x, u16 y, u16 num, u8 len, u16 fc, u16 bc, u8 sizey);      // ��ʾ��������
void LCD_ShowFloatNum1(u16 x, u16 y, float num, u8 len, u16 fc, u16 bc, u8 sizey); // ��ʾ��λС������

void LCD_ShowPicture(u16 x, u16 y, u16 length, u16 width, const u8 pic[]); // ��ʾͼƬ

// ������ɫ
#define WHITE      0xFFFF
#define BLACK      0x0000
#define BLUE       0x001F
#define BRED       0XF81F
#define GRED       0XFFE0
#define GBLUE      0X07FF
#define RED        0xF800
#define MAGENTA    0xF81F
#define GREEN      0x07E0
#define CYAN       0x7FFF
#define YELLOW     0xFFE0
#define BROWN      0XBC40 // ��ɫ
#define BRRED      0XFC07 // �غ�ɫ
#define GRAY       0X8430 // ��ɫ
#define DARKBLUE   0X01CF // ����ɫ
#define LIGHTBLUE  0X7D7C // ǳ��ɫ
#define GRAYBLUE   0X5458 // ����ɫ
#define LIGHTGREEN 0X841F // ǳ��ɫ
#define LGRAY      0XC618 // ǳ��ɫ(PANNEL),���屳��ɫ
#define LGRAYBLUE  0XA651 // ǳ����ɫ(�м����ɫ)
#define LBBLUE     0X2B12 // ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)

#endif
