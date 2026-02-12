#pragma once
#include "../suku_foundation/array.h"
#include "../suku_foundation/string.h"
#include <functional>

namespace suku
{
    using Microsoft::WRL::ComPtr;
	using memory::Array2D;
	class Color;
	class Transform;
	class Bitmap;
	class RenderBitmap;

	class Bitmap
	{
	public:
		Bitmap() = default;
		// Create an empty bitmap
		Bitmap(UINT _width, UINT _height);
		// Create bitmap from file
		Bitmap(String _url);
		Bitmap(const char* _url) : Bitmap(String(_url)) {}
		Bitmap(const wchar_t* _url) : Bitmap(String(_url)) {}
		Bitmap(String _url, UINT _x, UINT _y, UINT _width, UINT _height);
		// Create bitmap from Array2D<Color>
		Bitmap(const Array2D<Color>& _pixels, UINT _width, UINT _height);
		Bitmap(const Array2D<Color>& _pixels, UINT _x, UINT _y, UINT _width, UINT _height);
		// Constructors from COM bitmaps: use ComPtr to avoid raw COM pointers
		explicit Bitmap(ComPtr<ID2D1Bitmap1> _d2dBitmap);
		explicit Bitmap(ComPtr<IWICBitmap> _wicBitmap);
		Bitmap(const Bitmap& _otherBitmap);
		Bitmap(Bitmap&& _otherBitmap)noexcept;
		~Bitmap();

		bool isValid()const;

		UINT getPixelByte();
		ComPtr<ID2D1Bitmap1> getD2DBitmap();
		ComPtr<IWICBitmap> getWICBitmap();

		void paint(float _x, float _y, float _alpha = 1.0f);
		void paint(float _x, float _y, Transform _transform, float _alpha = 1.0f);
		void paint(Transform _transform, float _alpha = 1.0f);
		UINT getWidth()const;
		UINT getHeight()const;
		std::pair<UINT, UINT> getSize()const;

		// Make sure the size of color array is not smaller than the bitmap size
		void getPixelDetail(Array2D<Color>& _pColorArray)const;

		BYTE* getDataPointer()const;

		Bitmap& operator=(const Bitmap& _bitmap);
		Bitmap& operator=(Bitmap&& _bitmap)noexcept;

		void updatePixelDetail(const Array2D<Color>& _detail);
		void updatePixelDetail(const Array2D<Color>& _detail, UINT _startX, UINT _startY);
		void changePixelDetailRough(std::function<void(Color&)> _function);

		// Parameters: x, y, the corresponding color of position (x,y)
		void changePixelDetail(std::function<void(UINT, UINT, Color&)> _function);

		// Parameters: x, y, the corresponding color of position (x,y)
		void viewPixelDetail(std::function<void(UINT, UINT, const Color&)> _viewFunction)const;

		friend class Shape;
		friend class PaintLayer;
	private:
		bool isValid_ = false;
		UINT bytesPerPixel_ = 0;
		UINT width_ = 0, height_ = 0;
		ComPtr<ID2D1Bitmap1> d2dBitmap_ = nullptr;
		ComPtr<IWICBitmap> wicBitmap_ = nullptr;
		// 目前不存在 WIC 位图版本比 D2D 位图版本旧的情况
		bool d2dBitmapUpdateTag_ = false;
		// bool wicBitmapUpdateTag_ = false;
		void refreshD2DBitmap();
		// void refreshWICBitmap();
		void refreshPixelByte();
	};

	void getHitAreaFromBitmap(Array2D<bool>& _hitArea, const Bitmap& _bitmap, float _alphaThreshold = 0.0f);

	class RenderBitmap
	{
	public:
		RenderBitmap() = default;
		// Create bitmap from D2D bitmap
		explicit RenderBitmap(ComPtr<ID2D1Bitmap1> _d2dBitmap) : d2dBitmap_(_d2dBitmap) {}
		// Constructors from COM bitmaps: use ComPtr to avoid raw COM pointers
		RenderBitmap(const RenderBitmap& _otherBitmap) : d2dBitmap_(_otherBitmap.d2dBitmap_) {}
		RenderBitmap(RenderBitmap&& _otherBitmap)noexcept : d2dBitmap_(std::move(_otherBitmap.d2dBitmap_)) {}

		ComPtr<ID2D1Bitmap1> getD2DBitmap() { return d2dBitmap_; }
		UINT getWidth()const;
		UINT getHeight()const;
		std::pair<UINT, UINT> getSize()const { return { getWidth(), getHeight() }; }

		void paint(float _alpha = 1.0f) const;
		void paint(float _x, float _y, float _alpha = 1.0f) const;
		void paint(float _x, float _y, Transform _transform, float _alpha = 1.0f) const;
		void paint(Transform _transform, float _alpha = 1.0f) const;

		friend class PaintLayer;
	private:
		ComPtr<ID2D1Bitmap1> d2dBitmap_ = nullptr;
	};
}