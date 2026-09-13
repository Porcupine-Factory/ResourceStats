#!/bin/bash
# Automate the creation of a new resource stat.

if [ -z "$1" ]; then
    read -r -p "Enter the stat name to create (e.g. Magic Armor): " STAT_NAME
    if [ -z "$STAT_NAME" ]; then
        echo "Error: No stat name provided. Exiting."
        exit 1
    fi
else
    STAT_NAME="$*"
fi

# Loop through each word, capitalize the first letter, and concatenate without spaces
FORMATTED_NAME=""
for word in $STAT_NAME; do
    FORMATTED_NAME+="${word^}"
done

# Use camelCase for parameters
STAT_NAME="$FORMATTED_NAME"
STAT_NAME_LOWER="$(echo "${STAT_NAME:0:1}" | tr '[:upper:]' '[:lower:]')${STAT_NAME:1}"
MAX_STAT_NAME="Max${STAT_NAME}"
DELTA_STAT_NAME="${STAT_NAME}Delta"
RPC_NAME="Send${STAT_NAME}Delta"
PARAM_NAME="delta${STAT_NAME}"
DATA_ELEMENT_1="Initial $STAT_NAME"
DATA_ELEMENT_2="Maximum $STAT_NAME"

PROJECT_ROOT=$(dirname "$(readlink -f "$0")")
BUS_H="$PROJECT_ROOT/Code/Include/ResourceStats/ResourceStatsComponentBus.h"
XML="$PROJECT_ROOT/Code/Source/AutoGen/NetworkResourceStats.AutoComponent.xml"
CLIENT_H="$PROJECT_ROOT/Code/Source/Clients/ResourceStatsComponent.h"
CLIENT_CPP="$PROJECT_ROOT/Code/Source/Clients/ResourceStatsComponent.cpp"
NET_H="$PROJECT_ROOT/Code/Source/Multiplayer/NetworkResourceStats.h"
NET_CPP="$PROJECT_ROOT/Code/Source/Multiplayer/NetworkResourceStats.cpp"

# Update ResourceStatsComponentBus.h
awk -v stat="$STAT_NAME" -v delta="$DELTA_STAT_NAME" '
/#ifdef FIRST_PERSON_CONTROLLER/ && !done {
    print "        virtual AZ::u32 Get" stat "() const = 0;"
    print "        virtual void Set" stat "(const AZ::u32) = 0;"
    print "        virtual AZ::u32 GetMax" stat "() const = 0;"
    print "        virtual void SetMax" stat "(const AZ::u32) = 0;"
    print "        virtual void Set" delta "(const AZ::s64) = 0;"
    done=1
}
{ print }
' "$BUS_H" > tmp_bus && mv tmp_bus "$BUS_H"

# Update NetworkResourceStats.AutoComponent.xml
sed -i "/<NetworkProperty Type=\"AZ::u32\" Name=\"Stamina\"/i \\    <NetworkProperty Type=\"AZ::u32\" Name=\"$STAT_NAME\"\n                     Init=\"100\" ReplicateFrom=\"Authority\"\n                     ReplicateTo=\"Client\" Container=\"Object\"\n                     IsPublic=\"true\" IsRewindable=\"true\"\n                     IsPredictable=\"false\" ExposeToEditor=\"false\"\n                     ExposeToScript=\"true\" GenerateEventBindings=\"true\"\n                     Description=\"The amount of $STAT_NAME\"/>\n    <NetworkProperty Type=\"AZ::u32\" Name=\"$MAX_STAT_NAME\"\n                     Init=\"100\" ReplicateFrom=\"Authority\"\n                     ReplicateTo=\"Client\" Container=\"Object\"\n                     IsPublic=\"true\" IsRewindable=\"true\"\n                     IsPredictable=\"false\" ExposeToEditor=\"false\"\n                     ExposeToScript=\"true\" GenerateEventBindings=\"true\"\n                     Description=\"Maximum $STAT_NAME\"/>" "$XML"

sed -i "/<RemoteProcedure Name=\"SendStaminaDelta\"/i \\    <RemoteProcedure Name=\"$RPC_NAME\" InvokeFrom=\"Server\" HandleOn=\"Authority\" IsPublic=\"true\" IsReliable=\"true\" GenerateEventBindings=\"true\" Description=\"$STAT_NAME changed RPC\">\n        <Param Type=\"AZ::s64\" Name=\"$PARAM_NAME\"/>\n    </RemoteProcedure>\n" "$XML"

