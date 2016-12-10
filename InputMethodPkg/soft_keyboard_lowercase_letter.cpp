#include "soft_keyboard_lowercase_letter.h"

Soft_Keyboard_Lowercase_Letter::Soft_Keyboard_Lowercase_Letter(QWidget *parent) : QWidget(parent)
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");

    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);

    char key_Lowercase_Letter[26]={
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
        'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
        'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
        'y', 'z'
    };

    for(int i=0; i<26; i++)
    {
        pushBtn_Lowercase_Letter[i] = new QPushButton(QString("%1").arg(QString::fromUtf8(&key_Lowercase_Letter[i], 1)));
        pushBtn_Lowercase_Letter[i]->setFocusPolicy(Qt::NoFocus);
        pushBtn_Lowercase_Letter[i]->setFixedHeight(40);
        pushBtn_Lowercase_Letter[i]->setStyleSheet("color:rgb(39, 83, 102); border:none; border-radius:10px; background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,stop:0 rgba(188, 222, 237, 255), stop:1 rgba(116, 173, 196, 255)); font-size:22px;");
    }

    gridLayout = new QGridLayout;
    gridLayout->setVerticalSpacing(2);
    gridLayout->setHorizontalSpacing(2);
    int rows = 4, columns = 8, btnid = 0;
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            if(btnid < 26){
                gridLayout->addWidget(pushBtn_Lowercase_Letter[btnid], i, j);
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

    setLayout(vBoxLayout);
}

Soft_Keyboard_Lowercase_Letter::~Soft_Keyboard_Lowercase_Letter()
{

}

