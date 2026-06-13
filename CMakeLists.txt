cmake_minimum_required(VERSION 3.22)
project(ColectivoLaguneoRhythmSenior VERSION 1.0.0)

add_definitions(-DJUCE_VST3_CAN_REPLACE_VST2=0)

set(JUCE_ENABLE_MODULE_SOURCE_GROUPS ON)
set(JUCE_BUILD_EXTRAS OFF)
set(JUCE_BUILD_EXAMPLES OFF)

if(DEFINED JUCE_DIR)
    add_subdirectory(${JUCE_DIR} juce)
else()
    add_subdirectory(C:/dev/JUCE juce)
endif()

add_library(PluginSources STATIC
    Source/PluginProcessor.h
    Source/PluginProcessor.cpp
    Source/PluginEditor.h
    Source/PluginEditor.cpp
    Source/DraggableMidiLabel.h
    Source/DraggableMidiLabel.cpp
    Source/MidiGenerator.h
    Source/MidiGenerator.cpp
)
target_include_directories(PluginSources PUBLIC Source)
target_link_libraries(PluginSources PUBLIC juce_audio_basics juce_audio_formats juce_core juce_gui_basics)

juce_add_plugin(ColectivoLaguneoRhythmSenior
    PRODUCT_NAME "Colectivo Laguneo RhythmSenior"
    COMPANY_NAME "Colectivo Laguneo"
    PLUGIN_MANUFACTURER_CODE "ColL"
    BUNDLE_ID "com.colectivolaguneo.rhythmsenior"
    FORMATS VST3
    IS_SYNTH TRUE
    NEEDS_MIDI_OUTPUT TRUE
    MODULES
        juce_audio_plugin_client
        juce_gui_basics
        juce_audio_basics
        juce_audio_formats
        juce_core
    SOURCES
        Source/PluginProcessor.h
        Source/PluginProcessor.cpp
        Source/PluginEditor.h
        Source/PluginEditor.cpp
        Source/DraggableMidiLabel.h
        Source/DraggableMidiLabel.cpp
        Source/MidiGenerator.h
        Source/MidiGenerator.cpp
)

target_link_libraries(ColectivoLaguneoRhythmSenior PRIVATE PluginSources)