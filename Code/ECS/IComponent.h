#pragma once

#include "Utility.h"

namespace VSEngine {
namespace ECS {

struct IComponent
{
    IComponent() = delete;
};

using ComponentsList = None;

#define ADD_COMPONENT(NewComponent) \
    using ComponentList = Append<ComponentsList, NewComponent>::Result;

}
}