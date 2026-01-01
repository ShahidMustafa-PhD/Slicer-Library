#pragma once
//#include "../libslic3r/libslic3r.h"
//#include "../clipper.hpp"
#include "../libslic3r/ExPolygon.hpp"
#include "../libslic3r/Line.hpp"
//#include "../libslic3r/TriangleMesh.hpp"
#include "../libslic3r/Surface.hpp"
//#include "../libslic3r/Print.hpp"
//#include "../libslic3r/ExtrusionEntity.hpp"
//#include "../libslic3r/SLAPrint.hpp"
//#include "MarcFile.hpp"
//#include <memory>
namespace Marc {
// -------------------------------------------
// Custom Binary File Format: .marc
// Used in L-PBF (Laser Powder Bed Fusion) 3D Metal Printer
// -------------------------------------------

// -------------------------------
// File Header Structure (160 bytes)
// -------------------------------
struct MarcHeader {
    char magic[4] = {'M','A','R','C'};         // 4 bytes: File identifier
    uint32_t version = 1;                      // 4 bytes: Version number of format
    uint32_t totalLayers = 0;                  // 4 bytes: Total number of layers
    uint64_t indexTableOffset = 0;             // 8 bytes: File offset where index table starts
    uint64_t timestamp = std::time(nullptr);   // 8 bytes: File creation timestamp
    char printerId[32] = {0};                  // 32 bytes: Optional printer identification
    char reserved[100] = {0};                  // 100 bytes: Reserved for future use
};

// -------------------------------
// Geometry Structures
// -------------------------------
struct Point {
    float x;  // X-coordinate
    float y;  // Y-coordinate
};

struct Line {
    Point a;  // Start point
    Point b;  // End point
};

struct GeometryTag {
    uint32_t type;          // 4 bytes: Subtype (0–15)
    uint32_t category;      // 4 bytes: 1=hatch, 2=polyline, 3=polygon, 4=point
    uint32_t pointCount;    // 4 bytes: Total number of points
};

struct Circle {
    GeometryTag tag;  // Geometry metadata
    Point center;    // center  points
    float radius;
};
struct Hatch {
    GeometryTag tag;              // Geometry metadata
    std::vector<Line> lines;      // Vector of line segments
};

struct Polyline {
    GeometryTag tag;              // Geometry metadata
    std::vector<Point> points;    // Vector of connected points
};

struct Polygon {
   GeometryTag tag;              // Geometry metadata
    std::vector<Point> points;    // Vector of polygon points
};

// -------------------------------
// Layer Data Block
// -------------------------------
struct Layer {
    uint32_t layerNumber;                      // Unique layer number
    float layerHeight;
    float layerThickness;                         // Layer thickness
    std::vector<Hatch> hatches;                // Collection of hatches
    std::vector<Polyline> polylines;           // Collection of polylines
    std::vector<Polygon> polygons;             // Collection of polygons
    std::vector<Circle> support_circles;       // Collection of support circles
};
class ExportMarcSlice
{
    public:
   
// Default constructor
ExportMarcSlice();

// Initializer list constructor
ExportMarcSlice(const Marc::Point& _origin);
       
   Point origin;
   
   // ~ExportMarcSlice() {  }

   // void m_convert();
   

    void  convrt(const Slic3r::Line& line, Line& m_line);
    //void draw(const ThickLine &line);
    void  convrt(const Slic3r::Lines& lines,std::vector<Marc::Line>& m_lines);
    //void draw(const IntersectionLines &lines);

    void convrt(const Slic3r::ExPolygon& expolygon, std::vector<Polygon>& m_polygons);
    
    void convrt(const Slic3r::ExPolygons& expolygons,std::vector<Polygon>& m_polygons);
    
   
    void convrt(const Slic3r::Surface& surface,std::vector<Polygon>& m_polygons);
    
    void convrt(const Slic3r::Surfaces& surfaces,std::vector<Polygon>& m_polygons);
    
