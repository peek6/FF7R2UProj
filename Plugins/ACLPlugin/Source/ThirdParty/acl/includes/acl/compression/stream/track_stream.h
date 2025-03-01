#pragma once

////////////////////////////////////////////////////////////////////////////////
// The MIT License (MIT)
//
// Copyright (c) 2017 Nicholas Frechette & Animation Compression Library contributors
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

#include "acl/core/iallocator.h"
#include "acl/core/compiler_utils.h"
#include "acl/core/error.h"
#include "acl/core/track_types.h"
#include "acl/core/utils.h"
#include "acl/math/quat_32.h"
#include "acl/math/quat_packing.h"
#include "acl/math/vector4_32.h"
#include "acl/math/vector4_packing.h"

#include <cstdint>

ACL_IMPL_FILE_PRAGMA_PUSH

namespace acl
{
	class TrackStream
	{
	public:
		uint8_t* get_raw_sample_ptr(uint32_t sample_index)
		{
			ACL_ASSERT(sample_index < m_num_samples, "Invalid sample index. %u >= %u", sample_index, m_num_samples);
			uint32_t offset = sample_index * m_sample_size;
			return m_samples + offset;
		}

		const uint8_t* get_raw_sample_ptr(uint32_t sample_index) const
		{
			ACL_ASSERT(sample_index < m_num_samples, "Invalid sample index. %u >= %u", sample_index, m_num_samples);
			uint32_t offset = sample_index * m_sample_size;
			return m_samples + offset;
		}

		template<typename SampleType>
		SampleType ACL_SIMD_CALL get_raw_sample(uint32_t sample_index) const
		{
			const uint8_t* ptr = get_raw_sample_ptr(sample_index);
			return *safe_ptr_cast<const SampleType>(ptr);
		}

#if defined(ACL_NO_INTRINSICS)
		template<typename SampleType>
		void ACL_SIMD_CALL set_raw_sample(uint32_t sample_index, const SampleType& sample)
#else
		template<typename SampleType>
		void ACL_SIMD_CALL set_raw_sample(uint32_t sample_index, SampleType sample)
#endif
		{
			ACL_ASSERT(m_sample_size == sizeof(SampleType), "Unexpected sample size. %u != %zu", m_sample_size, sizeof(SampleType));
			uint8_t* ptr = get_raw_sample_ptr(sample_index);
			*safe_ptr_cast<SampleType>(ptr) = sample;
		}

		uint32_t get_num_samples() const { return m_num_samples; }
		uint32_t get_sample_size() const { return m_sample_size; }
		float get_sample_rate() const { return m_sample_rate; }
		AnimationTrackType8 get_track_type() const { return m_type; }
		uint8_t get_bit_rate() const { return m_bit_rate; }
		bool is_bit_rate_variable() const { return m_bit_rate != k_invalid_bit_rate; }
		float get_duration() const { return calculate_duration(m_num_samples, m_sample_rate); }

		uint32_t get_packed_sample_size() const
		{
			if (m_type == AnimationTrackType8::Rotation)
				return get_packed_rotation_size(m_format.rotation);
			else
				return get_packed_vector_size(m_format.vector);
		}

	protected:
		TrackStream(AnimationTrackType8 type, TrackFormat8 format) : m_allocator(nullptr), m_samples(nullptr), m_num_samples(0), m_sample_size(0), m_sample_rate(0.0F), m_type(type), m_format(format), m_bit_rate(0) {}
		ACL_DEPRECATED("Use a floating point sample rate instead, to be removed in v2.0")
		TrackStream(IAllocator& allocator, uint32_t num_samples, uint32_t sample_size, uint32_t sample_rate, AnimationTrackType8 type, TrackFormat8 format, uint8_t bit_rate)
			: m_allocator(&allocator)
			, m_samples(reinterpret_cast<uint8_t*>(allocator.allocate(sample_size * num_samples + k_padding, 16)))
			, m_num_samples(num_samples)
			, m_sample_size(sample_size)
			, m_sample_rate(float(sample_rate))
			, m_type(type)
			, m_format(format)
			, m_bit_rate(bit_rate)
		{}

