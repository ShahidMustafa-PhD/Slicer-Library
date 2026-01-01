#ifndef MARC_API_INTERFACE_HPP
#define MARC_API_INTERFACE_HPP

#ifdef _WIN32
  //#ifdef MARC_EXPORTS
    // If we're building the DLL, use dllexport
    //#define MARC_API __declspec(dllexport)
  //#else
    // If we're using the DLL, use dllimport
    #define MARC_API __declspec(dllimport)
  //#endif
#else
  #define MARC_API
#endif

#include <cstddef>
#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

typedef int MarcErrorCode;

#define MARC_S_OK        0x00000000
#define MARC_E_FAIL      0x80004005
#define MARC_E_INVALID   0x80070057
#define MARC_E_NOTIMPL   0x80004001

MARC_API const char* get_last_error_message();

// Callback function signature
typedef void (*MarcProgressCallback)(const char* message, int percent);

// C-style struct to pass paths
struct MarcPathArray {
    const char** paths;
    std::size_t count;
};

// Opaque pointer to the internal implementation
typedef void* MarcHandle;

// Factory functions
MARC_API MarcHandle create_marc_api(float bed_width, float bed_depth, float spacing);
MARC_API void destroy_marc_api(MarcHandle handle);

// Set input data
MARC_API MarcErrorCode set_models(MarcHandle handle, MarcPathArray models);
MARC_API MarcErrorCode set_configs(MarcHandle handle, MarcPathArray configs);
MARC_API MarcErrorCode set_actions(MarcHandle handle, const char** actions, std::size_t count);
MARC_API MarcErrorCode set_transforms(MarcHandle handle, const char** transforms, std::size_t count);

// Operations
MARC_API MarcErrorCode update_model(MarcHandle handle);
MARC_API MarcErrorCode duplicate(MarcHandle handle);
MARC_API MarcErrorCode duplicate_grid(MarcHandle handle);
MARC_API MarcErrorCode around_center(MarcHandle handle);
MARC_API MarcErrorCode align_xy(MarcHandle handle, float px, float py);
MARC_API MarcErrorCode rotate_x(MarcHandle handle, float angle);
MARC_API MarcErrorCode rotate_y(MarcHandle handle, float angle);
MARC_API MarcErrorCode rotate_z(MarcHandle handle, float angle);
MARC_API MarcErrorCode scale(MarcHandle handle, float s);
MARC_API MarcErrorCode scale_to_fit(MarcHandle handle);
MARC_API MarcErrorCode split(MarcHandle handle);
MARC_API MarcErrorCode repair(MarcHandle handle);
MARC_API MarcErrorCode info(MarcHandle handle);
MARC_API MarcErrorCode print(MarcHandle handle);

// Export
MARC_API MarcErrorCode export_slm_file(MarcHandle handle);

// Register callback
MARC_API void register_progress_callback(MarcHandle handle, MarcProgressCallback callback);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // MARC_API_INTERFACE_HPP
