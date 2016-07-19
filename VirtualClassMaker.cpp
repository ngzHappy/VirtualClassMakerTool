#include "VirtualClassMaker.hpp"
#include <QtCore/qdebug.h>


VirtualClassMaker::VirtualClassMaker()
{

}


namespace {
const QString empty_file=QString::fromUtf8(u8R"___(
#ifndef _PRAGMA_ONCE_VIRTUAL_CLASS__HPP_
#define _PRAGMA_ONCE_VIRTUAL_CLASS__HPP_

#include <utility>
#include <cstddef>
#include <cstdint>
#include <cstdbool>
#include <type_traits>

class VirtualClass {
    using _Type=std::intptr_t;
public:

    enum class Type:_Type{
        IndexVoid,
        IndexConstVoid,
    }_type_;

private:
    union  {
        void * _data_void_;
    };

public:

    VirtualClass() :_type_(Type::IndexVoid),_data_void_(nullptr){}
    VirtualClass(decltype(nullptr)):VirtualClass() {}

    bool isConstPointer() const { return static_cast<_Type>(_type_)&1; }
    Type type() const{ return _type_; }
    void * data() const { return _data_void_; }

    /*defined in JustInCppFileVirtualClass.hpp*/
    template<typename Type>Type getData() const{return nullptr;}

};
)___");

}

QString VirtualClassMaker::make(
    const QList<ClassNameReadWrite::ClassName> &arg) const{
    if(arg.isEmpty()){return empty_file;}

    QString ans;
    ans.reserve(1024*1024);

    {
        const static QString first_part_0=QString::fromUtf8(u8R"___(
#ifndef _PRAGMA_ONCE_VIRTUAL_CLASS__HPP_
#define _PRAGMA_ONCE_VIRTUAL_CLASS__HPP_

#include <utility>
#include <cstddef>
#include <cstdint>
#include <cstdbool>
#include <type_traits>

)___");
        ans+=first_part_0;

        {/*导入声明类*/
            for (const auto & varI:arg) {
                for (const auto & varNamespace:varI.namespaceNames) {
                    ans+="namespace/**/"+varNamespace+"{";
                }
                ans+="class "+varI.className+" ;";
                const auto varSize=varI.namespaceNames.size();
                for (auto i=0; i<varSize;++i) {
                    ans+="/**/}";
                }
                ans+="\n";
            }
        }

        const static QString first_part_1=QString::fromUtf8(u8R"___(

class VirtualClass {
    using _Type=std::intptr_t;
public:

    enum class Type:_Type{
        IndexVoid,
        IndexConstVoid,
        /*Index...*/
)___");
        /*out put first part*/
        ans+=first_part_1;

        {/*make Type*/
            for (const auto & varI:arg) {
                const auto varFullName=varI.makeFullName();
                ans+="        Index_"+varFullName+",\n";
                ans+="        IndexConst_"+varFullName+",\n";
            }
        }

        const static QString second_part=QString::fromUtf8(u8R"( 
        }_type_;

private:
    union  {
        void * _data_void_;
        /*data...*/
)");

        ans+=second_part;

        {
            for (const auto & varI:arg) {
                ans+="        "+varI.makeFullNamespace()+varI.className+" * ";
                ans+="_data_"+varI.makeFullName()+"_;\n";
            }
        }

        const static QString third_part=QString::fromUtf8(u8R"____(
    };

public:

    VirtualClass() :_type_(Type::IndexVoid),_data_void_(nullptr){}
    VirtualClass(decltype(nullptr)):VirtualClass() {}
)____");

        ans+=third_part;

        {
            for (const auto & varI:arg) {

                ans+="    VirtualClass("+varI.makeFullNamespace()+varI.className
                    +" *arg):_type_(Type::Index_"+
                    varI.makeFullName()
                    +"),_data_"+
                    varI.makeFullName()
                    +"_(arg) {}\n";

                ans+="    VirtualClass(const "+varI.makeFullNamespace()+varI.className
                    +" *arg):_type_(Type::IndexConst_"+
                    varI.makeFullName()
                    +"),_data_"+
                    varI.makeFullName()
                    +"_(const_cast< "+
                    varI.makeFullNamespace()+varI.className
                    +" * >(arg)) {}\n";

                ans+="\n";
            }
        }

        const static QString last_part=QString::fromUtf8(u8R"____(
    bool isConstPointer() const { return static_cast<_Type>(_type_)&1; }
    Type type() const{ return _type_; }
    void * data() const { return _data_void_; }

    /*defined in JustInCppFileVirtualClass.hpp*/
    template<typename Type>Type getData() const;

};

#endif

)____");

        ans+=last_part;

    }

    //qDebug().noquote()<<ans;

    /*free data not use*/
    ans.squeeze();
    return std::move(ans);
}

