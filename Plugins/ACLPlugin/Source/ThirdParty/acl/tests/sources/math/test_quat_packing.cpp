////////////////////////////////////////////////////////////////////////////////
// The MIT License (MIT)
//
// Copyright (c) 2018 Nicholas Frechette & Animation Compression Library contributors
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
////////////////////////////////////////////////////////////////////////////////

#include <catch.hpp>

#include <acl/math/quat_packing.h>

using namespace acl;

TEST_CASE("quat packing math", "[math][quat][packing]")
{
	struct UnalignedBuffer
	{
		uint32_t padding0;
		uint16_t padding1;
		uint8_t buffer[250];
	};
	static_assert((offsetof(UnalignedBuffer, buffer) % 2) == 0, "Minimum packing alignment is 2");

	const Quat_32 quat0 = quat_set(0.39564531008956383F, 0.044254239301713752F, 0.22768840967675355F, 0.88863059760894492F);

	{
		UnalignedBuffer tmp0;
		pack_quat_128(quat0, &tmp0.buffer[0]);
		Quat_32 quat1 = unpack_quat_128(&tmp0.buffer[0]);
		REQUIRE(quat_get_x(quat0) == quat_get_x(quat1));
		REQUIRE(quat_get_y(quat0) == quat_get_y(quat1));
		REQUIRE(quat_get_z(quat0) == quat_get_z(quat1));
		REQUIRE(quat_get_w(quat0) == quat_get_w(quat1));
	}

	{
		UnalignedBuffer tmp0;
		pack_quat_96(quat0, &tmp0.buffer[0]);
		Quat_32 quat1 = unpack_quat_96_unsafe(&tmp0.buffer[0]);
		REQUIRE(quat_get_x(quat0) == quat_get_x(quat1));
		REQUIRE(quat_get_y(quat0) == quat_get_y(quat1));
		REQUIRE(quat_get_z(quat0) == quat_get_z(quat1));
		REQUIRE(scalar_near_equal(quat_get_w(quat0), quat_get_w(quat1), 1.0E-4F));
	}

	{
		UnalignedBuffer tmp0;
		pack_quat_48(quat0, &tmp0.buffer[0]);
		Quat_32 quat1 = unpack_quat_48(&tmp0.buffer[0]);
		REQUIRE(scalar_near_equal(quat_get_x(quat0), quat_get_x(quat1), 1.0E-4F));
		REQUIRE(scalar_near_equal(quat_get_y(quat0), quat_get_y(quat1), 1.0E-4F));
		REQUIRE(scalar_near_equal(quat_get_z(quat0), quat_get_z(quat1), 1.0E-4F));
		REQUIRE(scalar_near_equal(quat_get_w(quat0), quat_get_w(quat1), 1.0E-4F));
	}

	{
		UnalignedBuffer tmp0;
		pack_quat_32(quat0, &tmp0.buffer[0]);
		Quat_32 quat1 = unpack_quat_32(&tmp0.buffer[0]);
		REQUIRE(scalar_near_equal(quat_get_x(quat0), quat_get_x(quat1), 1.0E-3F));
		REQUIRE(scalar_near_equal(quat_get_y(quat0), quat_get_y(quat1), 1.0E-3F));
		REQUIRE(scalar_near_equal(quat_get_z(quat0), quat_get_z(quat1), 1.0E-3F));
		REQUIRE(scalar_near_equal(quat_get_w(quat0), quat_get_w(quat1), 1.0E-3F));
	}

	REQUIRE(get_packed_rotation_size(RotationFormat8::Quat_128) == 16);
	REQUIRE(get_packed_rotation_size(RotationFormat8::QuatDropW_96) == 12);
	REQUIRE(get_packed_rotation_size(RotationFormat8::QuatDropW_48) == 6);
	REQUIRE(get_packed_rotation_size(RotationFormat8::QuatDropW_32) == 4);

	REQUIRE(get_range_reduction_rotation_size(RotationFormat8::Quat_128) == 32);
	REQUIRE(get_range_reduction_rotation_size(RotationFormat8::QuatDropW_96) == 24);
	REQUIRE(get_range_reduction_rotation_size(RotationFormat8::QuatDropW_48) == 24);
	REQUIRE(get_range_reduction_rotation_size(RotationFormat8::QuatDropW_32) == 24);
	REQUIRE(get_range_reduction_rotation_size(RotationFormat8::QuatDropW_Variable) == 24);
}
