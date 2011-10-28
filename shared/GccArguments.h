#ifndef ARGS_H
#define ARGS_H

#include <QHash>
#include <QByteArray>
#include <QDataStream>
#include <QSharedData>
#include <QSharedDataPointer>
#include <QDebug>
#include "Path.h"

class GccArguments
{
public:
    enum Language { LangUndefined, LangC, LangCPlusPlus, LangObjC,
                    LangObjCPlusPlus, LangCPlusPlusHeader, LangHeader };

    GccArguments();
#ifdef WITH_GPERF
    bool parse(const QByteArray& cmd, const Path &resolvedPath);
#endif
    QByteArray raw() const;
    bool isNull() const;
    bool isEmpty() const;
    QString errorString() const;

    QList<QByteArray> arguments() const;
    QList<QByteArray> arguments(const QByteArray& prefix) const;
    int argumentCount() const;
    QList<Path> includePaths() const;
    enum ClangArgFlag {
        IncludePaths = 0x01,
        Defines = 0x02,
        OtherArgs = 0x04,
        AllArgs = IncludePaths|Defines|OtherArgs
    };
    int getClangArgs(const char **args, int max, unsigned flags) const;

    void setPreprocess(bool pre);
    void setLanguage(Language language);

    QByteArray compiler() const;
    QList<Path> input() const;
    QByteArray firstInput() const;
    QByteArray output() const;
    Language language() const;

    bool hasInput() const;
    bool hasOutput() const;

    bool isCompile() const;
    bool operator==(const GccArguments &other) const;
    static const char* languageString(Language language);
 private:
    Path parseCD(const QByteArray& cmd, const Path& path) const;

    class Data : public QSharedData
    {
    public:
        Data();

        GccArguments::Language guessLanguage() const;

        struct Argument
        {
            Argument(int p = 0, const QByteArray& a = QByteArray(), const QByteArray& v = QByteArray())
                : pos(p), arg(a), value(v)
            {}

            int pos;
            QByteArray arg;
            QByteArray value;
        };

        QList<int> input;
        int output;
        int x;
        int c;
        QString error;
        Language language;

        QByteArray raw;
        QList<Argument> args;
    };

    QSharedDataPointer<Data> m_ptr;

    friend QDataStream& operator<<(QDataStream& stream, const GccArguments& args);
    friend QDataStream& operator>>(QDataStream& stream, GccArguments& args);
    friend QDataStream& operator<<(QDataStream& stream, const GccArguments::Data::Argument& args);
    friend QDataStream& operator>>(QDataStream& stream, GccArguments::Data::Argument& args);
};

static inline QDebug operator<<(QDebug dbg, const GccArguments &args)
{
    dbg.nospace() << "GccArguments(" << args.raw() << ")";
    return dbg.maybeSpace();
}

QDataStream& operator<<(QDataStream& stream, const GccArguments& args);
QDataStream& operator>>(QDataStream& stream, GccArguments& args);

QDataStream& operator<<(QDataStream& stream, const GccArguments::Data::Argument &arg);
QDataStream& operator>>(QDataStream& stream, GccArguments::Data::Argument &arg);

#endif