QString VirtualClassMaker::make_headerin_cpp(
    const QList<ClassNameReadWrite::ClassName> &arg)const{

    if(arg.isEmpty()){return "";}

    QString ans;
    ans.reserve(1024*1024);

    {
        const static QString part_0= QString::fromUtf8(u8R"___(
#ifndef PRAGMA_IN_CPP_FILE_ONCE_VIRTUAL_CLASS_
#define PRAGMA_IN_CPP_FILE_ONCE_VIRTUAL_CLASS_

/*
此文件只能被cpp文件包含
此文件必须被最后包含
此文件在不同cpp文件中编译结果不同
*/

#include "VirtualClass.hpp"

)___");

        ans+=part_0;

        const static QString part_quick_cast=QString::fromUtf8(u8R"___1(
namespace __VirtualClass_{
namespace __private {
class __QuickCast {
protected:
    /*测试是否可以执行static_cast*/
    class can_not_static_cast final { public:can_not_static_cast(decltype(nullptr)) {} };
    class can_static_cast final {};

    template<typename ...>
    static can_not_static_cast test_is_static_cast(...) { return nullptr; }

    template<typename ToType_,typename FromType_>
    static auto test_is_static_cast(FromType_ arg)->decltype((static_cast<ToType_>(arg),can_static_cast{})) {
        return (static_cast<ToType_>(arg),can_static_cast{});
    }
    /*测试是否可以执行dynamic_cast*/
    class can_not_dynamic_cast final { public:can_not_dynamic_cast(decltype(nullptr)) {} };
    class can_dynamic_cast final {};

    template<typename ...>
    static can_not_dynamic_cast test_is_dynamic_cast(...) { return nullptr; }

    template<typename ToType_,typename FromType_>
    static auto test_is_dynamic_cast(FromType_ arg)->decltype((dynamic_cast<ToType_>(arg),can_dynamic_cast{})) {
        return (dynamic_cast<ToType_>(arg),can_dynamic_cast{});
    }
};
}/*~__private*/

/*static_cast or dynamic_cast*/
template<typename TypeFrom,typename TypeTo>
class QuickCast final :private __private::__QuickCast {
private:
    /*can not create*/
    ~QuickCast()=delete;
    QuickCast()=delete;

    /*test ans*/
    using test_static_cast_ans=
        decltype(test_is_static_cast<TypeTo,TypeFrom>(std::declval<TypeFrom>()));
    using test_dynamic_cast_ans=
        decltype(test_is_dynamic_cast<TypeTo,TypeFrom>(std::declval<TypeFrom>()));

    /*static_cast*/
    template<
        typename TypeNotUse,
        typename=std::remove_reference_t<test_static_cast_ans>,
        typename=std::remove_reference_t<test_dynamic_cast_ans>
    >class Cast
        final :public std::true_type {
    public:
        static TypeTo cast(TypeFrom arg) { return static_cast<TypeTo>(arg); }
    };

    /*can not cast*/
    template<typename TypeNotUse>
    class Cast<TypeNotUse,can_not_static_cast,can_not_dynamic_cast>
        final :public std::false_type {
    public:
        static TypeTo cast(TypeFrom) { return nullptr; }
    };

    /*dynamic_cast*/
    template<typename TypeNotUse>
    class Cast <TypeNotUse,can_not_static_cast,can_dynamic_cast>
        final :public std::false_type {
    public:
        static TypeTo cast(TypeFrom arg) { return dynamic_cast<TypeTo>(arg); }
    };

    using cast_type=Cast<void>;
    using type=cast_type;
public:
    using from_type=TypeFrom;
    using to_type=TypeTo;
    constexpr static bool value=cast_type::value/*is static cast*/;
    static TypeTo cast(TypeFrom arg) { return type::cast(arg); }
};

/*static_cast or dynamic_cast*/
template<typename TypeTo_,typename TypeFrom_>
inline TypeTo_ quickCast(TypeFrom_ && arg) {
    return QuickCast<TypeFrom_,TypeTo_>::cast(std::forward<TypeFrom_>(arg));
}

/*static_cast or dynamic_cast*/
template<typename TypeTo_,typename TypeFrom_>
inline TypeTo_ quickCast(TypeFrom_ * arg) {
    static_assert(std::is_pointer<TypeTo_>::value,"TypeTo_ must be pointer");
    return QuickCast<TypeFrom_ *,TypeTo_>::cast(arg);
}

}/*~__VirtualClass_*/

)___1");

        {
            ans+=QString::fromUtf8(u8R"(namespace {
/*强制编译器为每一个cpp文件生成不同的函数*/
)");
            ans+=part_quick_cast;

            ans+=QString::fromUtf8(u8R"__(template<typename Type>
Type _cppfile_VirtualClass_getData(const VirtualClass * arg)  {
    switch (arg->type()) {
        case VirtualClass::Type::IndexVoid:return nullptr; break;
        case VirtualClass::Type::IndexConstVoid:return nullptr; break;

)__");

            for (const auto & varI:arg) {
                {
                    ans+="        case VirtualClass::Type::Index_";
                    ans+=varI.makeFullName();
                    ans+=":return __VirtualClass_::quickCast<Type>(reinterpret_cast<";
                    ans+=varI.makeFullNamespace()+varI.className;
                    ans+="*>(arg->data())); break;\n";
                }
                {
                    ans+="        case VirtualClass::Type::IndexConst_";
                    ans+=varI.makeFullName();
                    ans+=":return __VirtualClass_::quickCast<Type>(reinterpret_cast<const ";
                    ans+=varI.makeFullNamespace()+varI.className;
                    ans+="*>(arg->data())); break;\n";
                }
                ans+="\n";
            }

            ans+=QString::fromUtf8(u8R"(
        default:return nullptr; break;
    }
})");
            ans+=QString::fromUtf8(u8R"(
}/*~namespace*/
)");
        }

        const static QString part_last=QString::fromUtf8(u8R"____(
template<typename Type>
Type VirtualClass::getData() const{
    static_assert(std::is_pointer<Type>::value,"Type must be pointer");
    if (_data_void_==nullptr) { return nullptr; }
    return _cppfile_VirtualClass_getData<Type>(this);
}


#endif

)____");

        ans+=part_last;

    }

    //qDebug().noquote()<<ans;

    /*free data not use*/
    ans.squeeze();
    return std::move(ans);
}
/**/

