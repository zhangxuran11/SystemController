#include "soft_keyboard_capital_letter.h"

Soft_keyboard_Capital_Letter::Soft_keyboard_Capital_Letter(QWidget *parent) : QWidget(parent)
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");

    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);

    char key_Capital_Letter[26]={
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
        'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
        'Y', 'Z'
    };
    for(int i=0; i<26; i++)
    {
        pushBtn_Capital_Letter[i] = new QPushButton(QString("%1").arg(QString::fromUtf8(&key_Capital_Letter[i], 1)));
        pushBtn_Capital_Letter[i]->setFocusPolicy(Qt::NoFocus);
        pushBtn_Capital_Letter[i]->setFixedHeight(40);
        pushBtn_Capital_Letter[i]->setStyleSheet("color:rgb(39, 83, 102); border:none; border-radius:10px; background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,stop:0 rgba(188, 222, 237, 255), stop:1 rgba(116, 173, 196, 255)); font-size:20px;");
    }

    gridLayout = new QGridLayout;
    gridLayout->setVerticalSpacing(2);
    gridLayout->setHorizontalSpacing(2);
    int rows = 4, columns = 8, btnid = 0;
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            if(btnid < 26){
                gridLayout->addWidget(pushBtn_Capital_Letter[btnid], i, j);
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

Soft_keyboard_Capital_Letter::~Soft_keyboard_Capital_Letter()
{

}

