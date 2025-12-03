#include "ModelLoader.h"
#include <iostream>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

std::shared_ptr<Mesh> ModelLoader::load(const std::string &filepath) {

  tinyobj::ObjReaderConfig reader_config;
  reader_config.mtl_search_path = "";

  tinyobj::ObjReader reader;

  if (!reader.ParseFromFile(filepath, reader_config)) {
    if (!reader.Error().empty()) {
      std::cerr << "TinyObjReader Error: " << reader.Error() << std::endl;
    }
    return nullptr;
  }

  if (!reader.Warning().empty()) {
    std::cout << "TinyObjReader Warning: " << reader.Warning() << std::endl;
  }

  auto &attrib = reader.GetAttrib();
  auto &shapes = reader.GetShapes();
  auto &materials = reader.GetMaterials();

  std::vector<Vertex> vertices;
  std::vector<uint32_t> indices;

  for (const auto &shape : shapes) {
    size_t index_offset = 0;

    for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
      int fv = shape.mesh.num_face_vertices[f]; // Should be 3 (Triangle)

      for (size_t v = 0; v < fv; v++) {
        tinyobj::index_t idx = shape.mesh.indices[index_offset + v];

        Vertex vertex;

        // Position
        vertex.position.x = attrib.vertices[3 * idx.vertex_index + 0];
        vertex.position.y = attrib.vertices[3 * idx.vertex_index + 1];
        vertex.position.z = attrib.vertices[3 * idx.vertex_index + 2];

        // Normals
        if (idx.normal_index >= 0) {
          vertex.normal.x = attrib.normals[3 * idx.normal_index + 0];
          vertex.normal.y = attrib.normals[3 * idx.normal_index + 1];
          vertex.normal.z = attrib.normals[3 * idx.normal_index + 2];
        } else {
          // Fallback if OBJ has no normals
          // vertex.normal = {0.0f, 1.0f, 0.0f};
        }

        // Color
        vertex.color = {1.0f, 1.0f, 1.0f, 1.0f};

        // TexCoords
        if (idx.texcoord_index >= 0) {
          vertex.tex_coord.x = attrib.texcoords[2 * idx.texcoord_index + 0];
          vertex.tex_coord.y =
              1.0f - attrib.texcoords[2 * idx.texcoord_index + 1]; // Flip Y
        } else {
          vertex.tex_coord = {0.0f, 0.0f};
        }

        vertices.push_back(vertex);
        indices.push_back((uint32_t)indices.size());
      }
      index_offset += fv;
    }
  }

  std::cout << "Loaded Model: " << filepath << " (" << vertices.size()
            << " verts)" << std::endl;

  return std::make_shared<Mesh>(vertices, indices);
}
