
#include "Mesh.h"
#include <sstream>
#include <vector>
#include <fstream>
#include <string.h>
#include <algorithm>
#include <iostream>

static glm::vec3 y_axis(0.f, 1.f, 0.f);
static glm::vec3 x_axis(1.f, 0.f, 0.f);
static glm::vec3 z_axis(0.f, 0.f, 1.f);

Mesh::Mesh()
{
	SetZero();
}

Mesh::Mesh(const Mesh& mesh)
{
	SetZero();
	Assign(mesh);
}


Mesh::~Mesh()
{
	Clear();
}

void
Mesh::SetZero()
{
	vertices_ = 0;
	uvs_ = 0;
	normals_ = 0;
	face_sizes_ = 0;
	face_vertex_ids_ = 0;
	face_uv_ids_ = 0;
	face_normal_ids_ = 0;
	triangle_ids_ = 0;
	colors_ = 0;

	num_vertices_ = 0;
	num_uvs_ = 0;
	num_normals_ = 0;
	num_faces_ = 0;

}

void
Mesh::Clear()
{
	if ( vertices_!=0 )       delete[] vertices_;
	if ( uvs_!=0 )            delete[] uvs_;
	if ( normals_!=0 )        delete[] normals_;
	if ( face_sizes_!=0 )     delete[] face_sizes_;
	if ( triangle_ids_ !=0 )  delete[] triangle_ids_;
	if ( colors_ !=0 )        delete[] colors_;
	

	if ( face_vertex_ids_!=0 )
	{
		for ( int i=0; i<num_faces_; i++ )
		{
			delete[] face_vertex_ids_[i];
		}
		delete[] face_vertex_ids_;
	}

	if ( face_uv_ids_!=0 )
	{
		for ( int i=0; i<num_faces_; i++ )
		{
			delete[] face_uv_ids_[i];
		}
		delete[] face_uv_ids_;
	}

	if ( face_normal_ids_!=0 )
	{
		for ( int i=0; i<num_faces_; i++ )
		{
			delete[] face_normal_ids_[i];
		}
		delete[] face_normal_ids_;
	}


	SetZero();
}


void
Mesh::SetVertexUvNormalFaceSizes(int num_vertices, int num_uvs, int num_normals, int num_faces)
{
	SetZero();

	num_vertices_ = num_vertices;
	num_uvs_      = num_uvs;
	num_normals_  = num_normals;
	num_faces_    = num_faces;

	if ( num_faces > 0 )
	{
		face_sizes_ = new int[num_faces_];
		if ( num_vertices_ > 0 ) face_vertex_ids_ = new int*[num_faces_];
		if ( num_uvs_ > 0 )      face_uv_ids_     = new int*[num_faces_];
		if ( num_normals_ > 0 )  face_normal_ids_ = new int*[num_faces_];
	}

	if ( num_vertices_ > 0 ) vertices_ = new float[num_vertices_*3];
	if ( num_uvs_ > 0 )      uvs_      = new float[num_uvs_*2];
	if ( num_normals_ > 0 )  normals_  = new float[num_normals_*3];
	if ( num_vertices_ > 0 ) colors_ = new float[num_vertices_*4];
}


void
Mesh::SetFaceVertexAndNormalIds(int face_id, ...)
{
	va_list vl;

	va_start(vl,face_id);

	for ( int i=0; i<face_sizes_[face_id]; i++)
	{
		int v_id = va_arg(vl, int);
		int n_id = va_arg(vl, int);
		face_vertex_ids_[face_id][i] = v_id;
		face_normal_ids_[face_id][i] = n_id;
	}

	va_end(vl);

}






/// Transform
void
Mesh::TranslateVertices(glm::vec3 t)
{
	for ( int i=0; i<num_vertices_; i++ )
	{
		vertex(i, vertex(i)+t);
	}
}

void
Mesh::RotateVertices(glm::quat q, glm::vec3 center)
{
	glm::vec3 v;
	for ( int i=0; i<num_vertices_; i++ )
	{
		v = rotate(q, vertex(i)-center) + center;
		vertex(i, v);
	}

	for ( int i=0; i<num_normals_; i++ )
	{
		v = rotate(q, normal(i));
		normal(i, v);
	}
}

void
Mesh::RotateVertices(float angle, glm::vec3 axis)
{

	glm::quat q = glm::angleAxis(angle, axis);
	RotateVertices(q);
}

void
Mesh::ScaleUniformlyVertices(float s, glm::vec3 center)
{
	glm::vec3 v;
	for ( int i=0; i<num_vertices_; i++ )
	{
		v = s*(vertex(i)-center) + center;
		vertex(i, v);
	}
}

void
Mesh::ScaleVertices(glm::vec3 s, glm::vec3 center)
{
	glm::vec3 v;
	for ( int i=0; i<num_vertices_; i++ )
	{
		v = (vertex(i)-center);
		v[0] *= s[0];
		v[1] *= s[1];
		v[2] *= s[2];
		v += center;
		vertex(i, v);
	}

	for ( int i=0; i<num_normals_; i++ )
	{
		v = normal(i);
		v[0] *= s[0];
		v[1] *= s[1];
		v[2] *= s[2];
		v = glm::normalize(v);
		normal(i, v);
	}
}

void
Mesh::TransformVertices(const glm::mat4 &m)
{
	glm::vec3 v;
	for ( int i=0; i<num_vertices_; i++ )
	{
		v = glm::vec3( m*glm::vec4(vertex(i), 1.f) );
		vertex(i, v);
	}

	for ( int i=0; i<num_normals_; i++ )
	{
		v = glm::vec3( m*glm::vec4(normal(i), 0.f) );
		v = glm::normalize(v);
		normal(i, v);
	}
}







/// Create Premitives
void
Mesh::CreateQuad(float width, float depth)
{
	Clear();

	num_vertices_ = 4;
	num_normals_ = 1;
	num_uvs_ = 0;
	num_faces_ = 1;

	/// Allocation
	SetVertexUvNormalFaceSizes(num_vertices_, num_uvs_, num_normals_, num_faces_);

	for ( int i=0; i<num_faces_; i++ )
	{
		face_sizes_[i] = 4;
		face_vertex_ids_[i] = new int[ face_sizes_[i] ];
		face_normal_ids_[i] = new int[ face_sizes_[i] ];
	}

	/// Set Data
	float half_w = 0.5f * width;
	float half_d = 0.5f * depth;

	vertex(0, glm::vec3(-half_w, 0, +half_d));
	vertex(1, glm::vec3(+half_w, 0, +half_d));
	vertex(2, glm::vec3(+half_w, 0, -half_d));
	vertex(3, glm::vec3(-half_w, 0, -half_d));

	normal(0, y_axis);

	SetFaceVertexAndNormalIds(0, 0, 0
								, 1, 0 
								, 2, 0 
								, 3, 0);
	
}

