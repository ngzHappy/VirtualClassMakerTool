#ifndef CLASSNAMEREADWRITE_HPP
#define CLASSNAMEREADWRITE_HPP

#include <QtCore/qstring.h>
#include <QtCore/qlist.h>

class ClassNameReadWrite
{
    ClassNameReadWrite(ClassNameReadWrite&)=delete;
    ClassNameReadWrite(ClassNameReadWrite&&)=delete;
    ClassNameReadWrite&operator=(ClassNameReadWrite&)=delete;
    ClassNameReadWrite&operator=(ClassNameReadWrite&&)=delete;
public:
    class ClassName{
    public:
        QString className;
        QStringList namespaceNames;
        QString makeFullName() const{
            QString ans;
            for (const auto & varNamespace:qAsConst(namespaceNames)) {
                ans+=varNamespace.toUpper()+"_";
            }
            return ans+className;
        }
        QString makeFullNamespace() const{
            QString ans;
            for (const auto & varNamespace:qAsConst(namespaceNames)) {
                ans+=varNamespace+"::";
            }
            return std::move(ans);
        }
    };
    QString _fileName_;

public:
    ClassNameReadWrite(const QString &/*fileName*/="");
    ~ClassNameReadWrite();

    QList<ClassName> readFile(const QString &/*fileName*/);
    QList<ClassName> readFile(){return readFile(_fileName_);}

    void writeFile(const QString &/*fileName*/,const QList<ClassName>&);

};

#endif // CLASSNAMEREADWRITE_HPP
