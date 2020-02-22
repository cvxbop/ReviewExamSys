#include "examdialog.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include <QFont>
#include <QPainter>
#include <QImage>
#include <QApplication>

ExamDialog::ExamDialog(QWidget *parent):QDialog(parent)
{
    setWindowTitle("考试已用时: 0分0秒");
    setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    QFont font;
    font.setPointSize(12);
    this->setFont(font);
    setPalette(QPalette(QColor(209,215,255)));

    resize(900,900);
    timeInit();    
    layoutInit();

    if(!textEditInit())
    {
        QMessageBox::information(this,"提示","初始化题库数据文件失败!");
        QTimer::singleShot(0,qApp,SLOT(quit())) ;
    }
    buttonsInit();


}

void ExamDialog::refreshTime()  //计时器触发timeout信号,执行的槽函数
{
    static int timeGo=0;        //timeGo表示时间增加的数值
    timeGo++;
    QString str=QString("考试已用时: %1分%2秒").arg(timeGo/60).arg(timeGo%60);
    setWindowTitle(str);
}

void ExamDialog::timeInit()     //初始化计时功能
{
    //计时器
    timer.setInterval(1000);
    timer.start();
    connect(&timer,&QTimer::timeout,this,&ExamDialog::refreshTime);

}

bool ExamDialog::textEditInit()
{
    textEdit=new QTextEdit(this);
    textEdit->setReadOnly(true);
    QFile file("../exam.txt");
    QTextStream stream(&file);
    stream.setCodec("UTF-8");

    if(file.open(QIODevice::ReadOnly))
    {
        int lineCount=0;            //文本行数
        QString textStr;            //整个试题保存在此字符串中
        while(!stream.atEnd())
        {
            QString str=stream.readLine();            
            if((lineCount%6==0&&lineCount!=0)||lineCount==58)
            {
                QStringList list=str.split(" ");
                answerList.push_back(list.at(1));
                qDebug()<<list.at(1);
                lineCount++;
                textStr+="\n";
            }
            else
            {
                textStr+=str;
                textStr+="\n";
                lineCount++;
            }
        }
        textEdit->setText(textStr);
        layout->addWidget(textEdit,0,0,1,10);
        file.close();
        return true;
    }
    else return false;
}

void ExamDialog::layoutInit()
{
    layout=new QGridLayout(this);
    layout->setSpacing(10);   //设置控件间的间距
    layout->setMargin(10);    //设置窗体与控件间的间隙
}

void ExamDialog::buttonsInit()
{
    for(int i=0;i<10;i++)
    {
        labels[i]=new QLabel(this);
        labels[i]->setText(QString("第%1题").arg(i+1));
        layout->addWidget(labels[i],1,i);
    }

    QStringList choices;
    choices.push_back("A");
    choices.push_back("B");
    choices.push_back("C");
    choices.push_back("D");

    for(int i=0;i<10;i++)
    {
        if(i<8)
        {
            buttonGroups[i]=new QButtonGroup(this);
            for(int j=0;j<4;j++)
            {
                radiobtnsFront[i*4+j]=new QRadioButton(this);
                radiobtnsFront[i*4+j]->setText(choices.at(j));
                layout->addWidget(radiobtnsFront[i*4+j],j+2,i);
                buttonGroups[i]->addButton(radiobtnsFront[i*4+j]);
            }
        }
        else if(i==8)
        {
            for(int j=0;j<4;j++)
            {
                checkBoxs[j]=new QCheckBox(this);
                checkBoxs[j]->setText(choices.at(j));
                layout->addWidget(checkBoxs[j],j+2,8);
            }
        }
        else
        {
            buttonGroups[8]=new QButtonGroup(this);
            radiobtnsBack[0]=new QRadioButton(this);
            radiobtnsBack[0]->setText("正确");
            layout->addWidget(radiobtnsBack[0],2,9);
            buttonGroups[8]->addButton(radiobtnsBack[0]);

            radiobtnsBack[1]=new QRadioButton(this);
            radiobtnsBack[1]->setText("错误");
            layout->addWidget(radiobtnsBack[1],3,9);
            buttonGroups[8]->addButton(radiobtnsBack[1]);
        }
    }
    pushButton=new QPushButton(this);
    pushButton->setText("提交");
    layout->addWidget(pushButton,6,9);
}

void ExamDialog::getScores()
{
    if(hasNoSelect())
    {
        int score=0;

        for(int i=0;i<10;i++)
        {
           if(i<8)
           {
                if(buttonGroups[i]->checkedButton()->text()== answerList.at(i))
                {
                   score+=10;
                   //qDebug()<<i+1<<"##"<<buttonGroups[i]->checkedButton()->text();
                   //qDebug()<<answerList.at(i);
                }
//                else
//                {
//                    qDebug()<<i+1<<"##"<<buttonGroups[i]->checkedButton()->text();
//                    qDebug()<<answerList.at(i);
//                }
           }
           if(i==9)
           {
              if(buttonGroups[8]->checkedButton()->text()== answerList.at(i))
                  score+=10;
           }
           if(i==8)
           {
               QString strBoxAnswers=NULL;
               for(int i=0;i<4;i++)
               {
                   if(checkBoxs[i]->isChecked())
                   {
                       strBoxAnswers+=checkBoxs[i]->text();                      
                   }
               }
               qDebug()<<"strBoxAnswers:"<<strBoxAnswers;
               qDebug()<<"answerList.at(8):"<<answerList.at(8);
               if(strBoxAnswers==answerList.at(8))
                   score+=10;
           }
        }
        QString strScores=QString("您的最终得分为%1,是否选择再次答题?").arg(score);
        int res=QMessageBox::information(this,"提示",strScores,QMessageBox::Yes|QMessageBox::No);
        if(res==QMessageBox::Yes)
        {           
            return;
        }
        else close();
    }
    else
    {
        QMessageBox::warning(this,"提示","您有题目未答完!");
        return;
    }

}

bool ExamDialog::hasNoSelect()
{
    int selectBtnNum=0;
    for(int i=0;i<9;i++)
    {
        if(buttonGroups[i]->checkedButton())
            selectBtnNum++;
    }
    if(selectBtnNum<9)
        return false;

    selectBtnNum=0;
    for(int i=0;i<4;i++)
    {
       if(checkBoxs[i]->isChecked())
       {
          selectBtnNum++;
       }
    }
    if(selectBtnNum==0)
        return false;
    return true;
}