void
Mesh::CreateBox(float width, float height, float depth)
{
	Clear();

	num_vertices_ = 24;
	num_normals_ = 6;
	num_uvs_ = 0;
	num_faces_ = 6;

	/// Allocation
	SetVertexUvNormalFaceSizes(num_vertices_, num_uvs_, num_normals_, num_faces_);

	for ( int i=0; i<num_faces_; i++ )
	{
		face_sizes_[i] = 4;
		face_vertex_ids_[i] = new int[ face_sizes_[i] ];
		face_normal_ids_[i] = new int[ face_sizes_[i] ];
	}

	/// Set Data
	float half_w = 0.5f * width;
	float half_h = 0.5f * height;
	float half_d = 0.5f * depth;

	glm::vec3 corners[8];
	corners[0] = glm::vec3(-half_w,  half_h, +half_d);
	corners[1] = glm::vec3(+half_w,  half_h, +half_d);
	corners[2] = glm::vec3(+half_w,  half_h, -half_d);
	corners[3] = glm::vec3(-half_w,  half_h, -half_d);

	corners[4] = glm::vec3(-half_w, -half_h, +half_d);
	corners[5] = glm::vec3(+half_w, -half_h, +half_d);
	corners[6] = glm::vec3(+half_w, -half_h, -half_d);
	corners[7] = glm::vec3(-half_w, -half_h, -half_d);

	vertex( 0, corners[ 0] );
	vertex( 1, corners[ 1] );
	vertex( 2, corners[ 2] );
	vertex( 3, corners[ 3] );

	vertex( 4, corners[ 7] );
	vertex( 5, corners[ 6] );
	vertex( 6, corners[ 5] );
	vertex( 7, corners[ 4] );

	vertex( 8, corners[ 2] );
	vertex( 9, corners[ 1] );
	vertex(10, corners[ 5] );
	vertex(11, corners[ 6] );

	vertex(12, corners[ 0] );
	vertex(13, corners[ 3] );
	vertex(14, corners[ 7] );
	vertex(15, corners[ 4] );

	vertex(16, corners[ 0] );
	vertex(17, corners[ 4] );
	vertex(18, corners[ 5] );
	vertex(19, corners[ 1] );

	vertex(20, corners[ 2] );
	vertex(21, corners[ 6] );
	vertex(22, corners[ 7] );
	vertex(23, corners[ 3] );

	normal(0,  x_axis);
	normal(1, -x_axis);
	normal(2,  y_axis);
	normal(3, -y_axis);
	normal(4,  z_axis);
	normal(5, -z_axis);

	SetFaceVertexAndNormalIds(0, 0, 2
								, 1, 2 
								, 2, 2 
								, 3, 2);
	
	SetFaceVertexAndNormalIds(1, 4, 3
								, 5, 3
								, 6, 3 
								, 7, 3);

	SetFaceVertexAndNormalIds(2, 8, 0
								, 9, 0
								, 10, 0 
								, 11, 0);

	SetFaceVertexAndNormalIds(3, 12, 1
								, 13, 1
								, 14, 1 
								, 15, 1);

	SetFaceVertexAndNormalIds(4, 16, 4
								, 17, 4
								, 18, 4 
								, 19, 4);
	
	SetFaceVertexAndNormalIds(5, 20, 5
								, 21, 5
								, 22, 5 
								, 23, 5);

}


void
Mesh::CreateCylinder(glm::vec3 bottom_center, glm::vec3 top_center, float radius, int slices, int stacks)
{

	glm::vec3 v = top_center-bottom_center;
	float h = glm::length(v);
	glm::vec3 n = normalize(v);
	
	glm::vec3 rotation_axis;
	float rotation_angle;

	if ( glm::length(n-y_axis) <= 0.0001 )
	{
		rotation_axis = glm::vec3(0, 1, 0);
		rotation_angle = 0;
	}
	else
	{
		rotation_axis = glm::cross(y_axis, n);
		rotation_angle = atan2( glm::length(rotation_axis), glm::dot(y_axis,n) );
		rotation_axis = glm::normalize(rotation_axis);
	}
	
	CreateCylinder(h, radius, slices, stacks);
	TranslateVertices(glm::vec3(0, 0.5*h, 0));
	RotateVertices(glm::angleAxis(rotation_angle, rotation_axis));
	TranslateVertices(bottom_center);
}

