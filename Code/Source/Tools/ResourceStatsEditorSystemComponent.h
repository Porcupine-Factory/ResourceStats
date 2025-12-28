
#pragma once

#include <AzToolsFramework/API/ToolsApplicationAPI.h>

#include <Clients/ResourceStatsSystemComponent.h>

namespace ResourceStats
{
    /// System component for ResourceStats editor
    class ResourceStatsEditorSystemComponent
        : public ResourceStatsSystemComponent
        , protected AzToolsFramework::EditorEvents::Bus::Handler
    {
        using BaseSystemComponent = ResourceStatsSystemComponent;
    public:
        AZ_COMPONENT_DECL(ResourceStatsEditorSystemComponent);

        static void Reflect(AZ::ReflectContext* context);

        ResourceStatsEditorSystemComponent();
        ~ResourceStatsEditorSystemComponent();

    private:
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        // AZ::Component
        void Activate() override;
        void Deactivate() override;
    };
} // namespace ResourceStats
