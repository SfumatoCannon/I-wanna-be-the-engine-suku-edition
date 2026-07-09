#pragma once
#include "UIElement.h"
#include "suku_config/config_element.h"
#include "suku_foundation/input.h"

namespace suku
{
	class OptionPool;

	class OptionElement : public UIElement
	{
	public:
		OptionElement* prev = nullptr;
		OptionElement* next = nullptr;

		template<typename T> OptionElement(
			ConfigElement<T>& _bindedConfig, String _label, float _x, float _y, int _width, int _height);
		template<typename T> OptionElement(ConfigElement<T>& _bindedConfig, String _label) : OptionElement(_bindedConfig, _label, 0, 0, 256, 32) {}

		virtual void update() override { onUpdateFunc_(this); }
		virtual bool onPaint() override { return onPaintFunc_(this); }
	private:
		friend class OptionPool;
		String label_;
		std::function<void(OptionElement*)> onUpdateFunc_;
		std::function<bool(OptionElement*)> onPaintFunc_;
		bool isSelected_ = false;
	};

	template<typename T>
	inline OptionElement::OptionElement(ConfigElement<T>& _bindedConfig, String _label, float _x, float _y, int _width, int _height)
		: UIElement(_x, _y, _width, _height), label_(_label)
	{
		onUpdateFunc_ = [&_bindedConfig](OptionElement* _element)
			{
				if (!_element->isSelected_)
					return;

				if (input::isKeyDown(VK_LEFT))
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
					else
					{
						if constexpr (std::is_same_v<bool, T>)
						{
							_bindedConfig.setValue(!_bindedConfig.getValue());
						}
						else if constexpr (std::is_arithmetic_v<T>)
						{
							_bindedConfig.setValue(_bindedConfig.getValue() - 1);
						}
					}
				}
				else if (input::isKeyDown(VK_RIGHT))
				{
					if (_bindedConfig.hasRangeConstraint())
					{
						auto range = _bindedConfig.getRange();
						T newValue = _bindedConfig.getValue() + 1;
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
					else
					{
						if constexpr (std::is_same_v<bool, T>)
						{
							_bindedConfig.setValue(!_bindedConfig.getValue());
						}
						else if constexpr (std::is_arithmetic_v<T>)
						{
							_bindedConfig.setValue(_bindedConfig.getValue() + 1);
						}
					}
				}
			};
		onPaintFunc_ = [&_bindedConfig](OptionElement* _element)
			{
				RectangleShape area(_element->getWidth(), _element->getHeight());
				area.setFill(_element->isSelected_ ? Color(128, 128, 128, 0.5f) : Color(64, 64, 64, 0.5f));
				area.paint(_element->x, _element->y, _element->transform);
				Text text("Consolas", 12, TextAlign::MiddleRight);
				text.setBrush(Color(255, 255, 255));
				text.textContent = std::to_wstring(_bindedConfig.value());
				text.paint(_element->x + 8, _element->y + 8, _element->getWidth() - 16, _element->getHeight() - 16, graphics::createSolidColorBrush(Color(255, 255, 255)));
				text.textContent = _element->label_;
				text.setTextAlign(TextAlign::MiddleLeft);
				text.paint(_element->x + 8, _element->y + 8, _element->getWidth() - 16, _element->getHeight() - 16, graphics::createSolidColorBrush(Color(255, 255, 255)));
				return false;
			};
	}
}