void
Mesh::CreateCylinder(float height, float radius, int slices, int stacks)
{
	Clear();

	glm::vec3 top    = glm::vec3(0, height/2, 0);
	glm::vec3 bottom = -1.f * top;

	std::vector< glm::vec3> round_points;
	for ( int i=0; i<slices; i++ )
	{
		glm::vec3 tmp_p = glm::rotate( glm::angleAxis(i*(2*glm::pi<float>())/slices, glm::vec3(0, 1, 0))
								, glm::vec3(0, 0, radius) );
		round_points.push_back( tmp_p );
	}

	SetVertexUvNormalFaceSizes( 2 + 2*slices + (stacks+1)*slices,  0, round_points.size()+2, (stacks+2)*slices );

	
	// set vertices
	{
		int v_count = 0;
		// top center, index 0;
		vertex(v_count, top);			
		v_count++;
		// bottom center, index 1;
		vertex(v_count, bottom);		
		v_count++;

		// top and bottom circles.
		for ( int i=0; i<2; i++ )
		{
			glm::vec3 y;
			if ( i==0 ) y = top;
			else y = bottom;

			for ( unsigned int j=0; j<round_points.size(); j++ )
			{
				vertex(v_count, y + round_points[j]);
				v_count++;
			}
		}

		// Side vertices from top to bottom. From vertex index 2.
		for ( int i=0; i<=stacks; i++ )
		{
			glm::vec3 y = top - glm::vec3(0, (height/stacks)*i, 0);
			if ( i==stacks ) y = bottom;

			for ( unsigned int j=0; j<round_points.size(); j++ )
			{
				vertex(v_count, y + round_points[j]);
				v_count++;
			}
		}

		//gtest		EXPECT_EQ( num_vertices_, v_count );
	}

	// set normals
	{
		int n_count = 0;
		// top, index 0
		normal(n_count, y_axis);			
		n_count++;

		// bottom, index 1
		normal(n_count, -1.f*y_axis);		
		n_count++;

		// Side normals, from index 2 to 2+slices-1.
		for ( int j=0; j<slices; j++ )
		{
			//gtest EXPECT_LE( n_count, num_normals_ );
			normal(n_count, glm::normalize(round_points[j]));
			n_count++;
		}
	}

	// set faces
	{
		// Allocate for face indices.
		for ( int i=0; i<num_faces_; i++ )
		{
			// top and bottom circle-faces.
			if ( i<2*slices )
			{
				face_sizes_[i] = 3;
			}

			// Side Walls.
			else
			{
				face_sizes_[i] = 4;
			}

			face_vertex_ids_[i] = new int[face_sizes_[i]];
			face_normal_ids_[i] = new int[face_sizes_[i]];
		}

		int f_count = 0;
		// triangles for top circular face.
		for ( int i=0; i<slices; i++ )
		{
			if ( i==slices-1 )
			{
				SetFaceVertexAndNormalIds(f_count, 0, 0, 
									i+2, 0, 
									2, 0);
				f_count++;
			}
			else
			{
				SetFaceVertexAndNormalIds(f_count, 0, 0, 
									i+2, 0, 
									i+3, 0);
				f_count++;
			}
		}

		// triangles for bottom circular face.
		for ( int i=0; i<slices; i++ )
		{
			if ( i==slices-1 )
			{
				SetFaceVertexAndNormalIds(f_count, 1, 1, 
									2+2*slices-i-1, 1, 
									2+2*slices-1, 1);
				f_count++;
			}
			else
			{
				SetFaceVertexAndNormalIds(f_count, 1, 1, 
									2+2*slices-i-1, 1, 
									2+2*slices-i-2, 1);
				f_count++;
			}
		}


		// Side wall quads.
		for ( int i=0; i<stacks; i++ )
		{
			for ( int j=0; j<slices; j++ )
			{
				if ( j==slices-1 )
				{
					int left_top_vertex_id     = 2+2*slices+(slices*i)+j;
					int left_botoom_vertex_id  = 2+2*slices+(slices*(i+1))+j;
					int right_botoom_vertex_id = 2+2*slices+(slices*(i+1));
					int right_top_vertex_id    = 2+2*slices+(slices*i);
					int left_normal_id  = 2+j;
					int right_normal_id = 2;
					SetFaceVertexAndNormalIds(f_count, 
											left_top_vertex_id,     left_normal_id, 
											left_botoom_vertex_id,  left_normal_id, 
											right_botoom_vertex_id, right_normal_id, 
											right_top_vertex_id,    right_normal_id);
					//gtest EXPECT_LE( f_count, num_faces_ );
					f_count++;
				}
				else
				{
					int left_top_vertex_id     = 2+2*slices+(slices*i)+j;
					int left_botoom_vertex_id  = 2+2*slices+(slices*(i+1))+j;
					int right_botoom_vertex_id = 2+2*slices+(slices*(i+1))+j+1;
					int right_top_vertex_id    = 2+2*slices+(slices*i)+j+1;
					int left_normal_id  = 2+j;
					int right_normal_id = 2+j+1;
					SetFaceVertexAndNormalIds(f_count, 
											left_top_vertex_id,     left_normal_id, 
											left_botoom_vertex_id,  left_normal_id, 
											right_botoom_vertex_id, right_normal_id, 
											right_top_vertex_id,    right_normal_id);
					//gtest EXPECT_LE( f_count, num_faces_ );
					f_count++;
				}
			}
		}
	}
}

void
Mesh::CreateOpenedCylinder(float height, float radius, int slices, int stacks)
{
	Clear();

	glm::vec3 top    = glm::vec3(0, height/2, 0);
	glm::vec3 bottom = -1.f * top;

	std::vector< glm::vec3> round_points;
	for ( int i=0; i<slices; i++ )
	{
		glm::vec3 tmp_p = glm::rotate( glm::angleAxis(i*(2*glm::pi<float>())/slices, glm::vec3(0, 1, 0))
								, glm::vec3(0, 0, radius) );
		round_points.push_back( tmp_p );
	}

	SetVertexUvNormalFaceSizes( (stacks+1)*slices,  0, round_points.size(), stacks*slices );

	
	// set vertices
	{
		int v_count = 0;

		// Side vertices from top to bottom. From vertex index 2.
		for ( int i=0; i<=stacks; i++ )
		{
			glm::vec3 y = top - glm::vec3(0, (height/stacks)*i, 0);
			if ( i==stacks ) y = bottom;

			for ( unsigned int j=0; j<round_points.size(); j++ )
			{
				vertex(v_count, y + round_points[j]);
				v_count++;
			}
		}

		//gtest EXPECT_EQ( num_vertices_, v_count );
	}

	// set normals
	{
		int n_count = 0;

		// Side normals, from index 2 to 2+slices-1.
		for ( int j=0; j<slices; j++ )
		{
			//gtest EXPECT_LE( n_count, num_normals_ );
			normal(n_count, normalize(round_points[j]));
			n_count++;
		}
	}

	// set faces
	{
		// Allocate for face indices.
		for ( int i=0; i<num_faces_; i++ )
		{
			// Side Walls.
			face_sizes_[i] = 4;

			face_vertex_ids_[i] = new int[face_sizes_[i]];
			face_normal_ids_[i] = new int[face_sizes_[i]];
		}

		int f_count = 0;

		// Side wall quads.
		for ( int i=0; i<stacks; i++ )
		{
			for ( int j=0; j<slices; j++ )
			{
				if ( j==slices-1 )
				{
					int left_top_vertex_id     = (slices*i)+j;
					int left_botoom_vertex_id  = (slices*(i+1))+j;
					int right_botoom_vertex_id = (slices*(i+1));
					int right_top_vertex_id    = (slices*i);
					int left_normal_id  = j;
					int right_normal_id = 0;
					SetFaceVertexAndNormalIds(f_count, 
											left_top_vertex_id,     left_normal_id, 
											left_botoom_vertex_id,  left_normal_id, 
											right_botoom_vertex_id, right_normal_id, 
											right_top_vertex_id,    right_normal_id);
					//gtest EXPECT_LE( f_count, num_faces_ );
					f_count++;
				}
				else
				{
					int left_top_vertex_id     = (slices*i)+j;
					int left_botoom_vertex_id  = (slices*(i+1))+j;
					int right_botoom_vertex_id = (slices*(i+1))+j+1;
					int right_top_vertex_id    = (slices*i)+j+1;
					int left_normal_id  = j;
					int right_normal_id = j+1;
					SetFaceVertexAndNormalIds(f_count, 
											left_top_vertex_id,     left_normal_id, 
											left_botoom_vertex_id,  left_normal_id, 
											right_botoom_vertex_id, right_normal_id, 
											right_top_vertex_id,    right_normal_id);
					//gtest EXPECT_LE( f_count, num_faces_ );
					f_count++;
				}
			}
		}
	}
}

