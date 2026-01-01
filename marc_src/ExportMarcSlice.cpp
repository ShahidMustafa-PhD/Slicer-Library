
#include "ExportMarcSlice.hpp"
#include <iostream>
#include "../libslic3r/Surface.hpp"
#define COORD(x) (static_cast<float>(Slic3r::unscale((x))))   //  scaled to 1mm.

namespace Marc{

 // Default constructor
ExportMarcSlice::ExportMarcSlice()
: origin()  // Calls Point's default constructor (should initialize to 0,0)
{}

// Constructor with parameters for origin
ExportMarcSlice::ExportMarcSlice(const Marc::Point& _origin)
: origin(_origin)  // Uses Point's initializer list constructor
{}

    void
    ExportMarcSlice::convrt(const Slic3r::Line &line, Line &m_line)
    {

        Point a;
        Point b;
         a.x=COORD(line.a.x )- origin.x;  // Shift back
         a.y=COORD(line.a.y )- origin.y;
         b.x=COORD(line.b.x )- origin.x;
         b.y=COORD(line.b.y) - origin.y;//COORD(stroke_width*100000));
        //p1.x = COORD(line.a.x);
        //p1.y=COORD(line.a.y );
        //p2.x=COORD(line.b.x ); 
        //p2.y=COORD(line.b.y );//COORD(stroke_width*100000));
    m_line.a=a;
    m_line.b=b;
      
}


// m_lines are returned....

void ExportMarcSlice::convrt(const Slic3r::Lines& lines, std::vector<Line>& m_lines)
{
    for (const auto& line : lines)
    {
        Line m_line;
        this->convrt(line, m_line);
        m_lines.emplace_back(std::move(m_line)); // More efficient
    }
}





void ExportMarcSlice::convrt(const Slic3r::ExPolygon& expolygon, std::vector<Polygon>& m_polygons)
{
    Polygon contour_polygon;
    convrt(expolygon.contour, contour_polygon);
    //m_polygons.push_back(contour_polygon);
    m_polygons.emplace_back(std::move(contour_polygon)); // More efficient
    for (const auto& hole : expolygon.holes)
    {
        Polygon hole_polygon;
        convrt(hole, hole_polygon);
        //m_polygons.push_back(hole_polygon);
        m_polygons.emplace_back(std::move(hole_polygon)); // More efficient
    }
}

void ExportMarcSlice::convrt(const Slic3r::ExPolygons& expolygons, std::vector<Polygon>& m_polygons)
{
    for (const auto& expolygon : expolygons)
    {
        convrt(expolygon, m_polygons);
    }
}

void ExportMarcSlice::convrt(const Slic3r::Polygon& polygon, Polygon& m_polygon)
{
    std::vector<Point> m_points;
    for (const auto& p : polygon.points)
    {
        Point m_point;
        m_point.x = COORD(p.x)-origin.x;
        m_point.y = COORD(p.y)-origin.y;
        m_points.push_back(m_point);
    }
    m_polygon.points = std::move(m_points);
}



void ExportMarcSlice::convrt(const Slic3r::Polyline& polyline, Polyline& m_polyline)
{
    std::vector<Point> m_points;
    for (const auto& p : polyline.points)
    {
        Point m_point;
        m_point.x = COORD(p.x )-origin.x;
        m_point.y = COORD(p.y )-origin.y;
        m_points.push_back(m_point);
    }
    m_polyline.points = std::move(m_points);
}
    



void ExportMarcSlice::convrt(const Slic3r::Polylines& polylines, std::vector<Polyline>& m_polylines)
{
    for (const auto& polyline : polylines)
    {
        Polyline m_polyline;
        convrt(polyline, m_polyline);
        //m_polylines.push_back(m_polyline);
        m_polylines.emplace_back(std::move(m_polyline)); // More efficient
    }
}





void ExportMarcSlice::convrt(const Slic3r::Point& point, Point& m_point)
{
    m_point.x = COORD(point.x )-origin.x;
    m_point.y = COORD(point.y )-origin.y;
}

void ExportMarcSlice::convrt(const Slic3r::Points& points, std::vector<Point>& m_points)
{
    for (const auto& point : points)
    {
        Point m_point;
        this->convrt(point, m_point);
       // m_points.push_back(m_point);
        m_points.emplace_back(std::move(m_point)); // More efficient
    }
}


void ExportMarcSlice::convrt(const Slic3r::MultiPoint& mp, Polyline& m_polyline) const
{
    std::vector<Point> m_points;
    for (const auto& p : mp.points)
    {
        Point m_point;
        m_point.x = COORD(p.x )-origin.x;
        m_point.y = COORD(p.y )-origin.y;
        m_points.push_back(m_point);
    }
    m_polyline.points = std::move(m_points);
}

void ExportMarcSlice::convrt(const Slic3r::Polygons& polygons, std::vector<Polygon>& m_polygons)
{
    for (const auto& polygon : polygons)
    {
        Polygon m_polygon;
        convrt(polygon, m_polygon);  // This calls the existing draw function for Polygon
        //m_polygons.push_back(std::move(m_polygon));  // Use emplace_back or push_back with move for efficiency
        m_polygons.emplace_back(std::move(m_polygon)); // More efficient
    }
}

void
ExportMarcSlice::convrt(const Slic3r::Surface& surface,std::vector<Polygon>& m_polygons)
{
    convrt(surface.expolygon,m_polygons);
}



void
ExportMarcSlice::convrt(const Slic3r::Surfaces& surfaces, std::vector<Polygon>& m_polygons)
{
    for (const auto& surface : surfaces)
    {
        convrt(surface, m_polygons);
    }
}








void
ExportMarcSlice::Close()
{
 
}


//void ExportMarcSlice::m_convert()
//{// make m-point
        // make m_Line
        // make EntityGroup
        // make m_Layer
        // push into std::vector<m_Layer> layers
     
    
    
//}
 
    
}
/* enum BuildStyleID
    {
        CoreContour_Volume           = 1, // core contour on volume 
        CoreContour_Overhang         = 2, // core contour on powder 
        HollowShell1Contour_Volume   = 3,//  shell1 contour on volume 
        HollowShell1Contour_Overhang = 4,//  shell1 contour on powder 
        HollowShell2Contour_Volume   = 5,//  shell2 contour on volume 
        HollowShell2Contour_Overhang = 6,//  shell2 contour on powder 
        CoreOverhangHatch            = 7, //  core hatch on powder 
        CoreNormalHatch              = 8, //  core hatch on volume 
        CoreContourHatch             = 9, //  core contour hatch 
        HollowShell1OverhangHatch    = 10, //  shell1 hatch on powder 
        HollowShell1NormalHatch      = 11,  //  shell1 hatch on volume 
        HollowShell1ContourHatch     = 12, //  shell1 contour hatch 
        HollowShell2OverhangHatch    = 13, //  shell2 hatch on powder 
        HollowShell2NormalHatch      = 14,  //  shell2 hatch on volume 
        HollowShell2ContourHatch     = 15, //  shell2 contour hatch 
        SupportContourVolume         = 16, //  support contour 
        SupportHatch                 = 17,  //  support hatch 
        PointSequence                = 18, //  point sequence 
        ExternalSupports             = 19, //  Externe Stützen 
        CoreContourHatchOverhang     = 20, //  HollowCore Konturversatz - Overhang 
        HollowShell1ContourHatchOverhang = 21, //  HollowShell1  - Overhang 
        HollowShell2ContourHatchOverhang = 22, //  HollowShell2 Konturversatz - Overhang 
    };*/

