#include "soft_keyboard_symbol.h"

Soft_Keyboard_Symbol::Soft_Keyboard_Symbol(QWidget *parent) : QWidget(parent)
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");

    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);

    char key_Symbol1[32]={
        33, 34, 35, 36, 37, 38, 39, 40,
        41, 42, 43, 44, 45, 46, 47, 58,
        59, 60, 61, 62, 63, 64, 91, 92,
        93, 94, 95, 96, 123, 124, 125,126
    };

//    unsigned int key_Symbol2[6]={
//        '\xff\x01',
//        '\xff\x02',
//        '\xff\x03',
//        '\xff\x04',
//        '\xff\x05',
//        '\xff\x06',
//        '\xff\x07',
//        '\xff\x08',
//        '\xff\x09',
//        '\xff\x0a',
//        '\xff\x0b',
//        '\xff\x0c',
//        '\xff\x0d',
//        '\xff\x0e',
//        '\xff\x0f',
//        '\xff\x1a',
//        '\xff\x1b',
//        '\xff\x1c',
//        '\xff\x1d',
//        '\xff\x1e',
//        '\xff\x1f',
//        '\xff\x20',
//        '\xff\x3b',
//        '\xff\x3c',
//        '\xff\x3d',
//        '\xff\x3e',
//        '\xff\x3f',
//        '\xff\x40',
//        '\xff\x5b',
//        '\xff\x5c',
//        '\xff\x5d',
//        '\xff\x5e',
//        '\xff\x5f',     // ｟
//        '\xff\x60',     // ｠
//        '\xff\x61',     // ｡
//        '\xff\x64',     // ､
//        '\xff\x65',     // ･
//        '\xff\x70'      // ｰ
//    };

    for(int i=0; i<38; i++){
        if(5 == i){
            pushBtn_Symbol[i] = new QPushButton(QString("&&"));
        }
        else{
            pushBtn_Symbol[i] = new QPushButton(QString("%1").arg(QString::fromUtf8(&key_Symbol1[i], 1)));
        }
        pushBtn_Symbol[i]->setFocusPolicy(Qt::NoFocus);
        pushBtn_Symbol[i]->setFixedHeight(40);
        pushBtn_Symbol[i]->setStyleSheet("color:rgb(39, 83, 102); border:none; border-radius:10px; background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,stop:0 rgba(188, 222, 237, 255), stop:1 rgba(116, 173, 196, 255)); font-size:20px;");
    }

    gridLayout = new QGridLayout;
    gridLayout->setVerticalSpacing(2);
    gridLayout->setHorizontalSpacing(2);
    int rows = 5, columns = 8, btnid = 0;
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            if(btnid < 32){
                gridLayout->addWidget(pushBtn_Symbol[btnid], i, j);
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

Soft_Keyboard_Symbol::~Soft_Keyboard_Symbol()
{

}