void
Mesh::CreateCapsule(float cylinder_height, float radius)
{
	Clear();

	CreateOpenedCylinder(cylinder_height, radius);
	UpdateNormalVectorsBasedOnFace();

	// Top Hemisphere.
	Mesh top_hemi;
	top_hemi.CreateHemisphere(radius);
	top_hemi.TranslateVertices(glm::vec3(0, cylinder_height/2, 0));
	top_hemi.UpdateNormalVectorsBasedOnVertex();

	// Bottom Hemisphere.
	Mesh bottom_hemi;
	bottom_hemi.CreateHemisphere(radius);
	bottom_hemi.RotateVertices(glm::angleAxis(glm::pi<float>(), glm::vec3(0, 0, 1)));
	bottom_hemi.TranslateVertices(glm::vec3(0, -1*cylinder_height/2, 0));
	bottom_hemi.UpdateNormalVectorsBasedOnVertex();

	// Merge
	Merge(top_hemi);
	Merge(bottom_hemi);
}

void
Mesh::WriteObjFormatStreamV(std::ostream &out) const
{
	// vertices
	for ( int i=0; i<num_vertices_; i++ )
	{
		out << "v " << vertices_[i*3+0] 
		    << " "  << vertices_[i*3+1]
		    << " "  << vertices_[i*3+2]
		    << std::endl;
	}
}

void
Mesh::WriteObjFormatStreamVT(std::ostream &out) const
{
	// uvs
	for ( int i=0; i<num_uvs_; i++ )
	{
		out << "vt " << uvs_[i*2+0] 
		    << " "  << uvs_[i*2+1]
		    << std::endl;
	}
}

void
Mesh::WriteObjFormatStreamVN(std::ostream &out) const
{
	// normals
	for ( int i=0; i<num_normals_; i++ )
	{
		out << "vn " << normals_[i*3+0] 
		    << " "  << normals_[i*3+1]
		    << " "  << normals_[i*3+2]
		    << std::endl;
	}
}

void
Mesh::WriteObjFormatStreamF(std::ostream &out, int v_offset, int vt_offset, int vn_offset) const
{
	
	// faces
	if ( face_vertex_ids_ != 0 && face_uv_ids_ != 0 && face_normal_ids_ != 0 )
	{
		// vertex/uv/normal
		for ( int i=0; i<num_faces_; i++ )
		{
			out << "f ";
			for ( int j=0; j<face_sizes_[i]; j++ )
			{
				out << face_vertex_ids_[i][j]+1+v_offset
					<< "/" << face_uv_ids_[i][j]+1+vt_offset
					<< "/" << face_normal_ids_[i][j]+1+vn_offset
					<< " ";
			}		
			out << std::endl;
		}
	}
	else if ( face_vertex_ids_ != 0 && face_uv_ids_ == 0 && face_normal_ids_ != 0 )
	{
		// vertex//normal
		for ( int i=0; i<num_faces_; i++ )
		{
			out << "f ";
			for ( int j=0; j<face_sizes_[i]; j++ )
			{
				out << face_vertex_ids_[i][j]+1+v_offset
					<< "//" << face_normal_ids_[i][j]+1+vn_offset
					<< " ";
			}		
			out << std::endl;
		}
	}
	else if ( face_vertex_ids_ != 0 && face_uv_ids_ != 0 && face_normal_ids_ == 0 )
	{
		// vertex/uv
		for ( int i=0; i<num_faces_; i++ )
		{
			out << "f ";
			for ( int j=0; j<face_sizes_[i]; j++ )
			{
				out << face_vertex_ids_[i][j]+1+v_offset
					<< "/" << face_uv_ids_[i][j]+1+vt_offset
					<< " ";
			}		
			out << std::endl;
		}
	}
	else if ( face_vertex_ids_ != 0 && face_uv_ids_ == 0 && face_normal_ids_ == 0 )
	{
		// vertex
		for ( int i=0; i<num_faces_; i++ )
		{
			out << "f ";
			for ( int j=0; j<face_sizes_[i]; j++ )
			{
				out << face_vertex_ids_[i][j]+1+v_offset
					<< " ";
			}		
			out << std::endl;
		}
	}
}

void
Mesh::WriteObjFormatStreamG(std::ostream &out, std::string group_name) const
{
	out << "g " << group_name << std::endl;
}

void
Mesh::WriteObjFormatStream(std::ostream &out, std::string group_name, int v_offset, int vt_offset, int vn_offset) const
{
	WriteObjFormatStreamV(out);
	WriteObjFormatStreamVT(out);
	WriteObjFormatStreamVN(out);
	WriteObjFormatStreamG(out, group_name);
	WriteObjFormatStreamF(out, v_offset, vt_offset, vn_offset);
}

