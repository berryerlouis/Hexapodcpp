cmake_minimum_required(VERSION 3.22)

set(SRC_FILES_SERVICES
        Service/Battery/ServiceBattery.cpp
        Service/Button/ServiceButton.cpp
        Service/Communication/ServiceCommunication.cpp
        Service/Control/ServiceControl.cpp
        Service/Display/ServiceDisplay.cpp
        Service/Event/EventListener.cpp
        Service/General/ServiceGeneral.cpp
        Service/Orientation/ServiceOrientation.cpp
        Service/Proximity/ServiceProximity.cpp
        Service/Sound/ServiceSound.cpp
        Service/Services/Services.cpp
        Service/Message/MessageListener.cpp
        Service/Body/ServiceBody.cpp
        PARENT_SCOPE
)