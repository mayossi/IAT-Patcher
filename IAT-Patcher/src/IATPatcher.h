#pragma once

#include "Windows/Hook/Hook.h"
#include "Windows/Hook/IHookInstaller.h"


namespace patcher
{
	class IATPatcher final : public clib::windows::hook::IHookInstaller
	{
	public:
		void install(const clib::windows::hook::Hook& hook) override;

		void uninstall(const clib::windows::hook::Hook& hook) override;
	};
} // namespace patcher1