    void Close();
   
    
    void  convrt(const Slic3r::Polygon& polygon, Polygon& m_polygon);
    void convrt(const Slic3r::MultiPoint& mp, Polyline& m_polyline ) const;
    void convrt(const Slic3r::Polygons& polygons,std::vector<Polygon>& m_polygons);
    void convrt(const Slic3r::Polyline& polyline,Polyline& m_polyline);
    void convrt(const Slic3r::Polylines& polylines, std::vector<Polyline>& m_polylines);
    
    void  convrt(const Slic3r::Point& point, Point& m_point);
    void convrt(const Slic3r::Points& points, std::vector<Point>& m_points);

   
    
    private:
    std::string filename;
   
    //std::unique_ptr<Slic3r::SLAPrint> slaprint;
    //std::vector<Layer> m_layers; // Layers to be filled
    //LayerPtrs layers;
   
    
    
    //GeometryTag  m_tag;
    
       
    //Layer* m_layer;
        


};


}

//  Driver Programme. follow  this programme. it also generates the svg files of layers...

/*
#include <iostream>
#include <vector>
#include "MarcFile.hpp"

#include <fstream>



#include <sstream>
#include <iomanip>


void printMarcFileContent(const MarcFile& marcFile) ;
int test() ;
void generateSVGForMarcFile(const MarcFile& marcFile, const std::string& outputDir);
void generateSVGForLayer(const Layer& layer, const std::string& outputPath);
int svg();
int main() {
    // Create some sample layer data for testing
    std::vector<Layer> layers;
    for (int i = 0; i < 900; ++i) {
       
   
    // Create Layer 1 with some hatches, polylines, and polygons
    Layer layer1;
    layer1.layerNumber = i;
    layer1.layerHeight = 0.05f;  // 5mm layer height

    // Create some hatch lines for Layer 1
    Hatch hatch1;
    hatch1.tag.type = 0;           // Type 0 (e.g., Hatching)
    hatch1.tag.category = 1;       // Hatch category
    hatch1.tag.pointCount = 4;     // Two lines (4 points)
    hatch1.lines.push_back({{100.0f, 100.0f}, {500.0f, 100.0f}});
    hatch1.lines.push_back({{100.0f, 100.0f}, {500.0f, 200.0f}});
    layer1.hatches.push_back(hatch1);

    // Create some polyline points for Layer 1
    Polyline polyline1;
    polyline1.tag.type = 1;        // Type 1 (e.g., Polyline)
    polyline1.tag.category = 2;    // Polyline category
    polyline1.tag.pointCount = 3;  // Three points
    polyline1.points.push_back({100.0f, 100.0f});
    polyline1.points.push_back({200.0f, 100.0f});
    polyline1.points.push_back({200.0f, 800.0f});
    layer1.polylines.push_back(polyline1);

    // Create some polygon points for Layer 1
    Polygon polygon1;
    polygon1.tag.type = 2;         // Type 2 (e.g., Polygon)
    polygon1.tag.category = 3;     // Polygon category
    polygon1.tag.pointCount = 4;   // Four points
    polygon1.points.push_back({100.0f, 100.0f});
    polygon1.points.push_back({200.0f, 100.0f});
    polygon1.points.push_back({200.0f, 200.0f});
    polygon1.points.push_back({100.0f, 200.0f});
    layer1.polygons.push_back(polygon1);

    // Add Layer 1 to layers vector
    layers.push_back(layer1);

    // Create Layer 2 with different data
    Layer layer2;
    layer2.layerNumber = i+1;
    layer2.layerHeight = 0.1f;  // 10mm layer height

    // Add some hatches, polylines, and polygons to Layer 2 (similar to Layer 1)
    Hatch hatch2;
    hatch2.tag.type = 0;
    hatch2.tag.category = 1;
    hatch2.tag.pointCount = 4;
    hatch2.lines.push_back({{300.0f, 300.0f}, {300.0f, 100.0f}});
    hatch2.lines.push_back({{300.0f, 10.0f}, {600.0f, 10.0f}});
    layer2.hatches.push_back(hatch2);

    Polyline polyline2;
    polyline2.tag.type = 1;
    polyline2.tag.category = 2;
    polyline2.tag.pointCount = 3;
    polyline2.points.push_back({10.0f, 10.0f});
    polyline2.points.push_back({30.0f, 10.0f});
    polyline2.points.push_back({30.0f, 30.0f});
    layer2.polylines.push_back(polyline2);

    Polygon polygon2;
    polygon2.tag.type = 2;
    polygon2.tag.category = 3;
    polygon2.tag.pointCount = 4;
    polygon2.points.push_back({50.0f, 50.0f});
    polygon2.points.push_back({90.0f, 50.0f});
    polygon2.points.push_back({90.0f, 90.0f});
    polygon2.points.push_back({50.0f, 90.0f});
    layer2.polygons.push_back(polygon2);

    // Add Layer 2 to layers vector
    layers.push_back(layer2);
//
  // Create Layer 2 with different data
  
  // Create Layer 2 with different data
  Layer layer3;
  layer3.layerNumber = i+2;
  layer3.layerHeight = 0.1f;  // 10mm layer height

  Polygon polygon3;

  
  polygon3.tag.pointCount = 2;
  polygon3.points.push_back({50.0f, 50.0f});
  polygon3.points.push_back({90.0f, 50.0f});
 
  layer3.polygons.push_back(polygon3);

  // Add some hatches, polylines, and polygons to Layer 3 (similar to Layer 1)
  Hatch hatch3;
  hatch3.tag.type = 0;
  hatch3.tag.category = 1;
  hatch3.tag.pointCount = 4;
  hatch3.lines.push_back({{300.0f, 300.0f}, {300.0f, 100.0f}});
  hatch3.lines.push_back({{300.0f, 10.0f}, {600.0f, 10.0f}});
  layer3.hatches.push_back(hatch2);

  
 

  

  layers.push_back(layer3);
    }
    // Create MarcFile object and set its layers
    MarcFile marcFile;
    marcFile.layers = layers;

    // Write the .marc file
    std::string filename = "test.marc";
    if (marcFile.writeToFile(filename)) {
        std::cout << "File written successfully!" << std::endl;
    } else {
        std::cerr << "Failed to write the file!" << std::endl;
        return 1;
    }

    // Read the .marc file
    MarcFile readMarcFile;
    if (readMarcFile.readFromFile(filename)) {
        std::cout << "File read successfully!" << std::endl;

        // Optionally, print out read data to verify correctness
        std::cout << "Number of layers read: " << readMarcFile.layers.size() << std::endl;
        for (const auto& layer : readMarcFile.layers) 
        {
            std::cout << "Layer " << layer.layerNumber << " has " 
                      << layer.hatches.size() << " hatches, " 
                      << layer.polylines.size() << " polylines, and " 
                      << layer.polygons.size() << " polygons." << std::endl;
        } 
    } else {
        std::cerr << "Failed to read the file!" << std::endl;
        return 1;
    }
  //test();
  //svg();
    return 0;
}


// Function to print the content of the MarcFile for debugging purposes
void printMarcFileContent(const MarcFile& marcFile) {
    std::cout << "Number of Layers: " << marcFile.layers.size() << std::endl;
    
    // Iterate over all layers
    for (size_t i = 0; i < marcFile.layers.size(); ++i) {
        const Layer& layer = marcFile.layers[i];
        std::cout << "Layer " << layer.layerNumber << " (Height: " << layer.layerHeight << "):\n";
        
        // Print Hatches
        std::cout << "  Hatches: " << layer.hatches.size() << std::endl;
        for (const auto& hatch : layer.hatches) {
            std::cout << "    Hatch Type: " << hatch.tag.type 
                      << ", Category: " << hatch.tag.category 
                      << ", Points: " << hatch.tag.pointCount << std::endl;
            for (const auto& line : hatch.lines) {
                std::cout << "      Line: (" << line.p1.x << ", " << line.p1.y << ") -> (" 
                          << line.p2.x << ", " << line.p2.y << ")\n";
            }
        }

        // Print Polylines
        std::cout << "  Polylines: " << layer.polylines.size() << std::endl;
        for (const auto& polyline : layer.polylines) {
            std::cout << "    Polyline Type: " << polyline.tag.type 
                      << ", Category: " << polyline.tag.category 
                      << ", Points: " << polyline.tag.pointCount << std::endl;
            for (const auto& point : polyline.points) {
                std::cout << "      Point: (" << point.x << ", " << point.y << ")\n";
            }
        }

        // Print Polygons
        std::cout << "  Polygons: " << layer.polygons.size() << std::endl;
        for (const auto& polygon : layer.polygons) {
            std::cout << "    Polygon Type: " << polygon.tag.type 
                      << ", Category: " << polygon.tag.category 
                      << ", Points: " << polygon.tag.pointCount << std::endl;
            for (const auto& point : polygon.points) {
                std::cout << "      Point: (" << point.x << ", " << point.y << ")\n";
            }
        }
    }
}

int test() {
   // Create MarcFile object and read the contents of a .marc file
   MarcFile marcFile;
   std::string filename = "test.marc";  // Replace with your actual file path
   
   if (marcFile.readFromFile(filename)) {
       std::cout << "File read successfully!\n";
       // Print out the contents of the MarcFile for debugging
       printMarcFileContent(marcFile);
   } else {
       std::cerr << "Failed to read the file!\n";
       return 1;
   }

   return 0;
}



// Helper function to create an SVG file for each layer
void generateSVGForLayer(const Layer& layer, const std::string& outputPath) {
    std::ostringstream svgContent;
    
    // SVG Header
    svgContent << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"500\" height=\"500\">\n";

    // Set a simple background color (optional)
    svgContent << "<rect width=\"100%\" height=\"100%\" fill=\"white\" />\n";

    // Draw Hatches
    for (const auto& hatch : layer.hatches) {
        for (const auto& line : hatch.lines) {
            svgContent << "<line x1=\"" << line.p1.x << "\" y1=\"" << line.p1.y << "\" "
                       << "x2=\"" << line.p2.x << "\" y2=\"" << line.p2.y << "\" "
                       << "stroke=\"black\" stroke-width=\"1\" />\n";
        }
    }

    // Draw Polylines
    for (const auto& polyline : layer.polylines) {
        for (size_t i = 0; i < polyline.points.size() - 1; ++i) {
            const Point& p1 = polyline.points[i];
            const Point& p2 = polyline.points[i + 1];
            svgContent << "<line x1=\"" << p1.x << "\" y1=\"" << p1.y << "\" "
                       << "x2=\"" << p2.x << "\" y2=\"" << p2.y << "\" "
                       << "stroke=\"blue\" stroke-width=\"2\" />\n";
        }
    }

    // Draw Polygons
    for (const auto& polygon : layer.polygons) {
        svgContent << "<polygon points=\"";
        for (const auto& point : polygon.points) {
            svgContent << point.x << "," << point.y << " ";
        }
        svgContent << "\" fill=\"none\" stroke=\"green\" stroke-width=\"2\" />\n";
    }

    // SVG Footer
    svgContent << "</svg>\n";

    // Write to file
    std::ofstream outFile(outputPath);
    if (outFile.is_open()) {
        outFile << svgContent.str();
        outFile.close();
        std::cout << "SVG file generated: " << outputPath << std::endl;
    } else {
        std::cerr << "Failed to create SVG file: " << outputPath << std::endl;
    }
}

// Function to iterate through the layers and generate SVG for each layer
void generateSVGForMarcFile(const MarcFile& marcFile, const std::string& outputDir) {
    for (size_t i = 0; i < marcFile.layers.size(); ++i) {
        const Layer& layer = marcFile.layers[i];
        std::string outputPath = "layer_" + std::to_string(layer.layerNumber) + ".svg";
        generateSVGForLayer(layer, outputPath);
    }
}

int svg() {
    // Create MarcFile object and read the contents of a .marc file
    MarcFile marcFile;
    std::string filename = "test.marc";  // Replace with your actual file path
    std::string outputDir = "./output";  // Output directory to save SVGs

    if (marcFile.readFromFile(filename)) {
        std::cout << "File read successfully!\n";
        
        // Generate SVG files for each layer
        generateSVGForMarcFile(marcFile, outputDir);
    } else {
        std::cerr << "Failed to read the file!\n";
        return 1;
    }

    return 0;
}

*/