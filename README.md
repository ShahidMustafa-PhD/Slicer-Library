#  DMLS Slicer-Library

![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)
![Platform: Windows](https://img.shields.io/badge/Platform-Windows-blue)
![Language: C++17](https://img.shields.io/badge/Language-C%2B%2B17-blue)
![Build: CMake](https://img.shields.io/badge/Build-CMake-orange)

A high-performance C++ slicing library and DLL for Laser Powder Bed Fusion (L-PBF) metal 3D printers, built on top of the Slic3r core geometry engine.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Architecture](#architecture)
- [Installation](#installation)
- [Usage](#usage)
- [Configuration](#configuration)
- [Testing](#testing)
- [Contributing](#contributing)
- [License](#license)
- [Acknowledgements](#acknowledgements)

## Overview

### Motivation
Selective Laser Melting (SLM) and other L-PBF technologies require specialized slicing strategies that differ significantly from standard FDM/FFF printing. This library bridges that gap by providing a robust slicing engine that generates machine-readable build files (`.marc`) specifically optimized for metal additive manufacturing.

### Target Users
- **Machine Manufacturers**: Integrating custom slicing into printer control software.
- **Researchers**: Developing new scanning strategies and exposure parameters for metal AM.
- **Software Engineers**: Building CAM software for industrial 3D printing.

### Significance
By leveraging the geometric robustness of Slic3r and extending it with SLM-specific logic (hatching, support generation, thermal segments), this project provides an industrial-grade solution for preparing metal 3D print jobs.

## Features

- **Advanced Slicing**:
  - Custom layer height processing.
  - Robust perimeter and infill generation.
  - Specialized support structure generation for metal overhangs.
- **L-PBF Specifics**:
  - Hatch generation (scan paths) for laser systems.
  - Thermal segment classification (Core, Overhang, Contour, etc.).
  - Multi-part build plate processing.
- **Binary Export**:
  - Optimized `.marc` binary format for fast machine data transfer.
  - Includes full layer geometry: hatches, polylines, and polygons.
  - Index table for efficient random access to layers.
- **Visualization**:
  - SVG export for layer-by-layer inspection of scan paths.
- **API Integration**:
  - C-style DLL interface for seamless integration with GUI applications (C#, Qt, etc.).

## Architecture

The project is structured into modular components to ensure separation of concerns:

- **Slic3r Core (`src/libslic3r`)**: The backbone geometry engine handling polygons, clipping, and basic slicing.
- **Marc Library (`marc_src/`)**: The specialized SLM logic layer.
  - `SlmPrint`: Orchestrates the slicing process.
  - `MarcFile`: Handles binary serialization/deserialization.
  - `MarcAPI`: C++ High-level API class.
  - `scanSegments`: Implements advanced scanning strategies.
- **DLL Interface (`marc_dll/`)**: A C-compatible wrapper (`MarcAPIInterface.h`) that exposes functionality to external applications.
- **Tests (`marcAPI_manager/`)**: Integration tests and CLI drivers.

### Code Structure

```plaintext
DMLSV_DLL_V4.0/
??? CMakeLists.txt          # Main build configuration
??? CMakePresets.json       # CMake presets for Ninja/VS
??? src/                    # External dependencies (Clipper, Poly2Tri, Boost, etc.)
??? marc_src/               # Core SLM logic implementation
?   ??? MarcFile.cpp        # Binary file I/O
?   ??? SlmPrint.cpp        # Main slicing controller
?   ??? ...
??? marc_dll/               # DLLExport/Import Interface
??? marcAPI_manager/        # Test application (Main.cpp)
??? Test/                   # Sample models and configs
```

## Installation

### Prerequisites

- **CMake**: Version 3.9 or higher (3.31.6+ recommended).
- **Compiler**: C++17 compliant compiler (GCC/MinGW 15+ recommended for Windows).
- **Build System**: Ninja is the preferred generator.
- **Boost**: Static libraries (System, Filesystem), specifically version 1.86 or compatible.
  - Configured path example: `C:/Dev/Boost-mingw-static`

### Build Instructions (Windows/MinGW)

1. **Clone the repository**:
   ```bash
   git clone https://github.com/ShahidMustafa-PhD/Slicer-Library.git
   cd Slicer-Library
   ```

2. **Configure with CMake Presets**:
   ```bash
   cmake --preset mingw-release
   ```
   *Note: Ensure your `CMakePresets.json` points to the correct vcpkg toolchain if used, or manually specify the toolchain.*

3. **Build**:
   ```bash
   cmake --build build-mingw-release
   ```

4. **Verify Build**:
   The output artifacts will be in `build-mingw-release/`:
   - `marcapi.dll` (The shared library)
   - `Marctest.exe` (The test executable)

## Usage

### Running the Test Driver

The `Marctest.exe` application demonstrates how to use the API to slice STL files.

```bash
cd build-mingw-release
./Marctest.exe
```

This will:
1. Load sample STL files configured in `Main.cpp`.
2. Apply SLM configurations (`.json`).
3. Slice the models.
4. Export the resulting `.marc` binary file and SVG debug layers.

### C API Example

If you are integrating `marcapi.dll` into your own application (e.g., C# or Python ctypes):

```c
#include "MarcAPIInterface.h"

// 1. Create API Instance
MarcHandle api = create_marc_api(250.0f, 250.0f, 5.0f);

// 2. Define Models
GuiData modelData = {0};
strcpy(modelData.path, "part.stl");
strcpy(modelData.buildconfig, "config.json");
modelData.number = 1;

GuiDataArray models = { &modelData, 1 };
set_models(api, models);

// 3. Process
update_model(api);
export_slm_file(api); // Generates output.marc

// 4. Cleanup
destroy_marc_api(api);
```

## Configuration

The slicer behavior is controlled via JSON configuration files.

- **Build Config (`marc_build_config.json`)**:
  - Layer thickness
  - Laser power/speed
  - Hatch spacing
  - Scaling factors

- **Build Styles (`marc_build_styles.json`)**:
  - Parameter sets for different material types or scanning strategies.

## Contributing

We welcome contributions to improve slicing algorithms, optimization, or platform support!

1. **Fork the repository**.
2. **Create a feature branch**: `git checkout -b feature/new-hatching-algo`
3. **Commit your changes**: `git commit -m "Add concentric hatching"`
4. **Push to the branch**: `git push origin feature/new-hatching-algo`
5. **Open a Pull Request**.

### Coding Standards
- Use **C++17** features where appropriate.
- Follow the existing style: 4-space indentation, clear variable names.
- Ensure all new features are covered by tests in `Marctest`.

## License

This project is licensed under the **MIT License** - see the LICENSE file for details.
*Note: Portions of the code are based on Slic3r (AGPLv3), please verify specific file headers for compliance if using commercially.*

## Acknowledgements

- **Slic3r**: For the foundational geometry libraries.
- **Boost**: For filesystem and system utilities.
- **Clipper**: For robust polygon offset and boolean operations.
- **Poly2Tri**: For constrained Delaunay triangulation.