		TrackStream(IAllocator& allocator, uint32_t num_samples, uint32_t sample_size, float sample_rate, AnimationTrackType8 type, TrackFormat8 format, uint8_t bit_rate)
			: m_allocator(&allocator)
			, m_samples(reinterpret_cast<uint8_t*>(allocator.allocate(sample_size * num_samples + k_padding, 16)))
			, m_num_samples(num_samples)
			, m_sample_size(sample_size)
			, m_sample_rate(sample_rate)
			, m_type(type)
			, m_format(format)
			, m_bit_rate(bit_rate)
		{}

		TrackStream(const TrackStream&) = delete;
		TrackStream(TrackStream&& other)
			: m_allocator(other.m_allocator)
			, m_samples(other.m_samples)
			, m_num_samples(other.m_num_samples)
			, m_sample_size(other.m_sample_size)
			, m_sample_rate(other.m_sample_rate)
			, m_type(other.m_type)
			, m_format(other.m_format)
			, m_bit_rate(other.m_bit_rate)
		{
			new(&other) TrackStream(other.m_type, other.m_format);
		}

		~TrackStream()
		{
			if (m_allocator != nullptr && m_num_samples != 0)
				m_allocator->deallocate(m_samples, m_sample_size * m_num_samples + k_padding);
		}

		TrackStream& operator=(const TrackStream&) = delete;
		TrackStream& operator=(TrackStream&& rhs)
		{
			std::swap(m_allocator, rhs.m_allocator);
			std::swap(m_samples, rhs.m_samples);
			std::swap(m_num_samples, rhs.m_num_samples);
			std::swap(m_sample_size, rhs.m_sample_size);
			std::swap(m_sample_rate, rhs.m_sample_rate);
			std::swap(m_type, rhs.m_type);
			std::swap(m_format, rhs.m_format);
			std::swap(m_bit_rate, rhs.m_bit_rate);
			return *this;
		}

		void duplicate(TrackStream& copy) const
		{
			ACL_ASSERT(copy.m_type == m_type, "Attempting to duplicate streams with incompatible types!");
			if (m_allocator != nullptr)
			{
				copy.m_allocator = m_allocator;
				copy.m_samples = reinterpret_cast<uint8_t*>(m_allocator->allocate(m_sample_size * m_num_samples + k_padding, 16));
				copy.m_num_samples = m_num_samples;
				copy.m_sample_size = m_sample_size;
				copy.m_sample_rate = m_sample_rate;
				copy.m_format = m_format;
				copy.m_bit_rate = m_bit_rate;

				std::memcpy(copy.m_samples, m_samples, (size_t)m_sample_size * m_num_samples);
			}
		}

		// In order to guarantee the safety of unaligned SIMD loads of every byte, we add some padding
		static constexpr uint32_t k_padding = 15;

		IAllocator*				m_allocator;
		uint8_t*				m_samples;
		uint32_t				m_num_samples;
		uint32_t				m_sample_size;
		float					m_sample_rate;

		AnimationTrackType8		m_type;
		TrackFormat8			m_format;
		uint8_t					m_bit_rate;
	};

