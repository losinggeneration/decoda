/*

Decoda
Copyright (C) 2007-2013 Unknown Worlds Entertainment, Inc. 

This file is part of Decoda.

Decoda is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Decoda is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Decoda.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "Channel.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

Channel::Channel()
{
}

Channel::~Channel()
{
    Destroy();
}

bool Channel::Create(const char* name)
{
}

bool Channel::Connect(const char* name)
{
}

bool Channel::WaitForConnection()
{
#if 0
    return ConnectNamedPipe(m_pipe, NULL) != FALSE;
#endif
}

void Channel::Destroy()
{
}

bool Channel::Write(const void* buffer, unsigned int length)
{
#if 0
    assert(m_pipe != INVALID_HANDLE_VALUE);

    if (length == 0)
    {
        // Because of the way message pipes work, writing 0 is different than
        // writing nothing.
        return true;
    }

    OVERLAPPED overlapped = { 0 };
    overlapped.hEvent = m_readEvent;

    BOOL result = WriteFile(m_pipe, buffer, length, NULL, &overlapped) != 0;

    if (result == FALSE)
    {
        // Wait for the operation to complete so that we don't need to keep around
        // the buffer.
        WaitForSingleObject(m_readEvent, INFINITE);

        DWORD numBytesWritten = 0;

        if (GetOverlappedResult(m_pipe, &overlapped, &numBytesWritten, FALSE))
        {
            result = (numBytesWritten == length);
        }    
    
    }

    return result == TRUE;
#endif
}

bool Channel::WriteUInt32(unsigned int value)
{
    unsigned int temp = value;
    return Write(&temp, 4);
}

bool Channel::WriteString(const char* value)
{
    unsigned int length = static_cast<int>(strlen(value));
    if (!WriteUInt32(length))
    {
        return false;
    }
    if (length > 0)
    {
        return Write(value, length);
    }
    return true;
}

bool Channel::WriteString(const std::string& value)
{
    unsigned int length = value.length();
    if (!WriteUInt32(length))
    {
        return false;
    }
    if (length > 0)
    {
        return Write(value.c_str(), length);
    }
    return true;
}

bool Channel::WriteBool(bool value)
{
    return WriteUInt32(value ? 1 : 0);
}

bool Channel::ReadUInt32(unsigned int& value)
{
    uint32_t temp;
    if (!Read(&temp, 4))
    {
        return false;
    }
    value = temp;
    return true;
}

bool Channel::ReadString(std::string& value)
{
    
    unsigned int length;
    
    if (!ReadUInt32(length))
    {
        return false;
    }

    if (length != 0)
    {

        char* buffer = new char[length + 1];

        if (!Read(buffer, length))
        {
            delete [] buffer;
            return false;
        }

        buffer[length] = 0;
        value = buffer;
        
        delete [] buffer;

    }
    else
    {
        value.clear();
    }

    return true;

}

bool Channel::ReadBool(bool& value)
{

    unsigned int temp;

    if (ReadUInt32(temp))
    {
        value = temp != 0;
        return true;
    }

    return false;

}

bool Channel::Read(void* buffer, unsigned int length)
{
#if 0
    assert(m_pipe != INVALID_HANDLE_VALUE);
    
    if (length == 0)
    {
        // Because of the way message pipes work, reading 0 is different than
        // reading nothing.
        return true;
    }

    OVERLAPPED overlapped = { 0 };
    overlapped.hEvent = m_readEvent;

    DWORD numBytesRead;
    BOOL result = ReadFile(m_pipe, buffer, length, &numBytesRead, &overlapped);

    if (result == FALSE)
    {

        DWORD error = GetLastError();

        if (error == ERROR_IO_PENDING)
        {
        
            // Wait for the operation to complete.
            
            HANDLE events[] =
                {
                    m_readEvent,
                    m_doneEvent,
                };

            WaitForMultipleObjects(2, events, FALSE, INFINITE);

            if (WaitForSingleObject(m_doneEvent, 0) == WAIT_OBJECT_0)
            {
                // The pipe has been closed.
                result = FALSE;
            }
            else if (GetOverlappedResult(m_pipe, &overlapped, &numBytesRead, FALSE))
            {
                result = (numBytesRead == length);
            }
        
        }

    }

    return result == TRUE;
#endif
}

void Channel::Flush()
{
    //FlushFileBuffers(m_pipe);
}