void
Mesh::ReadObjFormatStream(std::istream &in)
{
	Clear();

	// Count numburs of each element.
	int num_v = 0;
	int num_vt = 0;
	int num_vn = 0;
	int num_f = 0;

	std::string line;
	while ( !in.eof() )
	{
		std::getline(in, line);
		if ( in.fail() ) break;

		if ( line.compare(0, 2, "v ") == 0 ) num_v++;
		else if ( line.compare(0, 2, "f ") == 0 ) num_f++;
		else if ( line.compare(0, 3, "vt ") == 0 ) num_vt++;
		else if ( line.compare(0, 3, "vn ") == 0 ) num_vn++;
	}

	// Allocate
	SetVertexUvNormalFaceSizes(num_v, num_vt, num_vn, num_f);

	in.clear();
	in.seekg(0);
	std::stringstream sstr_line;
	std::string word;
	int count_v=0;
	int count_vt=0;
	int count_vn=0;
	int count_f=0;
	while ( !in.eof() )
	{
		std::getline(in, line);
		if ( in.fail() ) break;

		sstr_line.clear();
		sstr_line.str(line);

		sstr_line >> word;

		if ( word.compare("v") ==  0 )
		{
			float x, y, z;
			sstr_line >> x >> y >> z;
			vertices_[count_v*3+0] = x;
			vertices_[count_v*3+1] = y;
			vertices_[count_v*3+2] = z;
			count_v++;
		}
		else if ( word.compare("vt") ==  0 )
		{
			float u, v;
			sstr_line >> u >> v;
			uvs_[count_vt*2+0] = u;
			uvs_[count_vt*2+1] = v;
			count_vt++;
		}
		else if ( word.compare("vn") ==  0 )
		{
			float x, y, z;
			sstr_line >> x >> y >> z;
			normals_[count_vn*3+0] = x;
			normals_[count_vn*3+1] = y;
			normals_[count_vn*3+2] = z;
			count_vn++;
		}
		else if ( word.compare("f") ==  0 )
		{
			std::vector<std::string> points;

			while ( !sstr_line.eof() )
			{
				sstr_line >> word;
				if ( word.empty() ) break;
				if ( sstr_line.fail() ) break;
				points.push_back(word);
			}

			int num_points = (int)points.size();
			face_sizes_[count_f] = num_points;

			if ( num_points > 0 )
			{
				if ( num_v > 0 && num_vt > 0 && num_vn > 0 )
				{
					face_vertex_ids_[count_f] = new int[num_points];
					face_uv_ids_[count_f]     = new int[num_points];
					face_normal_ids_[count_f] = new int[num_points];

					for ( int i=0; i<num_points; i++ )
					{
						std::string tmp;
						// vertex
						tmp.assign(points[i].begin(), points[i].begin()+points[i].find("/"));
						face_vertex_ids_[count_f][i] = std::stoi(tmp)-1;
						// uv
						tmp.assign(points[i].begin()+points[i].find("/")+1, points[i].begin()+points[i].rfind("/"));
						face_uv_ids_[count_f][i] = std::stoi(tmp)-1;
						// normal
						tmp.assign(points[i].begin()+points[i].rfind("/")+1, points[i].end());
						face_normal_ids_[count_f][i] = std::stoi(tmp)-1;
					}
				}
				else if ( num_v > 0 && num_vt > 0 && num_vn == 0 )
				{
					face_vertex_ids_[count_f] = new int[num_points];
					face_uv_ids_[count_f]     = new int[num_points];

					for ( int i=0; i<num_points; i++ )
					{
						std::string tmp;
						// vertex
						tmp.assign(points[i].begin(), points[i].begin()+points[i].find("/"));
						face_vertex_ids_[count_f][i] = std::stoi(tmp)-1;
						// uv
						tmp.assign(points[i].begin()+points[i].find("/")+1, points[i].end());
						face_uv_ids_[count_f][i] = std::stoi(tmp)-1;
					}
				}
				else if ( num_v > 0 && num_vt == 0 && num_vn > 0 )
				{
					face_vertex_ids_[count_f] = new int[num_points];
					face_normal_ids_[count_f] = new int[num_points];

					for ( int i=0; i<num_points; i++ )
					{
						std::string tmp;
						// vertex
						tmp.assign(points[i].begin(), points[i].begin()+points[i].find("/"));
						face_vertex_ids_[count_f][i] = std::stoi(tmp)-1;
						// normal
						tmp.assign(points[i].begin()+points[i].rfind("/")+1, points[i].end());
						face_normal_ids_[count_f][i] = std::stoi(tmp)-1;
					}
				}
				else if ( num_v > 0 && num_vt == 0 && num_vn == 0 )
				{
					face_vertex_ids_[count_f] = new int[num_points];

					for ( int i=0; i<num_points; i++ )
					{
						face_vertex_ids_[count_f][i] = std::stoi(points[i])-1;
					}
				}
			}

			count_f++;
		}
	}
}

void
Mesh::ReadObjFormatFile(std::string file)
{
	std::ifstream fin(file.c_str());
	ReadObjFormatStream(fin);
	fin.close();
}


glm::vec3
Mesh::CalculFaceNormal(int face_id) const
{
	if ( face_id >= num_faces_ ) return glm::vec3(0, 0, 0);
	if ( face_vertex_ids_ == 0 ) return glm::vec3(0, 0, 0);
	if ( face_sizes_[face_id] < 2 ) return glm::vec3(0, 0, 0);
	

	glm::vec3 vertex0 = vertex(face_vertex_ids_[face_id][0]);
	glm::vec3 vertex1 = vertex(face_vertex_ids_[face_id][1]);
	glm::vec3 vertex2 = vertex(face_vertex_ids_[face_id][2]);
	
	glm::vec3 side0 = vertex1 - vertex0; 
	glm::vec3 side1 = vertex2 - vertex1;

	return normalize(glm::cross(side0,side1));
}

glm::vec3
Mesh::CalculVertexNormal(int vertex_id) const
{
	if ( vertex_id >= num_vertices_ ) return glm::vec3(0, 0, 0);
	if ( num_faces_ == 0 ) return glm::vec3(0, 0, 0);
	if ( face_vertex_ids_ == 0 ) return glm::vec3(0, 0, 0);


	// "normal_v" will be the average vector of all normal vectors of faces, which are having the vertex of "vertex_id" as a point.
	glm::vec3 normal_v(0, 0, 0);
	int num_of_faces_including_the_vertex = 0;
	for ( int face_id=0; face_id<num_faces_; face_id++ )
	{
		// Check all points of this face.
		for ( int point_id=0; point_id<face_sizes_[face_id]; point_id++ )
		{
			// If this point is same to the "vertex_id",
			// then calculate the normal of the face of "face_id"
			// and add it to "normal_v".
			if ( face_vertex_ids_[face_id][point_id] == vertex_id )
			{
				num_of_faces_including_the_vertex++;
				normal_v += CalculFaceNormal(face_id);
				break;
			}
		}
	}

	if ( num_of_faces_including_the_vertex == 0 ) return glm::vec3(0, 0, 0);
	normal_v = normal_v / (float)num_of_faces_including_the_vertex;

	return glm::normalize(normal_v);
}

void
Mesh::UpdateNormalVectorsBasedOnFace()
{
	if ( num_vertices_ == 0 || num_faces_ == 0 ) return;
	if ( vertices_ == 0 || face_sizes_ == 0 || face_vertex_ids_ == 0 ) return;

	/// Clear Normals, if they exist.
	if ( normals_!=0 )    delete[] normals_;

	/// Allocate Space for Normal Vectors.
	// The number of normal vertors is same to the number of faces.
	num_normals_ = num_faces_;
	normals_ = new float[num_normals_*3];

	/// Set Normals per Face.
	for ( int face_id=0; face_id<num_faces_; face_id++ )
	{
		normal(face_id, CalculFaceNormal(face_id));
	}

	/// Allocate space for face_normal_ids_, if it didn't.
	if ( face_normal_ids_ == 0 )
	{
		face_normal_ids_ = new int*[num_faces_];
		for ( int face_id=0; face_id<num_faces_; face_id++ )
		{
			face_normal_ids_[face_id] = new int[ face_sizes_[face_id] ];
		}
	}
	


	/// Set Normal Ids.
	// The normal id for every point in a face will be assigned to the normal id of the face normal.
	for ( int face_id=0; face_id<num_faces_; face_id++ )
	{
		for ( int point_id=0; point_id<face_sizes_[face_id]; point_id++ )
		{
			face_normal_ids_[face_id][point_id] = face_id;
		}
	}

}

