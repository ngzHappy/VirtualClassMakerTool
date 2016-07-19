#ifndef VIRTUALCLASSMAKER_HPP
#define VIRTUALCLASSMAKER_HPP

#include <QtCore/qstring.h>
#include <QtCore/qlist.h>
#include "ClassNameReadWrite.hpp"

class VirtualClassMaker
{
    VirtualClassMaker(const VirtualClassMaker&)=delete;
    VirtualClassMaker(VirtualClassMaker&&)=delete;
    VirtualClassMaker&operator=(const VirtualClassMaker&)=delete;
    VirtualClassMaker&operator=(VirtualClassMaker&&)=delete;
public:
    VirtualClassMaker();

    QString make(const QList<ClassNameReadWrite::ClassName> &)const;
    QString make_headerin_cpp(const QList<ClassNameReadWrite::ClassName> &)const;
};

#endif // VIRTUALCLASSMAKER_HPP
