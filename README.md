# JvmAttachEnabler

A DLL to enable JVM attachment by modifying the DisableAttachMechanism flag.

## Description

JvmAttachEnabler is a Windows DLL designed to be injected into Java processes. It locates the jvm.dll module and modifies the DisableAttachMechanism flag, allowing for attachment to the JVM even when this mechanism is initially disabled.

## Features

- Locates jvm.dll using Windows internal structures
- Modifies the DisableAttachMechanism flag
- Provides console output for debugging

## Installation

1. Clone this repository
2. Compile the project as a DLL using a C++ compiler (e.g., Visual Studio)

## Usage

1. Inject the compiled DLL into the target Java process
2. The DLL will attempt to modify the DisableAttachMechanism flag
3. Check the console output for results

## How does this work?
JvmAttachEnabler works by modifying the DisableAttachMechanism flag in the JVM.

The flag is defined in the [OpenJDK source code](https://github.com/openjdk/jdk17/blob/4afbcaf55383ec2f5da53282a1547bac3d099e9d/src/hotspot/share/runtime/thread.cpp#L2944)

This static boolean controls whether the JVM allows attaching debuggers and tools.
The DLL finds the memory location of this flag in the loaded jvm.dll and sets it to false, enabling attachments.
By changing this flag at runtime, the DLL bypasses restrictions on attaching to the JVM process.

## Offsets
To find JVM offsets:
1. Use a disassembler like IDA Pro to analyze jvm.dll
2. Look for references to "DisableAttachMechanism" string
3. Find the corresponding boolean variable nearby
4. Calculate offset from dll base address
5. Use debugging and runtime analysis to verify offsets

## Contributing
Contributions are welcome. Please open an issue first to discuss what you would like to change.

## MIT License

Copyright (c) 2023 [Your Name]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
