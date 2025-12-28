
#pragma once

namespace ResourceStats
{
    // System Component TypeIds
    inline constexpr const char* ResourceStatsSystemComponentTypeId = "{A65973D0-C001-48DB-9C04-76BEDCC38234}";
    inline constexpr const char* ResourceStatsEditorSystemComponentTypeId = "{99BA2BF7-3468-4BC3-93F3-6D97B818B3F3}";

    // Module derived classes TypeIds
    inline constexpr const char* ResourceStatsModuleInterfaceTypeId = "{B327FE62-79E8-4CF7-BA79-A8B49977EDF2}";
    inline constexpr const char* ResourceStatsModuleTypeId = "{3C23359B-8EA9-411A-9045-E1337687F000}";
    // The Editor Module by default is mutually exclusive with the Client Module
    // so they use the Same TypeId
    inline constexpr const char* ResourceStatsEditorModuleTypeId = ResourceStatsModuleTypeId;

    // Interface TypeIds
    inline constexpr const char* ResourceStatsRequestsTypeId = "{33DDDC12-BD56-4511-8901-1D742192034D}";
} // namespace ResourceStats
