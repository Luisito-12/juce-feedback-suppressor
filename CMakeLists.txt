cmake_minimum_required(VERSION 3.21)
project(FeedbackSuppressor VERSION 1.0.0 LANGUAGES CXX)

# Windows solo
if(NOT WIN32)
    message(FATAL_ERROR "Este proyecto está configurado solo para Windows")
endif()

# Descargar JUCE si no existe
include(FetchContent)
FetchContent_Declare(JUCE
    URL https://github.com/juce-framework/JUCE/releases/download/7.0.9/JUCE-7.0.9-Windows.zip
)
FetchContent_MakeAvailable(JUCE)

# Plugin VST3
juce_add_plugin(FeedbackSuppressor
    PRODUCT_NAME "Feedback Suppressor"
    COMPANY_NAME "Audio Engineering"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE Aeng
    PLUGIN_CODE FbSu
    FORMATS VST3
    VST3_CATEGORIES "Fx"
)

# Fuentes
target_sources(FeedbackSuppressor PRIVATE
    src/PluginProcessor.cpp
    src/PluginProcessor.h
    src/PluginEditor.cpp
    src/PluginEditor.h
    src/DSP/FeedbackAnalyzer.cpp
    src/DSP/FeedbackAnalyzer.h
    src/DSP/NotchFilter.cpp
    src/DSP/NotchFilter.h
    src/DSP/PeakTracker.cpp
    src/DSP/PeakTracker.h
)

# Incluye JUCE
target_link_libraries(FeedbackSuppressor PRIVATE
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_formats
    juce::juce_audio_plugin_client
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_core
    juce::juce_data_structures
    juce::juce_dsp
    juce::juce_events
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_gui_extra
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
)

# C++17
target_compile_features(FeedbackSuppressor PRIVATE cxx_std_17)

# Optimizaciones de release (MSVC)
target_compile_options(FeedbackSuppressor PRIVATE /O2 /fp:fast /W4)
