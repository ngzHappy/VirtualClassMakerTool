#include "MainWindow.hpp"
#include "VirtualClassMaker.hpp"
#include "ClassNameReadWrite.hpp"
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qformlayout.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qfiledialog.h>
#include <QtCore/qfile.h>
#include <QtCore/qtextcodec.h>
#include <QtCore/qtextstream.h>
#include <QtCore/qdebug.h>

namespace {

class Widget :public QWidget{
    QLineEdit * inputFileName_;
    QLineEdit * outPutDirName_;
public:
    Widget() {
    
        auto varLayout=new QFormLayout;
        this->setLayout(varLayout);

        {
            auto varHLayout=new QHBoxLayout;
            varLayout->addRow(QString::fromUtf8(u8R"(输入文件名)"),varHLayout);

            auto varLineEdit=new QLineEdit;
            varHLayout->addWidget(varLineEdit);
            
            auto varPushButton=new QPushButton(QString::fromUtf8(u8R"(选择文件)"));
            varHLayout->addWidget(varPushButton);

            connect(varPushButton,&QPushButton::pressed,this,
                [varLineEdit]() {
                QString fileName= QFileDialog::getOpenFileName();
                if (fileName.isEmpty()) { return; }
                varLineEdit->setText(fileName);
            });

            inputFileName_=varLineEdit;

        }

        {
            auto varHLayout=new QHBoxLayout;
            varLayout->addRow(QString::fromUtf8(u8R"(输出文件夹)"),varHLayout);

            auto varLineEdit=new QLineEdit;
            varHLayout->addWidget(varLineEdit);

            auto varPushButton=new QPushButton(QString::fromUtf8(u8R"(选择文件夹)"));
            varHLayout->addWidget(varPushButton);

            connect(varPushButton,&QPushButton::pressed,this,
                [varLineEdit]() {
                QString fileName= QFileDialog::getExistingDirectory();
                if (fileName.isEmpty()) { return; }
                varLineEdit->setText(fileName);
            });

            outPutDirName_=varLineEdit;

        }

        {
            auto varButton=new QPushButton(QString::fromUtf8(u8R"(执行)"));
            varLayout->addRow(varButton);

            connect(varButton,&QPushButton::pressed,this,
                [this]() {

                QString header,in_cpp_header;

                {
                    ClassNameReadWrite varReadWrite;
                    auto varFile=varReadWrite.readFile(inputFileName_->text());

                    if (varFile.isEmpty()) { return; }

                    VirtualClassMaker varClassMaker;
                    header=varClassMaker.make(varFile);
                    in_cpp_header=varClassMaker.make_headerin_cpp(varFile);;
                }

                QString dirName=outPutDirName_->text();
                {
                    QFile file(dirName+"/VirtualClass.hpp");
                    file.open(QIODevice::WriteOnly|QIODevice::Text);
                    if (false==file.isOpen()) { 
                        qDebug()<<"can not create"<<(dirName+"/VirtualClass.hpp");
                        return; 
                    }
                    QTextStream varStream(&file);
                    varStream.setCodec("UTF-8");
                    varStream.setGenerateByteOrderMark(true);
                    varStream<<header;
                }

                {
                    QFile file(dirName+"/JustInCppFileVirtualClass.hpp");
                    file.open(QIODevice::WriteOnly|QIODevice::Text);
                    if (false==file.isOpen()) { 
                        qDebug()<<"can not create"<<(dirName+"/JustInCppFileVirtualClass.hpp");
                        return; 
                    }
                    QTextStream varStream(&file);
                    varStream.setCodec("UTF-8");
                    varStream.setGenerateByteOrderMark(true);
                    varStream<<in_cpp_header;
                }

            });

        }
    }
};

}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(600,300);
    setCentralWidget(new Widget);
}

MainWindow::~MainWindow()
{

}


