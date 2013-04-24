#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtGui>
#include <cassert>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    
private slots:

    void calc(double d, int expBitsCount, int fracBitsCount, QLineEdit *destLineEdit);
    void reverseCalc(const QString &s, int expBitsCount, int fracBitsCount, QLineEdit *destLineEdit);
    void on_quitButton_clicked();

    void on_inputLineEdit_textEdited(const QString &arg1);

    void on_floatLineEdit_textEdited(const QString &arg1);

    void on_doubleLineEdit_textEdited(const QString &arg1);

    //void inputLineEdit_helper(const QString &arg1);

    void on_aboutButton_clicked();

private:
    Ui::Widget *ui;

    //Could be used to indicate whether an overflow or special case occured
    //Not Implemented
    bool goodStatIndicator;

    QString *floatBinaryMask;
    QString *doubleBinaryMask;

    double d;

    //QString inputLineEditBeforeText;
};

#endif // WIDGET_H
