# AI Coding Instructions for DMLSV_DLL_V4.0

## Architecture Overview
This is a 3D printing slicer DLL for Selective Laser Melting (SLM), built on Slic3r core library. Key components:
- **Slic3r**: Core geometry processing and slicing (static lib in `src/libslic3r/`)
- **Marc namespace**: SLM-specific extensions (`marc_src/`) including `SlmPrint` (inherits `Slic3r::SimplePrint`)
- **MarcAPI**: C++ API class for model/config management (`marc_src/MarcAPI.hpp`)
- **MarcFile**: Binary file serializer for layer data (`marc_src/MarcFile.hpp/cpp`)
- **DLL Interface**: C API wrapper (`marc_dll/`) for external integration
- **Data Flow**: STL models → Slic3r slicing → SLM processing (hatches/supports) → MARC file export

## Critical Workflows
- **Build**: `cmake -S . -B build && cmake --build build` (requires Boost at `C:/Dev/Boost-mingw-static`, static linking)
- **Test Run**: Execute `Test/Marctest.exe` (hardcoded paths in `marcAPI_manager/Main.cpp`)
- **Debug**: Use RelWithDebInfo build type; attach debugger to Marctest.exe
- **Config**: JSON files for build styles (`marc_build_styles.json`) and SLM config (`marc_build_config.json`)

## Project-Specific Patterns

### Layer Processing Pipeline
The `MarcFile::addLayers()` method demonstrates the complete layer data extraction workflow:
1. **Hatches**: Extract scan segments via `SlmHatch` objects (primary laser scan paths)
2. **Perimeters**: Process via `ExtrusionPath` and `ExtrusionLoop` (boundary contours)
3. **Fills**: Extract from `ExtrusionEntityCollection` (infill geometry)
4. **Support Surfaces**: Convert `SurfaceCollection` to Marc polygons with `ExPolygon` handling (contours + holes)

### Polymorphic Entity Processing
- **ExtrusionPath**: Single polyline → convert to Marc::Polyline
- **ExtrusionLoop**: Closed contour of paths → iterate paths and convert each
- **ExtrusionEntityCollection**: Nested containers → recursive processing (see `processExtrusionEntity()`)
- **Dynamic casting pattern**: Used for safe type-checking at runtime before processing

### Geometry Conversion
All conversions done via `ExportMarcSlice` converter class with overloaded `convrt()` methods:
- `convrt(Slic3r::Lines, std::vector<Marc::Line>&)` - For hatches
- `convrt(Slic3r::Polyline, Marc::Polyline&)` - For extrusion paths
- `convrt(Slic3r::Polygon, Marc::Polygon&)` - For support surfaces

### Data Organization
- **GeometryTag struct**: Metadata for all geometry (type, category, pointCount)
- **Categories**: 1=hatch, 2=polyline, 3=polygon, 4=point
- **Type field**: Stores ExtrusionRole enum or ThermalSegmentType for later processing
- **Null checks**: Always verify pointers before dereferencing (converter, entities)

## Integration Points
- **Slic3r Integration**: `SlmPrint` extends `SimplePrint`; calls `print.process()` to fill data structures
- **External Deps**: Boost filesystem/system, static linking; third-party libs (clipper, expat, poly2tri) in `src/`
- **File Formats**: Input STL, output .marc binary file (160-byte header + layer blocks + index table)
- **Error Handling**: `MarcErrorCode` enums (MARC_S_OK, MARC_E_FAIL) with `get_last_error_message()`

## Key Files
- `marc_src/SlmPrint.cpp`: Core SLM processing logic (calls `marcFile->addLayers()`)
- `marc_src/MarcFile.cpp`: Binary file I/O and layer extraction
- `marc_src/ExportMarcSlice.hpp`: Geometry conversion utilities
- `marc_dll/MarcAPIInterface.cpp`: DLL C wrapper
- `CMakeLists.txt`: Build configuration with Boost paths and static targets</content>
<parameter name="filePath">c:\Projects\DMLSV_DLL_V4.0\.github\copilot-instructions.md