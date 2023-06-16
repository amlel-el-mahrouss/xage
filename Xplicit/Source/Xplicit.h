/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: Xplicit.h
 *			Purpose: 
 *
 * =====================================================================
 */

#pragma once

#include "Config.h"

#ifdef XPLICIT_END_OF_BUFFER
#undef XPLICIT_END_OF_BUFFER
#endif // XPLICIT_END_OF_BUFFER

#define XPLICIT_END_OF_BUFFER '\0'

XPLICIT_API void xplicit_log(const char* str);
XPLICIT_API size_t fstrlen(const char* str);
XPLICIT_API time_t xplicit_get_epoch();
XPLICIT_API FILE* xplicit_get_logger();
XPLICIT_API bool xplicit_open_logger();

#ifdef _MSC_VER
#define PACKED_STRUCT(DECL) __pragma( pack(push, 1) ) DECL; __pragma( pack(pop))
#else
#define PACKED_STRUCT(DECL)
#endif

namespace Xplicit 
{
	namespace FS = std::filesystem;

	using Thread = std::jthread;
	using String = std::string;

	template <typename CharType>
	using BasicString = std::basic_string<CharType>;

	// platform dependent chars
#ifdef XPLICIT_WINDOWS
	using PChar = wchar_t;
#else
	using PChar = char;
#endif

	using PString = BasicString<PChar>;

	class EngineError : public std::runtime_error
	{
	public:
		EngineError(const char* error = "Unidentified Engine Error") : std::runtime_error(error) {}
		~EngineError() = default; // let the ABI define that.

		EngineError& operator=(const EngineError&) = default;
		EngineError(const EngineError&) = default;

	};

#ifdef XPLICIT_WINDOWS
	class Win32Error : public std::runtime_error 
	{
	public:
		Win32Error(const std::string& what) : std::runtime_error(what) { m_hr = GetLastError(); }
		~Win32Error() = default; // let the ABI define that.

		Win32Error& operator=(const Win32Error&) = default;
		Win32Error(const Win32Error&) = default;

		HRESULT hr() { return m_hr; }

	private:
		HRESULT m_hr;

	};

	template <typename T>
	class ModuleType;
	class Win32Helpers;
	class ModuleManagerWin32;

	template <typename T>
	class ModuleType final 
	{
		ModuleType() = default;
		friend ModuleManagerWin32;

	public:
		ModuleType(T t) : m_ptr(t) {}

		T get() { return m_ptr; }
		T operator->() { return m_ptr; }

	private:
		T m_ptr;

	};

	class Win32Helpers final 
	{
	public:
		static HWND find_wnd(const wchar_t* compare) 
		{
			if (!compare) return nullptr;

			HWND hCurWnd = nullptr;
			wchar_t string[256];

			do
			{
				ZeroMemory(&string, 255);

				hCurWnd = FindWindowEx(nullptr, hCurWnd, nullptr, nullptr);
				GetWindowTextW(hCurWnd, string, 256);

				if (string == compare || !lstrcmpW(compare, string))
					return hCurWnd;

			} while (hCurWnd != nullptr);

			return nullptr;
		}

	};

	class ModuleManagerWin32 final 
	{
	public:
		ModuleManagerWin32() = delete;

		explicit ModuleManagerWin32(const std::string& module_name)
			: hMod(nullptr)
		{ 
			assert(!module_name.empty());
			hMod = LoadLibraryExA(module_name.c_str(), nullptr, 0);

			if (!hMod) 
				throw Win32Error("LoadLibraryExA failed!");
		}

		~ModuleManagerWin32() 
		{
			if (hMod)
				FreeLibrary(hMod);
		}

		ModuleManagerWin32& operator=(const ModuleManagerWin32&) = default;
		ModuleManagerWin32(const ModuleManagerWin32&) = default;

		template <typename T>
		ModuleType<T> get(const std::string& ordinal)
		{
			if (!hMod) return {};
			if (ordinal.empty()) return {};

			T the{ reinterpret_cast<T>(GetProcAddress(hMod, ordinal.c_str())) };

			return the;
		}

	private:
		HMODULE hMod;

	};

	using ModuleManager = ModuleManagerWin32;
#endif // XPLICIT_WINDOWS

	template<typename T>
	concept IsPointer = (std::is_pointer<T>::value);

	template<typename T>
	concept NotNull = (!std::is_same<T, std::nullptr_t>::value);

	template<typename T>
	concept IsPointerAndNotNull = (IsPointer<T> && NotNull<T>);

	template <typename T>
	class Ref 
	{
	public:
		Ref() : m_ptr(nullptr) {}

		Ref(IsPointerAndNotNull auto ptr) 
			: m_ptr(ptr) 
		{
			m_ref_cnt = new int32_t(0);
			assert(m_ref_cnt);

			this->_inc_ref();
		}

