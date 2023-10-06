#include "mesh.h"

Mesh::Mesh( std::vector<Vertex> vertices, std::vector<unsigned int> indices,
            std::vector<Texture> textures ) {
    this->m_Vertices = vertices;
    this->m_Indices = indices;
    this->m_Textures = textures;
}

void Mesh::Draw( Shader& shader ) {}

void Mesh::SetupMesh() {
    GLCall( glGenVertexArrays( 1, &VAO ) );
    GLCall( glGenBuffers( 1, &VBO ) );
    GLCall( glGenBuffers( 1, &EBO ) );

    GLCall( glBindVertexArray( VAO ) );
    GLCall( glBindBuffer( GL_ARRAY_BUFFER, VBO ) );

    GLCall( glBufferData( GL_ARRAY_BUFFER, m_Vertices.size() * sizeof( Vertex ),
                          &m_Vertices[0], GL_STATIC_DRAW ) );

    GLCall( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO ) );
    GLCall( glBufferData( GL_ELEMENT_ARRAY_BUFFER,
                          m_Indices.size() * sizeof( unsigned int ),
                          &m_Indices[0], GL_STATIC_DRAW ) );

    GLCall( glEnableVertexAttribArray( 0 ) );
    GLCall( glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ),
                                   (void*)0 ) );
    GLCall( glEnableVertexAttribArray( 1 ) );
    GLCall( glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ),
                                   (void*)offsetof( Vertex, Normal ) ) );
    GLCall( glEnableVertexAttribArray( 2 ) );
    GLCall( glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, sizeof( Vertex ),
                                   (void*)offsetof( Vertex, TexCoords ) ) );
}
