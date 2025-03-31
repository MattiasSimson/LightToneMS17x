# 🐺 WolfSound's Audio Plugin Template

![Cmake workflow success badge](https://github.com/JanWilczek/audio-plugin-template/actions/workflows/cmake.yml/badge.svg)

Want to create an audio plugin (e.g., a VST3 plugin) with C++ but don't know how to go about?

Heard about the [JUCE C++ framework](https://github.com/juce-framework/JUCE) but not sure how to start a JUCE project?

Want to use CMake with JUCE but don't know how?

Want to be able to easily integrate third-party C++ libraries to your project?

Want to unit test your audio plugin?

Want to ensure maximum safety of your software?

And all this with a click of a button?

Well, this template allows you to immediately start your JUCE C++ framework audio plugin project with a CMake-based project structure. It involves

* clear repo structure
* C++ 23 standard
* effortless handling of third-party dependencies with the CPM package manager; use the C++ libraries you want together with JUCE
* highest warning level and "treat warnings as errors"
* ready-to-go unit test project with GoogleTest

Additionally

* continuous integration made easy with Github actions: build and run tests on the main branch and on every pull request
* automatic clang-format on C++ files run on every commit; don't worry about code formatting anymore!

I am personally using this template all the time.

Feel free to propose suggestions 😉

## Usage

This is a template repository which means you can right click "Use this template" on GitHub and create your own repo out of it.

After cloning it locally, you can proceed with the usual CMake workflow.

In the main repo directory execute

```bash
$ cmake -S . -B build
$ cmake --build build
```

The first run will take the most time because the dependencies (CPM, JUCE, and googletest) need to be downloaded.

Alternatively, you can use bundled CMake presets:

```bash
$ cmake --preset default # uses the Ninja build system
$ cmake --build build
$ ctest --preset default
```

Existing presets are `default`, `release`, and `Xcode`.

To run clang-format on every commit, in the main directory execute

```bash
pre-commit install
```

(for this you may need to install `pre-commit` with `pip`: `pip install pre-commit`).

Don't forget to change "YourPluginName" to, well, your plugin name everywhere 😉

## How was this template built?

See how I create this template step by step in this video:

[![Audio plugin template tutorial video](http://img.youtube.com/vi/Uq7Hwt18s3s/0.jpg)](https://www.youtube.com/watch?v=Uq7Hwt18s3s "Audio plugin template tutorial video")

# LightTone MS-17x

A professional-grade stereo EQ plugin with independent left and right channel controls. Built with JUCE framework.

## Features

- 4-band EQ per channel (8 bands total)
- Independent left and right channel control
- Frequency bands:
  - Low: 100Hz (low shelf)
  - Low-Mid: 400Hz (peak)
  - High-Mid: 2kHz (peak)
  - High: 8kHz (high shelf)
- Gain range: -24dB to +24dB
- Q factor: 0.7 (for peak filters)
- Real-time parameter control
- VST3 format support

## Building from Source

### Prerequisites

- CMake 3.16 or higher
- C++17 compatible compiler
- JUCE framework (automatically downloaded via CMake)

### Build Instructions

1. Clone the repository:
```bash
git clone https://github.com/MattiasSimson/LightToneMS17x.git
cd LightToneMS17x
```

2. Create and enter build directory:
```bash
mkdir build
cd build
```

3. Configure with CMake:
```bash
cmake ..
```

4. Build the project:
```bash
cmake --build .
```

The built VST3 plugin will be available in the `build/plugin/AudioPlugin_artefacts/Debug/VST3/` directory.

## Usage

1. Load the plugin in your DAW
2. Use the left knobs to control the left channel EQ
3. Use the right knobs to control the right channel EQ
4. Each frequency band can be adjusted independently
5. The plugin supports both mono and stereo input/output

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Author

Mattias Simson
