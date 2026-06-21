#pragma once
#include "UIElement.h"
#include "suku_config/config_element.h"
#include "suku_foundation/input.h"

namespace suku
{
	class OptionElement : public UIElement
	{
	public:
		OptionElement* prev = nullptr;
		OptionElement* next = nullptr;
		template<typename T> OptionElement(
			ConfigElement<T>& _bindedConfig, String _label, float _x, float _y, int _width, int _height);
		virtual void update() override { onUpdateFunc_(); }
		virtual bool onPaint() override { return onPaintFunc_(); }
	private:
		String label_;
		std::function<void()> onUpdateFunc_;
		std::function<bool()> onPaintFunc_;
		bool isSelected_ = true;
	};

	template<typename T>
	inline OptionElement::OptionElement(ConfigElement<T>& _bindedConfig, String _label, float _x, float _y, int _width, int _height)
		: UIElement(_x, _y, _width, _height), label_(_label)
	{
		onUpdateFunc_ = [this, &_bindedConfig]()
			{
				if (!isSelected_)
					return;
				if (input::isKeyDown(VK_UP))
				{
					if (prev)
					{
						isSelected_ = false;
						prev->isSelected_ = true;
					}
				}
				else if (input::isKeyDown(VK_DOWN))
				{
					if (next)
					{
						isSelected_ = false;
						next->isSelected_ = true;
					}
				}
				else if (input::isKeyDown(VK_LEFT))
				{
					if (_bindedConfig.hasRangeConstraint())
					{
						auto range = _bindedConfig.getRange();
						T newValue = _bindedConfig.getValue() - 1;
						if (newValue < range.first)
							newValue = range.first;
						_bindedConfig.setValue(newValue);
					}
					else if (_bindedConfig.hasListConstraint())
					{
						auto list = _bindedConfig.getValueList();
						auto it = std::find(list.begin(), list.end(), _bindedConfig.value());
						if (it != list.end() && it != list.begin())
							_bindedConfig.setValue(*(it - 1));
					}
				}
				else if (input::isKeyDown(VK_RIGHT))
				{
					if (_bindedConfig.hasRangeConstraint())
					{
						auto range = _bindedConfig.getRange();
						T newValue = T();
						if (newValue > range.second)
							newValue = range.second;
						_bindedConfig.setValue(newValue);
					}
					else if (_bindedConfig.hasListConstraint())
					{
						auto list = _bindedConfig.getValueList();
						auto it = std::find(list.begin(), list.end(), _bindedConfig.value());
						if (it != list.end() && it != list.end() - 1)
							_bindedConfig.setValue(*(it + 1));
					}
				}
			};
		onPaintFunc_ = [this, &_bindedConfig]()
			{
				RectangleShape area(width_, height_);
				area.setFill(isSelected_ ? Color(128, 128, 128, 0.5f) : Color(64, 64, 64, 0.5f));
				area.paint(x, y, transform);
				Text text("Consolas", 12, TextAlign::MiddleRight);
				text.setBrush(Color(255, 255, 255));
				text.textContent = std::to_wstring(_bindedConfig.value());
				text.paint(x + 8, y + 8, width_ - 16, height_ - 16, graphics::createSolidColorBrush(Color(255, 255, 255)));
				text.textContent = label_;
				text.setTextAlign(TextAlign::MiddleLeft);
				text.paint(x + 8, y + 8, width_ - 16, height_ - 16, graphics::createSolidColorBrush(Color(255, 255, 255)));
				return false;
			};
	}
}