#include "IATPatcher.h"

#include "Utilities/String/String.h"
#include "ExceptionBase/Exception.h"
#include "Windows/PE/Parser/ImportsParser.h"

using namespace clib::windows;
using namespace clib::utilities;


namespace patcher
{
	void IATPatcher::install(const hook::Hook& hook)
	{
		pe::PeImportsTable peImports = pe::parser::getImportsTable(GetModuleHandle(NULL));

		for (const auto& importedModule : peImports.imports)
		{
			if (string::toLower(importedModule.name) == string::toLower(hook.moduleName))
			{
				for (const auto& importedFunction : importedModule.functions)
				{
					if (string::toLower(importedFunction.name) == string::toLower(hook.functionName)
						&& !importedFunction.isImportedByOrdinal)
					{
						DWORD oldMemoryProtection;
						VirtualProtect(importedFunction.relAddress, sizeof(void*), PAGE_READWRITE, &oldMemoryProtection);
						*static_cast<void**>(importedFunction.relAddress) = hook.pHook;
						VirtualProtect(importedFunction.relAddress, sizeof(void*), oldMemoryProtection, &oldMemoryProtection);

						return;
					}
				}

				throw clib::exception::Exception("Target function not found.");
			}
		}

		throw clib::exception::Exception("Target module not found in PE imports.");
	}

	void IATPatcher::uninstall(const hook::Hook& hook)
	{
		pe::PeImportsTable peImports = pe::parser::getImportsTable(GetModuleHandle(NULL));

		for (const auto& importedModule : peImports.imports)
		{
			if (string::toLower(importedModule.name) == string::toLower(hook.moduleName))
			{
				for (const auto& importedFunction : importedModule.functions)
				{
					if (string::toLower(importedFunction.name) == string::toLower(hook.functionName)
						&& !importedFunction.isImportedByOrdinal)
					{
						DWORD oldProtect;
						VirtualProtect(importedFunction.relAddress, sizeof(void*), PAGE_READWRITE, &oldProtect);
						*static_cast<void**>(importedFunction.relAddress) = hook.pOriginal;
						VirtualProtect(importedFunction.relAddress, sizeof(void*), oldProtect, &oldProtect);

						return;
					}
				}

				throw clib::exception::Exception("Target function not found.");
			}
		}

		throw clib::exception::Exception("Target module not found in PE imports.");
	}

} // namespace patcher
