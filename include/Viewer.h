//
// Created by ziqwang on 2019-10-17.
//
#ifndef FRAME_VIEWER_H
#define FRAME_VIEWER_H

#include <nanogui/opengl.h>
#include <nanogui/screen.h>
#include <nanogui/window.h>
#include <nanogui/layout.h>
#include <nanogui/label.h>
#include <nanogui/checkbox.h>
#include <nanogui/button.h>
#include <nanogui/toolbutton.h>
#include <nanogui/popupbutton.h>
#include <nanogui/combobox.h>
#include <nanogui/progressbar.h>
#include <nanogui/icons.h>
#include <nanogui/messagedialog.h>
#include <nanogui/textbox.h>
#include <nanogui/slider.h>
#include <nanogui/imagepanel.h>
#include <nanogui/imageview.h>
#include <nanogui/vscrollpanel.h>
#include <nanogui/colorwheel.h>
#include <nanogui/colorpicker.h>
#include <nanogui/graph.h>
#include <nanogui/tabwidget.h>
#include <nanogui/texture.h>
#include <nanogui/shader.h>
#include <nanogui/renderpass.h>
#include <enoki/transform.h>
#include <stb_image.h>

using namespace nanogui;

class Viewer : public nanogui::Screen {
public:
    Viewer() : nanogui::Screen(Vector2i(1024, 768), "Viewer")
    {
        inc_ref();

        Window *window = new Window(this, "Mesh Viewer");
        window->set_position(Vector2i(15, 15));
        window->set_layout(new GroupLayout());


        Button *b = new Button(window, "Open", FA_ROCKET);
        b->set_callback([&] {
            std::cout << "File dialog result: " << file_dialog(
                    { {"obj", "Wavefront 3D Object File"}}, false) << std::endl;
        });

        perform_layout();

        init_shader();
    }

    ~Viewer() {

    }

    void init_shader()
    {
        m_render_pass = new RenderPass({ this });

        m_render_pass->set_clear_color(0, Color(0.3f, 0.3f, 0.32f, 1.f));

        m_shader = new Shader(
                m_render_pass,

                /* An identifying name */
                "a_simple_shader",

                R"(using namespace metal;
            struct VertexOut {
                float4 position [[position]];
            };

            vertex VertexOut vertex_main(const device packed_float3 *position,
                                         constant float4x4 &mvp,
                                         uint id [[vertex_id]]) {
                VertexOut vert;
                vert.position = mvp * float4(position[id], 1.f);
                return vert;
            })",

                /* Fragment shader */
                R"(
                using namespace metal;
                fragment float4 fragment_main(const constant float &intensity)
                {
                    return float4(intensity);
                })"
        );

        uint32_t indices[3*2] = {
                0, 1, 2,
                2, 3, 0
        };

        float positions[3*4] = {
                -1.f, -1.f, 0.f,
                1.f, -1.f, 0.f,
                1.f, 1.f, 0.f,
                -1.f, 1.f, 0.f
        };

        m_shader->set_buffer("indices", enoki::EnokiType::UInt32, 1, {3*2, 1, 1}, indices);
        m_shader->set_buffer("position", enoki::EnokiType::Float32, 2, {4, 3, 1}, positions);
        m_shader->set_uniform("intensity", 0.5f);
    }

    virtual bool keyboard_event(int key, int scancode, int action, int modifiers) {
        if (Screen::keyboard_event(key, scancode, action, modifiers))
            return true;
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            set_visible(false);
            return true;
        }
        return false;
    }

    virtual void draw(NVGcontext *ctx) {
        /* Draw the user interface */
        Screen::draw(ctx);
    }

    virtual void draw_contents()
    {
        Matrix4f mvp = enoki::scale<Matrix4f>(Vector3f(
                (float) m_size.y() / (float) m_size.x() * 0.25f, 0.25f, 0.25f)) *
                       enoki::rotate<Matrix4f>(Vector3f(0, 0, 1), (float) glfwGetTime());

        m_shader->set_uniform("mvp", mvp);

        m_render_pass->resize(framebuffer_size());
        m_render_pass->begin();

        m_shader->begin();
        m_shader->draw_array(Shader::PrimitiveType::Triangle, 0, 6, true);
        m_shader->end();

        m_render_pass->end();
    }
private:
    ref<Shader> m_shader;
    ref<RenderPass> m_render_pass;
};

#endif //FRAME_VIEWER_H

