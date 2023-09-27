#include "godotCDT.h"
#include <godot_cpp/core/class_db.hpp>

using Point = CDT::V2d<float>;
using Edge = CDT::Edge;

using namespace godot;

void ConstrainedTriangulation::_bind_methods() {

    ClassDB::bind_method(D_METHOD("init", "optomizeInsertOrder", "resolveConstrainedEdges", "minDistToConstraintEdge"), &ConstrainedTriangulation::init);
    ClassDB::bind_method(D_METHOD("insert_vertices", "vertices"), &ConstrainedTriangulation::insert_vertices);
    ClassDB::bind_method(D_METHOD("insert_edges", "edges"), &ConstrainedTriangulation::insert_edges);
    ClassDB::bind_method(D_METHOD("insert_conforming_edges", "edges"), &ConstrainedTriangulation::insert_conforming_edges);
    ClassDB::bind_method(D_METHOD("erase_super_triangle"), &ConstrainedTriangulation::erase_super_triangle);
    ClassDB::bind_method(D_METHOD("erase_outer_triangles"), &ConstrainedTriangulation::erase_outer_triangles);
    ClassDB::bind_method(D_METHOD("erase_outer_triangles_and_holes"), &ConstrainedTriangulation::erase_outer_triangles_and_holes);
    ClassDB::bind_method(D_METHOD("get_vertex_count"), &ConstrainedTriangulation::get_vertex_count);
    ClassDB::bind_method(D_METHOD("get_all_vertices"), &ConstrainedTriangulation::get_all_vertices);
    ClassDB::bind_method(D_METHOD("get_all_triangles"), &ConstrainedTriangulation::get_all_triangles);
    ClassDB::bind_method(D_METHOD("get_all_halfedges"), &ConstrainedTriangulation::get_all_halfedges);
    ClassDB::bind_method(D_METHOD("get_vertex", "vertIndex"), &ConstrainedTriangulation::get_vertex);
    ClassDB::bind_method(D_METHOD("get_triangle", "triIndex"), &ConstrainedTriangulation::get_triangle);
    ClassDB::bind_method(D_METHOD("get_triangle_neighbors", "triIndex"), &ConstrainedTriangulation::get_triangle_neighbors);
    ClassDB::bind_method(D_METHOD("get_triangle_at_point", "point"), &ConstrainedTriangulation::get_triangle_at_point);
    ClassDB::bind_method(D_METHOD("generate_vertex_triangles"), &ConstrainedTriangulation::generate_vertex_triangles);
    ClassDB::bind_method(D_METHOD("get_vertex_triangles", "vertIndex"), &ConstrainedTriangulation::get_vertex_triangles);
    ClassDB::bind_method(D_METHOD("flip_edge", "firstTri", "secondTri"), &ConstrainedTriangulation::flip_edge);
    ClassDB::bind_method(D_METHOD("remove_triangles", "triangles"), &ConstrainedTriangulation::remove_triangles);

}

ConstrainedTriangulation::ConstrainedTriangulation() {
    // Initialize any variables here.
    // _err_print_error("", "", 0, "this is a test warning", true, true);

}

ConstrainedTriangulation::~ConstrainedTriangulation() {
    // Add your cleanup here.
}

