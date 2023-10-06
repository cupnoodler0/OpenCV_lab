#pragma once

#include "test.h"

#include "../vertex_buffer.h"
#include "../vertex_buffer_layout.h"
#include "../texture.h"

#include <memory>

namespace test {
class Test3D : public Test {
   public:
    Test3D();
    ~Test3D();

    void OnUpdate( float deltaTime ) override;
    void OnRender() override;
    void OnImGuiRender() override;

   private:
    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<VertexBuffer> m_VertexBuffer;
    std::unique_ptr<IndexBuffer> m_IndexBuffer;
    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<Texture> m_Texture;

    glm::mat4 m_Proj, m_View;
    glm::vec3 m_Translation;
    glm::vec3 m_Rotation;
};
}  // namespace test