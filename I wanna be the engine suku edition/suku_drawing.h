#pragma once
#include "framework.h"
#include "suku_maths.h"

namespace suku
{
#define SAFE_ADDREF(P) if(P) P->AddRef();
#define SAFE_RELEASE(P) if(P){P -> Release(); P = nullptr;}

	class Transform;
	class Shape;
	class Bitmap;
	class Color;
	class Effect;

	extern ID2D1Factory* g_pD2DFactory;
	extern ID2D1HwndRenderTarget* g_pRenderTarget;
	extern IWICImagingFactory* g_pIWICFactory;

	extern TCHAR strPath[MAX_PATH + 1];
	extern size_t Path_len;
	extern HWND game_hWnd;

	class Transform
	{
	public:
		D2D1::Matrix3x2F matrix;

		Transform();
		Transform(D2D1::Matrix3x2F _matrix);

		void transformPoint(float* _x, float* _y);
		Vector transformPoint(float _x, float _y);
		Transform invertTransform();

		Transform operator +(Transform _x);	//recommend using this
		Transform operator *(Transform _x);
		void operator =(Transform _x);
	};

	Transform translation(float _shiftX, float _shiftY);
	Transform rotation(float _rotateCenterX, float _rotateCenterY, float _angle);
	Transform scale(float _centerX, float _centerY, float _scaleX, float _scaleY);
	Transform skew(float _centerX, float _centerY, float _angleX, float _angleY);
	void setPaintingTransform(Transform _transform);

	class Shape
	{
	public:
		Transform transform;
		ID2D1TransformedGeometry* currentGeometry;
		ID2D1Geometry* originalGeometry;
		//ID2D1Brush* fillBrush;
		//ID2D1Brush* outlineBrush;
		//ID2D1StrokeStyle* outlineStrokeStyle;
		//float outlineWidth;

		//Shape& operator= (Shape& _x)const = delete;

		Shape();
		Shape(const Shape& _x);
		Shape(ID2D1Geometry* _geometry);
		//Shape(ID2D1Geometry* _geometry, ID2D1Brush* _fillBrush, ID2D1Brush* _outlineBrush,
		//	float _edgeWidth = 1.0, ID2D1StrokeStyle* _outlineStrokeStyle = nullptr);
		void join();
		~Shape();

		void setOriginalGeometry(ID2D1Geometry* _geometry);
		void setTransform(Transform _transform);
		void paint(float _x, float _y,
			ID2D1Brush* _fillBrush, ID2D1Brush* _outlineBrush, float _outlineWidth = 1.0,
			ID2D1StrokeStyle* outlineStrokeStyle = nullptr);
		void paint(float _x, float _y, Transform _paintingTransform,
			ID2D1Brush* _fillBrush, ID2D1Brush* _outlineBrush, float _outlineWidth = 1.0,
			ID2D1StrokeStyle* outlineStrokeStyle = nullptr);
		void paint(Transform _paintingTransform,
			ID2D1Brush* _fillBrush, ID2D1Brush* _outlineBrush, float _outlineWidth = 1.0,
			ID2D1StrokeStyle* outlineStrokeStyle = nullptr);

		bool isCrashed(Shape& _x);

		Shape& operator= (const Shape& _x);
		Shape operator-(const Shape& _x);
		Shape operator&(const Shape& _x);
		Shape operator|(const Shape& _x);
		Shape operator^(const Shape& _x);

		bool operator==(const Shape& other) const = default;
	};

	class SquareShape :public Shape
	{
	public:
		SquareShape(float _length, float _beginningX = 0, float _beginningY = 0);
	};

	class RectangleShape :public Shape
	{
	public:
		RectangleShape(float _width, float _height, float _beginningX = 0, float _beginningY = 0);
	};

	class CircleShape :public Shape
	{
	public:
		CircleShape(float _radius, float _beginningX = 0, float _beginningY = 0);
	};

	class EllipseShape :public Shape
	{
	public:
		EllipseShape(float _radiusX, float _radiusY, float _beginningX = 0, float _beginningY = 0);
	};

	class Bitmap
	{
	public:
		//Create an empty bitmap
		Bitmap(UINT _width, UINT _height);
		//Create bitmap from file
		Bitmap(LPCTSTR _url);
		Bitmap(LPCTSTR _url, UINT _x, UINT _y, UINT _width, UINT _height);
		//Create bitmap from Color[][]
		Bitmap(Color** _pixels, UINT _width, UINT _height);
		Bitmap(Color** _pixels, UINT _x, UINT _y, UINT _width, UINT _height);
		Bitmap(const Bitmap& _otherBitmap);
		~Bitmap();

