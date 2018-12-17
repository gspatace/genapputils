#pragma once

#ifdef WIN32
#ifdef GENAPPUTILS_EXPORTS
#define mDLLIMPORTEXPORT __declspec(dllexport)
#define EXPIMP
#else
#define mDLLIMPORTEXPORT __declspec(dllimport)
#define	EXPIMP extern
#endif // GENAPPUTILS_EXPORTS
#else  // WIN32
#define mDLLIMPORTEXPORT
#endif

//EXPIMP template class mDLLIMPORTEXPORT std::allocator<char>;
//EXPIMP template struct mDLLIMPORTEXPORT std::char_traits<char>;
//EXPIMP template class mDLLIMPORTEXPORT  std::basic_string<char>;
////EXPIMP template class mDLLIMPORTEXPORT std::string;
//EXPIMP template class mDLLIMPORTEXPORT std::variant<std::string, bool>;
//EXPIMP template class mDLLIMPORTEXPORT std::_Variant_storage_<true, bool>;