	class RotationTrackStream final : public TrackStream
	{
	public:
		RotationTrackStream() : TrackStream(AnimationTrackType8::Rotation, TrackFormat8(RotationFormat8::Quat_128)) {}
		ACL_DEPRECATED("Use a floating point sample rate instead, to be removed in v2.0")
		RotationTrackStream(IAllocator& allocator, uint32_t num_samples, uint32_t sample_size, uint32_t sample_rate, RotationFormat8 format, uint8_t bit_rate = k_invalid_bit_rate)
			: TrackStream(allocator, num_samples, sample_size, float(sample_rate), AnimationTrackType8::Rotation, TrackFormat8(format), bit_rate)
		{}
		RotationTrackStream(IAllocator& allocator, uint32_t num_samples, uint32_t sample_size, float sample_rate, RotationFormat8 format, uint8_t bit_rate = k_invalid_bit_rate)
			: TrackStream(allocator, num_samples, sample_size, sample_rate, AnimationTrackType8::Rotation, TrackFormat8(format), bit_rate)
		{}
		RotationTrackStream(const RotationTrackStream&) = delete;
		RotationTrackStream(RotationTrackStream&& other)
			: TrackStream(std::forward<TrackStream>(other))
		{}
		~RotationTrackStream() = default;

		RotationTrackStream& operator=(const RotationTrackStream&) = delete;
		RotationTrackStream& operator=(RotationTrackStream&& rhs)
		{
			TrackStream::operator=(std::forward<TrackStream>(rhs));
			return *this;
		}

		RotationTrackStream duplicate() const
		{
			RotationTrackStream copy;
			TrackStream::duplicate(copy);
			return copy;
		}

		RotationFormat8 get_rotation_format() const { return m_format.rotation; }
	};

	class TranslationTrackStream final : public TrackStream
	{
	public:
		TranslationTrackStream() : TrackStream(AnimationTrackType8::Translation, TrackFormat8(VectorFormat8::Vector3_96)) {}
		ACL_DEPRECATED("Use a floating point sample rate instead, to be removed in v2.0")
		TranslationTrackStream(IAllocator& allocator, uint32_t num_samples, uint32_t sample_size, uint32_t sample_rate, VectorFormat8 format, uint8_t bit_rate = k_invalid_bit_rate)
			: TrackStream(allocator, num_samples, sample_size, float(sample_rate), AnimationTrackType8::Translation, TrackFormat8(format), bit_rate)
		{}
		TranslationTrackStream(IAllocator& allocator, uint32_t num_samples, uint32_t sample_size, float sample_rate, VectorFormat8 format, uint8_t bit_rate = k_invalid_bit_rate)
			: TrackStream(allocator, num_samples, sample_size, sample_rate, AnimationTrackType8::Translation, TrackFormat8(format), bit_rate)
		{}
		TranslationTrackStream(const TranslationTrackStream&) = delete;
		TranslationTrackStream(TranslationTrackStream&& other)
			: TrackStream(std::forward<TrackStream>(other))
		{}
		~TranslationTrackStream() = default;

		TranslationTrackStream& operator=(const TranslationTrackStream&) = delete;
		TranslationTrackStream& operator=(TranslationTrackStream&& rhs)
		{
			TrackStream::operator=(std::forward<TrackStream>(rhs));
			return *this;
		}

		TranslationTrackStream duplicate() const
		{
			TranslationTrackStream copy;
			TrackStream::duplicate(copy);
			return copy;
		}

		VectorFormat8 get_vector_format() const { return m_format.vector; }
	};

	class ScaleTrackStream final : public TrackStream
	{
	public:
		ScaleTrackStream() : TrackStream(AnimationTrackType8::Scale, TrackFormat8(VectorFormat8::Vector3_96)) {}
		ACL_DEPRECATED("Use a floating point sample rate instead, to be removed in v2.0")
		ScaleTrackStream(IAllocator& allocator, uint32_t num_samples, uint32_t sample_size, uint32_t sample_rate, VectorFormat8 format, uint8_t bit_rate = k_invalid_bit_rate)
			: TrackStream(allocator, num_samples, sample_size, float(sample_rate), AnimationTrackType8::Scale, TrackFormat8(format), bit_rate)
		{}
		ScaleTrackStream(IAllocator& allocator, uint32_t num_samples, uint32_t sample_size, float sample_rate, VectorFormat8 format, uint8_t bit_rate = k_invalid_bit_rate)
			: TrackStream(allocator, num_samples, sample_size, sample_rate, AnimationTrackType8::Scale, TrackFormat8(format), bit_rate)
		{}
		ScaleTrackStream(const ScaleTrackStream&) = delete;
		ScaleTrackStream(ScaleTrackStream&& other)
			: TrackStream(std::forward<TrackStream>(other))
		{}
		~ScaleTrackStream() = default;

