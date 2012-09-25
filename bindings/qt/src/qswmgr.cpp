#include "qswmgr.h"
#include "qswutils.h"

#include "qstring.h"
#include "qstringlist.h"

#include "swmgr.h"

#include <string>

QSWMgr::QSWMgr(const QSWMgr &mgr) :
    SWMgr(mgr.configPath, true, mgr.filterMgr) {

}

QSWMgr::QSWMgr(QString &iConfigPath, bool autoload, sword::SWFilterMgr* filterMgr, bool multiMod, bool augmentHome) :
    sword::SWMgr(iConfigPath.toUtf8().constData(),
          autoload,
          filterMgr,
          multiMod,
          augmentHome) {
}

QSWMgr::~QSWMgr() {

}

void QSWMgr::augmentModules(const QString &path, bool multiMod) {
    const char* ccMultiMod = QSWUtils::ToCString(path);

    sword::SWMgr::augmentModules(ccMultiMod, multiMod);
}

void QSWMgr::deleteModule(const QString &module) {
    const char*ccModule = QSWUtils::ToCString(module);

    sword::SWMgr::deleteModule(ccModule);
}

char QSWMgr::filterText(const QString &filterName, QString &text, const sword::SWKey *key, const sword::SWModule *module) {
    const char* ccFilterName = QSWUtils::ToCString(filterName);
    sword::SWBuf buffer = QSWUtils::ToCString(text);

    return sword::SWMgr::filterText(ccFilterName,
                                    buffer,
                                    key,
                                    module);
}

const QString QSWMgr::getGlobalOption(const QString &option) {
    const char* ccOption = QSWUtils::ToCString(option);

    QString optValue = QSWUtils::ToQString(sword::SWMgr::getGlobalOption(ccOption));
    return optValue;
}

QStringList* QSWMgr::getGlobalOptionsQ() {
    sword::StringList   slist = sword::SWMgr::getGlobalOptions();
    QStringList* qslist = new QStringList();
    sword::StringList::iterator it;
    for(it = slist.begin(); it != slist.end(); it++) {
        QString s = QString::fromUtf8(*it);
        qslist->append(s);
    }

    return qslist;
}

sword::SWModule* QSWMgr::getModule(const QString &modName) {
    const char* ccModName = QSWUtils::ToCString(modName);

    return sword::SWMgr::getModule(ccModName);
}

#include "qswmgr.moc"
