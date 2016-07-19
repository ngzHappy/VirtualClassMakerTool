#include "ClassNameReadWrite.hpp"
#include <QtCore/qtextstream.h>
#include <QtCore/qfile.h>
#include <QtCore/qdebug.h>

ClassNameReadWrite::ClassNameReadWrite(const QString & argFileName):
    _fileName_(argFileName){
}

QList<ClassNameReadWrite::ClassName>
ClassNameReadWrite::readFile(const QString & argFileName){
    QFile varFile{argFileName};
    varFile.open(QIODevice::ReadOnly|QIODevice::Text);
    if (varFile.isOpen()==false) { qDebug().noquote()<<"can not open file "<<argFileName; }
    QTextStream varStream{&varFile};
    QList<ClassNameReadWrite::ClassName> ans;
    {
        const static QString split_="$$$";
        while (varStream.atEnd()==false) {
            QString varLine=varStream.readLine();
            auto varNames=varLine.split(split_,QString::SplitBehavior::SkipEmptyParts);
            ClassName varAns;
            const auto varSize=varNames.size();
            if (varSize==1) {
                varAns.className=varNames.takeFirst().trimmed();
            }
            else if (varSize>1) {
                varAns.className=varNames.takeLast().trimmed();
                for (auto & varI:qAsConst(varNames)) {
                    varAns.namespaceNames.push_back(varI.trimmed());
                }
            }
            if (varSize>0) { 
                ans.push_back(std::move(varAns));
            }
        }
    }
    return std::move(ans);
}

ClassNameReadWrite::~ClassNameReadWrite() {}

void ClassNameReadWrite::writeFile(
    const QString & argFileName,
    const QList<ClassName> & argData){
    if (argData.isEmpty()) { return; }
    QFile varFile{argFileName};
    varFile.open(QIODevice::WriteOnly|QIODevice::Text);
    if (varFile.isOpen()==false) { qDebug().noquote()<<"can not open file "<<argFileName; }
    QTextStream varStream{&varFile};

    for (const auto & varI:argData) {
        for (const auto & varNamespace:varI.namespaceNames) {
            varStream<<varNamespace<<"$$$";
        }
        varStream<<varI.className;
        varStream<<endl;
    }

}
