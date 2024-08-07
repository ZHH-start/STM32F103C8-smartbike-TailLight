# 基于最经典的STM32型号-C8T6实现的车尾灯

## 主要功能说明

该项目是一个简单的智能车尾灯。具有三个功能：防盗模式，通过6050加速度检测判断。骑行保护模式，通过6050硬件解算判断，同时具备简单的GPIO控制尾灯功能，骑行保护模式时点亮。另外有一个按键用于切换模式。

## 环境说明
该项目使用Keil工具链ArmCC5编译链，EIDE工程。直接使用VScode打开smartbike-TailLight.code-workspace文件即可。

### VScode配置要求
可参考知乎专栏[VScode搭配EIDE开发单片机](https://www.zhihu.com/column/c_1595902252421754880)，以及EIDE官方说明
1. 完成VScode的C和C++工具链配置。
2. 至少有jlink，STlink或DAPlink调试器中的一种。（工程配置为DAPlink）
3. 完成EIDE的编译工具链，下载工具链相关配置

## 外设概述

该项目实现了基于C8T6的多项功能。主要外设如下：
- ATGM336H串口接收和解析融合定位数据
- MPU6050读取加速度和姿态解算
- OLED屏幕，驱动为SSD1306
- 单按键，扫描使用定时器10ms中断
- 串口发送经纬度数据给Air780EG，附带MQTT主题。780解析时会根据主题对应发送。

## 代码解析
具体代码解析见代码注释

## 使用引脚

|   外设    | Pin脚 |
|:------:|:------:|
|  OLED   | SCL-PB8 SDA-PB9 |
|  LED    |   PC13（板载）  |
| Air780EG |  TX-PA9 RX-PA10|
| ATGM336H | TXPA2 RX-PA3 |
| MPU6050 | SCL-PB10 SDA-PB11 |
|  按键 | PA7 |

## 其他
如有问题可以提issue，看到会回复的，另外如果喜欢请点个star，鼓励开源分享，右上角小星星点一下即可。