		~Ref()
		{
			if (m_ref_cnt && *m_ref_cnt == 0)
			{
				if (m_ptr)
				{
					delete m_ptr;
				}
			}
		}

		int32_t count() { return *m_ref_cnt; }

		T get() { return m_ptr; }
		T operator->() { return m_ptr; }

	public:
		Ref& operator=(const Ref& ref)
		{
			m_ptr = ref.m_ptr;
			this->m_ref_cnt = ref.m_ref_cnt;

			return *this;
		}

		Ref(const Ref& ref)
		{
			m_ptr = ref.m_ptr;
			this->m_ref_cnt = ref.m_ref_cnt;
		}

		operator bool() { return m_ptr; }

		void _inc_ref() 
		{
			if (m_ref_cnt)
				++*(m_ref_cnt); 
		}

	private:
		T m_ptr;
		int32_t* m_ref_cnt{ nullptr };

	};

	template <typename T>
	static inline Ref<T> make_ref(Ref<T> cls)
	{
		Ref<T> ref{ cls };
		ref._inc_ref();

		return ref;
	}

	template <typename T>
	static inline Ref<T> make_ref(T cls)
	{
		Ref<T> ref{ cls };

		return ref;
	}

#ifdef XPLICIT_WINDOWS
	using PrivShellData = HWND;

	constexpr const char* SHELL_MANAGER_EXEC_OPEN = "open";

	class ShellManager 
	{
	public:
		static Ref<HINSTANCE> open(std::string& appname, PrivShellData priv) 
		{
			if (appname.empty()) 
				return {};

			Ref<HINSTANCE> instance = ShellExecuteA(priv, SHELL_MANAGER_EXEC_OPEN, appname.c_str(), nullptr, nullptr, SW_SHOW);
			if (!instance) throw Win32Error("HINSTANCE Error, check hr()");

			return instance;
		}

		static Ref<HINSTANCE> open(const char* appname, PrivShellData priv) 
		{
			if (!appname) 
				return {};

			Ref<HINSTANCE> instance = ShellExecuteA(priv, SHELL_MANAGER_EXEC_OPEN, appname, nullptr, nullptr, SW_SHOW);
			if (!instance) throw Win32Error("HINSTANCE Error, check hr()");

			return instance;
		}

	};
#endif

	template <typename Fn, typename... Args>
	class AsyncAction final 
	{
	public:
		using FnT = Fn;

	public:
		AsyncAction() = delete;

		AsyncAction(FnT fn, Args&&... args)
		{
			XPLICIT_ASSERT(fn);
			m_thread = std::thread(fn, args...);
		}

		~AsyncAction() {}

		void detach() noexcept
		{
			m_thread.detach();
		}

		std::thread& operator->() const { return m_thread; }

	public:
		AsyncAction& operator=(const AsyncAction&) = default;
		AsyncAction(const AsyncAction&) = default;

	private:
		std::jthread m_thread;

	};

	static inline std::string get_at_current_path(const std::string& program_name) 
	{
		auto path = std::filesystem::current_path();
		path /= program_name;

		try 
		{
			std::ifstream io(path);

			// if it aint exist, throw and error
			if (!io.is_open())
				throw std::bad_alloc();

		}
		catch (std::bad_alloc err) 
		{
			return "";
		}

		return path.string() + program_name;
	}

#ifdef XPLICIT_WINDOWS
	static inline bool init_winsock(WSADATA* dat) 
	{
		int result = WSAStartup(MAKEWORD(2, 2), dat);
		if (result != 0) {
			return false;
		}

		return true;
	}
#endif

#ifdef XPLICIT_WINDOWS
	// for debug builds, also new the in-game console :p
	static inline void open_terminal(FILE* fp = stdout)
	{
		AllocConsole();
		(void)freopen("CONOUT$", "w", fp);
	}
#endif

	class DialogHelper
	{
	public:
		static int32_t message_box(LPCWSTR title, LPCWSTR message, int flags = MB_OK)
		{
			if (!title)
				return -1;

			if (!message)
				return -1;

#ifdef XPLICIT_WINDOWS
			return MessageBoxExW(nullptr, message, title, flags, LANG_ENGLISH);
#endif // !XPLICIT_WINDOWS
		}

		static int32_t message_box(LPCWSTR title, LPCWSTR header, LPCWSTR message, PCWSTR icon, _TASKDIALOG_COMMON_BUTTON_FLAGS buttonFlags)
		{
#ifdef XPLICIT_WINDOWS
			if (!title)
				return -1;

			if (!header)
				return -1;

			if (!message)
				return -1;

			TASKDIALOGCONFIG config = { sizeof(TASKDIALOGCONFIG) };
			int32_t clicked_button = 0;
		
			config.pszContent = message;
			config.pszMainInstruction = header;
			config.pszWindowTitle = title;
			config.pszMainIcon = icon;
			config.dwCommonButtons = TDCBF_YES_BUTTON;
			config.hwndParent = nullptr;

			TaskDialogIndirect(&config, &clicked_button, nullptr, nullptr);

			return clicked_button;
#endif // !XPLICIT_WINDOWS
		}

	};

