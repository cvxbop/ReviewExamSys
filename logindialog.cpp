#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QTextStream>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("驾校科目一考试登录");
    this->setWindowFlags(Qt::WindowCloseButtonHint);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_pushButtonLogin_clicked()
{
    //得到输入的账号和密码
    QString inputAccount=ui->lineEditAccount->text();
    QString inputPassword=ui->lineEditPassword->text();

    //正则验证邮箱地址 用户名@域名
    //元字符解释:^表示规则字符串的开始 $表示规则字符串的结束
    //+表示匹配次数≥1次 *表示匹配任意次数(可为0次) {n,m}表示匹配次数至少n次,至多m次
    QRegExp rx("^[A-Za-z0-9]+([_.][A-Za-z0-9]+)*@([A-Za-z0-9\-]+[.])+[A-Za-z]{2,6}$");
     //账号格式匹配不成功
    if(!rx.exactMatch(inputAccount))
    {
        QMessageBox::warning(this,"提示","账号邮箱格式错误!");
        ui->lineEditAccount->clear();
        ui->lineEditPassword->clear();
        ui->lineEditAccount->setFocus();
        return;
    }

    qDebug()<<"账号格式匹配成功";
    //按行读取txt文件账号密码
    QString lineStr;                    //保存读取整行数据
    QString accountStr;                 //保存读取到的账号
    QString passwordStr;                //保存读取的密码
    QStringList list;                   //保存账号+密码
    QFile file("../account.txt");
    QTextStream stream(&file);          //让文本流与文件产生关联
    if(!file.open(QIODevice::ReadOnly)) //只读方式打开
    {
        QMessageBox::warning(this,"警告","账号文件打开失败!");
        return;
    }

    while(!stream.atEnd())
    {
        lineStr=stream.readLine();
        list=lineStr.split(",");    //以","分割,保存进list
        accountStr=list.at(0);
        passwordStr=list.at(1);
        if(accountStr==inputAccount)
        {
        //账号一致
            //是否密码也一致
            if(passwordStr==inputPassword)
            {
                QMessageBox::information(this,"提示","欢迎进入科目一考试系统!");
                file.close();
                //进入下一个界面
                done(Accepted);
                return;
            }
            else
            {
                QMessageBox::warning(this,"提示","请输入正确的密码!");
                ui->lineEditPassword->clear();
                ui->lineEditPassword->setFocus();
                file.close();
                return;
            }
        }
    }
    //如果还是没有正确的账号
    QMessageBox::warning(this,"提示","请输入正确的账号!");
    ui->lineEditAccount->clear();
    ui->lineEditPassword->clear();
    ui->lineEditAccount->setFocus();
    file.close();
    return;
}

void LoginDialog::on_pushButtonClose_clicked()
{
    done(Rejected);
}
