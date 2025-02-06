
#include <sdk.hpp>

#include "Winfixes.hpp"

#include "NodejsComponent.hpp"

COMPONENT_ENTRY_POINT()
{
    DoWinFixes();

    return NodejsComponent::getInstance();
}
