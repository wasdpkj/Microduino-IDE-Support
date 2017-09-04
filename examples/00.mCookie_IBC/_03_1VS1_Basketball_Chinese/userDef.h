#define DEBUG        0       //串口调试功能  打开/关闭.
#define PIN_KEY      6       //碰撞开关引脚.
#define PIN_BUZZER   10      //蜂鸣器引脚.
#define PIN_LED      12      //彩灯引脚.
#define PIN_LINEA    A0      //灰度传感器A引脚.
#define PIN_LINEB    A2      //灰度传感器B引脚.
#define LED_NUM      2

//=================================自己动手修改参数!========================================//
//示例: 将 "#define BRIGHT_MAX 128" 修改成 "#define BRIGHT_MAX 180".                        //
//         意味着LED彩灯的显示亮度会变亮.                                                   //
//                                                                                          //
//提示: 参数修改完后，记得将代码重新上传到mBattery模块！                                    //
//==========================================================================================//
#define BRIGHT_MAX  128      //LED彩灯显示的最大亮度。最大亮度是255，最小亮度是0

#define DOCK_MAX     500     //灰度传感器的最小触发值。增大该值后，意味着你需要颜色更深的物体去触发灰度传感器

#define SCORE_MAX    5       //判断游戏获胜的分差，默认为5分，即当一方得分比另一方得分多5分时，得分多者获胜！

#define leftColor   COLOR_GREEN     //左灯颜色
#define rightColor   COLOR_PURPLE    //右灯颜色