void ConstrainedTriangulation::init(bool optomizeInsertOrder, bool resolveConstrainedEdges, float minDistToConstraintEdge){

    CDT::VertexInsertionOrder::Enum order;
    CDT::IntersectingConstraintEdges::Enum resolve;

    order = (optomizeInsertOrder) ? CDT::VertexInsertionOrder::Auto : CDT::VertexInsertionOrder::AsProvided;

    resolve = (resolveConstrainedEdges) ? CDT::IntersectingConstraintEdges::Resolve : CDT::IntersectingConstraintEdges::Ignore;

    triangulation = Triangulation(order, resolve, minDistToConstraintEdge);
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

void ConstrainedTriangulation::insert_conforming_edges(PackedInt32Array edges){
    int edgeCount = edges.size()/2;
	std::vector<Edge> edgesToInsert;
	edgesToInsert.reserve(edgeCount); 

	for(int i = 0; i < edgeCount; ++i){
		edgesToInsert.push_back(Edge(edges[2*i], edges[2*i+1]));
	}
	triangulation.conformToEdges(edgesToInsert);
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

int ConstrainedTriangulation::get_vertex_count(){
    return triangulation.vertices.size();
}

PackedVector2Array ConstrainedTriangulation::get_all_vertices(){
    
    PackedVector2Array allVerts;
    allVerts.resize(triangulation.vertices.size());

    for(int i = 0; i < triangulation.vertices.size(); ++i){
        allVerts[i].x = triangulation.vertices[i].x;
        allVerts[i].y = triangulation.vertices[i].y;
    }

    return allVerts;
}

PackedInt32Array ConstrainedTriangulation::get_all_triangles(){ // TODO should this return a PackedVector3iArray instead?
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

PackedInt32Array ConstrainedTriangulation::get_all_halfedges(){ 
    PackedInt32Array allHalfedges; 
    int trisCount = triangulation.triangles.size();     
    allHalfedges.resize(trisCount*3);
    for(int t = 0; t < trisCount; ++t){
        for(int s = 0; s < 3; ++s){
            std::pair next = triangulation.triangles[t].next(triangulation.triangles[t].vertices[s]);
            if( next.first != -1){ // TriInd 
                allHalfedges[(3*t)+s] = next.second; // VertInd
            } else {
                allHalfedges[(3*t)+s] = -1;
            }
        }
    }
    return allHalfedges;
}

Vector2 ConstrainedTriangulation::get_vertex(int vertIndex){
    Vector2 vert;
    vert.x = triangulation.vertices[vertIndex].x;
    vert.y = triangulation.vertices[vertIndex].y;
    return vert;
}
Vector3i ConstrainedTriangulation::get_triangle(int triIndex){
    Vector3i tri;
    tri.x = triangulation.triangles[triIndex].vertices[0];
    tri.y = triangulation.triangles[triIndex].vertices[1];
    tri.z = triangulation.triangles[triIndex].vertices[2];
    return tri;
}
Vector3i ConstrainedTriangulation::get_triangle_neighbors(int triIndex){
    Vector3i triNeighbors;
    triNeighbors.x = triangulation.triangles[triIndex].neighbors[0];
    triNeighbors.y = triangulation.triangles[triIndex].neighbors[1];
    triNeighbors.z = triangulation.triangles[triIndex].neighbors[2];
    return triNeighbors;
}
int ConstrainedTriangulation::get_triangle_at_point(Vector2 point){
    Point target;
    target.x = point.x;
    target.y = point.y;

    for(int t = 0; t < triangulation.triangles.size(); ++t)
	{
		CDT::VerticesArr3 verts = triangulation.triangles[t].vertices;
		Point a = triangulation.vertices[verts[0]];
		Point b = triangulation.vertices[verts[1]];
		Point c = triangulation.vertices[verts[2]];
		if(CDT::locatePointTriangle<float>(target, a, b, c) != CDT::PtTriLocation::Outside)
		{
			return t;
		}
	}
	return -1;

}
void ConstrainedTriangulation::generate_vertex_triangles(){
    vertTris = CDT::calculateTrianglesByVertex(triangulation.triangles, triangulation.vertices.size());
}

PackedInt32Array ConstrainedTriangulation::get_vertex_triangles(int vertIndex){
    PackedInt32Array tris;
    tris.resize(vertTris[vertIndex].size());
    for(int i = 0; i < vertTris[vertIndex].size(); ++i){
        tris[i] = vertTris[vertIndex][i];
    }
    return tris;
}
void ConstrainedTriangulation::flip_edge(int firstTri, int secondTri){
    triangulation.flipEdge(firstTri, secondTri);
}
void ConstrainedTriangulation::remove_triangles(PackedInt32Array triangles){
    CDT::TriIndUSet trisToRemove;
    for(int i = 0; i < triangles.size(); ++i){
        trisToRemove.insert(triangles[i]);
    }
    triangulation.removeTriangles(trisToRemove);
}

