# Copyright (c) 2023-2025, Mobica Limited
#
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 the "License";
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

get_filename_component(FOLDER_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
get_filename_component(PARENT_DIR ${CMAKE_CURRENT_LIST_DIR} PATH)
get_filename_component(CATEGORY_NAME ${PARENT_DIR} NAME)

add_sample(
    ID ${FOLDER_NAME}
    CATEGORY ${CATEGORY_NAME}
    AUTHOR "Mobica"
    NAME "Geometry shader to mesh shader"
    DESCRIPTION "Sample shows migration from geometry shader pipeline to mesh shader one"
    SHADER_FILES_GLSL
        "gshader_to_mshader/gshader_to_mshader.vert"
        "gshader_to_mshader/gshader_to_mshader.frag"
        "gshader_to_mshader/gshader_to_mshader.geom"
        "gshader_to_mshader/gshader_to_mshader.mesh"
        "gshader_to_mshader/gshader_to_mshader_base.frag"
        "gshader_to_mshader/gshader_to_mshader_base.vert"
        "gshader_to_mshader/gshader_to_mshader_mesh.frag"
    GLSLC_ADDITIONAL_ARGUMENTS
        "--target-spv=spv1.4"
        )