void
Mesh::UpdateNormalVectorsBasedOnVertex()
{
	if ( num_vertices_ == 0 || num_faces_ == 0 ) return;
	if ( vertices_ == 0 || face_sizes_ == 0 || face_vertex_ids_ == 0 ) return;

	/// Clear Normals, if they exist.
	if ( normals_!=0 )    delete[] normals_;

	/// Allocate Space for Normal Vectors.
	// The number of normal vertors is same to the number of vertices.
	num_normals_ = num_vertices_;
	normals_ = new float[num_normals_*3];

	/// Set Normals per Vertex.
	for ( int vertex_id=0; vertex_id<num_vertices_; vertex_id++ )
	{
		normal(vertex_id, CalculVertexNormal(vertex_id));
	}

	/// Allocate space for face_normal_ids_, if it didn't.
	if ( face_normal_ids_ == 0 )
	{
		face_normal_ids_ = new int*[num_faces_];
		for ( int face_id=0; face_id<num_faces_; face_id++ )
		{
			face_normal_ids_[face_id] = new int[ face_sizes_[face_id] ];
		}
	}
	


	/// Set Normal Ids.
	// The normal id for a point in a face will be assigned to the normal id of the corresponding vertex normal.
	for ( int face_id=0; face_id<num_faces_; face_id++ )
	{
		for ( int point_id=0; point_id<face_sizes_[face_id]; point_id++ )
		{
			face_normal_ids_[face_id][point_id] = face_vertex_ids_[face_id][point_id];
		}
	}
}

void
Mesh::SetAllNormals(glm::vec3 unified_normal)
{
	if ( num_vertices_ == 0 || num_faces_ == 0 ) return;
	if ( vertices_ == 0 || face_sizes_ == 0 || face_vertex_ids_ == 0 ) return;

	/// Clear Normals, if they exist.
	if ( normals_!=0 )    delete[] normals_;

	/// Allocate Space for Normal Vectors.
	// The number of normal vertors is 1.
	num_normals_ = 1;
	normals_ = new float[1*3];

	normal(0, unified_normal);

	/// Allocate space for face_normal_ids_, if it didn't.
	if ( face_normal_ids_ == 0 )
	{
		face_normal_ids_ = new int*[num_faces_];
		for ( int face_id=0; face_id<num_faces_; face_id++ )
		{
			face_normal_ids_[face_id] = new int[ face_sizes_[face_id] ];
		}
	}

	/// Set All Normal Ids as 0.
	for ( int face_id=0; face_id<num_faces_; face_id++ )
	{
		for ( int point_id=0; point_id<face_sizes_[face_id]; point_id++ )
		{
			face_normal_ids_[face_id][point_id] = 0;
		}
	}
}

void
Mesh::SetAllUVs(float u, float v)
{
	if ( num_vertices_ == 0 || num_faces_ == 0 ) return;
	if ( vertices_ == 0 || face_sizes_ == 0 || face_vertex_ids_ == 0 ) return;

	/// Clear UV, if they exist.
	if ( num_uvs_!=0 )    delete[] normals_;

	/// Allocate Space for UVs.
	// The number of UV is 1.
	num_uvs_ = 1;
	uvs_ = new float[1*2];

	uv(0, u, v);

	/// Allocate space for face_uv_ids_, if it didn't.
	if ( face_uv_ids_ == 0 )
	{
		face_uv_ids_ = new int*[num_faces_];
		for ( int face_id=0; face_id<num_faces_; face_id++ )
		{
			face_uv_ids_[face_id] = new int[ face_sizes_[face_id] ];
		}
	}

	/// Set All UV Ids as 0.
	for ( int face_id=0; face_id<num_faces_; face_id++ )
	{
		for ( int point_id=0; point_id<face_sizes_[face_id]; point_id++ )
		{
			face_uv_ids_[face_id][point_id] = 0;
		}
	}
}


void
Mesh::SetColor(float r, float g, float b, float alpha)
{
	if ( num_vertices_ == 0 ) return;
	if ( colors_ == 0 ) return;

	for ( int i=0; i<num_vertices_; i++ )
	{
		colors_[i*4+0] = r;
		colors_[i*4+1] = g;
		colors_[i*4+2] = b;
		colors_[i*4+3] = alpha;
	}
}

void
Mesh::Assign(const Mesh &in_mesh)
{
	if ( in_mesh.num_vertices() == 0 || in_mesh.num_faces() == 0 ) return;
	const Mesh &src = in_mesh;

	Clear();
	SetVertexUvNormalFaceSizes(src.num_vertices(), src.num_uvs(), src.num_normals(), src.num_faces());

	if ( num_vertices_ > 0 ) memcpy((void*)vertices_, (void*)src.vertices_, sizeof(float)*num_vertices_*3);
	if ( num_normals_ > 0 )  memcpy((void*)normals_,  (void*)src.normals_,  sizeof(float)*num_normals_*3);
	if ( num_uvs_ > 0 )      memcpy((void*)uvs_,      (void*)src.uvs_,      sizeof(float)*num_uvs_*2);
	if ( num_vertices_ > 0 ) memcpy((void*)colors_,    (void*)src.colors_,  sizeof(float)*num_vertices_*4);

	
	if ( num_faces_ > 0 )    
	{
		memcpy((void*)face_sizes_, (void*)src.face_sizes_, sizeof(int)*num_faces_);

		for ( int i=0; i<num_faces_; i++ )
		{
			if ( num_vertices_ > 0 )
			{
				face_vertex_ids_[i] = new int[face_sizes_[i]];
				for ( int j=0; j<face_sizes_[i]; j++ )
				{
					face_vertex_ids_[i][j] = src.face_vertex_ids_[i][j];
				}
			}

			if ( num_normals_ > 0 )
			{
				face_normal_ids_[i] = new int[face_sizes_[i]];
				for ( int j=0; j<face_sizes_[i]; j++ )
				{
					face_normal_ids_[i][j] = src.face_normal_ids_[i][j];
				}
			}

			if ( num_uvs_ > 0 )
			{
				face_uv_ids_[i] = new int[face_sizes_[i]];
				for ( int j=0; j<face_sizes_[i]; j++ )
				{
					face_uv_ids_[i][j] = src.face_uv_ids_[i][j];
				}
			}
		}

		
	}

	if ( src.num_triangles() > 0 && src.triangle_ids() != 0 ) 
	{
		triangle_ids_ = new unsigned int[src.num_triangles()*3];
		memcpy((void*)triangle_ids_,      (void*)src.triangle_ids_,      sizeof(unsigned int)*num_triangles()*3);
	}
}

