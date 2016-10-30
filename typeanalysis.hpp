#pragma once

#include <iostream>
#include <iomanip>
#include <typeinfo>
#include <type_traits>
#include <cxxabi.h>

#define lf setw(FW) << left
#define rf setw(FW) << right

///Contains funcs and types used in implementation of `analyzeType()'
///Because they're all static, class def has to go before `analyzeType()'
class Internals
{
    template <class>
    friend void analyzeType();

    static std::string article(std::string word)
    {
        if (word.size() == 0)
            return "<error>";

        if (word[0] == 'a' || word[0] == 'e' || word[0] == 'i' ||
            word[0] == 'o' || word[0] == 'u' || word == "lvalue reference" ||
            word == "rvalue reference")
        {
            return "An";
        }

        else if (word == "void" || word == "nullptr_t")
            return "The";

        return "A";
    }

    template <class T>
    static std::string getPrimaryType()
    {
        using namespace std;

        if (is_array<T>::value)
            return "array";

        else if (is_class<T>::value)
            return "class";

        else if (is_enum<T>::value)
            return "enum";

        else if (is_floating_point<T>::value)
            return "floating point";

        else if (is_function<T>::value)
            return "function";

        else if (is_integral<T>::value)
            return "integral";

        else if (is_lvalue_reference<T>::value)
            return "lvalue reference";

        else if (is_member_function_pointer<T>::value)
            return "member function pointer";

        else if (is_member_object_pointer<T>::value)
            return "member object pointer";

        else if (is_pointer<T>::value)
            return "pointer";

        else if (is_rvalue_reference<T>::value)
            return "rvalue reference";

        else if (is_union<T>::value)
            return "union";

        else if (is_void<T>::value)
            return "void";

        else if (typeid(T) == typeid(nullptr_t))
            return "nullptr_t";

        else
            return "<YOU HAVE SOMEHOW MADE A TYPE THAT'S BOTH COMPLETE AND INCOMPLETE WHAT ARE YOU DOING>";
    }

    template <class T>
    static std::string getCompositeTypes()
    {
        using namespace std;

        string types;

        if (is_arithmetic<T>::value)
            types += "Arithmetic Type, ";

        if (is_compound<T>::value)
            types += "Compound Type, ";

        if (is_fundamental<T>::value)
            types += "Fundamental Type, ";

        if (is_member_pointer<T>::value)
            types += "Member Pointer Type, ";

        if (is_object<T>::value)
            types += "Object Type, ";

        if (is_reference<T>::value)
            types += "Reference Type, ";

        if (is_scalar<T>::value)
            types += "Scalar Type, ";

        types.erase(types.size() - 2, 2);

        return types;
    }



    ///is_complete
    template <class T, int(*)[sizeof(T)] = nullptr>
    using void_t = void;

    template <class T, typename = void>
    struct is_complete
    {
        static void analyze()
        {
            std::cout << "\n\n\nCannot perform type analysis because the type provided is incomplete.\n\n";
        }
    };

    template <class T>
    struct is_complete<T, void_t<T>>
    {
        //magic solution for detecting incomplete types is basically magic.
        //(I had a problem with my original approach, then i thought of this.
        //This still uses SFINAE, just C++14 style w/ templates and shit.)
        static void analyze()
        {
            using namespace std;

            const int FW = 30;
            const char* const tname = typeid(T).name();
            const size_t sz = sizeof(T), algn = alignof(T);

            cout << "\n\n\nType analysis for `" << abi::__cxa_demangle(tname, 0, 0, 0) << "':\n\n"
                 << lf << "Compiled Name:" << tname << endl
                 << lf << "Size:" << sz << (sz == 1 ? " byte" : " bytes") << endl
                 << lf << "Alignment:" << algn << (algn == 1 ? " byte" : " bytes") << "\n\n"
                 << lf << "Primary Type Category:" << getPrimaryType<T>();
        }
    };
};



template <class T>
void analyzeType()
{
    //and that is why it's basically magic.
    //this dodges the "invalid use of `T' in `sizeof(T)'" compiler error like fucking Neo
    //https://youtu.be/voQD3_FPb2w?t=16
    Internals::is_complete<T>::analyze();
}
































