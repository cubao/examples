#include "fx/gltf.h"
#include <iostream>

// https://github.com/jessey-git/fx-gltf/issues/53

struct vec3
{
    float x;
    float y;
    float z;
};

template <typename T>
uint32_t serialize(std::vector<T> const &from, std::vector<uint8_t> &to,
                   std::size_t offset)
{
    uint32_t bytesToSerialize = sizeof(T) * static_cast<uint32_t>(from.size());

    to.resize(to.size() + bytesToSerialize);
    std::memcpy(&to[offset], &from[0], bytesToSerialize);

    return bytesToSerialize;
}

int main()
{
    // Let's create geometry for a square
    //   4 vec3's defining the vertex positions
    //   4 vec3's defining the vertex normals (all point same way)
    //   Index buffer so the gfx api knows what how to draw the 2 triangles
    std::vector<vec3> positions{vec3{-1, 0, 1}, vec3{1, 0, -1}, vec3{-1, 0, -1},
                                vec3{1, 0, 1}};
    std::vector<uint32_t> indices{0, 1, 2, 0, 3, 1};

    // "Serialize" the above information into a raw byte array
    // Note: this implementation is unoptimized and potentially dangerous.
    // Review for your
    // own projects accordingly...
    fx::gltf::Buffer buffer{};
    uint32_t indexSize = serialize(indices, buffer.data, 0);
    uint32_t positionSize = serialize(positions, buffer.data, indexSize);

    // At this point our raw buffer contains 120 total bytes of information
    // It's arranged as follows: [indices | positions]
    buffer.byteLength = static_cast<uint32_t>(buffer.data.size());
    buffer.SetEmbeddedResource();

    // Build the document
    fx::gltf::Document doc{};
    doc.asset.generator = "Test Generator";
    doc.asset.version = "2.0";

    doc.buffers.push_back(buffer);

    // Build the "views" into the "buffer"
    fx::gltf::BufferView indexBufferView{};
    fx::gltf::BufferView positionBufferView{};
    indexBufferView.buffer = 0;
    indexBufferView.byteLength = indexSize;
    indexBufferView.byteOffset = 0;
    positionBufferView.buffer = 0;
    positionBufferView.byteLength = positionSize;
    positionBufferView.byteOffset = indexSize;

    doc.bufferViews.push_back(indexBufferView);
    doc.bufferViews.push_back(positionBufferView);

    // Now we need "accessors" to know how to interpret each of those "views"
    fx::gltf::Accessor indexAccessor{};
    fx::gltf::Accessor positionAccessor{};
    indexAccessor.bufferView = 0;
    indexAccessor.componentType =
        fx::gltf::Accessor::ComponentType::UnsignedInt;
    indexAccessor.count = static_cast<uint32_t>(indices.size());
    indexAccessor.type = fx::gltf::Accessor::Type::Scalar;
    positionAccessor.bufferView = 1;
    positionAccessor.componentType = fx::gltf::Accessor::ComponentType::Float;
    positionAccessor.count = static_cast<uint32_t>(positions.size());
    positionAccessor.type = fx::gltf::Accessor::Type::Vec3;

    // My viewer requires min/max (shame)
    indexAccessor.min = {0};
    indexAccessor.max = {3};
    positionAccessor.min = {-1, 0, -1};
    positionAccessor.max = {1, 0, 1};

    doc.accessors.push_back(indexAccessor);
    doc.accessors.push_back(positionAccessor);

    /*
    "materials": [
        {
            "pbrMetallicRoughness": {
                "baseColorFactor": [
                    0.803921568627451,
                    0.1450980392156863,
                    0.1450980392156863,
                    1
                ],
                "metallicFactor": 0,
                "roughnessFactor": 0.9
            },
            "emissiveFactor": [
                0.47843137254901963,
                0.09019607843137255,
                0.09019607843137255
            ],
            "extensions": {
                "KHR_materials_unlit": {}
            },
            "doubleSided": true
        }
    ],
    */
    // materials
    // /home/conan/.cmake_install/include/cubao_bundles/fx/gltf.h
    fx::gltf::Material material{};
    material.pbrMetallicRoughness.baseColorFactor = {1.0f, 0.0f, 0.0f, 1.0f};
    material.pbrMetallicRoughness.metallicFactor = 0.0f;
    material.pbrMetallicRoughness.roughnessFactor = 0.9f;
    material.emissiveFactor = {1.0f, 0.0f, 0.0f};
    material.doubleSided = true;
    material.extensionsAndExtras = {
        {"KHR_materials_unlit", nlohmann::json::object()},
    };
    doc.materials.push_back(material);
    doc.extensionsUsed.push_back("KHR_materials_unlit");

    // We need "meshes" to describe how to assemble the darn thing...
    fx::gltf::Mesh squareMesh{};
    fx::gltf::Primitive squarePrimitive{};
    squareMesh.name = "SampleSquare";
    squarePrimitive.indices = 0;                // accessor 0
    squarePrimitive.attributes["POSITION"] = 1; // accessor 1
    squarePrimitive.material = 0;
    squareMesh.primitives.push_back(squarePrimitive);

    doc.meshes.push_back(squareMesh);

    // We need "nodes" to list what "meshes" to use...
    fx::gltf::Node node{};
    node.mesh = 0;

    doc.nodes.push_back(node);

    // We need a "scene" to list what "nodes" to use...
    fx::gltf::Scene scene{};
    scene.nodes.push_back(0);

    doc.scenes.push_back(scene);
    doc.scene = 0;

    // Simple right?
    std::string path = "square.gltf";
    fx::gltf::Save(doc, path, false);
    std::cout << "wrote to " << path << std::endl;
    return 0;
}