		void paint(float _x, float _y, float _alpha = 1.0f)const;
		void paint(float _x, float _y, Transform _transform, float _alpha = 1.0f)const;
		void paint(Transform _transform, float _alpha = 1.0f)const;
		UINT getWidth()const;
		UINT getHeight()const;
		std::pair<UINT, UINT> getSize()const;

		//This will create a new piece of memory; remember to use free2D() to delete it after using!
		Color** getPixelDetail()const;

		//This will not create a new piece of memory
		BYTE* getDataPointer()const;
		
		Bitmap& operator=(const Bitmap& _bitmap);

		void updatePixelDetail(Color** _detail);
		void updatePixelDetail(Color** _detail, UINT _startX, UINT _startY);
		void changePixelDetailRough(std::function<void(Color&)> _changingFunction);

		//Parameters: x, y, the corresponding color of position (x,y)
		void changePixelDetail(std::function<void(UINT, UINT, Color&)> _changingFunction);

		//Parameters: x, y, the corresponding color of position (x,y)
		void viewPixelDetail(std::function<void(UINT, UINT, const Color&)> _viewFunction)const;
	private:
		UINT width_, height_;
		ID2D1Bitmap* d2d1Bitmap_;
		IWICBitmap* wicBitmap_;
	};

	class Color
	{
	public:
		float alpha;	//range: 0.0f ~ 1.0f

		Color();
		Color(float _r, float _g, float _b, float _alpha = 1.0f);

		inline float r()const { return r_; }
		inline float g()const { return g_; }
		inline float b()const { return b_; }
		float h()const;
		float s()const;
		float v()const;
		inline void setR(float _r) { r_ = _r; }
		inline void setG(float _g) { g_ = _g; }
		inline void setB(float _b) { b_ = _b; }
		inline void setRGB(float _r, float _g, float _b) { r_ = _r, g_ = _g, b_ = _b; }
		void setH(float _h);
		void setS(float _s);
		void setV(float _v);
		void setHSV(float _h, float _s, float _v);
	private:
		float r_, g_, b_;	//range: 0.0f ~ 255.0f
	};

	class Effect
	{
	public:

	};

	LPCTSTR AbsolutePath(LPCTSTR _relativePath);

	void SwapByte(BYTE* _a, BYTE* _b);

	void beginDraw(HWND hWnd);
	void endDraw();
	void clearScreen();
	void createD2DResource(HWND hWnd);
	void cleanup();

	HRESULT loadWICBitmap(
		IWICBitmap** _pWicBitmap,
		LPCTSTR uri
	);
	HRESULT loadWICBitmap(
		IWICBitmap** _pWicBitmap,
		LPCTSTR uri,
		UINT _x, UINT _y, UINT _width, UINT _height
	);
	//Due to technical problem, _width and _height should <= 2048
	HRESULT createWICBitmap(
		IWICBitmap** _pWicBitmap,
		UINT _width,
		UINT _height
	);
	HRESULT getD2DBitmap(
		IWICBitmap** _pWicBitmap,
		ID2D1Bitmap** _pD2dBitmap,
		int _width,
		int _height
	);

	//This will create a new piece of memory; remember to use free2D() to delete it after using!
	Color** getPixelDetailFromWICBitmap(IWICBitmap* _sourceBitmap);

	//This will create a new piece of memory; remember to use free2D() to delete it after using!
	Color** getPixelDetailFromWICBitmap(IWICBitmap* _sourceBitmap, UINT _x, UINT _y, UINT _width, UINT _height);

	ID2D1Brush* createSolidColorBrush(const Color _color);

	std::pair<UINT, UINT> getSizeFromWICBitmap(IWICBitmap* _pBitmap, HRESULT* _pHResult = nullptr);
	void getHitAreaFromBitmap(bool** _pHitArea, IWICBitmap* _pBitmap, float _alphaThreshold = 0.0f);
	void getHitAreaFromBitmap(bool** _pHitArea, const Bitmap& _pBitmap, float _alphaThreshold = 0.0f);
	//void drawBitmap(ID2D1Bitmap* _pBitmap, float _x, float _y,
	//	float _width, float _height, float _alpha,
	//	float _angle, float _rotatingCenterX, float _rotatingCenterY);
	void drawBitmap(ID2D1Bitmap* _pBitmap, float _x, float _y,
		float _width, float _height, float _alpha,
		Transform _transform = Transform());
	void drawBitmap(ID2D1Bitmap* _pBitmap,
		float _width, float _height, float _alpha,
		Transform _transform = Transform());
}