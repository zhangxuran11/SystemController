#include "soft_keyboard_numbers.h"

Soft_Keyboard_Numbers::Soft_Keyboard_Numbers(QWidget *parent) : QWidget(parent)
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");

    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);

    unsigned int key_Numbers1[10]={
        0x0e50,     //๐
        0x0e51,     //๑
        0x0e52,     //๒
        0x0e53,     //๓
        0x0e54,     //๔
        0x0e55,     //๕
        0x0e56,     //๖
        0x0e57,     //๗
        0x0e58,     //๘
        0x0e59     //๙
    };

    char key_Numbers2[11]={
        '0', '1', '2', '3', '4',
        '5', '6', '7', '8', '9',
        '.'
    };

    for(int i=0; i<21; i++)
    {
        if(i < 10){
            pushBtn_Numbers[i] = new QPushButton(QString("%1").arg(QString::fromUcs4(&key_Numbers1[i], 1)));
        }
        else{
            pushBtn_Numbers[i] = new QPushButton(QString("%1").arg(QString::fromUtf8(&key_Numbers2[i-10], 1)));
        }
        pushBtn_Numbers[i]->setFocusPolicy(Qt::NoFocus);
        pushBtn_Numbers[i]->setFixedHeight(40);
        pushBtn_Numbers[i]->setStyleSheet("color:rgb(39, 83, 102); border:none; border-radius:10px; background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,stop:0 rgba(188, 222, 237, 255), stop:1 rgba(116, 173, 196, 255)); font-size:22px;");
    }

    gridLayout = new QGridLayout;
    gridLayout->setVerticalSpacing(2);
    gridLayout->setHorizontalSpacing(2);
    int rows = 5, columns = 5, btnid = 0;
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            if(btnid < 21){
                gridLayout->addWidget(pushBtn_Numbers[btnid], i, j);
                btnid++;
            }
            else{
                break;
            }
        }
    }

    vBoxLayout = new QVBoxLayout;
    vBoxLayout->addLayout(gridLayout);
    vBoxLayout->addStretch();

    lineEdit = new QLineEdit;
    lineEdit->setText("");
    setLayout(vBoxLayout);
}

Soft_Keyboard_Numbers::~Soft_Keyboard_Numbers()
{

}
