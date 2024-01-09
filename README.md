# Stereo Kontrol

## Introduction
The "Stereo Kontrol" is an audio plugin developed using the JUCE framework. This plugin focuses on manipulating the stereo image of audio tracks, allowing users to dynamically adjust the stereo width of their audio.

## Features
The key feature of the plugin is the ability to control the stereo spread of the audio signal:

- **Stereo Width (`stereoWidth`):** This parameter adjusts the stereo width of the audio signal. It ranges from 0.0 (mono) to 1.5 (full stereo spread + 50%), allowing for precise control over the stereo field.

## Usage
1. Insert the plugin into your audio processing chain in your DAW (Digital Audio Workstation).
2. Adjust the `Stereo Width` parameter to achieve the desired stereo spread effect.
3. Experiment with different settings to see how they affect the stereo image of your audio.

## Technical Details
- The plugin uses a Mid-Side processing technique to manage the stereo image. It calculates the Mid (sum) and Side (difference) components of the stereo signal and adjusts the Side component based on the `Stereo Width` parameter.

![Stereo Kontrol](https://github.com/P23ASURE/kardboard-stereokontrol/blob/main/stereokontrol_screenshot.png)
