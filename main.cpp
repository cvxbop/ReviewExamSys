#include "logindialog.h"
#include <QApplication>
#include "examdialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginDialog w;
    int res=w.exec();
    if(res==QDialog::Rejected) return 0;
    else
    {
        ExamDialog w1;
        w1.show();
        return a.exec();
    }

}
