#include "godotCDT.h"
#include <godot_cpp/core/class_db.hpp>

using Point = CDT::V2d<float>;
using Edge = CDT::Edge;

using namespace godot;

void ConstrainedTriangulation::_bind_methods() {
    //ClassDB::bind_method(D_METHOD("getTestVar"), &ConstrainedTriangulation::getTestVar);
    ClassDB::bind_method(D_METHOD("insert_vertices", "vertices"), &ConstrainedTriangulation::insert_vertices);
    ClassDB::bind_method(D_METHOD("insert_edges", "edges"), &ConstrainedTriangulation::insert_edges);
    ClassDB::bind_method(D_METHOD("erase_super_triangle"), &ConstrainedTriangulation::erase_super_triangle);
    ClassDB::bind_method(D_METHOD("erase_outer_triangles"), &ConstrainedTriangulation::erase_outer_triangles);
    ClassDB::bind_method(D_METHOD("erase_outer_triangles_and_holes"), &ConstrainedTriangulation::erase_outer_triangles_and_holes);
    ClassDB::bind_method(D_METHOD("get_all_vertices"), &ConstrainedTriangulation::get_all_vertices);
    ClassDB::bind_method(D_METHOD("get_all_triangles"), &ConstrainedTriangulation::get_all_triangles);
}

ConstrainedTriangulation::ConstrainedTriangulation() {
    // Initialize any variables here.
}

ConstrainedTriangulation::~ConstrainedTriangulation() {
    // Add your cleanup here.
}

void ConstrainedTriangulation::insert_vertices(PackedVector2Array vertices){
    std::vector<Point> vertsToInsert;
    vertsToInsert.resize(vertices.size());

    for(int i = 0; i < vertices.size(); ++i){
        vertsToInsert[i].x = vertices[i].x;
        vertsToInsert[i].y = vertices[i].y;
    }
	triangulation.insertVertices(vertsToInsert);
	vert_count += vertsToInsert.size();
}

void ConstrainedTriangulation::insert_edges(PackedInt32Array edges){
	int edgeCount = edges.size()/2;
	std::vector<Edge> edgesToInsert;
	edgesToInsert.reserve(edgeCount); ///TODO: why not resize like insert verts?  

	for(int i = 0; i < edgeCount; ++i){
		edgesToInsert.push_back(Edge(edges[2*i], edges[2*i+1]));
	}
	triangulation.insertEdges(edgesToInsert);
}

void ConstrainedTriangulation::erase_super_triangle(){
	triangulation.eraseSuperTriangle();
}
void ConstrainedTriangulation::erase_outer_triangles(){
	triangulation.eraseOuterTriangles();
}
void ConstrainedTriangulation::erase_outer_triangles_and_holes(){
	triangulation.eraseOuterTrianglesAndHoles();
}

PackedVector2Array ConstrainedTriangulation::get_all_vertices(){
    
    PackedVector2Array allVerts;
    allVerts.resize(triangulation.vertices.size());

    for(int i = 0; i < triangulation.vertices.size(); ++i){
        allVerts[i].x = triangulation.vertices[i].x;
        allVerts[i].y = triangulation.vertices[i].y;
    }

    return allVerts;
	// PoolVector2Array ret = s2g<Point, PoolVector2Array>(triangulation.vertices);
	// return ret;
}

PackedInt32Array ConstrainedTriangulation::get_all_triangles(){
    PackedInt32Array allTris; 
    int trisCount = triangulation.triangles.size();
    allTris.resize(trisCount*3);
    for(int i = 0; i < trisCount; ++i){
        allTris[3*i] = triangulation.triangles[i].vertices[0];
        allTris[3*i+1] = triangulation.triangles[i].vertices[1];
		allTris[3*i+2] = triangulation.triangles[i].vertices[2];
    }
    return allTris;

}

