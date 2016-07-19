#include "MainWindow.hpp"
#include <QApplication>

#include "ClassNameReadWrite.hpp"
#include "VirtualClassMaker.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    {

        ClassNameReadWrite rw;
        VirtualClassMaker maker;
        maker.make(rw.readFile(u8R"(D:\x\m\VirtualClassMaker\test.txt)"));
        maker.make_headerin_cpp(rw.readFile(u8R"(D:\x\m\VirtualClassMaker\test.txt)"));
    }

    {
        const auto varReturn=app.exec();
        return varReturn;
    }
}
