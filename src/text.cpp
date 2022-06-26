#include "text.h"

#include "shaderprogram.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "resources.h"


Text::Text(std::string text) {
    SetText(text);
}

void Text::Draw(const Camera& camera) const {
    spText->use();

    glUniform4fv(spText->u("textColor"), 1, glm::value_ptr(color));
    glUniform2fv(spText->u("offset"), 1, glm::value_ptr(origin * camera.GetScreenSize()));

    glActiveTexture(GL_TEXTURE0);

    // render glyph texture over quads
    glBindTexture(GL_TEXTURE_2D, Resources::ft_OpenSans.texture);

    glUniformMatrix4fv(spText->u("P"), 1, false, glm::value_ptr(camera.GetOrtho()));

    glEnableVertexAttribArray(spText->a("vertex"));
    glVertexAttribPointer(spText->a("vertex"), 4, GL_FLOAT, false, 0, vertices.data());

    // render quads
    glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 4);

    glDisableVertexAttribArray(spText->a("vertex"));

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Text::SetOrigin(glm::vec2 pos) {
    origin = pos;
    updateMesh();
}

void Text::SetText(std::string text) {
    this->text = text;
    updateMesh();
}

void Text::SetScale(float scale) {
    this->scale = scale;
    updateMesh();
}

void Text::SetColor(glm::vec4 color) {
    this->color = color;
}

// Adapted from https://learnopengl.com/In-Practice/Text-Rendering
void Text::updateMesh() {
    float x = 0;
    float y = 0;

    vertices.clear();

    std::string::const_iterator it;
    for (it = text.begin(); it != text.end(); it++) {
        if (*it == '\n') {
            y -= Resources::ft_OpenSans.glyphHeight * scale;
            x = 0;
        }

        Resources::Character ch = Resources::ft_OpenSans.characters[*it];

        float xpos = x + ch.bearing.x * scale;
        float ypos = y - (ch.size.y - ch.bearing.y) * scale;

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        vertices.insert(vertices.end(), {
            xpos,     ypos + h,   ch.texCoords.x, ch.texCoords.z,            
            xpos,     ypos,       ch.texCoords.x, ch.texCoords.w,
            xpos + w, ypos,       ch.texCoords.y, ch.texCoords.w,

            xpos,     ypos + h,   ch.texCoords.x, ch.texCoords.z,
            xpos + w, ypos,       ch.texCoords.y, ch.texCoords.w,
            xpos + w, ypos + h,   ch.texCoords.y, ch.texCoords.z
        });

        x += (ch.advance >> 6) * scale;
    }
}

float Text::GetOrder(const Camera& camera) const {
    return zIndex;
}

