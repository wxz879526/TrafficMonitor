#include "globalobjects.h"
#include <QFontDatabase>

QFont GlobalObjects::iconFont;

void GlobalObjects::init()
{
    int fontId = QFontDatabase::addApplicationFont(":/res/iconfont.ttf");
    QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
    iconFont.setFamily(fontFamilies.at(0));
}
