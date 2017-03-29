#pragma once

struct Vec2 {
	float X = 0.0f, Y = 0.0f;

	static inline Vec2 Zero() { return Vec2{ 0.0f, 0.0f }; }
	static inline Vec2 UnitX() { return Vec2{ 1.0f, 0.0f }; }
	static inline Vec2 UnitY() { return Vec2{ 0.0f, 1.0f }; }

	inline Vec2 operator+(Vec2 other) const { return Vec2{ X + other.X, Y + other.Y }; }
	inline Vec2 operator-(Vec2 other) const { return Vec2{ X - other.X, Y - other.Y }; }
	inline Vec2 operator*(Vec2 other) const { return Vec2{ X * other.X, Y * other.Y }; }
	inline Vec2 operator/(Vec2 other) const { return Vec2{ X / other.X, Y / other.Y }; }

	inline Vec2& operator+=(Vec2 other) { *this = *this + other; return *this; }
	inline Vec2& operator-=(Vec2 other) { *this = *this - other; return *this; }
	inline Vec2& operator*=(Vec2 other) { *this = *this * other; return *this; }
	inline Vec2& operator/=(Vec2 other) { *this = *this / other; return *this; }

	inline Vec2 operator*(float f) const { return Vec2{ X * f, Y * f }; }
	inline Vec2 operator/(float f) const { return Vec2{ X / f, Y / f }; }

	inline Vec2& operator*=(float f) { *this = *this * f; return *this; }
	inline Vec2& operator/=(float f) { *this = *this / f; return *this; }

	inline Vec2& operator=(Vec2 other) { X = other.X; Y = other.Y; return *this; }

	inline bool operator==(Vec2 other) const { return X == other.X && Y == other.Y; }
	inline bool operator!=(Vec2 other) const { return X != other.X || Y != other.Y; }
};


struct Vec3 {
	float X = 0.0f, Y = 0.0f, Z = 0.0f;

	static inline Vec3 Zero() { return Vec3{ 0.0f, 0.0f, 0.0f }; }
	static inline Vec3 UnitX() { return Vec3{ 1.0f, 0.0f, 0.0f }; }
	static inline Vec3 UnitY() { return Vec3{ 0.0f, 1.0f, 0.0f }; }
	static inline Vec3 UnitZ() { return Vec3{ 0.0f, 0.0f, 1.0f }; }

	inline Vec3 operator+(Vec3 other) const { return Vec3{ X + other.X, Y + other.Y, Z + other.Z }; }
	inline Vec3 operator-(Vec3 other) const { return Vec3{ X - other.X, Y - other.Y, Z - other.Z }; }
	inline Vec3 operator*(Vec3 other) const { return Vec3{ X * other.X, Y * other.Y, Z * other.Z }; }
	inline Vec3 operator/(Vec3 other) const { return Vec3{ X / other.X, Y / other.Y, Z / other.Z }; }

	inline Vec3& operator+=(Vec3 other) { *this = *this + other; return *this; }
	inline Vec3& operator-=(Vec3 other) { *this = *this - other; return *this; }
	inline Vec3& operator*=(Vec3 other) { *this = *this * other; return *this; }
	inline Vec3& operator/=(Vec3 other) { *this = *this / other; return *this; }

	inline Vec3 operator*(float f) const { return Vec3{ X * f, Y * f, Z * f }; }
	inline Vec3 operator/(float f) const { return Vec3{ X / f, Y / f, Z / f }; }

	inline Vec3& operator*=(float f) { *this = *this * f; return *this; }
	inline Vec3& operator/=(float f) { *this = *this / f; return *this; }

	inline Vec3& operator=(Vec3 other) { X = other.X; Y = other.Y; Z = other.Z; return *this; }

	inline bool operator==(Vec3 other) const { return X == other.X && Y == other.Y && Z == other.Z; }
	inline bool operator!=(Vec3 other) const { return X != other.X || Y != other.Y || Z != other.Z; }

	static float Dot(Vec3, Vec3);
	static Vec3 Cross(Vec3, Vec3);

	Vec3 ToNormal() const;
	float Magnitude() const;
};

struct Vec4 {
	float X = 0.0f, Y = 0.0f, Z = 0.0f, W = 0.0f;

	static inline Vec4 Zero() { return Vec4{ 0.0f, 0.0f, 0.0f, 0.0f }; }
	static inline Vec4 UnitX() { return Vec4{ 1.0f, 0.0f, 0.0f, 0.0f }; }
	static inline Vec4 UnitY() { return Vec4{ 0.0f, 1.0f, 0.0f, 0.0f }; }
	static inline Vec4 UnitZ() { return Vec4{ 0.0f, 0.0f, 1.0f, 0.0f }; }
	static inline Vec4 UnitW() { return Vec4{ 0.0f, 0.0f, 0.0f, 1.0f }; }

	inline Vec4 operator+(Vec4 other) const { return Vec4{ X + other.X, Y + other.Y, Z + other.Z, W + other.W }; }
	inline Vec4 operator-(Vec4 other) const { return Vec4{ X - other.X, Y - other.Y, Z - other.Z, W - other.W }; }
	inline Vec4 operator*(Vec4 other) const { return Vec4{ X * other.X, Y * other.Y, Z * other.Z, W * other.W }; }
	inline Vec4 operator/(Vec4 other) const { return Vec4{ X / other.X, Y / other.Y, Z / other.Z, W / other.W }; }

	inline Vec4& operator+=(Vec4 other) { *this = *this + other; return *this; }
	inline Vec4& operator-=(Vec4 other) { *this = *this - other; return *this; }
	inline Vec4& operator*=(Vec4 other) { *this = *this * other; return *this; }
	inline Vec4& operator/=(Vec4 other) { *this = *this / other; return *this; }

	inline Vec4 operator*(float f) const { return Vec4{ X * f, Y * f, Z * f, W * f }; }
	inline Vec4 operator/(float f) const { return Vec4{ X / f, Y / f, Z / f, W / f }; }

	inline Vec4& operator*=(float f) { *this = *this * f; return *this; }
	inline Vec4& operator/=(float f) { *this = *this / f; return *this; }

	inline Vec4& operator=(Vec4 other) { X = other.X; Y = other.Y; Z = other.Z; W = other.W; return *this; }

	inline bool operator==(Vec4 other)const { return X == other.X && Y == other.Y && Z == other.Z && W == other.W; }
	inline bool operator!=(Vec4 other)const { return X != other.X || Y != other.Y || Z != other.Z || W != other.W; }
};