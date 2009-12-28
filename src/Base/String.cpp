#include <stdio.h>
#include <stdarg.h>

#include "Macros.h"
#include "String.h"

namespace Finch
{
    const char * String::sEmptyString = "";
    
    String String::Format(const String & format, ...)
    {
        char result[FormattedStringMax];
        
        va_list args;
        va_start (args, format);
        
        vsprintf(result, format.CString(), args);

        va_end (args);
        
        return String(result);
    }
    
    String::String(const char* chars)
    {
        Init(chars, false);
    }
    
    String::String(char c)
    {
        char chars[2];
        chars[0] = c;
        chars[1] = '\0';
        
        Init(chars, false);
    }
    
    bool String::operator <(const String & other) const
    {
        return strcmp(CString(), other.CString()) < 0;
    }
    
    bool String::operator <=(const String & other) const
    {
        return strcmp(CString(), other.CString()) <= 0;
    }
    
    bool String::operator >(const String & other) const
    {
        return strcmp(CString(), other.CString()) > 0;
    }
    
    bool String::operator >=(const String & other) const
    {
        return strcmp(CString(), other.CString()) >= 0;
    }
    
    bool String::operator ==(const String & other) const
    {
        if (this == &other) return true;
        if (mData == other.mData) return true;
        
        return strcmp(CString(), other.CString()) == 0;
    }
    
    bool String::operator !=(const String & other) const
    {
        return !(*this == other);
    }
    
    const char & String::operator[] (int index) const
    {
        ASSERT_INDEX(index, Length() + 1); // allow accessing the terminator

        if (mData.IsNull()) return sEmptyString[0];
        return mData->chars[index];
    }
    
    String String::operator +(const String & other) const
    {
        return String(*this, other);
    }
    
    String & String::operator +=(const String & other)
    {
        *this = *this + other;
        return *this;
    }

    const char* String::CString() const
    {
        if (mData.IsNull()) return sEmptyString;
        
        return mData->chars;
    }

    int String::Length() const
    {
        if (mData.IsNull()) return 0;
        
        return mData->length;
    }

    String String::Substring(int startIndex) const
    {
        if (startIndex < 0)
        {
            // count from end
            startIndex = Length() + startIndex;
        }
        
        ASSERT_INDEX(startIndex, Length());
        
        int length = Length() - startIndex;
        char* heap = new char[length + 1];
        strcpy(heap, CString() + startIndex);
        
        return String(heap, true);
    }
    
    String String::Substring(int startIndex, int count) const
    {
        if (startIndex < 0)
        {
            // count from end
            startIndex = Length() + startIndex;
        }
        
        if (count < 0)
        {
            count = Length() + count - startIndex;
        }
        
        ASSERT_INDEX(startIndex, Length());
        ASSERT(startIndex + count <= Length(), "Range must not go past end of string.");
        
        char* heap = new char[count + 1];
        strncpy(heap, CString() + startIndex, count);
        heap[count] = '\0';
        
        return String(heap, true);
    }

    String::String(const String & left, const String & right)
    {
        // make a new buffer on the heap
        int length = left.Length() + right.Length();
        char* heap = new char[length + 1];
        
        // concatenate the strings
        strcpy(heap, left.CString());
        strcpy(&heap[left.Length()], right.CString());
        
        Init(heap, true);
    }
    
    String::String(const char * text, bool isOnHeap)
    {
        Init(text, isOnHeap);
    }
        
    void String::Init(const char * text, bool isOnHeap)
    {
        if (isOnHeap)
        {
            mData = Ref<StringData>(new StringData(text));
        }
        else
        {
            // hoist it onto the heap
            int length = strlen(text);
            char * heap = new char[length + 1];
            strcpy(heap, text);
            
            mData = Ref<StringData>(new StringData(heap));
        }
    }

    bool operator ==(const char * left, const String & right)
    {
        return strcmp(left, right.CString()) == 0;
    }
    
    bool operator !=(const char * left, const String & right)
    {
        return strcmp(left, right.CString()) != 0;
    }
    
    bool operator ==(const String & left, const char * right)
    {
        return strcmp(left.CString(), right) == 0;
    }
    
    bool operator !=(const String & left, const char * right)
    {
        return strcmp(left.CString(), right) != 0;
    }
    
    ostream & operator <<(ostream & cout, const String & string)
    {
        cout << string.CString();
        return cout;
    }
}