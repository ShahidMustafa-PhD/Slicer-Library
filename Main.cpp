	

//#include "mainwindow.h"
//#include <QApplication>
//#include <crtdbg.h>
#include "marcAPI_manager/MarcAPIManager.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <cstdlib>  
//#include <filesystem>

/**
 * @file Main.cpp
 * @brief Entry point for the application demonstrating the usage of the MarcAPI.
 *
 * This program creates an instance of the MarcAPI using a factory function, sets up
 * models, configurations, actions, and transformations, and performs operations such
 * as updating the model, duplicating it, and printing its state. Finally, it cleans up
 * by destroying the API instance.
 *
 * @details
 * - The MarcAPI instance is created with initial parameters (200.0f, 200.0f).
 * - Models, actions, transformations, and configurations are set using respective methods.
 * - The `updateModel`, `duplicate`, and `print` methods demonstrate the API's functionality.
 * - Proper cleanup is ensured by calling `Marc::destroy_marc_api` to release resources.
 *
 * @return int Returns 0 upon successful execution.
 */
 int main() {
        // Create
    //MarcPathArray model_paths;
    //model_paths.paths = modPaths;
    //model_paths.count = modCount;

    //MarcPathArray config_paths;
    //config_paths.paths = cofPaths;
    //config_paths.count = cofCount;
   
        //"C:/Projects/DMLS_DLL_Test/build/bunny.STL",
      
        // "C:/Projects/DMLSV_DLL/Test/femure.STL",
           //"C:/Projects/DMLSV_DLL/Test/bridge2.STL"
               //model_paths.paths = modPaths1;
    //model_paths.count = 2;


   
        //"C:/Projects/DMLS_DLL_Test/build/bunny.ini",
    
       //"C:/Projects/DMLSV_DLL/Test/femure.ini" ,
      //"C:/Projects/DMLSV_DLL/Test/build/S.ini",
       //"C:/Projects/DMLSV_DLL/Test/bridge2.ini"
        

    
       std::vector<std::filesystem::path> model_paths;
std::filesystem::path filePath = "C:/Projects/DMLSV_DLL/Test/femure.STL";
model_paths.push_back(filePath);
    
    
filePath =  "C:/Projects/DMLSV_DLL/Test/femure.ini";
std::vector<std::filesystem::path> config_paths;
config_paths.push_back(filePath);

Marc::MarcAPIManager manager(200.0f, 200.0f);

manager.setModels(model_paths);
manager.setConfigs(config_paths);
manager.updateModel();
manager.exportSlmFile();
    //manager.save_conf();
    //manager.repair();
    //manager.info();
    //manager.print();
    //manager.exportslasvg();
    //manager.exportSlmFile();
    return 0;
    }
    

