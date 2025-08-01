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

#pragma once

#include "common/helpers.h"
#include "common/vk_common.h"

namespace vkb
{
namespace core
{
template <vkb::BindingType bindingType>
class Device;
using DeviceC = Device<vkb::BindingType::C>;
}        // namespace core

enum ImageFormat
{
	sRGB,
	UNORM
};

struct SwapchainProperties
{
	VkSwapchainKHR                old_swapchain;
	uint32_t                      image_count{3};
	VkExtent2D                    extent{};
	VkSurfaceFormatKHR            surface_format{};
	uint32_t                      array_layers;
	VkImageUsageFlags             image_usage;
	VkSurfaceTransformFlagBitsKHR pre_transform;
	VkCompositeAlphaFlagBitsKHR   composite_alpha;
	VkPresentModeKHR              present_mode;
};

class Swapchain
{
  public:
	/**
	 * @brief Constructor to create a swapchain by changing the extent
	 *        only and preserving the configuration from the old swapchain.
	 */
	Swapchain(Swapchain &old_swapchain, const VkExtent2D &extent);

	/**
	 * @brief Constructor to create a swapchain by changing the image count
	 *        only and preserving the configuration from the old swapchain.
	 */
	Swapchain(Swapchain &old_swapchain, const uint32_t image_count);

	/**
	 * @brief Constructor to create a swapchain by changing the image usage
	 *        only and preserving the configuration from the old swapchain.
	 */
	Swapchain(Swapchain &old_swapchain, const std::set<VkImageUsageFlagBits> &image_usage_flags);

	/**
	 * @brief Constructor to create a swapchain by changing the extent
	 *        and transform only and preserving the configuration from the old swapchain.
	 */
	Swapchain(Swapchain &swapchain, const VkExtent2D &extent, const VkSurfaceTransformFlagBitsKHR transform);

	/**
	 * @brief Constructor to create a swapchain by changing the compression settings
	 *        only and preserving the configuration from the old swapchain.
	 */
	Swapchain(Swapchain &swapchain, const VkImageCompressionFlagsEXT requested_compression, const VkImageCompressionFixedRateFlagsEXT requested_compression_fixed_rate);

	/**
	 * @brief Constructor to create a swapchain.
	 */
	Swapchain(vkb::core::DeviceC                       &device,
	          VkSurfaceKHR                              surface,
	          const VkPresentModeKHR                    present_mode,
	          const std::vector<VkPresentModeKHR>      &present_mode_priority_list       = {VK_PRESENT_MODE_FIFO_KHR,
	                                                                                        VK_PRESENT_MODE_MAILBOX_KHR},
	          const std::vector<VkSurfaceFormatKHR>    &surface_format_priority_list     = {{VK_FORMAT_R8G8B8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR},
	                                                                                        {VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR}},
	          const VkExtent2D                         &extent                           = {},
	          const uint32_t                            image_count                      = 3,
	          const VkSurfaceTransformFlagBitsKHR       transform                        = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
	          const std::set<VkImageUsageFlagBits>     &image_usage_flags                = {VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_IMAGE_USAGE_TRANSFER_SRC_BIT},
	          const VkImageCompressionFlagsEXT          requested_compression            = VK_IMAGE_COMPRESSION_DEFAULT_EXT,
	          const VkImageCompressionFixedRateFlagsEXT requested_compression_fixed_rate = VK_IMAGE_COMPRESSION_FIXED_RATE_NONE_EXT);

	/**
	 * @brief Constructor to create a swapchain from the old swapchain
	 *        by configuring all parameters.
	 */
	Swapchain(Swapchain                                &old_swapchain,
	          vkb::core::DeviceC                       &device,
	          VkSurfaceKHR                              surface,
	          const VkPresentModeKHR                    present_mode,
	          const std::vector<VkPresentModeKHR>      &present_mode_priority_list       = {VK_PRESENT_MODE_FIFO_KHR, VK_PRESENT_MODE_MAILBOX_KHR},
	          const std::vector<VkSurfaceFormatKHR>    &surface_format_priority_list     = {{VK_FORMAT_R8G8B8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR},
	                                                                                        {VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR}},
	          const VkExtent2D                         &extent                           = {},
	          const uint32_t                            image_count                      = 3,
	          const VkSurfaceTransformFlagBitsKHR       transform                        = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
	          const std::set<VkImageUsageFlagBits>     &image_usage_flags                = {VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_IMAGE_USAGE_TRANSFER_SRC_BIT},
	          const VkImageCompressionFlagsEXT          requested_compression            = VK_IMAGE_COMPRESSION_DEFAULT_EXT,
	          const VkImageCompressionFixedRateFlagsEXT requested_compression_fixed_rate = VK_IMAGE_COMPRESSION_FIXED_RATE_NONE_EXT);

	Swapchain(const Swapchain &) = delete;

	Swapchain(Swapchain &&other);

	~Swapchain();

	Swapchain &operator=(const Swapchain &) = delete;

	Swapchain &operator=(Swapchain &&) = delete;

	bool is_valid() const;

	vkb::core::DeviceC &get_device();

	VkSwapchainKHR get_handle() const;

	VkResult acquire_next_image(uint32_t &image_index, VkSemaphore image_acquired_semaphore, VkFence fence = VK_NULL_HANDLE) const;

	const VkExtent2D &get_extent() const;

	VkFormat get_format() const;

	VkSurfaceFormatKHR get_surface_format() const;

	const std::vector<VkImage> &get_images() const;

	VkSurfaceTransformFlagBitsKHR get_transform() const;

	VkSurfaceKHR get_surface() const;

	VkImageUsageFlags get_usage() const;

	VkPresentModeKHR get_present_mode() const;

	VkImageCompressionFlagsEXT get_applied_compression() const;

	/**
	 * Helper functions for compression controls
	 */

	struct SurfaceFormatCompression
	{
		VkSurfaceFormat2KHR             surface_format{VK_STRUCTURE_TYPE_SURFACE_FORMAT_2_KHR};
		VkImageCompressionPropertiesEXT compression_properties{VK_STRUCTURE_TYPE_IMAGE_COMPRESSION_PROPERTIES_EXT};
	};

	static std::vector<SurfaceFormatCompression> query_supported_fixed_rate_compression(vkb::core::DeviceC &device, const VkSurfaceKHR &surface);

  private:
	vkb::core::DeviceC &device;

	VkSurfaceKHR surface{VK_NULL_HANDLE};

	VkSwapchainKHR handle{VK_NULL_HANDLE};

	std::vector<VkImage> images;

	SwapchainProperties properties;

	// A list of present modes in order of priority (vector[0] has high priority, vector[size-1] has low priority)
	std::vector<VkPresentModeKHR> present_mode_priority_list = {
	    VK_PRESENT_MODE_FIFO_KHR,
	    VK_PRESENT_MODE_MAILBOX_KHR};

	// A list of surface formats in order of priority (vector[0] has high priority, vector[size-1] has low priority)
	std::vector<VkSurfaceFormatKHR> surface_format_priority_list = {
	    {VK_FORMAT_R8G8B8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR},
	    {VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR}};

	std::set<VkImageUsageFlagBits> image_usage_flags;

	VkImageCompressionFlagsEXT requested_compression{VK_IMAGE_COMPRESSION_DEFAULT_EXT};

	VkImageCompressionFixedRateFlagsEXT requested_compression_fixed_rate{VK_IMAGE_COMPRESSION_FIXED_RATE_NONE_EXT};
};
}        // namespace vkb
