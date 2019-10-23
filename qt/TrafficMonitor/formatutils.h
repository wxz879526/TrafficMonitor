#ifndef FORMATUTILS_H
#define FORMATUTILS_H

#pragma execution_character_set("UTF-8")

#include <QString>

//网速单位
enum class SpeedUnit
{
    AUTO,		//自动
    KBPS,		//KB/s
    MBPS		//MB/s
};

class FormatUtils final
{
public:
    /*根据数据的大小转换成以KB、MB、GB为单位的字符串
        size：数据的大小，单位为字节
        short_mode：是否使用精简模式（减小小数点位数，单位不显示“B”）
        speed_unit：数据的单位，可以是自动、KB或MB
        hide_unit：是否隐藏单位
        返回值：转换后的字符串
        */
    static QString SpeedToString(unsigned int speedSize, bool short_mode = false, SpeedUnit unit = SpeedUnit::AUTO, bool hide_unit = false);

};

#endif // FORMATUTILS_H