# Update ResourceStatsComponent.h
awk -v stat="$STAT_NAME" -v lower="$STAT_NAME_LOWER" -v delta="$DELTA_STAT_NAME" -v max="$MAX_STAT_NAME" '
/#ifdef FIRST_PERSON_CONTROLLER/ {
    count++
    if (count == 1) {
        print "        AZ::u32 Get" stat "() const override;"
        print "        void Set" stat "(const AZ::u32 " lower ") override;"
        print "        AZ::u32 GetMax" stat "() const override;"
        print "        void SetMax" stat "(const AZ::u32 max" stat ") override;"
        print "        void Set" delta "(const AZ::s64 " lower "Delta) override;"
    } else if (count == 3) {
        print "        AZ::u32 m_" lower " = 100;"
        print "        AZ::u32 m_max" stat " = 100;"
    }
}
{ print }
' "$CLIENT_H" > tmp_client && mv tmp_client "$CLIENT_H"

# Update ResourceStatsComponent.cpp
TOTAL_IFDEFS=$(grep -c "#ifdef FIRST_PERSON_CONTROLLER" "$CLIENT_CPP")

awk -v stat="$STAT_NAME" -v lower="$STAT_NAME_LOWER" -v max="$MAX_STAT_NAME" -v delta="$DELTA_STAT_NAME" -v e1="$DATA_ELEMENT_1" -v e2="$DATA_ELEMENT_2" -v total="$TOTAL_IFDEFS" '
/#ifdef FIRST_PERSON_CONTROLLER/ {
    count++
    if (count == 1) {
        print "                ->Field(\"" gensub(/([[:lower:]])([[:upper:]])/, "\\1 \\2", "g", stat) "\", &ResourceStatsComponent::m_" lower ")"
        print "                ->Attribute(AZ::Edit::Attributes::Min, 0)"
        print "                ->Field(\"" gensub(/([[:lower:]])([[:upper:]])/, "\\1 \\2", "g", max) "\", &ResourceStatsComponent::m_max" stat ")"
        print "                ->Attribute(AZ::Edit::Attributes::Min, 0)"
    } else if (count == 2) {
        print "                    ->DataElement(nullptr, &ResourceStatsComponent::m_" lower ", \"" gensub(/([[:lower:]])([[:upper:]])/, "\\1 \\2", "g", stat) "\", \"" e1 "\")"
        print "                    ->DataElement(nullptr, &ResourceStatsComponent::m_max" stat ", \"" gensub(/([[:lower:]])([[:upper:]])/, "\\1 \\2", "g", max) "\", \"" e2 "\")"
    } else if (count == 3) {
        print "                ->Event(\"Get " gensub(/([[:lower:]])([[:upper:]])/, "\\1 \\2", "g", stat) "\", &ResourceStatsComponentRequests::Get" stat ")"
        print "                ->Event(\"Set " gensub(/([[:lower:]])([[:upper:]])/, "\\1 \\2", "g", stat) "\", &ResourceStatsComponentRequests::Set" stat ")"
        print "                ->Event(\"Get " gensub(/([a-z])([A-Z])/, "\\1 \\2", "g", max) "\", &ResourceStatsComponentRequests::GetMax" stat ")"
        print "                ->Event(\"Set " gensub(/([a-z])([A-Z])/, "\\1 \\2", "g", max) "\", &ResourceStatsComponentRequests::SetMax" stat ")"
        print "                ->Event(\"Set " gensub(/([[:lower:]])([[:upper:]])/, "\\1 \\2", "g", stat) " Delta\", &ResourceStatsComponentRequests::Set" delta ")"
    } else if (count == 6) {
        print "            m_" lower " = m_networkResourceStatsControllerObject->Get" stat "();"
        print "            m_max" stat " = m_networkResourceStatsControllerObject->GetMax" stat "();"
    } else if (count == 7) {
        print "            m_networkResourceStatsControllerObject->Set" stat "(m_" lower ");"
        print "            m_networkResourceStatsControllerObject->SetMax" stat "(m_max" stat ");"
    }

    if (count == total) {
        print "    AZ::u32 ResourceStatsComponent::Get" stat "() const"
        print "    {"
        print "#ifdef NETWORKRESOURCESTATS"
        print "        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)"
        print "            return m_networkResourceStatsControllerObject->Get" stat "();"
        print "        else"
        print "#endif"
        print "            return m_" lower ";"
        print "    }"
        print "    void ResourceStatsComponent::Set" stat "(const AZ::u32 " lower ")"
        print "    {"
        print "        m_" lower " = " lower ";"
        print "#ifdef NETWORKRESOURCESTATS"
        print "        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)"
        print "            m_networkResourceStatsControllerObject->Set" stat "(m_" lower ");"
        print "#endif"
        print "    }"
        print "    AZ::u32 ResourceStatsComponent::GetMax" stat "() const"
        print "    {"
        print "#ifdef NETWORKRESOURCESTATS"
        print "        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)"
        print "            return m_networkResourceStatsControllerObject->GetMax" stat "();"
        print "        else"
        print "#endif"
        print "            return m_max" stat ";"
        print "    }"
        print "    void ResourceStatsComponent::SetMax" stat "(const AZ::u32 max" stat ")"
        print "    {"
        print "        m_max" stat " = max" stat ";"
        print "#ifdef NETWORKRESOURCESTATS"
        print "        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)"
        print "            m_networkResourceStatsControllerObject->SetMax" stat "(m_max" stat ");"
        print "#endif"
        print "    }"
        print "    void ResourceStatsComponent::Set" delta "(const AZ::s64 " lower "Delta)"
        print "    {"
        print "        if (" lower "Delta > 0 && " lower "Delta > AZStd::numeric_limits<AZ::u32>::max() - m_" lower ")"
        print "            m_" lower " = AZStd::numeric_limits<AZ::u32>::max();"
        print "        else"
        print "        {"
        print "            const AZ::s64 current" stat " = aznumeric_cast<AZ::s64>(m_" lower ");"
        print "            const AZ::s64 max" stat " = aznumeric_cast<AZ::s64>(m_max" stat ");"
        print "            const AZ::s64 new" stat " = AZStd::clamp(current" stat " + " lower "Delta, 0LL, max" stat ");"
        print "            m_" lower " = aznumeric_cast<AZ::u32>(new" stat ");"
        print "        }"
        print "#ifdef NETWORKRESOURCESTATS"
        print "        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)"
        print "            m_networkResourceStatsControllerObject->Set" stat "(m_" lower ");"
        print "#endif"
        print "    }"
    }
}
{ print }
' "$CLIENT_CPP" > tmp_cpp && mv tmp_cpp "$CLIENT_CPP"

