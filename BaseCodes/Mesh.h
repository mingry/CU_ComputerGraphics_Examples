
#pragma once

#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include <stdarg.h>
#include <string>

///////////////////////////////////////////
// Class: Mesh

class Mesh
{
public:
	Mesh();
	Mesh(const Mesh &in_mesh);
	~Mesh();
	void Clear();

	// Create Premitives.
	void CreateQuad(float width=1, float depth=1);
	void CreateBox(float width=1, float height=1, float depth=1);
	void CreateCylinder(glm::vec3 bottom_center, glm::vec3 top_center, float radius=1, int slices=32, int stacks=1);
	void CreateCylinder(float height=1, float radius=1, int slices=16, int stacks=1);
	void CreateOpenedCylinder(float height=1, float radius=1, int slices=16, int stacks=1);
	void CreateSphere(float radius=1);
	void CreateHemisphere(float radius=1);
	void CreateCapsule(float cylinder_height=1, float radius=1);
	void CreateHeadModel(float radius=1);

	// Transform.
	void TranslateVertices(glm::vec3 t);
	void RotateVertices(glm::quat q, glm::vec3 center=glm::vec3(0, 0, 0));
	void RotateVertices(float angle, glm::vec3 axis);
	void ScaleUniformlyVertices(float s, glm::vec3 center=glm::vec3(0, 0, 0));
	void ScaleVertices(glm::vec3 s, glm::vec3 center=glm::vec3(0, 0, 0));
	void TransformVertices(const glm::mat4 &m);

	// Assign, Merge
	void Assign(const Mesh& mesh);
	void Merge(const Mesh& mesh);

	// Calculate Normal Vector.
	glm::vec3 CalculFaceNormal(int face_id) const;
	glm::vec3 CalculVertexNormal(int vertex_id) const;
	void UpdateNormalVectorsBasedOnFace();
	void UpdateNormalVectorsBasedOnVertex();

	//
	void Triangulate();

	
	// Get Properties. 
	inline int num_vertices() const { return num_vertices_; }
	inline int num_uvs() const { return num_uvs_; }
	inline int num_normals() const { return num_normals_; }
	inline int num_faces() const { return num_faces_; }
	inline int num_triangles() const { return num_faces_; }
	
	inline float*  vertices() const { return vertices_; }
	inline float*  points() const { return vertices_; }
	inline float*  colors() const { return colors_; }
	inline float*  uvs() const { return uvs_; }
	inline float*  normals() const { return normals_; }
	inline int*     face_sizes() const { return face_sizes_; }
	inline int**    vertices_of_faces() const { return face_vertex_ids_; }
	inline int**    uvs_of_faces() const { return face_uv_ids_; }
	inline int**    normals_of_faces() const { return face_normal_ids_; }

	inline std::pair<float, float> uv(int id) const { return std::make_pair(uvs_[id*2+0], uvs_[id*2+1]); }
	inline glm::vec3 vertex(int id)              const { return glm::vec3(vertices_[id*3+0], vertices_[id*3+1], vertices_[id*3+2]); }
	inline glm::vec3 normal(int id)              const { return glm::vec3(normals_[id*3+0], normals_[id*3+1], normals_[id*3+2]); }

	inline int        face_size(int face_id)          const { return face_sizes_[face_id]; }
	inline const int* face_vertex_ids(int face_id) const { return face_vertex_ids_[face_id]; }
	inline const int* face_uv_ids(int face_id)     const { return face_uv_ids_[face_id]; }
	inline const int* face_normal_ids(int face_id) const { return face_normal_ids_[face_id]; }
	const unsigned int* triangle_ids() const { return triangle_ids_; }
	
	// Set Properties. 
	inline void vertex(int id, const glm::vec3 &v) { vertices_[id*3+0]=v.x; vertices_[id*3+1]=v.y; vertices_[id*3+2]=v.z; }
	inline void normal(int id, const glm::vec3 &n) { normals_[id*3+0]=n.x; normals_[id*3+1]=n.y; normals_[id*3+2]=n.z; }
	inline void uv(int id, float u, float v) { uvs_[id*2+0]=u; uvs_[id*2+1]=v; }
	void SetAllNormals(glm::vec3 unified_normal);
	void SetAllUVs(float u, float v);
	void SetColor(float r, float g, float b, float alpha=1.0f);
	

	// File IO
	void WriteObjFormatStream(std::ostream &out, std::string group_name="", int v_offset=0, int vt_offset=0, int vn_offset=0) const;
	void WriteObjFormatStreamV(std::ostream &out) const;	// Vertices.
	void WriteObjFormatStreamVT(std::ostream &out) const;	// UVs.
	void WriteObjFormatStreamVN(std::ostream &out) const;	// Normals.
	void WriteObjFormatStreamG(std::ostream &out, std::string group_name) const;	// Group name.
	void WriteObjFormatStreamF(std::ostream &out, int v_offset=0, int vt_offset=0, int vn_offset=0) const;		// Faces.
	void ReadObjFormatStream(std::istream &in);
	void ReadObjFormatFile(std::string file);


	// operators
	friend Mesh operator*(const glm::mat4 &m, const Mesh& mesh);
	friend Mesh operator+(const Mesh& A_mesh, const Mesh& B_mesh);
	Mesh& operator=(const Mesh& mesh) { Clear(); Assign(mesh); return *this; };
	Mesh& operator+=(const Mesh& mesh) { Merge(mesh); return *this; };

private:
	void SetZero();

	// Parameter: face_id, 1'st vertex id, 1'st normal id, ..., n'th vertex id, n'th normal id
	//            n is the number of points in the face (face_sizes_[face_id]).
	void SetFaceVertexAndNormalIds(int face_id, ...);

	// Set and Allocate
	void SetVertexUvNormalFaceSizes(int num_vertices, int num_uvs, int num_normals, int num_faces);

protected:
	int num_vertices_;
	int num_uvs_;
	int num_normals_;
	int num_faces_;

	float *vertices_;
	float *uvs_;
	float *normals_;
	int *face_sizes_;
	int **face_vertex_ids_;
	int **face_uv_ids_;
	int **face_normal_ids_;

	float *colors_;

	// trinalge_ids_ is valid only after Triangulate() is called.
	// It contains indices of vertices;
	// Every 3 consecutive indices represent a triangle.
	// The size is always a mulitple of 3.
	unsigned int *triangle_ids_;
};




Mesh operator*(const glm::mat4 &m, const Mesh& mesh);
Mesh operator+(const Mesh& A_mesh, const Mesh& B_mesh);