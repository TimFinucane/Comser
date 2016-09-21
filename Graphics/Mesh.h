#pragma once
#ifndef GRAPHICS_MESH_H
#define GRAPHICS_MESH_H

// This structure is implemented as an interface first if the program wants to use it in conjunction with a Physics mesh
struct MeshInterface
{
    // TODO
};

template <typename COORDINATE>
struct Mesh : public MeshInterface
{
    friend class Graphics;
public:
    Mesh();
    ~Mesh();

    void load( COORDINATE* coords, unsigned int count );

private:
    unsigned int _mesh;
};

#endif