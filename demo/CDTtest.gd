extends Node2D

var verts: PackedVector2Array
var tris: PackedInt32Array
var edges: PackedInt32Array = []

var cdt: ConstrainedTriangulation = ConstrainedTriangulation.new()

var hovered_tri_index: int = -1  # index of the triangle the mouse is over
@onready var highlight_tri = $"../highlightTriangle"

var tris_to_flip: Vector2i = Vector2i(-1,-1)
var tri_neighbors: Vector3i

func _ready():
	
	cdt.init(true, true, 0.1) 
	
	var edge_count: int = 0

	for c in get_children():  # automatically add any child nodes to be triangulated
		if c is Polygon2D:
			verts.append_array(c.polygon)
			for i in c.polygon.size():
				edges.append(i + edge_count)
				edges.append((i+1)%(c.polygon.size()) + edge_count)
			edge_count += c.polygon.size()
		if c is Line2D: # add lines as constrained edges
			verts.append_array(c.points)
			for i in c.points.size():
				if i != c.points.size() - 1:  # don't add the last edge between the last point and the first since this is a line
					edges.append(i + edge_count)
					edges.append((i+1) + edge_count)
			edge_count += c.points.size()
		if c is Path2D: # add Path2D as constrained edges
			var curvePoints: PackedVector2Array = c.curve.tessellate(2, 12.0)
			verts.append_array(curvePoints)
			for i in curvePoints.size():
				if i != curvePoints.size() - 1:  # don't add the last edge between the last point and the first since this is a line
					edges.append(i + edge_count)
					edges.append((i+1) + edge_count)
			edge_count += curvePoints.size()
	# insert any extra points, points are not constrained so this must be done after inserting any constrained edges
	for c in get_children():
		if c is Marker2D: # add markers as unconstrained vertexes
				verts.append(c.position)
	
	# always insert all vertices before any constrained edges
	cdt.insert_vertices(verts)
	
	# use one of the two methods below: 
	cdt.insert_edges(edges)
	#cdt.insert_conforming_edges(edges)
	
	# do one of the three below options to generate triangles: 
	#cdt.erase_super_triangle()
	#cdt.erase_outer_triangles()
	cdt.erase_outer_triangles_and_holes()
	
	verts = cdt.get_all_vertices()
	tris = cdt.get_all_triangles()
	

func _draw():
	for tri in tris.size() / 3:
		for i in 3:
			var from = verts[tris[3*tri + i]]
			var to = verts[tris[3*tri + (i+1)%3]]
			draw_line(from, to, Color(0.07, 0.47, 0.85), 1.0, true )
	for v in verts.size():
		var vert = verts[v]
		draw_circle(vert, 10, Color(0,0,0))
		draw_circle(vert, 5, Color(1,1,1))

func _input(event):
	if event is InputEventMouseMotion:
		# highlight the triangle the mouse is currently over:
		var tri = cdt.get_triangle_at_point(event.position)
		if tri != hovered_tri_index:
			hovered_tri_index = tri
			highlight_tri.visible = tri != -1
			if tri != -1:
				var indices = cdt.get_triangle(tri)
				var a = cdt.get_vertex(indices.x)
				var b = cdt.get_vertex(indices.y)
				var c = cdt.get_vertex(indices.z)
				highlight_tri.polygon = [a,b,c]
	if event is InputEventMouseButton:
		# right click triangle to delete it from the triangulation 
		if event.button_index == MOUSE_BUTTON_RIGHT and event.pressed:
			if hovered_tri_index != -1:
				var triToRemove: PackedInt32Array = [hovered_tri_index]
				cdt.remove_triangles(triToRemove)
				verts = cdt.get_all_vertices()
				tris = cdt.get_all_triangles()
				queue_redraw()
		# left click a triangle, then left click a neighboring triangle to flip the shared edge between them.
		if event.button_index == MOUSE_BUTTON_LEFT and event.pressed:
			if hovered_tri_index != -1:
				if tris_to_flip.x == -1: # select the first triangle
					tris_to_flip.x = hovered_tri_index
					tri_neighbors = cdt.get_triangle_neighbors(tris_to_flip.x)
				else: # select the second triangle
					var isNeighbor = false
					for i in [tri_neighbors.x,tri_neighbors.y,tri_neighbors.z]: # test if second tri is a neighbor of the first
						if i == hovered_tri_index:
							isNeighbor = true
					if isNeighbor: # flip edge! 
						tris_to_flip.y = hovered_tri_index
						cdt.flip_edge(tris_to_flip.x, tris_to_flip.y)
						tris_to_flip.x = -1
						tris_to_flip.y = -1
						verts = cdt.get_all_vertices()
						tris = cdt.get_all_triangles()
						queue_redraw()
					else: # was not a neighbor so set as first triangle
						tris_to_flip.x = hovered_tri_index
						tri_neighbors = cdt.get_triangle_neighbors(tris_to_flip.x)















