#include "formatutils.h"


QString FormatUtils::SpeedToString(unsigned int speedSize, bool short_mode, SpeedUnit unit, bool hide_unit)
{
    QString str;
    switch (unit)
    {
    case SpeedUnit::AUTO:
        if (short_mode)
        {
            //if (size <= 102)			//小于0.1KB时，显示0K
            //	str = _T("0K");
            /*else */
            if (speedSize < 1024 * 10)					//10KB以下以KB为单位，保留1位小数
               str = QString("%1K").arg(speedSize / 1024.0f, 0, 'f', 1);
            else if (speedSize < 1024 * 1024)			//1MB以下以KB为单位，保留整数
                //str.Format(_T("%.0fK"), speedSize / 1024.0f);
                str = QString("%1K").arg(speedSize / 1024.0f, 0, 'f', 0);
            else if (speedSize < 1024 * 1024 * 1024)		//1GB以下以MB为单位，保留1位小数
                //str.Format(_T("%.1fM"), speedSize / 1024.0f / 1024.0f);
                 str = QString("%1M").arg(speedSize / 1024.0f / 1024.0f, 0, 'f', 1);
            else
                //str.Format(_T("%.2fG"), speedSize / 1024.0f / 1024.0f / 1024.0f);
                str = QString("%1G").arg(speedSize / 1024.0f / 1024.0f / 1024.0f, 0, 'f', 2);
        }
        else
        {
            if (speedSize < 1024 * 10)					//10KB以下以KB为单位，保留2位小数
                //str.Format(_T("%.2fKB"), speedSize / 1024.0f);
                str = QString("%1KB").arg(speedSize / 1024.0f, 0, 'f', 2);
            else if (speedSize < 1024 * 1024)			//1MB以下以KB为单位，保留1位小数
                //str.Format(_T("%.1fKB"), speedSize / 1024.0f);
                str = QString("%1KB").arg(speedSize / 1024.0f, 0, 'f', 1);
            else if (speedSize < 1024 * 1024 * 1024)		//1GB以下以MB为单位，保留2位小数
                //str.Format(_T("%.2fMB"), speedSize / 1024.0f / 1024.0f);
                str = QString("%1MB").arg(speedSize / 1024.0f / 1024.0f, 0, 'f', 2);
            else
                //str.Format(_T("%.2fGB"), speedSize / 1024.0f / 1024.0f / 1024.0f);
                str = QString("%1GB").arg(speedSize / 1024.0f / 1024.0f / 1024.0f, 0, 'f', 2);
        }
        break;
    case SpeedUnit::KBPS:
        if (short_mode)
        {
            if (speedSize < 1024 * 10)					//10KB以下保留1位小数
            {
                if (hide_unit)
                    //str.Format(_T("%.1f"), speedSize / 1024.0f);
                    str = QString("%1").arg(speedSize / 1024.0f, 0, 'f', 1);
                else
                   // str.Format(_T("%.1fK"), speedSize / 1024.0f);
                    str = QString("%1K").arg(speedSize / 1024.0f, 0, 'f', 1);
            }
            else					//10KB以上保留整数
            {
                if (hide_unit)
                    //str.Format(_T("%.0f"), speedSize / 1024.0f);
                    str = QString("%1").arg(speedSize / 1024.0f, 0, 'f', 0);
                else
                    //str.Format(_T("%.0fK"), speedSize / 1024.0f);
                    str = QString("%1K").arg(speedSize / 1024.0f, 0, 'f', 0);
            }
        }
        else
        {
            if (speedSize < 1024 * 10)					//10KB以下保留2位小数
            {
                if (hide_unit)
                    //str.Format(_T("%.2f"), speedSize / 1024.0f);
                    str = QString("%1").arg(speedSize / 1024.0f, 0, 'f', 2);
                else
                   // str.Format(_T("%.2fKB"), speedSize / 1024.0f);
                    str = QString("%1KB").arg(speedSize / 1024.0f, 0, 'f', 2);
            }
            else			//10KB以上保留1位小数
            {
                if (hide_unit)
                    //str.Format(_T("%.1f"), speedSize / 1024.0f);
                    str = QString("%1").arg(speedSize / 1024.0f, 0, 'f', 1);
                else
                   // str.Format(_T("%.1fKB"), speedSize / 1024.0f);
                    str = QString("%1KB").arg(speedSize / 1024.0f, 0, 'f', 1);
            }
        }
        break;
    case SpeedUnit::MBPS:
        if (short_mode)
        {
            if (hide_unit)
                //str.Format(_T("%.1f"), speedSize / 1024.0f / 1024.0f);
                str = QString("%1").arg(speedSize / 1024.0f / 1024.0f, 0, 'f', 1);
            else
                //str.Format(_T("%.1fM"), size / 1024.0f / 1024.0f);
                str = QString("%1M").arg(speedSize / 1024.0f / 1024.0f, 0, 'f', 1);
        }
        else
        {
            if (hide_unit)
                //str.Format(_T("%.2f"), speedSize / 1024.0f / 1024.0f);
                str = QString("%1").arg(speedSize / 1024.0f / 1024.0f, 0, 'f', 2);
            else
                //str.Format(_T("%.2fMB"), speedSize / 1024.0f / 1024.0f);
                str = QString("%1MB").arg(speedSize / 1024.0f / 1024.0f, 0, 'f', 2);
        }
        break;
    }
    return str;
}
