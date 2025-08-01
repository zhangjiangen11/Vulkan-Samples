/* Copyright (c) 2019-2025, Arm Limited and Contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 the "License";
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "wait_idle.h"

#include "common/vk_common.h"
#include "filesystem/legacy.h"
#include "gltf_loader.h"
#include "gui.h"

#include "rendering/subpasses/forward_subpass.h"
#include "stats/stats.h"

WaitIdle::WaitIdle()
{
	auto &config = get_configuration();

	config.insert<vkb::IntSetting>(0, wait_idle_enabled, 0);
	config.insert<vkb::IntSetting>(1, wait_idle_enabled, 1);
}

bool WaitIdle::prepare(const vkb::ApplicationOptions &options)
{
	if (!VulkanSample::prepare(options))
	{
		return false;
	}

	// Load a scene from the assets folder
	load_scene("scenes/bonza/Bonza.gltf");

	// Attach a move script to the camera component in the scene
	auto &camera_node = vkb::add_free_camera(get_scene(), "main_camera", get_render_context().get_surface_extent());
	camera            = dynamic_cast<vkb::sg::PerspectiveCamera *>(&camera_node.get_component<vkb::sg::Camera>());

	// Example Scene Render Pipeline
	vkb::ShaderSource vert_shader("base.vert.spv");
	vkb::ShaderSource frag_shader("base.frag.spv");
	auto              scene_subpass   = std::make_unique<vkb::ForwardSubpass>(get_render_context(), std::move(vert_shader), std::move(frag_shader), get_scene(), *camera);
	auto              render_pipeline = std::make_unique<vkb::RenderPipeline>();
	render_pipeline->add_subpass(std::move(scene_subpass));
	set_render_pipeline(std::move(render_pipeline));

	// Add a GUI with the stats you want to monitor
	get_stats().request_stats({vkb::StatIndex::frame_times});
	create_gui(*window, &get_stats());

	return true;
}

void WaitIdle::create_render_context()
{
	set_render_context(std::make_unique<CustomRenderContext>(get_device(), get_surface(), *window, wait_idle_enabled));
}

WaitIdle::CustomRenderContext::CustomRenderContext(vkb::core::DeviceC &device, VkSurfaceKHR surface, const vkb::Window &window, int &wait_idle_enabled) :
    RenderContext(device, surface, window), wait_idle_enabled(wait_idle_enabled)
{}

void WaitIdle::CustomRenderContext::wait_frame()
{
	// POI
	//
	// If wait idle is enabled, wait using vkDeviceWaitIdle

	vkb::rendering::RenderFrameC &frame = get_active_frame();

	if (wait_idle_enabled)
	{
		get_device().wait_idle();
	}

	frame.reset();
}

void WaitIdle::draw_gui()
{
	bool     landscape = camera->get_aspect_ratio() > 1.0f;
	uint32_t lines     = landscape ? 1 : 2;

	get_gui().show_options_window(
	    /* body = */ [&]() {
		    ImGui::RadioButton("Wait Idle", &wait_idle_enabled, 1);
		    if (landscape)
		    {
			    ImGui::SameLine();
		    }
		    ImGui::RadioButton("Fences", &wait_idle_enabled, 0);
	    },
	    /* lines = */ lines);
}

std::unique_ptr<vkb::VulkanSampleC> create_wait_idle()
{
	return std::make_unique<WaitIdle>();
}
