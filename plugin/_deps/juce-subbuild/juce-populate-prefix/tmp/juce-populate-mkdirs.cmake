# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/Mattias Simson/KoodJhvi/PLUGINS/audio-plugin-starter/audio-plugin-template/libs/juce"
  "C:/Users/Mattias Simson/KoodJhvi/PLUGINS/audio-plugin-starter/audio-plugin-template/plugin/_deps/juce-build"
  "C:/Users/Mattias Simson/KoodJhvi/PLUGINS/audio-plugin-starter/audio-plugin-template/plugin/_deps/juce-subbuild/juce-populate-prefix"
  "C:/Users/Mattias Simson/KoodJhvi/PLUGINS/audio-plugin-starter/audio-plugin-template/plugin/_deps/juce-subbuild/juce-populate-prefix/tmp"
  "C:/Users/Mattias Simson/KoodJhvi/PLUGINS/audio-plugin-starter/audio-plugin-template/plugin/_deps/juce-subbuild/juce-populate-prefix/src/juce-populate-stamp"
  "C:/Users/Mattias Simson/KoodJhvi/PLUGINS/audio-plugin-starter/audio-plugin-template/plugin/_deps/juce-subbuild/juce-populate-prefix/src"
  "C:/Users/Mattias Simson/KoodJhvi/PLUGINS/audio-plugin-starter/audio-plugin-template/plugin/_deps/juce-subbuild/juce-populate-prefix/src/juce-populate-stamp"
)

set(configSubDirs Debug)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/Mattias Simson/KoodJhvi/PLUGINS/audio-plugin-starter/audio-plugin-template/plugin/_deps/juce-subbuild/juce-populate-prefix/src/juce-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/Mattias Simson/KoodJhvi/PLUGINS/audio-plugin-starter/audio-plugin-template/plugin/_deps/juce-subbuild/juce-populate-prefix/src/juce-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
