/*MT*
    
    MediaTomb - http://www.mediatomb.cc/
    
    exception.cc - this file is part of MediaTomb.
    
    Copyright (C) 2005 Gena Batyan <bgeradz@mediatomb.cc>,
                       Sergey 'Jin' Bostandzhyan <jin@mediatomb.cc>
    
    Copyright (C) 2006-2010 Gena Batyan <bgeradz@mediatomb.cc>,
                            Sergey 'Jin' Bostandzhyan <jin@mediatomb.cc>,
                            Leonhard Wimmer <leo@mediatomb.cc>
    
    MediaTomb is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2
    as published by the Free Software Foundation.
    
    MediaTomb is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    version 2 along with MediaTomb; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
    
    $Id$
*/

/// \file exception.cc

#include "exception.h"

#ifdef HAVE_EXECINFO_H
    #include <execinfo.h>
#endif

#define STRACE_TAG "_STRACE_"

Exception::Exception(std::string message, const char* file, int line, const char* function)
{
    this->message = message;
    this->file = file;
    this->function = function;
    this->line = line;
#if defined HAVE_BACKTRACE && defined HAVE_BACKTRACE_SYMBOLS
    void *b[100];
    int size = backtrace(b, 100);

    stackTrace = Ref<Array<StringBase> >(new Array<StringBase>(size));

    char **s = backtrace_symbols(b, size);
    for(int i = 0; i < size; i++)
    {
        Ref<StringBase> trace(new StringBase(s[i]));
        stackTrace->append(trace);
    }
    if (s)
        free(s);
#endif
}

Exception::Exception(std::string message)
{
    this->message = message;
    this->file = "";
    this->function = "";
    this->line = -1;
#if defined HAVE_BACKTRACE && defined HAVE_BACKTRACE_SYMBOLS
    void *b[100];
    int size = backtrace(b, 100);

    char **s = backtrace_symbols(b, size);
    for(int i = 0; i < size; i++)
    {
        std::striing trace = s[i];
        stackTrace.push_back(trace);
    }
    free(s);
#endif
}

std::string Exception::getMessage() const
{
    return message;
}

std::vector<std::string> Exception::getStackTrace()
{
    return stackTrace;
}

#ifdef TOMBDEBUG
void Exception::printStackTrace(FILE *file) const
{
    if (line >= 0)
    {
        fprintf(file, "Exception raised in [%s:%d] %s(): %s\n", 
                this->file.c_str(), line, function.c_str(), message.c_str());
    }
    else
    {
        fprintf(file, "Exception: %s\n", message.c_str());
    }
#if defined HAVE_BACKTRACE && defined HAVE_BACKTRACE_SYMBOLS
    for (int i = 0; i < stackTrace->size(); i++)
    {
        fprintf(file, "%s %i %s\n", STRACE_TAG, i, trace[i].c_str());
        fflush(file);
    }
#endif // __CYGWIN__
}
#endif // TOMBDEBUG
