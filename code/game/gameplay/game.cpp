#include "gameplay/game.h"
#include "framework/app.h"
#include "glare/render/renderer.h"
#include "glare/render/shader.h"
#include "glare/core/color.h"
#include "glare/render/vertex.h"
#include "imgui/imgui.h"

static renderer* g_renderer = nullptr;

game::game(app* app)
	:m_app(app)
{
	g_renderer = m_app->m_renderer;
}
game::~game()
{
	delete m_unlit_shader;
}

void game::start()
{
	m_unlit_shader = shader::load_from_xml("data/shader/unlit.shader", m_app->m_renderer);
	g_renderer->m_buffer_vbo->set_buffer_layout(buffer_layout::acquire_layout_of<vertex_pcu>());
	m_unlit_shader->create_dx_vbo_layout(g_renderer->m_buffer_vbo->get_buffer_layout());
}

void game::begin_frame()
{
}

static bool my_tool_active;
static rgba my_color;

void game::update()
{
	ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
			if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
			if (ImGui::MenuItem("Close", "Ctrl+W")) { my_tool_active = false; }
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	// Edit a color (stored as ~4 floats)
	ImGui::ColorEdit4("Color", reinterpret_cast<float*>(&my_color));

	// Plot some values
	const float my_values[] = {0.2f, 0.1f, 1.0f, 0.5f, 0.9f, 2.2f};
	ImGui::PlotLines("Frame Times", my_values, IM_ARRAYSIZE(my_values));

	// Display contents in a scrolling region
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Important Stuff");
	ImGui::BeginChild("Scrolling");
	for (int n = 0; n < 50; n++)
		ImGui::Text("%04d: Some text", n);
	ImGui::EndChild();
	ImGui::End();
}

void game::render() const
{
	static std::vector<vertex_pcu> verts =
	{
		{{0,0,0}, color::RED, {0,0}},
		{{1,0,0}, color::GREEN, {0,0}},
		{{0,1,0}, color::BLUE, {0,0}},
	};
	g_renderer->bind_shader(m_unlit_shader);
	g_renderer->m_buffer_vbo->buffer(verts.data(), verts.size());
	g_renderer->bind_vbo(g_renderer->m_buffer_vbo);
	g_renderer->draw(3, 0);
}

void game::end_frame()
{
}

void game::stop()
{
}