# Update ProcessInput logic
sed -i "/SetMaxStrength(m_resourceStatsObject->m_maxStrength);/a \\            Set$STAT_NAME(m_resourceStatsObject->m_$STAT_NAME_LOWER);\n            SetMax$STAT_NAME(m_resourceStatsObject->m_max$STAT_NAME);" "$CLIENT_CPP"

# Update NetworkResourceStats.h
sed -i "/void HandleSendStaminaDelta/i \\        void HandleSend$DELTA_STAT_NAME(AzNetworking::IConnection* invokingConnection, const AZ::s64& $PARAM_NAME) override;" "$NET_H"

# Update NetworkResourceStats.cpp
awk -v stat="$STAT_NAME" -v lower="$STAT_NAME_LOWER" '
/#ifdef FIRST_PERSON_CONTROLLER/ && !done {
    print "            Set" stat "(m_resourceStatsObject->m_" lower ");"
    print "            SetMax" stat "(m_resourceStatsObject->m_max" stat ");"
    done = 1
}
{ print }
' "$NET_CPP" > tmp_net && mv tmp_net "$NET_CPP"

sed -i "/void NetworkResourceStatsController::HandleSendStaminaDelta/i \\    void NetworkResourceStatsController::HandleSend$DELTA_STAT_NAME(\n        [[maybe_unused]] AzNetworking::IConnection* invokingConnection, const AZ::s64& $PARAM_NAME)\n    {\n        m_resourceStatsObject->Set$DELTA_STAT_NAME($PARAM_NAME);\n    }" "$NET_CPP"

echo "$STAT_NAME stat added!"
