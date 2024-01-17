#pragma once

#include <Windows.h>
#include <WinUser.h>


__interface IWindow
{
public:

    virtual void process_messages_queue() = 0;
    virtual bool is_terminated() const = 0;
};