void
Mesh::Merge(const Mesh &in_mesh)
{
	// If this mesh is empty, then assign the incoming.
	if ( this->num_vertices() == 0 || this->num_faces() == 0 )
	{
		Assign(in_mesh);
		return;
	}

	// if the input is empty, then return.
	if ( in_mesh.num_vertices() == 0 || in_mesh.num_faces() == 0 ) return;


	// A is source mesh, a copy of 'this'.
	// B is new mesh which will be merged with A.
	Mesh A; A.Assign(*this);
	Mesh B; B.Assign(in_mesh);
	
	/// To make the merge process easier, make both memory states as Same.
	// 1. If B has normals, but A doesn't
	if ( B.num_normals() > 0 && A.num_normals() == 0 )
	{
		A.SetAllNormals(x_axis);
	}
	// 2. If B has uvs, but A doesn't
	if ( B.num_uvs() > 0 && A.num_uvs() == 0 )
	{
		A.SetAllUVs(0, 0);
	}
	// 3. If A has normals, but B doesn't
	if ( A.num_normals() > 0 && B.num_normals() == 0 )
	{
		B.SetAllNormals(x_axis);
	}
	// 4. If A has uvs, but B doesn't
	if ( A.num_uvs() > 0 && B.num_uvs() == 0 )
	{
		B.SetAllUVs(0, 0);
	}

	// Reset 'this'.
	this->Clear();
	this->SetVertexUvNormalFaceSizes(A.num_vertices()+B.num_vertices()
									, A.num_uvs()+B.num_uvs()
									, A.num_normals()+B.num_normals()
									, A.num_faces()+B.num_faces());

	/// Set Space for Vertice.
	// Both 'num_vertices's must be greater than  at this point.
	memcpy((void*) this->vertices_,                     (void*)A.vertices_, sizeof(float)*A.num_vertices()*3);
	memcpy((void*)(this->vertices_+A.num_vertices()*3), (void*)B.vertices_, sizeof(float)*B.num_vertices()*3);
	memcpy((void*) this->colors_,                       (void*)A.colors_,   sizeof(float)*A.num_vertices()*4);
	memcpy((void*)(this->colors_  +A.num_vertices()*4), (void*)B.colors_  , sizeof(float)*B.num_vertices()*4);

	/// Set Space for Normals.
	if ( A.num_normals() > 0 ) memcpy((void*) this->normals_,                    (void*)A.normals_, sizeof(float)*A.num_normals()*3);
	if ( B.num_normals() > 0 ) memcpy((void*)(this->normals_+A.num_normals()*3), (void*)B.normals_, sizeof(float)*B.num_normals()*3);

	/// Set Space for UVs.
	if ( A.num_uvs() > 0 ) memcpy((void*) this->uvs_,                (void*)A.uvs_, sizeof(float)*A.num_uvs()*2);
	if ( B.num_uvs() > 0 ) memcpy((void*)(this->uvs_+A.num_uvs()*2), (void*)B.uvs_, sizeof(float)*B.num_uvs()*2);

	/// Set Space for Face Sizes.
	// Both 'num_faces's must be greater than 0 at this point.
	memcpy((void*) this->face_sizes_,                (void*)A.face_sizes(), sizeof(int)*A.num_faces());
	memcpy((void*)(this->face_sizes_+A.num_faces()), (void*)B.face_sizes(), sizeof(int)*B.num_faces());


	/// Copy face indices of A to 'this'.
	{
		for ( int i=0; i<A.num_faces(); i++ )
		{
			face_vertex_ids_[i] = new int[face_sizes_[i]];
			for ( int j=0; j<A.face_sizes_[i]; j++ )
			{
				this->face_vertex_ids_[i][j] = A.face_vertex_ids_[i][j];
			}

			if ( num_normals_ > 0 )
			{
				face_normal_ids_[i] = new int[face_sizes_[i]];
				for ( int j=0; j<A.face_sizes_[i]; j++ )
				{
					this->face_normal_ids_[i][j] = A.face_normal_ids_[i][j];
				}
			}

			if ( num_uvs_ > 0 )
			{
				face_uv_ids_[i] = new int[face_sizes_[i]];
				for ( int j=0; j<A.face_sizes_[i]; j++ )
				{
					this->face_uv_ids_[i][j] = A.face_uv_ids_[i][j];
				}
			}
		}
	}

	/// Copy face indices of B to 'this'.
	{
		for ( int i=0; i<B.num_faces(); i++ )
		{
			face_vertex_ids_[i+A.num_faces()] = new int[face_sizes_[i+A.num_faces()]];
			for ( int j=0; j<B.face_sizes_[i]; j++ )
			{
				this->face_vertex_ids_[i+A.num_faces()][j] = A.num_vertices() + B.face_vertex_ids_[i][j];
			}

			if ( B.num_normals() > 0 )
			{
				face_normal_ids_[i+A.num_faces()] = new int[face_sizes_[i+A.num_faces()]];
				for ( int j=0; j<B.face_sizes_[i]; j++ )
				{
					this->face_normal_ids_[i+A.num_faces()][j] = A.num_normals() + B.face_normal_ids_[i][j];
				}
			}
			else if ( num_normals_ > 0 )
			{
				face_normal_ids_[i+A.num_faces()] = new int[face_sizes_[i+A.num_faces()]];
				for ( int j=0; j<B.face_sizes_[i]; j++ )
				{
					this->face_normal_ids_[i+A.num_faces()][j] = num_normals_-1;
				}
			}

			if ( B.num_uvs() > 0 )
			{
				face_uv_ids_[i+A.num_faces()] = new int[face_sizes_[i+A.num_faces()]];
				for ( int j=0; j<B.face_sizes_[i]; j++ )
				{
					this->face_uv_ids_[i+A.num_faces()][j] = A.num_uvs() + B.face_uv_ids_[i][j];
				}
			}
			else if ( num_uvs_ > 0 )
			{
				face_uv_ids_[i+A.num_faces()] = new int[face_sizes_[i+A.num_faces()]];
				for ( int j=0; j<B.face_sizes_[i]; j++ )
				{
					this->face_uv_ids_[i+A.num_faces()][j] = num_uvs_-1;
				}
			}
		}


		
	}

	/// Traingle_ids
	int triangle_size = A.num_triangles()+B.num_triangles();
	if ( triangle_size > 0 && ( A.triangle_ids() != 0 || B.triangle_ids() != 0 ) )
	{
		if ( triangle_ids_ != 0 ) delete[] triangle_ids_;
		triangle_ids_ = new unsigned int[triangle_size*3];
		
		if ( A.num_triangles() > 0 && A.triangle_ids() != 0 )
			memcpy((void*)triangle_ids_, (void*)A.triangle_ids(), sizeof(unsigned int)*A.num_triangles()*3);
		

		if ( B.num_triangles() > 0 && B.triangle_ids() != 0 )
		{
			for ( int i=0; i<B.num_triangles()*3; i++ )
			{
				triangle_ids_[A.num_triangles()*3+i] = A.num_vertices()+B.triangle_ids()[i];
			}
		}
	}
	

}


