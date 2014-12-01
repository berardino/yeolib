/**
 This file is part of YeoLibrary.
 
 YeoLibrary is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, version 3.
 
 YeoLibrary is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 You should have received a copy of the GNU General Public License
 along with YeoLibrary.  If not, see <http://www.gnu.org/licenses/>.
 **/
#ifndef __YEO_IO_H__
#define __YEO_IO_H__

#if defined _WIN32 || defined __CYGWIN__
#define YEO_HELPER_DLL_IMPORT __declspec(dllimport)
#define YEO_HELPER_DLL_EXPORT __declspec(dllexport)
#define YEO_HELPER_DLL_LOCAL
#else
#if __GNUC__ >= 4
#define YEO_HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
#define YEO_HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
#define YEO_HELPER_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
#else
#define YEO_HELPER_DLL_IMPORT
#define YEO_HELPER_DLL_EXPORT
#define YEO_HELPER_DLL_LOCAL
#endif
#endif

#ifdef YEO_DLL
#ifdef YEO_DLL_EXPORTS
#define YEO_API YEO_HELPER_DLL_EXPORT
#else
#define YEO_API YEO_HELPER_DLL_IMPORT
#endif
#define YEO_LOCAL YEO_HELPER_DLL_LOCAL
#else
#define YEO_API
#define YEO_LOCAL
#endif // YEO_DLL


namespace yeo {
}

#endif //__YEO_IO_H__
