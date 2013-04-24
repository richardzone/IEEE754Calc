#include <QtGui/QApplication>
#include <QTextCodec>
#include <QPlastiqueStyle>
#include "widget.h"

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    QApplication::setStyle(new QPlastiqueStyle);
    QApplication a(argc, argv);


    QTranslator qtTranslator;
    if(qtTranslator.load("qt_" + QLocale().uiLanguages().at(0)))
        a.installTranslator(&qtTranslator);
    else
        qDebug() << "qt_" + QLocale().uiLanguages().at(0) << "load failed!!";

    QTranslator calcTranslator;
    if(calcTranslator.load("ieee754_" + QLocale().uiLanguages().at(0)))
        a.installTranslator(&calcTranslator);
    else
        qDebug() << "ieee754_" + QLocale().uiLanguages().at(0) << "load failed!!";


    Widget w;
    w.show();
    
    return a.exec();
}
