/* Copyright (c) 2022-2025, NVIDIA CORPORATION. All rights reserved.
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

#pragma once

#include "common/vk_common.h"
#include <memory>
#include <vulkan/vulkan.hpp>

namespace vkb
{
namespace rendering
{
class HPPRenderFrame;
}

namespace core
{
template <vkb::BindingType bindingType>
class CommandBuffer;
using CommandBufferCpp = CommandBuffer<vkb::BindingType::Cpp>;

class HPPDevice;

class HPPCommandPool
{
  public:
	HPPCommandPool(HPPDevice                      &device,
	               uint32_t                        queue_family_index,
	               vkb::rendering::HPPRenderFrame *render_frame = nullptr,
	               size_t                          thread_index = 0,
	               vkb::CommandBufferResetMode     reset_mode   = vkb::CommandBufferResetMode::ResetPool);
	HPPCommandPool(const HPPCommandPool &) = delete;
	HPPCommandPool(HPPCommandPool &&other);
	~HPPCommandPool();

	HPPCommandPool &operator=(const HPPCommandPool &) = delete;
	HPPCommandPool &operator=(HPPCommandPool &&)      = delete;

	HPPDevice                      &get_device();
	vk::CommandPool                 get_handle() const;
	uint32_t                        get_queue_family_index() const;
	vkb::rendering::HPPRenderFrame *get_render_frame();
	vkb::CommandBufferResetMode     get_reset_mode() const;
	size_t                          get_thread_index() const;
	vkb::core::CommandBufferCpp    &request_command_buffer(vk::CommandBufferLevel level = vk::CommandBufferLevel::ePrimary);
	void                            reset_pool();

  private:
	void reset_command_buffers();

  private:
	HPPDevice                                                &device;
	vk::CommandPool                                           handle             = nullptr;
	vkb::rendering::HPPRenderFrame                           *render_frame       = nullptr;
	size_t                                                    thread_index       = 0;
	uint32_t                                                  queue_family_index = 0;
	std::vector<std::unique_ptr<vkb::core::CommandBufferCpp>> primary_command_buffers;
	uint32_t                                                  active_primary_command_buffer_count = 0;
	std::vector<std::unique_ptr<vkb::core::CommandBufferCpp>> secondary_command_buffers;
	uint32_t                                                  active_secondary_command_buffer_count = 0;
	vkb::CommandBufferResetMode                               reset_mode                            = vkb::CommandBufferResetMode::ResetPool;
};
}        // namespace core
}        // namespace vkb
