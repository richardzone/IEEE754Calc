#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //ui->binaryInputLineEdit->setEnabled(false);
    QPalette palette;
    palette.setColor(ui->binaryInputLineEdit->backgroundRole(), Qt::lightGray);
    ui->binaryInputLineEdit->setPalette(palette);
    goodStatIndicator = true;      //Not implemented; see header file

    floatBinaryMask = new QString("BBBB BBBB BBBB BBBB BBBB BBBB BBBB BBBB");
    doubleBinaryMask = new QString("BBBB BBBB BBBB BBBB BBBB BBBB BBBB BBBB BBBB BBBB BBBB BBBB BBBB BBBB BBBB BBBB");
    ui->floatLineEdit->setInputMask(*floatBinaryMask);
    ui->doubleLineEdit->setInputMask(*doubleBinaryMask);

    ui->inputLineEdit->setMaxLength(12);
    ui->inputLineEdit->setValidator(new QDoubleValidator());
    ui->inputLineEdit->setText("23.85");
    ui->inputLineEdit->setFocus();
    on_inputLineEdit_textEdited(ui->inputLineEdit->text());
    //ui->inputLineEdit->setFocus();

    //on_inputLineEdit_textEdited(ui->inputLineEdit->text());
    //on_inputLineEdit_textEdited(ui->inputLineEdit->text());

    //inputLineEditBeforeText = ui->inputLineEdit->text();


}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_quitButton_clicked()
{
    QCoreApplication::quit();
}

void Widget::calc(double d, int expBitsCount, int fracBitsCount, QLineEdit *destLineEdit) {
    int bias = pow((double)2, expBitsCount - 1) - 1;
    int e = 0;
    QString sign = (d < 0) ?  "1" : "0";
    d = abs(d);

    long intpart = (long) d;
    //qDebug() << intpart;
    double mantissa = d - intpart;

    QString frac1;
    QString binary_intpart;
    if(intpart == 0) {
        binary_intpart = "0";
    } else {
        binary_intpart = QString::number(intpart, 2);
        frac1 = binary_intpart.mid(1);
    }

    //ui->floatLineEdit->setText(frac1);

    //convert fraction part to binary
    double num = mantissa;
    QString frac2;
    for(int i = 0; i < (fracBitsCount - frac1.length()); i++) {
        num *= 2;
        if(num >= 1) {
            num -= 1;
            frac2 += "1";
        } else {
            frac2 += "0";
        }
    }

    if(frac2 == ""){
        ui->binaryInputLineEdit->setText(binary_intpart + ".0");
    } else {
        ui->binaryInputLineEdit->setText(binary_intpart + "." + frac2);
    }

    if(intpart == 0) {
        e = -(frac2.indexOf("1") + 1);
        frac2 = frac2.mid(frac2.indexOf("1") + 1);
        assert(frac2.length() <= fracBitsCount);
        while(frac2.length() != fracBitsCount) {
            frac2.append("0");
        }
    } else {
        e = frac1.length();
    }

    QString exp = QString::number(bias + e, 2);

    assert(exp.length() <= expBitsCount);
    while(exp.length() != expBitsCount) {
        exp.prepend("0");
    }

    QString frac = frac1 + frac2;
    if(frac.length() != fracBitsCount) {
        assert(frac.length() > fracBitsCount);
        frac.truncate(fracBitsCount);           //Is it correct to do so??
    }
    assert(frac.length() == fracBitsCount);

    destLineEdit->setText(sign + exp + frac);
}

void Widget::reverseCalc(const QString &s, int expBitsCount, int fracBitsCount, QLineEdit *destLineEdit)
{
    int sign = ((s.at(0).toAscii() == '0') ? 1 : -1);
    int bias = pow((double)2, expBitsCount - 1) - 1;

    //QString s = str;      //remove CONSTness
    //s.replace(QString(" "),QString());    //remove spaces between every 4 binary digits

    QString strExp = s.mid(1, expBitsCount);
    long exp = 0;
    QString strFrac = s.right(fracBitsCount);
    qDebug() << "strExp: " << strExp;
    qDebug() << "strFrac: " << strFrac;
    quint64 frac = 0;

    //Convert binary exp part to decimal
    //Could use: int QString::toInt ( bool * ok = 0, int base = 10 ) instead
    for(int i = 0; i < strExp.length(); i++) {
        assert(strExp.at(i).digitValue() != -1);
        exp += strExp.at(i).digitValue();
        if(i != (strExp.length() - 1))
            exp *= 2;
    }
    //Convert binary frac part to decimal
    //Could use: int QString::toInt ( bool * ok = 0, int base = 10 ) instead
    for(int i = 0; i < strFrac.length(); i++) {
        assert(strFrac.at(i).digitValue() != -1);
        frac += strFrac.at(i).digitValue();
        if(i != (strFrac.length() - 1))
            frac *= 2;
    }

    qDebug() << "Exp: " << exp;
    qDebug() << "Frac: " << frac;

    qDebug() << sign;
    qDebug() << (1 + frac * pow((double)2,-fracBitsCount));
    qDebug() << pow((double)2, exp - bias);

    QString result = QString::number(sign * (1 + frac * pow((double)2,-fracBitsCount)) * pow((double)2, exp - bias));
    qDebug() << "reverseCalc Result: " << result;
    destLineEdit->setText(result);
}

