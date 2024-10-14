#pragma once

using s8  = signed char;
using s16 = short;
using s32 = int;
using s64 = long long;

using u8  = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long long;

using f32 = float;
using f64 = double;
using extended = long double; // 64 - 80 - 128 Bit floating point number

#define BIT(bitshift) (1u << (bitshift))

constexpr s8  S8_MIN  = (-127 - 1);
constexpr s16 S16_MIN = (-32767 - 1);
constexpr s32 S32_MIN = (-2147483647 - 1);
constexpr s64 S64_MIN = (-9223372036854775807 - 1);
constexpr s8  S8_MAX  = 127;
constexpr s16 S16_MAX = 32767;
constexpr s32 S32_MAX = 2147483647;
constexpr s64 S64_MAX = 9223372036854775807ll;
constexpr u8  U8_MAX  = 0xffu;
constexpr u16 U16_MAX = 0xffffu;
constexpr u32 U32_MAX = 0xffffffffu;
constexpr u64 U64_MAX = 0xffffffffffffffffllu;

namespace mstl
{

	struct Extend2D final
	{
		u32 Width;
		u32 Height;
	};

	struct Extend3D final
	{
		u32 Width;
		u32 Height;
		u32 Depth;
	};

	struct Viewport final
	{
		f32 X = 0.0f;
		f32 Y = 0.0f;
		f32 Width = 0.0f;
		f32 Height = 0.0f;
		f32 MinDepth = 0.0f;
		f32 MaxDepth = 1.0f;
	};

	struct Scissor final
	{
		s32 X = 0;
		s32 Y = 0;
		u32 Width = 0;
		u32 Height = 0;
	};

}