	class Logger final
	{
	public:
		Logger() = default;
		~Logger() = default;

		Logger& operator=(const Logger&) = default;
		Logger(const Logger&) = default;

	public:
		static Logger& get_singleton() noexcept
		{
			static Logger LOGGER;
			return LOGGER;
		}

		auto get()
		{
			static spdlog::logger* LOGGER = nullptr;

			if (!LOGGER)
			{
				auto info = std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>();
				info->set_level(spdlog::level::info);
				info->set_pattern("[%^XPLICIT%$] %v");

				auto critical = std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>();
				critical->set_level(spdlog::level::critical);
				critical->set_pattern("[%^XPLICIT%$] %v");

				auto err = std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>();
				err->set_level(spdlog::level::err);
				err->set_pattern("[%^XPLICIT%$] %v");

				LOGGER = new spdlog::logger("EngineSink", { err, info, critical });
			}

			return LOGGER;
		}

	};

	class Timer final
	{
	public:
		Timer() noexcept : m_then(std::chrono::steady_clock::now()) {}
		~Timer() {}

		std::chrono::steady_clock::time_point now() noexcept
		{
			return std::chrono::steady_clock::now();
		}

		const std::chrono::steady_clock::time_point& start() noexcept
		{
			return m_then;
		}

		std::chrono::duration<double> time_since(const std::chrono::steady_clock::time_point& at) noexcept
		{
			return at - m_then;
		}

	private:
		std::chrono::steady_clock::time_point m_then;

	};

	class XPLICIT_API UUIDFactory final
	{
	public:
		template <short ver>
		static uuids::uuid version()
		{
			switch (ver)
			{
			case 4:
			{
				std::random_device rd;
				auto seed_data = std::array<int, std::mt19937::state_size> {};
				
				std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
				
				std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
				std::mt19937 generator(seq);

				auto gen = uuids::uuid_random_generator(generator);

				return gen();
			}
			}

			return {};
		}

	};

	class XPLICIT_API FilesystemWrapper final
	{
	public:
		FilesystemWrapper() = default;
		~FilesystemWrapper() = default;

	public:
		std::ofstream write(const char* outPath) noexcept
		{
			if (std::filesystem::exists(outPath))
				return std::ofstream(outPath, std::ios::app);

			return std::ofstream(outPath);
		}

		std::ofstream open(const char* outPath)
		{
			if (!std::filesystem::exists(outPath))
				throw std::runtime_error("Bad File-system path!");

			return std::ofstream(outPath);
		}

		bool create_directory(const char* path)
		{
			return std::filesystem::create_directory(path);
		}

		std::filesystem::path get_temp() noexcept
		{
			return std::filesystem::temp_directory_path();
		}

	};

#define XPLICIT_MAX_POOL (1000)

	template <typename PtrType>
	struct Pool final
	{
	public:
		explicit Pool()
		{
			this->mPointer = (char*)malloc(sizeof(char) * (sizeof(PtrType) * XPLICIT_MAX_POOL));
			ZeroMemory(this->mPointer, (sizeof(PtrType) * XPLICIT_MAX_POOL));
		}

		~Pool()
		{
			if (this->mPointer)
			{
				free(this->mPointer);
			}
		}

	private:
		char* mPointer;

	public:
		template <typename... Args>
		PtrType* allocate(Args&&... args)
		{
			auto sz = sizeof(PtrType) * XPLICIT_MAX_POOL;
			auto idx = 0UL;

			bool good = true;

			while (idx < sz)
			{
				good = true;

				for (size_t i = 0; i < sz; i++)
				{
					if (mPointer[i + idx] != 0)
					{
						good = false;
						break;
					}

				}

				if (good)
				{
					PtrType* ptr = reinterpret_cast<PtrType*>(&mPointer[idx]);
					memset(ptr, 0, sizeof(PtrType));

					*ptr = PtrType(std::forward<Args>(args)...);

					return ptr;
				}

				idx += sizeof(PtrType);
			}

			return nullptr;
		}

		template <typename T>
		void deallocate(T* ptr)
		{
			ptr->~T();
			memset(ptr, 0, sizeof(T));
		}

	};
}

#define XPLICIT_INIT_COM XPLICIT_ASSERT(SUCCEEDED(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE)))
#define XPLICIT_FINI_COM CoUninitialize()

#define XPLICIT_CRITICAL(...) Xplicit::Logger::get_singleton().get()->critical(__VA_ARGS__)
#define XPLICIT_ERROR(...) Xplicit::Logger::get_singleton().get()->error(__VA_ARGS__)
#define XPLICIT_INFO(...) Xplicit::Logger::get_singleton().get()->info(__VA_ARGS__)