void Widget::on_inputLineEdit_textEdited(const QString &arg1)
{
    qDebug() << "inputLineEdit text edited";

    bool ok = false;
    d = arg1.toDouble(&ok);
    qDebug() << "input number: " << d;
    if(!ok || arg1.isEmpty()) {
        qDebug() << "Not a Valid inputLineEdit value";
        //ui->inputLineEdit->setText(inputLineEditBeforeText);
        ui->floatLineEdit->clear();
        ui->doubleLineEdit->clear();
        return;
    }

    if(!ui->doubleLineEdit->hasFocus())
        calc(d, 11, 52, ui->doubleLineEdit);
    if(!ui->floatLineEdit->hasFocus())
        calc(d, 8, 23, ui->floatLineEdit);
}

void Widget::on_floatLineEdit_textEdited(const QString &arg1)
{
    qDebug() << "floatLineEdit text edited";
    if(arg1.length() != 39) {
        qDebug() << "Not a Vaild floatLineEdit value";
        if(!floatBinaryMask->endsWith(";_")){
            int cur = ui->floatLineEdit->cursorPosition();
            floatBinaryMask->append(";_");
            ui->floatLineEdit->setInputMask(*floatBinaryMask);
            ui->floatLineEdit->setCursorPosition(cur);
        }
        qDebug() << "Length: " << arg1.length() << " != " << floatBinaryMask->length();
        qDebug() << arg1;
        ui->inputLineEdit->clear();
        ui->doubleLineEdit->clear();
        return;
    }

    //If the value is OK, restore original input mask
    if(floatBinaryMask->endsWith(";_")) {
        floatBinaryMask->truncate(floatBinaryMask->length() - 2);
        ui->floatLineEdit->setInputMask(*floatBinaryMask);
    }
    QString s = arg1;      //remove CONSTness
    s.replace(QString(" "),QString());    //remove spaces between every 4 binary digits

    if(!ui->inputLineEdit->hasFocus())
        reverseCalc(s, 8, 23, ui->inputLineEdit);
    if(!ui->doubleLineEdit->hasFocus())
        on_inputLineEdit_textEdited(ui->inputLineEdit->text());

    //bool ok = false;
    //ui->inputLineEdit->text().toDouble(ok)
    //calc(ui->inputLineEdit->text().toDouble(), 11, 52, ui->doubleLineEdit);
    /*
    if(!goodStatIndicator || text.isEmpty()) {
        qDebug() << "Not a Valid inputLineEdit value";
        //ui->inputLineEdit->setText(inputLineEditBeforeText);
        ui->floatLineEdit->clear();
        ui->doubleLineEdit->clear();
        return;
    }
    */
}

void Widget::on_doubleLineEdit_textEdited(const QString &arg1)
{
    qDebug() << "doubleLineEdit text edited";
    if(arg1.length() != 79) {
        qDebug() << "Not a Vaild doubleLineEdit value";
        if(!doubleBinaryMask->endsWith(";_")){
            int cur = ui->doubleLineEdit->cursorPosition();
            doubleBinaryMask->append(";_");
            ui->doubleLineEdit->setInputMask(*doubleBinaryMask);
            ui->doubleLineEdit->setCursorPosition(cur);
        }
        //qDebug() << "Length: " << s.length() << " != " << doubleBinaryMask->length();
        //qDebug() << arg1;
        ui->inputLineEdit->clear();
        ui->floatLineEdit->clear();
        return;
    }
    //If the value is OK, restore original input mask
    if(doubleBinaryMask->endsWith(";_")) {
        doubleBinaryMask->truncate(doubleBinaryMask->length() - 2);
        ui->doubleLineEdit->setInputMask(*doubleBinaryMask);
    }

    QString s = arg1;      //remove CONSTness
    s.replace(QString(" "),QString());    //remove spaces between every 4 binary digits

    if(!ui->inputLineEdit->hasFocus())
        reverseCalc(s, 11, 52, ui->inputLineEdit);
    if(!ui->floatLineEdit->hasFocus())
        on_inputLineEdit_textEdited(ui->inputLineEdit->text());
}

void Widget::on_aboutButton_clicked()
{
    QMessageBox::about(this, tr("明天，你好"), tr("任何建议和bug report，\n敬请访问 www.yiran.org\n\nTurner(vip@tingsoft.org)"));
}