void
Mesh::Triangulate()
{
	if ( num_faces_ ==  0 ) return;

	// Check if it is already triangulated.
	// If not, predict the number of faces after trianglation.
	int t_num_faces = 0;
	for ( int i=0; i<num_faces_; i++ )
	{
		t_num_faces += 1 + (face_sizes_[i] - 3);
	}

	// Already Triangulated..
	if ( t_num_faces == num_faces_ )
	{
		if ( triangle_ids_ != 0 )
			delete[] triangle_ids_;

		triangle_ids_ = new unsigned int[t_num_faces*3];

		for ( int i=0; i<num_faces_; i++ )
		{
			triangle_ids_[i*3+0] = face_vertex_ids_[i][0];
			triangle_ids_[i*3+1] = face_vertex_ids_[i][1];
			triangle_ids_[i*3+2] = face_vertex_ids_[i][2];
		}
		return;
	}

	

	// Calculate Trangulated faces and put them in 'std::vector's.
	std::vector< std::vector<int> > t_face_vertex_ids;
	std::vector< std::vector<int> > t_face_normal_ids;
	std::vector< std::vector<int> > t_face_uv_ids;
	{
		if ( face_vertex_ids_ != 0 )
		{
			for ( int i=0; i<num_faces_; i++ )
			{
				for ( int j=2; j<face_sizes_[i]; j++ )
				{
					t_face_vertex_ids.push_back( std::vector<int>() );
					t_face_vertex_ids.back().push_back( face_vertex_ids_[i][0  ] );
					t_face_vertex_ids.back().push_back( face_vertex_ids_[i][j-1] );
					t_face_vertex_ids.back().push_back( face_vertex_ids_[i][j  ] );
				}
			}
		}

		if ( face_normal_ids_ != 0 )
		{
			for ( int i=0; i<num_faces_; i++ )
			{
				for ( int j=2; j<face_sizes_[i]; j++ )
				{
					t_face_normal_ids.push_back( std::vector<int>() );
					t_face_normal_ids.back().push_back( face_normal_ids_[i][0  ] );
					t_face_normal_ids.back().push_back( face_normal_ids_[i][j-1] );
					t_face_normal_ids.back().push_back( face_normal_ids_[i][j  ] );
				}
			}
		}

		if ( face_uv_ids_ != 0 )
		{
			for ( int i=0; i<num_faces_; i++ )
			{
				for ( int j=2; j<face_sizes_[i]; j++ )
				{
					t_face_uv_ids.push_back( std::vector<int>() );
					t_face_uv_ids.back().push_back( face_uv_ids_[i][0  ] );
					t_face_uv_ids.back().push_back( face_uv_ids_[i][j-1] );
					t_face_uv_ids.back().push_back( face_uv_ids_[i][j  ] );
				}
			}
		}
	}

	// Delete Previous Memories.
	{
		if ( face_vertex_ids_ != 0 )
		{
			for ( int i=0; i<num_faces_; i++ )
			{
				delete[] face_vertex_ids_[i];
			}
			delete[] face_vertex_ids_;
		}

		if ( face_normal_ids_ != 0 )
		{
			for ( int i=0; i<num_faces_; i++ )
			{
				delete[] face_normal_ids_[i];
			}
			delete[] face_normal_ids_;
		}

		if ( face_uv_ids_ != 0 )
		{
			for ( int i=0; i<num_faces_; i++ )
			{
				delete[] face_uv_ids_[i];
			}
			delete[] face_uv_ids_;
		}

		delete[] face_sizes_;

		if ( triangle_ids_ != 0 )
			delete[] triangle_ids_;
	}

	// Allocate and Assign.
	{
		num_faces_ = t_num_faces;
		face_sizes_ = new int[num_faces_];
		triangle_ids_ = new unsigned int[t_num_faces*3];

		for ( int i=0; i<num_faces_; i++ )
		{
			face_sizes_[i] = 3;
		}

		if ( !t_face_vertex_ids.empty() )
		{
			face_vertex_ids_ = new int*[num_faces_];
			for ( int i=0; i<num_faces_; i++ )
			{
				face_vertex_ids_[i] = new int[3];
				face_vertex_ids_[i][0] = t_face_vertex_ids[i][0];
				face_vertex_ids_[i][1] = t_face_vertex_ids[i][1];
				face_vertex_ids_[i][2] = t_face_vertex_ids[i][2];

				triangle_ids_[i*3+0] = t_face_vertex_ids[i][0];
				triangle_ids_[i*3+1] = t_face_vertex_ids[i][1];
				triangle_ids_[i*3+2] = t_face_vertex_ids[i][2];
			}
		}

		if ( !t_face_normal_ids.empty() )
		{
			face_normal_ids_ = new int*[num_faces_];
			for ( int i=0; i<num_faces_; i++ )
			{
				face_normal_ids_[i] = new int[3];
				face_normal_ids_[i][0] = t_face_normal_ids[i][0];
				face_normal_ids_[i][1] = t_face_normal_ids[i][1];
				face_normal_ids_[i][2] = t_face_normal_ids[i][2];
			}
		}

		if ( !t_face_uv_ids.empty() )
		{
			face_uv_ids_ = new int*[num_faces_];
			for ( int i=0; i<num_faces_; i++ )
			{
				face_uv_ids_[i] = new int[3];
				face_uv_ids_[i][0] = t_face_uv_ids[i][0];
				face_uv_ids_[i][1] = t_face_uv_ids[i][1];
				face_uv_ids_[i][2] = t_face_uv_ids[i][2];
			}
		}


	}

	
}


///////////////////////////////
// Operators
Mesh operator*(const glm::mat4 &m, const Mesh& mesh)
{
	Mesh result;
	result = mesh;
	result.TransformVertices(m);

	return result;
}


Mesh operator+(const Mesh& A_mesh, const Mesh& B_mesh)
{
	Mesh result;
	result = A_mesh;
	result.Merge(B_mesh);

	return result;
}







