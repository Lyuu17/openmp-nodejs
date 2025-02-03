
#pragma once

#include "common.hpp"

#include "Resource.hpp"
#include "IJavaScriptClassExtension.hpp"

#include <sdk.hpp>

struct VehicleComponent final : public IExtension,
                                public IJavaScriptClassExtension
{
    PROVIDE_EXT_UID(0xC4E68E76688A82EF);

    IVehicle* m_vehicle;

    VehicleComponent(IVehicle*);
    ~VehicleComponent();

    void freeExtension() override;
    void reset() override;

    v8::Local<v8::Object> CreateJavaScriptObject() override;

    static void InitFunctions();
};
