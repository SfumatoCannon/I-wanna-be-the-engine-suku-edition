#pragma once
#include "UIElement.h"
#include "suku_config/config_element.h"
#include "suku_foundation/input.h"
#include "interfaces/IFocusable.h"

namespace suku
{
	class OptionPool;

	class ConfigOptionUIElement : public UIElement, public IFocusable
	{
	public:
		ConfigOptionUIElement* prev = nullptr;
		ConfigOptionUIElement* next = nullptr;

		template<typename T> ConfigOptionUIElement(
			ConfigElement<T>& _bindedConfig, String _label, real _x, real _y, real _width, real _height, String _info = String());
		template<typename T> ConfigOptionUIElement(ConfigElement<T>& _bindedConfig, String _label) : ConfigOptionUIElement(_bindedConfig, _label, 0, 0, 256, 32) {}
		template<typename T> ConfigOptionUIElement(ConfigElement<T>& _bindedConfig, String _label, String _info) : ConfigOptionUIElement(_bindedConfig, _label, 0, 0, 256, 32, _info) {}

		// Implement IFocusable interface
		virtual void focus() override;
		virtual bool unfocus() override;
		virtual bool isFocused() const override { return isFocused_; }

		virtual void onUpdate() override;
		virtual void onPaint() override;

		void setHeight(int _height) { height_ = originalHeight_ = _height; }
	private:
		friend class OptionPool;
		IConfigElement* pBindedConfig_;
		String label_;
		Text labelText_;
		Text valueText_;
		bool hasInfo_ = false;
		String info_;
		Text infoText_;
		bool isFocused_ = false;
		real originalHeight_;
	};

	template<typename T>
	inline ConfigOptionUIElement::ConfigOptionUIElement(ConfigElement<T>& _bindedConfig, String _label, real _x, real _y, real _width, real _height, String _info)
		: UIElement(_x, _y, _width, _height), label_(_label), originalHeight_(_height),
		labelText_({ "Consolas", _height / 2.0f, TextStyle::Align::MiddleLeft }), valueText_({ "Consolas", _height / 2.0f, TextStyle::Align::MiddleRight }), infoText_({ "Consolas", 16, TextStyle::Align::TopLeft }),
		pBindedConfig_(&_bindedConfig)
	{
		if (!_info.isEmpty())
		{
			hasInfo_ = true;
			info_ = _info;
			infoText_.text = _info;
		}
	}
}