		ScaleTrackStream& operator=(const ScaleTrackStream&) = delete;
		ScaleTrackStream& operator=(ScaleTrackStream&& rhs)
		{
			TrackStream::operator=(std::forward<TrackStream>(rhs));
			return *this;
		}

		ScaleTrackStream duplicate() const
		{
			ScaleTrackStream copy;
			TrackStream::duplicate(copy);
			return copy;
		}

		VectorFormat8 get_vector_format() const { return m_format.vector; }
	};

	// For a rotation track, the extent only tells us if the track is constant or not
	// since the min/max we maintain aren't valid rotations.
	// Similarly, the center isn't a valid rotation and is meaningless.
	class TrackStreamRange
	{
	public:
		static TrackStreamRange ACL_SIMD_CALL from_min_max(Vector4_32Arg0 min, Vector4_32Arg1 max)
		{
			return TrackStreamRange(min, vector_sub(max, min));
		}

		static TrackStreamRange ACL_SIMD_CALL from_min_extent(Vector4_32Arg0 min, Vector4_32Arg1 extent)
		{
			return TrackStreamRange(min, extent);
		}

		TrackStreamRange()
			: m_min(vector_zero_32())
			, m_extent(vector_zero_32())
		{}

		Vector4_32 ACL_SIMD_CALL get_min() const { return m_min; }
		Vector4_32 ACL_SIMD_CALL get_max() const { return vector_add(m_min, m_extent); }

		Vector4_32 ACL_SIMD_CALL get_center() const { return vector_add(m_min, vector_mul(m_extent, 0.5F)); }
		Vector4_32 ACL_SIMD_CALL get_extent() const { return m_extent; }

		bool is_constant(float threshold) const { return vector_all_less_than(vector_abs(m_extent), vector_set(threshold)); }

	private:
		TrackStreamRange(const Vector4_32& min, const Vector4_32& extent)
			: m_min(min)
			, m_extent(extent)
		{}

		Vector4_32	m_min;
		Vector4_32	m_extent;
	};

	struct BoneRanges
	{
		TrackStreamRange rotation;
		TrackStreamRange translation;
		TrackStreamRange scale;
	};

	struct SegmentContext;

	struct BoneStreams
	{
		SegmentContext* segment;
		uint16_t bone_index;
		uint16_t parent_bone_index;
		uint16_t output_index;

		RotationTrackStream rotations;
		TranslationTrackStream translations;
		ScaleTrackStream scales;

		bool is_rotation_constant;
		bool is_rotation_default;
		bool is_translation_constant;
		bool is_translation_default;
		bool is_scale_constant;
		bool is_scale_default;

		bool is_stripped_from_output() const { return output_index == k_invalid_bone_index; }

		BoneStreams duplicate() const
		{
			BoneStreams copy;
			copy.segment = segment;
			copy.bone_index = bone_index;
			copy.parent_bone_index = parent_bone_index;
			copy.output_index = output_index;
			copy.rotations = rotations.duplicate();
			copy.translations = translations.duplicate();
			copy.scales = scales.duplicate();
			copy.is_rotation_constant = is_rotation_constant;
			copy.is_rotation_default = is_rotation_default;
			copy.is_translation_constant = is_translation_constant;
			copy.is_translation_default = is_translation_default;
			copy.is_scale_constant = is_scale_constant;
			copy.is_scale_default = is_scale_default;
			return copy;
		}
	};
}

ACL_IMPL_FILE_PRAGMA_POP
